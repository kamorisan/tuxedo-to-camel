# クイックスタートガイド

**5分で始める AMQ Broker + Kafka デプロイ**

---

## 🎯 このガイドの対象者

- OpenShiftクラスタへのアクセス権限がある方
- `oc` CLIの基本操作ができる方
- AMQ/Kafkaを素早くデプロイしたい方

---

## ⚡ 最速デプロイ（コピペで完了）

### Step 1: OpenShiftにログイン

```bash
oc login --token=<your-token> --server=https://<your-cluster>:6443
```

### Step 2: デプロイキットをダウンロード

```bash
git clone https://github.com/kamorisan/tuxedo-to-camel.git
cd tuxedo-to-camel/deploy-kit
```

### Step 3: AMQ Brokerデプロイ（1コマンド）

```bash
./scripts/deploy-amq.sh
```

**待ち時間**: 約2-3分

**デプロイされるもの:**
- ✅ AMQ Broker Operator
- ✅ Namespace: `demo-amq`
- ✅ AMQ Broker インスタンス
- ✅ Queue: `EXAMPLE.QUEUE`

### Step 4: Kafkaデプロイ（1コマンド）

```bash
./scripts/deploy-kafka.sh
```

**待ち時間**: 約3-5分

**デプロイされるもの:**
- ✅ AMQ Streams Operator
- ✅ AMQ Streams Console Operator
- ✅ Namespace: `demo-kafka`
- ✅ Kafka Cluster: `my-kafka` (KRaftモード)
- ✅ Kafka Console (Web UI)

---

## ✅ 動作確認

### AMQ Broker確認

```bash
# Pod確認
oc get pods -n demo-amq

# 期待結果:
# NAME              READY   STATUS    RESTARTS   AGE
# amq-broker-ss-0   1/1     Running   0          2m
```

### Kafka確認

```bash
# Pod確認
oc get pods -n demo-kafka

# 期待結果:
# NAME                                READY   STATUS    RESTARTS   AGE
# my-kafka-my-kafka-pool-0            1/1     Running   0          3m
# my-kafka-entity-operator-xxx        2/2     Running   0          2m
# my-kafka-console-xxx                2/2     Running   0          1m
```

### Kafka Console アクセス

```bash
# Console URL取得
oc get route -n demo-kafka | grep console

# ブラウザでアクセス
# https://kafka-console.apps.<your-cluster-domain>
```

---

## 🎨 カスタムデプロイ

### 異なるNamespaceにデプロイ

```bash
# AMQ: "production-amq" namespaceにデプロイ
./scripts/deploy-amq.sh production-amq

# Kafka: "production-kafka" namespaceにデプロイ
./scripts/deploy-kafka.sh production-kafka
```

### カスタムQueue名

```bash
# "ORDER.QUEUE" というQueueを作成
./scripts/deploy-amq.sh demo-amq ORDER.QUEUE
```

### カスタムKafka Cluster名

```bash
# "production-cluster" という名前でデプロイ
./scripts/deploy-kafka.sh demo-kafka production-cluster
```

---

## 🧪 簡単な動作テスト

### AMQ: メッセージ送受信

```bash
# メッセージ送信
oc exec -n demo-amq amq-broker-ss-0 -- \
  /home/jboss/amq-broker/bin/artemis producer \
  --destination EXAMPLE.QUEUE \
  --message-count 5 \
  --message "Hello AMQ" \
  --url tcp://localhost:61616

# メッセージ受信
oc exec -n demo-amq amq-broker-ss-0 -- \
  /home/jboss/amq-broker/bin/artemis consumer \
  --destination EXAMPLE.QUEUE \
  --message-count 5 \
  --url tcp://localhost:61616
```

### Kafka: メッセージ送受信

```bash
# Topic作成
oc exec -n demo-kafka my-kafka-my-kafka-pool-0 -- \
  bin/kafka-topics.sh \
  --bootstrap-server localhost:9092 \
  --create \
  --topic test-topic \
  --partitions 3 \
  --replication-factor 1

# メッセージ送信（対話モード）
oc exec -it -n demo-kafka my-kafka-my-kafka-pool-0 -- \
  bin/kafka-console-producer.sh \
  --bootstrap-server localhost:9092 \
  --topic test-topic
# > Hello Kafka
# > Message 2
# > (Ctrl+D で終了)

# メッセージ受信
oc exec -n demo-kafka my-kafka-my-kafka-pool-0 -- \
  bin/kafka-console-consumer.sh \
  --bootstrap-server localhost:9092 \
  --topic test-topic \
  --from-beginning \
  --timeout-ms 5000
```

