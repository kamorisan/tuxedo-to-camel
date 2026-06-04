# Tuxedo統合戦略ドキュメント

最終更新: 2026-06-04

## 📋 現状まとめ

### ✅ 完了しているもの

1. **Tuxedoソースコード実装**（100%完了）
   - [src/MSGSVC.c](components/tuxedo/src/MSGSVC.c) - Tuxedoサービス本体
   - [src/amqclient.c](components/tuxedo/src/amqclient.c) / [src/amqclient.h](components/tuxedo/src/amqclient.h) - AMQPクライアント
   - [config/ubbconfig](components/tuxedo/config/ubbconfig) - Tuxedo設定
   - [scripts/boot.sh](components/tuxedo/scripts/boot.sh), [scripts/shutdown.sh](components/tuxedo/scripts/shutdown.sh), [scripts/entrypoint.sh](components/tuxedo/scripts/entrypoint.sh)
   - [Makefile](components/tuxedo/Makefile), [Containerfile](components/tuxedo/Containerfile)

2. **他コンポーネント実装**（100%完了）
   - Quarkus WebUI（OpenShift稼働中）
   - Apache Camel（実装完了、デプロイ待ち）
   - AMQマニフェスト（作成完了）
   - Kafkaマニフェスト（作成完了）

### 🚧 残っている課題

**Tuxedoベースイメージのビルド** - これが唯一のブロッカー

---

## 🎯 Tuxedoベースイメージビルド戦略

### 問題点

1. **アーキテクチャ問題**: MacのARM64環境ではx86_64用Tuxedoバイナリが動作しない
2. **依存関係**: 公式DockerfileはServerJRE 8ベースイメージが必要
3. **ローカルビルドの複雑性**: podmanでのクロスプラットフォームビルドは時間がかかる

### 解決策：OpenShift上で直接ビルド（推奨）

**理由:**
- ✅ OpenShiftはx86_64環境で自動ビルド
- ✅ GitHub Actionsや外部CIも不要
- ✅ WebUI/Camelと同じパターン
- ✅ 環境依存なし

**手順概要:**

#### 戦略A: 2段階BuildConfig（推奨）

**Step 1: Tuxedoベースイメージ用BuildConfig**

Oracle Linux 8ベースで、Tuxedoインストーラーを含めてビルド。

課題：
- Tuxedoインストーラー（275MB）をGitリポジトリに含められない
- インストーラーをどこかに配置する必要がある

**解決策の選択肢:**

1. **OpenShift内部でインストーラーを提供**
   - PersistentVolume (PV) にインストーラーを配置
   - BuildConfig内でマウントして使用
   - 手順:
     ```bash
     # PVC作成
     oc create -f pvc-tuxedo-installer.yaml
     
     # Podを起動してインストーラーをコピー
     oc run -it --rm upload-pod --image=registry.access.redhat.com/ubi8/ubi:latest \
       --overrides='{"spec":{"containers":[{"name":"upload","image":"registry.access.redhat.com/ubi8/ubi:latest","volumeMounts":[{"mountPath":"/data","name":"installer"}]}],"volumes":[{"name":"installer","persistentVolumeClaim":{"claimName":"tuxedo-installer"}}]}}'
     
     # Pod内でインストーラーを配置
     # （別ターミナルで）
     oc cp /Users/kamori/temp/Oracle_Tuxedo_22c/tuxedo221100_64_Linux_01_x86.zip upload-pod:/data/
     ```

2. **HTTP経由でダウンロード**
   - 内部HTTPサーバーや外部ストレージからダウンロード
   - Dockerfile内で `curl` または `wget`
   - セキュリティ上、一時的な対応のみ推奨

3. **Tuxedoインストーラーなしの簡易版**
   - Tuxedoの代わりにモックサービスを使用
   - デモ目的であれば、メッセージ中継機能だけを持つシンプルなサービスで代替
   - 本格実装は後回し

#### 戦略B: Tuxedoモックサービス（最速）

Tuxedoを使わず、同等機能を持つQuarkusサービスで代替：

```
[WebUI] 
  ↓ HTTP POST
[Tuxedo-Mock (Quarkus)] - WSL相当のREST APIを提供
  ↓ AMQP
[Red Hat AMQ Queue: TUXEDO.OUT]
  ↓
[Apache Camel] → [Kafka]
```

**メリット:**
- ✅ すぐにデプロイ・テスト可能
- ✅ OpenShiftで直接ビルド
- ✅ 全体フロー確認が最優先

**デメリット:**
- ❌ 本物のTuxedoではない
- ❌ デモとしてのインパクトが減る

**実装時間:** 30分

#### 戦略C: GitHub Actionsでx86_64ビルド

- GitHub Actionsはx86_64ランナー
- ビルドしたイメージをContainer Registry（Quay.io、DockerHub）にPush
- OpenShiftから取得

**メリット:**
- ✅ 正式なTuxedoイメージ
- ✅ 再利用可能

**デメリット:**
- ❌ GitHub Actionsセットアップが必要
- ❌ Container Registryアカウント必要
- ❌ インストーラーをGitHub Secretsで管理（複雑）

---

## 🏆 推奨アプローチ

### フェーズ1: 簡易版で全体フロー確認（今日）

**実装:** 戦略B - Tuxedoモックサービス

