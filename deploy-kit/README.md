# OpenShift Deployment Kit

**AMQ Broker**, **Kafka (AMQ Streams)**, **Kafka Console**を別環境のOpenShiftに簡単にデプロイするためのキットです。

---

## 📋 前提条件

- OpenShift 4.x クラスタへのアクセス
- `oc` CLI ツール (OpenShift CLI)
- cluster-admin または適切な権限

### 確認コマンド

```bash
# OpenShift CLIバージョン確認
oc version

# ログイン確認
oc whoami

# クラスタ情報確認
oc cluster-info
```

---

## 🚀 クイックスタート

### 1. AMQ Brokerのデプロイ

```bash
cd deploy-kit/scripts

# デフォルト設定でデプロイ (namespace: demo-amq, queue: EXAMPLE.QUEUE)
./deploy-amq.sh

# カスタム設定でデプロイ
./deploy-amq.sh my-namespace MY.CUSTOM.QUEUE
```

**実行内容:**
1. AMQ Broker Operatorインストール
2. Namespace作成
3. AMQ Broker インスタンスデプロイ
4. Queueアドレス作成

**デプロイ時間**: 約2-3分

---

### 2. Kafka (AMQ Streams) + Consoleのデプロイ

```bash
cd deploy-kit/scripts

# デフォルト設定でデプロイ (namespace: demo-kafka, cluster: my-kafka)
./deploy-kafka.sh

# カスタム設定でデプロイ
./deploy-kafka.sh my-namespace my-cluster-name

# Console hostnameを明示的に指定
./deploy-kafka.sh my-namespace my-cluster-name kafka-ui.apps.mycluster.example.com
```

**実行内容:**
1. AMQ Streams Operatorインストール
2. AMQ Streams Console Operatorインストール
3. Namespace作成
4. Kafka クラスターデプロイ (KRaftモード)
5. Kafka Consoleデプロイ

**デプロイ時間**: 約3-5分

---

## 📁 ディレクトリ構成

```
deploy-kit/
├── README.md                    # このファイル
├── amq/                         # AMQ Broker マニフェスト
│   ├── 00-operator.yaml         # Operator Subscription
│   ├── 01-namespace.yaml        # Namespace (プレースホルダー)
│   ├── 02-broker.yaml           # AMQ Broker インスタンス
│   └── 03-address-example.yaml  # Queue/Address サンプル
├── kafka/                       # Kafka マニフェスト
│   ├── 00-operator.yaml         # AMQ Streams Operator
│   ├── 01-console-operator.yaml # Console Operator
│   ├── 02-namespace.yaml        # Namespace (プレースホルダー)
│   ├── 03-kafka-cluster.yaml    # Kafka Cluster + NodePool
│   ├── 04-topic-example.yaml    # Topic サンプル
│   └── 05-console.yaml          # Kafka Console
└── scripts/                     # デプロイスクリプト
    ├── deploy-amq.sh            # AMQ 自動デプロイ
    └── deploy-kafka.sh          # Kafka 自動デプロイ
```

---

## 🔧 手動デプロイ（マニフェスト直接適用）

自動スクリプトを使わず、手動でデプロイする場合の手順です。

### AMQ Brokerの手動デプロイ

```bash
# 1. Operatorインストール
oc apply -f amq/00-operator.yaml

# 2. Operatorインストール確認（Succeededになるまで待つ）
oc get csv -n openshift-operators | grep amq-broker

# 3. Namespace作成（プレースホルダーを置換）
sed 's/NAMESPACE_PLACEHOLDER/my-amq-namespace/g' amq/01-namespace.yaml | oc apply -f -

# 4. Broker デプロイ
sed 's/NAMESPACE_PLACEHOLDER/my-amq-namespace/g' amq/02-broker.yaml | oc apply -f -

# 5. Queue作成（オプション）
sed -e 's/NAMESPACE_PLACEHOLDER/my-amq-namespace/g' \
    -e 's/EXAMPLE.QUEUE/MY.QUEUE.NAME/g' \
    amq/03-address-example.yaml | oc apply -f -

# 6. 確認
oc get pods -n my-amq-namespace
oc get svc -n my-amq-namespace
```

---

### Kafkaの手動デプロイ

