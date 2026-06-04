# Tuxedo OpenShiftデプロイメントガイド

**作成日**: 2026-06-04  
**対象**: Oracle Tuxedo Mock ServerをOpenShiftにデプロイする際の注意点

---

## 📋 概要

本ドキュメントは、Oracle Tuxedo（またはモック実装）をOpenShiftにデプロイする際に遭遇した課題と解決策をまとめたものです。

---

## ⚠️ 重要な注意点

### 1. Oracle Installerの制約

**問題:**
- Oracle Tuxedo Installerは**500MBのswap領域を必須要求**
- OpenShiftのBuildPodにはswap領域がない
- `-ignorePrereq`フラグが効かない

**解決策:**
- Oracle Installerを完全に回避
- 既存のTuxedoランタイムを活用
- または、モック実装で代替

### 2. Tuxedoランタイムの制限

**問題:**
- ランタイムには`bin/`, `lib/`のみ含まれる
- 開発用ツールがない:
  - `buildserver`コマンド
  - `include/`ディレクトリ（ヘッダーファイル）
  - ビルド用ライブラリ

**解決策:**
- 完全なTuxedo SDKが必要な場合は、別途インストール
- または、プリコンパイル済みバイナリを使用
- または、モック実装

### 3. コンテナイメージのENTRYPOINT

**問題:**
- ベースイメージの`ENTRYPOINT`が上書きできない場合がある
- Containerfileで`ENTRYPOINT []`が効かないケースあり

**解決策:**
- Deploymentマニフェストで`command:`を明示的に指定
```yaml
spec:
  containers:
    - name: tuxedo
      image: your-image:latest
      command: ["/path/to/your/script.sh"]
```

### 4. Binary Build

**利点:**
- 大容量ファイル（275MB+）をGitに含めない
- OpenShiftのx86_64環境で自動ビルド
- ローカルアーキテクチャ問題を回避

**使用方法:**
```bash
cd /path/to/build/context
oc start-build tuxedo-base --from-dir=. --follow -n demo-tuxedo
```

### 5. ImageStream参照

**問題:**
- Deployment内でイメージ名だけ指定するとPullエラー

**解決策:**
- 完全なレジストリパスを指定
```yaml
image: image-registry.openshift-image-registry.svc:5000/demo-tuxedo/tuxedo-msgsvc:latest
```

---

## 🐍 Python Mock実装の推奨事項

### なぜPython Mockを選択したか

1. **迅速なデプロイ**: コンパイル不要
2. **シンプルな依存関係**: Python3 + curlのみ
3. **デバッグが容易**: ログが明確
4. **拡張性**: AMQ統合が容易に追加可能

### Mock実装の構成

```
components/tuxedo/
├── scripts/
│   ├── msgsvc-mock.py      # メインHTTPサーバー
│   └── start-mock.sh       # 起動ラッパー
├── Containerfile           # Python3環境
└── include/               # （不要：Mockの場合）
```

### 必要なパッケージ

**Containerfile:**
```dockerfile
RUN yum -y install python3 curl && \
    yum clean all && \
    rm -rf /var/cache/yum
```

### ヘルスチェック

```yaml
livenessProbe:
  tcpSocket:
    port: 8080
  initialDelaySeconds: 30
  periodSeconds: 10
  
readinessProbe:
  tcpSocket:
    port: 8080
  initialDelaySeconds: 10
  periodSeconds: 5
```

---

## 🔧 トラブルシューティング

### Pod が CrashLoopBackOff

**確認事項:**
1. ログ確認
```bash
oc logs -n demo-tuxedo <pod-name>
```

2. イメージ確認
```bash
oc describe pod -n demo-tuxedo <pod-name>
```

3. Deploymentのcommand確認
```bash
oc get deployment tuxedo-msgsvc -n demo-tuxedo -o yaml | grep -A 5 command
```

### ImagePullBackOff

**原因:**
- イメージパスが不正
- ImageStreamが存在しない

