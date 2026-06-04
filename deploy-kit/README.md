# Tuxedo to Kafka Demo - デプロイキット

新しいOpenShift環境に完全なデモ環境を自動デプロイするスクリプト集です。

---

## 📋 目次

- [概要](#概要)
- [前提条件](#前提条件)
- [クイックスタート](#クイックスタート)
- [デプロイ内容](#デプロイ内容)
- [使い方](#使い方)
- [トラブルシューティング](#トラブルシューティング)
- [クリーンアップ](#クリーンアップ)

---

## 概要

このデプロイキットは、以下のコンポーネントを自動的にデプロイします：

```
Web UI → Tuxedo Service → Tuxedo /Q → Apache Camel → Kafka Broker
```

### デプロイされるコンポーネント

1. **AMQ Streams Operator** - Kafka管理用Operator
2. **Kafka Cluster** - メッセージブローカー（1レプリカ）
3. **Tuxedo C Implementation** - C言語によるTuxedo /Q実装
4. **Apache Camel** - Tuxedo /QとKafka間の統合
5. **Quarkus WebUI** - メッセージ送信用Webインターフェース

---

## 前提条件

### 必須

- ✅ OpenShift 4.x クラスター
- ✅ `oc` CLI インストール済み
- ✅ クラスターへのログイン済み
- ✅ cluster-admin権限 または 以下の権限:
  - Namespace作成
  - Operator管理
  - BuildConfig/ImageStream作成

### 確認コマンド

```bash
# oc CLIバージョン確認
oc version

# ログイン状態確認
oc whoami

# 権限確認
oc auth can-i create namespace
```

---

## クイックスタート

### 1行デプロイ

```bash
cd deploy-kit
chmod +x deploy-all.sh cleanup.sh
./deploy-all.sh
```

### 環境変数でカスタマイズ

```bash
# 自分のフォークからデプロイ
export GIT_REPO="https://github.com/YOUR_ORG/tuxedo-to-camel.git"
export GIT_BRANCH="main"

./deploy-all.sh
```

---

## デプロイ内容

### 作成されるNamespace

| Namespace | 用途 | 主なリソース |
|-----------|------|-------------|
| `demo-kafka` | Kafkaクラスター | Kafka, KafkaTopic |
| `demo-tuxedo-c` | Tuxedo C実装 | Deployment, Service, Route |
| `demo-camel` | Apache Camel統合 | Deployment |
| `demo-webui` | WebUI | Deployment, Service, Route |

### デプロイフロー

```
1. AMQ Streams Operator インストール (3分)
   └─> openshift-operators namespace

2. Kafka Cluster デプロイ (5分)
   └─> demo-kafka namespace
       ├─> Kafka (1 broker)
       ├─> Zookeeper (1 node)
       └─> KafkaTopic: demo-messages

3. Tuxedo C ビルド＆デプロイ (7-10分)
   └─> demo-tuxedo-c namespace
       ├─> BuildConfig: tuxedo-base (SDKイメージ)
       ├─> BuildConfig: tuxedo-msgsvc (アプリイメージ)
       ├─> Deployment: tuxedo-msgsvc
       ├─> Service: tuxedo-msgsvc:8080
       └─> Route: https://tuxedo-msgsvc-demo-tuxedo-c.apps...

4. Camel ビルド＆デプロイ (2-3分)
   └─> demo-camel namespace
       ├─> BuildConfig: camel-kafka-bridge
       └─> Deployment: camel-kafka-bridge

5. WebUI ビルド＆デプロイ (2-3分)
   └─> demo-webui namespace
       ├─> BuildConfig: webui
       ├─> Deployment: webui
       ├─> Service: webui:8080
       └─> Route: https://webui-demo-webui.apps...
```

**所要時間**: 約15-20分

---

## 使い方

### デプロイ実行

```bash
cd /path/to/tuxedo-to-camel/deploy-kit
./deploy-all.sh
```

### デプロイ完了後

スクリプトが完了すると、WebUI URLが表示されます：

```
🎉 Deployment Complete!

Access the demo at:
  🌐 WebUI: https://webui-demo-webui.apps.cluster-xxxxx.opentlc.com
```

### E2Eテスト

1. **WebUIにアクセス**
   ```bash
   # URLをブラウザで開く
   open https://webui-demo-webui.apps.cluster-xxxxx.opentlc.com
   ```

2. **メッセージ送信**
   - メッセージ入力欄に任意のテキストを入力
   - 「Tuxedoへ送信」ボタンをクリック
   - 成功メッセージを確認

3. **Kafkaでメッセージ確認**
   ```bash
   oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \
     --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 \
     -- bin/kafka-console-consumer.sh \
     --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
     --topic demo-messages \
     --from-beginning \
     --max-messages 10
   ```

### コンポーネントログ確認

```bash
# Tuxedo C
oc logs -n demo-tuxedo-c deployment/tuxedo-msgsvc --tail=20

# Camel
oc logs -n demo-camel deployment/camel-kafka-bridge --tail=20

# WebUI
oc logs -n demo-webui deployment/webui --tail=20
```

---

## トラブルシューティング

### ビルドが失敗する

**症状**: BuildConfigが Failed状態

**確認**:
```bash
# ビルドログ確認
oc logs -n demo-tuxedo-c build/tuxedo-base-1

# 再ビルド
oc start-build tuxedo-base -n demo-tuxedo-c --follow
```

**原因**:
- GitHubへのアクセス制限
- イメージレジストリの容量不足
- ネットワークタイムアウト

### Podが起動しない

**症状**: Pod が CrashLoopBackOff または ImagePullBackOff

**確認**:
```bash
# Pod状態確認
oc get pods -n demo-tuxedo-c
oc describe pod <pod-name> -n demo-tuxedo-c

# イベント確認
oc get events -n demo-tuxedo-c --sort-by='.lastTimestamp'
```

**対処**:
```bash
# イメージ再ビルド
oc start-build tuxedo-msgsvc -n demo-tuxedo-c --follow

# Deployment再起動
oc rollout restart deployment/tuxedo-msgsvc -n demo-tuxedo-c
```

### Operatorがインストールされない

**症状**: AMQ Streams Operator が Succeeded にならない

**確認**:
```bash
# CSV確認
oc get csv -n openshift-operators | grep amqstreams

# Subscription確認
oc get subscription amq-streams -n openshift-operators -o yaml
```

**対処**:
```bash
# 手動でOperator確認
oc get packagemanifests | grep amq-streams

# Subscriptionを削除して再作成
oc delete subscription amq-streams -n openshift-operators
# deploy-all.sh を再実行
```

### WebUIが表示されない

**症状**: 404 Not Found または Connection Refused

**確認**:
```bash
# Route確認
oc get route webui -n demo-webui

# Pod確認
oc get pods -n demo-webui
oc logs -n demo-webui deployment/webui
```

**対処**:
```bash
# Health check
oc exec -n demo-webui deployment/webui -- curl localhost:8080/q/health

# Rollout再起動
oc rollout restart deployment/webui -n demo-webui
```

---

## クリーンアップ

### 全リソース削除

```bash
./cleanup.sh
```

これにより以下が削除されます：
- ✅ demo-webui namespace
- ✅ demo-camel namespace
- ✅ demo-tuxedo-c namespace
- ✅ demo-kafka namespace
- ❓ AMQ Streams Operator (選択可能)

### 手動削除

```bash
# Namespace個別削除
oc delete namespace demo-webui
oc delete namespace demo-camel
oc delete namespace demo-tuxedo-c
oc delete namespace demo-kafka

# Operator削除
oc delete subscription amq-streams -n openshift-operators
oc delete csv -n openshift-operators -l operators.coreos.com/amq-streams.openshift-operators
```

---

## カスタマイズ

### Kafkaレプリカ数変更

`deploy-all.sh`の`deploy_kafka()`関数内を編集：

```yaml
spec:
  kafka:
    replicas: 3  # 1 → 3 に変更
```

### 独自イメージレジストリ使用

BuildConfig の `output.to` を変更：

```yaml
output:
  to:
    kind: DockerImage
    name: quay.io/your-org/tuxedo-msgsvc:latest
```

### リソース制限追加

Deployment に resources を追加：

```yaml
resources:
  limits:
    cpu: "1"
    memory: "1Gi"
  requests:
    cpu: "500m"
    memory: "512Mi"
```

---

## サブディレクトリ

```
deploy-kit/
├── deploy-all.sh           # 完全デプロイスクリプト
├── cleanup.sh              # クリーンアップスクリプト
├── README.md              # このファイル
├── tuxedo-q-c/            # Tuxedo C個別デプロイ用
└── manifests/             # Kubernetesマニフェスト（予備）
```

---

## サポート

### ドキュメント

- [QUICKSTART_LOCAL.md](../docs/QUICKSTART_LOCAL.md) - ローカル実行ガイド
- [E2E_TEST_GUIDE.md](../docs/E2E_TEST_GUIDE.md) - E2Eテストガイド
- [components/tuxedo/README_C_IMPLEMENTATION.md](../components/tuxedo/README_C_IMPLEMENTATION.md) - C実装詳細

### 問題報告

GitHub Issues: https://github.com/kamorisan/tuxedo-to-camel/issues

---

**Last Updated**: 2026-06-04