```bash
# 1. AMQ Streams Operatorインストール
oc apply -f kafka/00-operator.yaml
oc apply -f kafka/01-console-operator.yaml

# 2. Operatorインストール確認
oc get csv -n openshift-operators | grep amq-streams

# 3. Namespace作成
sed 's/NAMESPACE_PLACEHOLDER/my-kafka-namespace/g' kafka/02-namespace.yaml | oc apply -f -

# 4. Kafka Cluster デプロイ
sed -e 's/NAMESPACE_PLACEHOLDER/my-kafka-namespace/g' \
    -e 's/KAFKA_CLUSTER_NAME/my-cluster/g' \
    kafka/03-kafka-cluster.yaml | oc apply -f -

# 5. Kafka Console デプロイ (クラスタドメインを確認して置換)
CLUSTER_DOMAIN=$(oc whoami --show-console | sed 's|https://console-openshift-console.apps.||' | sed 's|/||')
sed -e 's/NAMESPACE_PLACEHOLDER/my-kafka-namespace/g' \
    -e 's/KAFKA_CLUSTER_NAME/my-cluster/g' \
    -e "s/CONSOLE_HOSTNAME_PLACEHOLDER/kafka-console.apps.$CLUSTER_DOMAIN/g" \
    kafka/05-console.yaml | oc apply -f -

# 6. Topicサンプル作成（オプション）
sed -e 's/NAMESPACE_PLACEHOLDER/my-kafka-namespace/g' \
    -e 's/KAFKA_CLUSTER_NAME/my-cluster/g' \
    kafka/04-topic-example.yaml | oc apply -f -

# 7. 確認
oc get kafka -n my-kafka-namespace
oc get pods -n my-kafka-namespace
oc get route -n my-kafka-namespace
```

---

## ✅ デプロイ確認

### AMQ Broker

```bash
# Pod状態確認
oc get pods -n <namespace>
# 期待結果: amq-broker-ss-0 が Running

# Service確認
oc get svc -n <namespace>
# AMQP: amq-broker-amqp-0-svc:5672
# Core: amq-broker-core-0-svc:61616

# Queue確認（Pod内から）
oc exec -n <namespace> amq-broker-ss-0 -- \
  /home/jboss/amq-broker/bin/artemis queue stat --url tcp://localhost:61616

# Console Route (デプロイされている場合)
oc get route -n <namespace>
```

---

### Kafka

```bash
# Kafka Cluster状態確認
oc get kafka -n <namespace>
# 期待結果: STATUS が Ready

# Pod状態確認
oc get pods -n <namespace>
# 期待結果:
#   - <cluster>-<pool>-0: Running (Broker Pod)
#   - <cluster>-entity-operator-xxx: Running (2/2)
#   - <cluster>-console-xxx: Running (2/2)

# Topic確認
oc get kafkatopic -n <namespace>

# Console URL確認
oc get route -n <namespace> | grep console

# Kafkaログ確認
oc logs -n <namespace> <cluster>-<pool>-0
```

---

## 📝 カスタマイズ

### AMQ Brokerのカスタマイズ

[amq/02-broker.yaml](amq/02-broker.yaml)を編集:

```yaml
spec:
  deploymentPlan:
    size: 3                        # ブローカー数（デフォルト: 1）
    persistenceEnabled: true       # 永続化（デフォルト: false）
  acceptors:
    - name: amqp
      protocols: amqp
      port: 5672
      sslEnabled: true             # TLS有効化（デフォルト: false）
```

---

### Kafkaのカスタマイズ

[kafka/03-kafka-cluster.yaml](kafka/03-kafka-cluster.yaml)を編集:

```yaml
# KafkaNodePool
spec:
  replicas: 3                      # ブローカー数（デフォルト: 1）
  storage:
    type: persistent-claim         # 永続化（デフォルト: ephemeral）
    size: 100Gi

# Kafka
spec:
  kafka:
    config:
      offsets.topic.replication.factor: 3    # レプリケーション数
      min.insync.replicas: 2
```

---

## 🧪 動作テスト

### AMQ Brokerテスト

```bash
# Producer（メッセージ送信）
oc exec -n <namespace> amq-broker-ss-0 -- \
  /home/jboss/amq-broker/bin/artemis producer \
  --destination <queue-name> \
  --message-count 10 \
  --message "Test message" \
  --url tcp://localhost:61616

# Consumer（メッセージ受信）
oc exec -n <namespace> amq-broker-ss-0 -- \
  /home/jboss/amq-broker/bin/artemis consumer \
  --destination <queue-name> \
  --message-count 10 \
  --url tcp://localhost:61616
```