**解決:**
```bash
# ImageStream確認
oc get istag -n demo-tuxedo

# 完全パス使用
image: image-registry.openshift-image-registry.svc:5000/demo-tuxedo/tuxedo-msgsvc:latest
```

### "null: command not found"

**原因:**
- ベースイメージのENTRYPOINTが実行されている
- スクリプトのshebang行に問題

**解決:**
1. Deploymentで`command:`を明示
2. スクリプトのshebangを確認: `#!/bin/bash` or `#!/usr/bin/env python3`
3. 実行権限確認: `chmod +x script.sh`

---

## 📦 デプロイ手順（ステップバイステップ）

### 1. Namespace作成

```bash
oc apply -f openshift/tuxedo/01-namespace.yaml
```

### 2. ベースイメージビルド（Binary Build）

```bash
cd /path/to/tuxedo/build/context
oc apply -f openshift/tuxedo/02-base-imagestream.yaml
oc apply -f openshift/tuxedo/03-base-buildconfig.yaml
oc start-build tuxedo-base --from-dir=. --follow -n demo-tuxedo
```

### 3. MSGSVCイメージビルド（Git Source）

```bash
oc apply -f openshift/tuxedo/04-msgsvc-imagestream.yaml
oc apply -f openshift/tuxedo/05-msgsvc-buildconfig.yaml
# 自動的にビルド開始（ImageChange trigger）
```

### 4. デプロイ

```bash
oc apply -f openshift/tuxedo/06-deployment.yaml
oc apply -f openshift/tuxedo/07-service.yaml
oc apply -f openshift/tuxedo/08-route.yaml
```

### 5. 動作確認

```bash
# Pod確認
oc get pods -n demo-tuxedo

# ログ確認
oc logs -f deployment/tuxedo-msgsvc -n demo-tuxedo

# Route取得
ROUTE=$(oc get route tuxedo-msgsvc -n demo-tuxedo -o jsonpath='{.spec.host}')

# テスト
curl -X POST https://$ROUTE/MSGSVC \
  -H "Content-Type: text/plain" \
  -d "Test message"
```

---

## 🔒 セキュリティ考慮事項

### 1. 本番環境での注意

- Mock実装は**デモ専用**
- 本番では本物のTuxedoを使用
- AMQ接続には認証を追加
- TLS/SSL設定

### 2. 環境変数

機密情報はSecretで管理:
```yaml
env:
  - name: AMQ_PASSWORD
    valueFrom:
      secretKeyRef:
        name: amq-credentials
        key: password
```

---

## 📚 参考リソース

### プロジェクト内ドキュメント

- [PROGRESS_20260604.md](PROGRESS_20260604.md) - 本日の詳細進捗
- [TUXEDO_STRATEGY.md](TUXEDO_STRATEGY.md) - Tuxedo統合戦略
- [design/architecture.md](design/architecture.md) - アーキテクチャ設計

### 外部ドキュメント

- [Oracle Tuxedo Documentation](https://docs.oracle.com/en/middleware/tuxedo/)
- [OpenShift BuildConfig](https://docs.openshift.com/container-platform/latest/cicd/builds/understanding-buildconfigs.html)
- [Python HTTP Server](https://docs.python.org/3/library/http.server.html)

---

## ✅ チェックリスト

デプロイ前の確認:

- [ ] Namespaceが作成されているか
- [ ] ベースイメージがビルド成功しているか
- [ ] MSGSVCイメージがビルド成功しているか
- [ ] Deploymentマニフェストのイメージパスが正しいか
- [ ] `command:`が指定されているか（必要な場合）
- [ ] 環境変数（AMQ_HOST等）が設定されているか
- [ ] Routeが作成されているか

デプロイ後の確認:

- [ ] Podが Running状態か
- [ ] ログにエラーがないか
- [ ] ヘルスチェックがパスしているか
- [ ] curlテストが成功するか
- [ ] メッセージがログに記録されているか

---

**最終更新**: 2026-06-04  
**作成者**: Claude Sonnet 4.5 with Human  
**プロジェクト**: tuxedo-to-camel demo