1. Quarkus REST APIで `/MSGSVC` エンドポイントを作成
2. メッセージをAMQPでキューへ送信
3. WebUI → Tuxedo-Mock → AMQ → Camel → Kafka の動作確認

**所要時間:** 30分  
**成果:** 全体フローが動作することを証明

### フェーズ2: 本物のTuxedo統合（次回セッション）

**実装:** 戦略A - OpenShift内でビルド

1. OpenShift PVCにTuxedoインストーラーを配置
2. カスタムBuildConfigでTuxedoベースイメージ作成
3. 2段階目のBuildConfigでMSGSVCイメージ作成
4. デプロイ・統合テスト

**所要時間:** 2-3時間  
**成果:** 本格的なTuxedo統合完成

---

## 📊 Tuxedo関連タスクの詳細

### フェーズ1タスク（今日実施可能）

| # | タスク | 詳細 | 所要時間 | 状態 |
|---|--------|------|---------|------|
| 1 | Tuxedoモック実装 | Quarkus REST API + AMQP送信 | 20分 | 🔲 未着手 |
| 2 | モックのOpenShiftマニフェスト | Namespace, BuildConfig, Deployment, Service, Route | 10分 | 🔲 未着手 |
| 3 | AMQ Operatorインストール | OpenShiftでOperator有効化 | 5分 | 🔲 未着手 |
| 4 | AMQデプロイ | `oc apply -f openshift/amq/` | 5分 | 🔲 未着手 |
| 5 | Kafkaデプロイ | Strimzi Operatorインストール + `oc apply -f openshift/kafka/` | 10分 | 🔲 未着手 |
| 6 | Camelデプロイ | `oc apply -f openshift/camel/` | 5分 | 🔲 未着手 |
| 7 | WebUI → Mock統合 | WebUIのTuxedo URL更新 | 5分 | 🔲 未着手 |
| 8 | 全体統合テスト | WebUIからメッセージ送信してKafkaまで確認 | 10分 | 🔲 未着手 |

**合計:** 約70分

### フェーズ2タスク（次回セッション）

| # | タスク | 詳細 | 所要時間 | 状態 |
|---|--------|------|---------|------|
| 1 | PVC作成 | Tuxedoインストーラー保存用 | 5分 | 🔲 未着手 |
| 2 | インストーラーアップロード | `oc cp` でPVCにコピー | 10分 | 🔲 未着手 |
| 3 | Tuxedoベース BuildConfig作成 | Oracle Linux 8 + インストーラー | 15分 | 🔲 未着手 |
| 4 | Tuxedoベースイメージビルド実行 | `oc start-build` | 20分 | 🔲 未着手 |
| 5 | Tuxedo MSGSVC BuildConfig作成 | ベースイメージ + ソースコード | 10分 | 🔲 未着手 |
| 6 | Tuxedo MSGSVCイメージビルド実行 | `oc start-build` | 10分 | 🔲 未着手 |
| 7 | Tuxedo Deploymentマニフェスト作成 | Deployment, Service, Route | 15分 | 🔲 未着手 |
| 8 | Tuxedoデプロイ | `oc apply -f openshift/tuxedo/` | 5分 | 🔲 未着手 |
| 9 | Tuxedo動作確認 | ログ確認、tmadmin実行 | 10分 | 🔲 未着手 |
| 10 | WebUI → Tuxedo統合 | WebUIのURL更新 | 5分 | 🔲 未着手 |
| 11 | 全体統合テスト | 本物のTuxedoで全フロー確認 | 15分 | 🔲 未着手 |

**合計:** 約120分

---

## 🔧 技術的詳細

### Tuxedoモックサービス仕様

**エンドポイント:** `POST /MSGSVC`

**Request:**
```
Content-Type: text/plain
Body: メッセージ本文
```

**Response:**
```json
{
  "status": "success",
  "message": "Message forwarded to AMQ"
}
```

**処理フロー:**
1. リクエストボディを受信
2. ログ出力: `[Tuxedo-Mock] Received: {message}`
3. AMQP経由でAMQキュー `TUXEDO.OUT` に送信
4. ログ出力: `[Tuxedo-Mock] Message queued to AMQ: {message}`
5. 成功レスポンス返却

---

## 🎬 次のアクション

### 今日実施する場合

1. **Tuxedoモックサービス実装** → すぐ開始可能
2. **AMQ/Kafka/Camelデプロイ** → 順次デプロイ
3. **統合テスト** → 全体動作確認

### 次回セッションで実施する場合

1. **PROGRESS.md更新** → 今日の成果を記録
2. **Tuxedo統合計画確認** → このドキュメントを参照
3. **フェーズ2実施** → 本物のTuxedo統合

---

## 💡 推奨

**今日はフェーズ1（モック版）で全体フロー確認**することを強く推奨します。

理由：
- ✅ すぐに動作するデモを作成できる
- ✅ AMQ/Camel/Kafkaの動作確認ができる
- ✅ 問題の早期発見
- ✅ 次回セッションでTuxedo追加が容易

Tuxedoの本格統合は、全体が動作することを確認してから進める方が安全です。

---

**最終更新者:** Claude Sonnet 4.5  
**次回作業:** フェーズ1（モック版）またはフェーズ2（本格Tuxedo）を選択