---

### Kafkaテスト

```bash
# Producer（メッセージ送信）
oc exec -n <namespace> <cluster>-<pool>-0 -- \
  bin/kafka-console-producer.sh \
  --bootstrap-server localhost:9092 \
  --topic <topic-name>
# （メッセージ入力後、Ctrl+D で終了）

# Consumer（メッセージ受信）
oc exec -n <namespace> <cluster>-<pool>-0 -- \
  bin/kafka-console-consumer.sh \
  --bootstrap-server localhost:9092 \
  --topic <topic-name> \
  --from-beginning \
  --timeout-ms 10000
```

---

## 🗑️ クリーンアップ

### AMQ Broker削除

```bash
# リソース削除
oc delete activemqartemis amq-broker -n <namespace>
oc delete activemqartemisaddress --all -n <namespace>
oc delete namespace <namespace>

# Operator削除（必要な場合のみ）
oc delete subscription amq-broker-rhel8 -n openshift-operators
oc delete csv -n openshift-operators -l operators.coreos.com/amq-broker-rhel8.openshift-operators
```

---

### Kafka削除

```bash
# リソース削除
oc delete console --all -n <namespace>
oc delete kafkatopic --all -n <namespace>
oc delete kafka --all -n <namespace>
oc delete kafkanodepool --all -n <namespace>
oc delete namespace <namespace>

# Operator削除（必要な場合のみ）
oc delete subscription amq-streams -n openshift-operators
oc delete subscription amq-streams-console -n openshift-operators
oc delete csv -n openshift-operators -l operators.coreos.com/amq-streams.openshift-operators
oc delete csv -n openshift-operators -l operators.coreos.com/amq-streams-console.openshift-operators
```

---

## 📚 参考リソース

### Red Hat 公式ドキュメント

- [AMQ Broker on OpenShift](https://access.redhat.com/documentation/en-us/red_hat_amq_broker/)
- [AMQ Streams (Kafka) on OpenShift](https://access.redhat.com/documentation/en-us/red_hat_amq_streams/)
- [OpenShift Container Platform](https://docs.openshift.com/)

### プロジェクトリソース

- **プロジェクトリポジトリ**: [tuxedo-to-camel](https://github.com/kamorisan/tuxedo-to-camel)
- **進捗ドキュメント**: [PROGRESS_20260604.md](../PROGRESS_20260604.md)
- **Tuxedoデプロイガイド**: [TUXEDO_DEPLOYMENT_NOTES.md](../TUXEDO_DEPLOYMENT_NOTES.md)

---

## ❓ トラブルシューティング

### Operatorがインストールされない

```bash
# Subscription状態確認
oc get subscription -n openshift-operators

# InstallPlan確認
oc get installplan -n openshift-operators

# Pod確認（Operatorが起動しているか）
oc get pods -n openshift-operators | grep amq
```

**対処法:**
- Subscription の `installPlanApproval: Manual` になっている場合、手動承認が必要
- CatalogSource の状態を確認: `oc get catalogsource -n openshift-marketplace`

---

### PodがPendingのまま

```bash
# Pod詳細確認
oc describe pod <pod-name> -n <namespace>

# Events確認
oc get events -n <namespace> --sort-by='.lastTimestamp'
```

**よくある原因:**
- リソース不足（CPU/Memory）
- PVC作成失敗（StorageClassが存在しない）
- ImagePullBackOff（イメージレジストリ認証）

---

### Kafka Consoleにアクセスできない

```bash
# Route確認
oc get route -n <namespace>

# Console Pod確認
oc get pods -n <namespace> | grep console

# Console ログ確認
oc logs -n <namespace> <console-pod-name>
```

**対処法:**
- Routeのhostnameが正しいか確認
- Console Operatorが正常にインストールされているか確認
- Kafkaクラスター名とlistener名が一致しているか確認

---

## 📞 サポート

問題が解決しない場合:
1. プロジェクトのIssueトラッカーを確認
2. OpenShiftクラスター管理者に相談
3. Red Hatサポートに問い合わせ（サブスクリプション契約者）

---

**最終更新**: 2026-06-04  
**バージョン**: 1.0.0  
**作成者**: Tuxedo-to-Camel Demo Project