---

## 📊 リソース使用状況

### デフォルト設定でのリソース

| コンポーネント | CPU Request | Memory Request | CPU Limit | Memory Limit |
|-------------|-------------|----------------|-----------|--------------|
| AMQ Broker  | ~500m       | ~512Mi         | ~1000m    | ~1Gi         |
| Kafka Broker| ~500m       | ~1Gi           | ~1000m    | ~2Gi         |
| Kafka Console| ~250m      | ~256Mi         | ~500m     | ~512Mi       |

**最小クラスタ要件:**
- Worker Nodes: 2台以上
- 合計CPU: 4 vCPU以上推奨
- 合計Memory: 8GB以上推奨

---

## 🔧 トラブルシューティング

### "command not found: oc"

**原因:** OpenShift CLIがインストールされていない

**対処法:**
```bash
# macOS
brew install openshift-cli

# Linux
wget https://mirror.openshift.com/pub/openshift-v4/clients/ocp/latest/openshift-client-linux.tar.gz
tar -xvf openshift-client-linux.tar.gz
sudo mv oc kubectl /usr/local/bin/
```

---

### "Error: Not logged in to OpenShift"

**原因:** OpenShiftクラスタにログインしていない

**対処法:**
```bash
# OpenShift Webコンソールから
# 右上のユーザー名 → "Copy login command" → トークンをコピー

oc login --token=<token> --server=https://<cluster>:6443
```

---

### Operatorがインストールされない

**確認:**
```bash
oc get csv -n openshift-operators | grep -E "amq-broker|amq-streams"
```

**対処法:**
- 30秒待ってから再確認
- Subscription状態確認: `oc get subscription -n openshift-operators`
- cluster-admin権限があるか確認

---

### PodがPendingのまま

**確認:**
```bash
oc describe pod <pod-name> -n <namespace>
```

**よくある原因:**
- Worker Nodeのリソース不足
- PVC用のStorageClassが存在しない
- ImagePullエラー

**対処法:**
```bash
# リソース確認
oc describe nodes

# StorageClass確認
oc get storageclass
```

---

## 🗑️ クリーンアップ（削除）

### 全リソース削除

```bash
# AMQ削除
oc delete activemqartemis --all -n demo-amq
oc delete namespace demo-amq

# Kafka削除
oc delete console --all -n demo-kafka
oc delete kafka --all -n demo-kafka
oc delete namespace demo-kafka
```

### Operator削除（他のプロジェクトで使用していない場合のみ）

```bash
# AMQ Broker Operator削除
oc delete subscription amq-broker-rhel8 -n openshift-operators
oc delete csv -n openshift-operators $(oc get csv -n openshift-operators | grep amq-broker | awk '{print $1}')

# AMQ Streams Operator削除
oc delete subscription amq-streams -n openshift-operators
oc delete subscription amq-streams-console -n openshift-operators
oc delete csv -n openshift-operators $(oc get csv -n openshift-operators | grep amqstreams | awk '{print $1}')
oc delete csv -n openshift-operators $(oc get csv -n openshift-operators | grep amq-streams-console | awk '{print $1}')
```

---

## 📚 次のステップ

1. **詳細ドキュメント**: [README.md](README.md) を参照
2. **カスタマイズ**: マニフェストファイルを編集してレプリカ数や永続化を設定
3. **統合**: AMQとKafkaを連携させる（Apache Camelなど）
4. **モニタリング**: PrometheusとGrafanaでメトリクス監視

---

## 🆘 ヘルプ

問題が解決しない場合:

```bash
# 全Pod状態確認
oc get pods --all-namespaces | grep -E "demo-amq|demo-kafka"

# Operator状態確認
oc get csv -n openshift-operators

# イベント確認
oc get events --all-namespaces --sort-by='.lastTimestamp' | tail -20
```

詳細は [README.md](README.md) のトラブルシューティングセクションを参照してください。

---

**所要時間まとめ:**
- OpenShiftログイン: 1分
- デプロイキットダウンロード: 1分
- AMQデプロイ: 2-3分
- Kafkaデプロイ: 3-5分
- **合計: 約7-10分**

**Happy Deploying! 🚀**
