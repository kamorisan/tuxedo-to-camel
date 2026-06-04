# Deployment Kit Index

## 📂 ファイル一覧

### ドキュメント
- [README.md](README.md) - 詳細なデプロイガイド（推奨）
- [QUICKSTART.md](QUICKSTART.md) - 5分で始めるクイックスタート
- [INDEX.md](INDEX.md) - このファイル

### AMQ Broker (`amq/`)
| ファイル | 説明 | プレースホルダー |
|---------|------|----------------|
| `00-operator.yaml` | AMQ Broker Operator Subscription | なし |
| `01-namespace.yaml` | Namespace定義 | `NAMESPACE_PLACEHOLDER` |
| `02-broker.yaml` | AMQ Broker インスタンス | `NAMESPACE_PLACEHOLDER` |
| `03-address-example.yaml` | Queue/Address サンプル | `NAMESPACE_PLACEHOLDER`, `EXAMPLE.QUEUE` |

### Kafka (`kafka/`)
| ファイル | 説明 | プレースホルダー |
|---------|------|----------------|
| `00-operator.yaml` | AMQ Streams Operator Subscription | なし |
| `01-console-operator.yaml` | AMQ Streams Console Operator | なし |
| `02-namespace.yaml` | Namespace定義 | `NAMESPACE_PLACEHOLDER` |
| `03-kafka-cluster.yaml` | Kafka Cluster + KafkaNodePool | `NAMESPACE_PLACEHOLDER`, `KAFKA_CLUSTER_NAME` |
| `04-topic-example.yaml` | Kafka Topic サンプル | `NAMESPACE_PLACEHOLDER`, `KAFKA_CLUSTER_NAME` |
| `05-console.yaml` | Kafka Console | `NAMESPACE_PLACEHOLDER`, `KAFKA_CLUSTER_NAME`, `CONSOLE_HOSTNAME_PLACEHOLDER` |

### スクリプト (`scripts/`)
| ファイル | 説明 | 使用方法 |
|---------|------|---------|
| `deploy-amq.sh` | AMQ 自動デプロイスクリプト | `./deploy-amq.sh [namespace] [queue-name]` |
| `deploy-kafka.sh` | Kafka 自動デプロイスクリプト | `./deploy-kafka.sh [namespace] [cluster-name] [console-hostname]` |

## 🚀 使い方

### 最も簡単な方法（推奨）

```bash
cd deploy-kit/scripts

# AMQデプロイ
./deploy-amq.sh

# Kafkaデプロイ
./deploy-kafka.sh
```

### プレースホルダー置換パターン

```bash
# 基本パターン
sed 's/NAMESPACE_PLACEHOLDER/my-namespace/g' file.yaml | oc apply -f -

# 複数置換
sed -e 's/NAMESPACE_PLACEHOLDER/my-namespace/g' \
    -e 's/KAFKA_CLUSTER_NAME/my-cluster/g' \
    file.yaml | oc apply -f -
```

## 📋 プレースホルダー一覧

| プレースホルダー | 説明 | 例 |
|---------------|------|---|
| `NAMESPACE_PLACEHOLDER` | デプロイ先のNamespace | `demo-amq`, `production-kafka` |
| `KAFKA_CLUSTER_NAME` | Kafkaクラスター名 | `my-kafka`, `prod-cluster` |
| `CONSOLE_HOSTNAME_PLACEHOLDER` | Kafka Console URL | `kafka-console.apps.example.com` |
| `EXAMPLE.QUEUE` | AMQ Queue名 | `ORDER.QUEUE`, `MESSAGE.IN` |

## 🎯 デプロイパターン別ガイド

### パターン1: デフォルト設定
```bash
./scripts/deploy-amq.sh          # demo-amq namespace
./scripts/deploy-kafka.sh        # demo-kafka namespace
```

### パターン2: カスタムNamespace
```bash
./scripts/deploy-amq.sh my-amq-ns
./scripts/deploy-kafka.sh my-kafka-ns
```

### パターン3: 完全カスタム
```bash
./scripts/deploy-amq.sh production-amq ORDER.PROCESSING
./scripts/deploy-kafka.sh production-kafka prod-cluster kafka-ui.example.com
```

### パターン4: 手動デプロイ（マニフェスト直接）
```bash
# AMQ
sed 's/NAMESPACE_PLACEHOLDER/my-ns/g' amq/01-namespace.yaml | oc apply -f -
sed 's/NAMESPACE_PLACEHOLDER/my-ns/g' amq/02-broker.yaml | oc apply -f -

# Kafka
sed 's/NAMESPACE_PLACEHOLDER/my-ns/g' kafka/02-namespace.yaml | oc apply -f -
sed -e 's/NAMESPACE_PLACEHOLDER/my-ns/g' -e 's/KAFKA_CLUSTER_NAME/my-cluster/g' \
    kafka/03-kafka-cluster.yaml | oc apply -f -
```

## 📊 デプロイ時間目安

| コンポーネント | Operator | インスタンス | 合計 |
|-------------|----------|-------------|------|
| AMQ Broker  | 30-60秒  | 1-2分       | 2-3分 |
| Kafka       | 30-60秒  | 2-3分       | 3-5分 |
| Kafka Console | 含む   | 1-2分       | 含む |

## ✅ デプロイ後の確認コマンド

```bash
# AMQ
oc get pods -n <namespace>
oc get svc -n <namespace>
oc get activemqartemis -n <namespace>

# Kafka
oc get pods -n <namespace>
oc get kafka -n <namespace>
oc get kafkatopic -n <namespace>
oc get route -n <namespace>
```

## 🗑️ クリーンアップ

```bash
# リソースのみ削除（Operatorは残す）
oc delete namespace <namespace>

# Operatorも削除
oc delete subscription <operator-name> -n openshift-operators
```

## 📞 サポートリソース

- **クイックスタート**: [QUICKSTART.md](QUICKSTART.md)
- **詳細ガイド**: [README.md](README.md)
- **プロジェクトリポジトリ**: https://github.com/kamorisan/tuxedo-to-camel
- **Red Hatドキュメント**: 
  - [AMQ Broker](https://access.redhat.com/documentation/en-us/red_hat_amq_broker/)
  - [AMQ Streams](https://access.redhat.com/documentation/en-us/red_hat_amq_streams/)

---

**バージョン**: 1.0.0  
**最終更新**: 2026-06-04
