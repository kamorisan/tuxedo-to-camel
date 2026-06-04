# E2E統合テストガイド

OpenShift上でTuxedo C実装の完全な統合テストを実施するガイドです。

---

## テスト環境

### デプロイ済みコンポーネント

| Namespace | Component | Status | URL |
|-----------|-----------|--------|-----|
| demo-webui | WebUI (Quarkus) | ✅ Running | https://webui-demo-webui.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com |
| demo-tuxedo-c | Tuxedo C Service | ✅ Running | https://tuxedo-msgsvc-demo-tuxedo-c.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com |
| demo-camel | Camel Integration | ✅ Running | Internal only |
| demo-kafka | Kafka Cluster | ✅ Running | Internal only |

### データフロー

```
Web UI → Tuxedo Service → Tuxedo /Q → Apache Camel → Kafka Broker
```

---

## テスト手順

### Step 1: WebUIアクセス

ブラウザで以下のURLを開く：

```
https://webui-demo-webui.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com
```

**期待結果:**
- タイトル: "Tuxedo Message Sender"
- サブタイトル: "Web → Tuxedo → AMQ → Camel → Kafka デモシステム"
- フロー図: `Web UI → Tuxedo Service → Tuxedo /Q → Apache Camel → Kafka Broker`

---

### Step 2: メッセージ送信

WebUIからメッセージを送信：

1. メッセージ入力欄に任意のテキストを入力
   - 例: `E2E Test from WebUI - 2026-06-04`

2. **「Tuxedoへ送信」** ボタンをクリック

**期待結果:**
- ボタンが「送信中...」に変わる
- 数秒後に成功メッセージが表示される
- 緑色の枠で「Success」と表示される

---

### Step 3: Tuxedo Cログ確認

ターミナルでTuxedo Cのログを確認：

```bash
oc logs -n demo-tuxedo-c deployment/tuxedo-msgsvc --tail=20
```

**期待結果:**
```
[HTTP] POST /MSGSVC - Received message
[Tuxedo-Q] Enqueued message to DEMO_QUEUE
[HTTP] Response: {"status":"success"...}
```

---

### Step 4: Camelログ確認

Camelがキューから取得してKafkaへ送信していることを確認：

```bash
oc logs -n demo-camel deployment/camel-kafka-bridge --tail=20
```

**期待結果:**
```
[Camel] Sending to Kafka: {"timestamp":"...","message":"E2E Test from WebUI - 2026-06-04","source":"tuxedo-q","queue":"DEMO_QUEUE","id":"..."}
[Camel] Sent to Kafka topic: demo-messages
```

---

### Step 5: Kafka確認

Kafkaトピックにメッセージが到達していることを確認：

```bash
# Kafka Consoleを起動
oc run kafka-consumer -n demo-kafka -it --rm --restart=Never \
  --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 \
  -- bin/kafka-console-consumer.sh \
  --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
  --topic demo-messages \
  --from-beginning \
  --max-messages 10
```

**期待結果:**
```json
{
  "id": "UUID",
  "timestamp": "2026-06-04T08:XX:XX.XXXZ",
  "message": "E2E Test from WebUI - 2026-06-04",
  "source": "tuxedo-q",
  "queue": "DEMO_QUEUE"
}
```

---

## 自動テストスクリプト

すべてのステップを自動化したスクリプト：

```bash
#!/bin/bash
# E2E Integration Test - Automated

WEBUI_URL="https://webui-demo-webui.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com"
TEST_MESSAGE="Automated E2E Test - $(date +%Y-%m-%d_%H:%M:%S)"

echo "=========================================="
echo "E2E Integration Test"
echo "=========================================="
echo ""

# Step 1: Send message via WebUI API
echo "Step 1: Sending message via WebUI..."
RESPONSE=$(curl -s -X POST "$WEBUI_URL/api/send" \
  -H "Content-Type: text/plain" \
  -d "$TEST_MESSAGE")
echo "$RESPONSE" | jq .
echo ""

# Step 2: Check Tuxedo C logs
echo "Step 2: Checking Tuxedo C logs..."
oc logs -n demo-tuxedo-c deployment/tuxedo-msgsvc --tail=5 | grep -A2 "POST /MSGSVC"
echo ""

# Step 3: Wait for Camel polling (max 5 seconds)
echo "Step 3: Waiting for Camel to poll (5s)..."
sleep 5

# Step 4: Check Camel logs
echo "Step 4: Checking Camel logs..."
oc logs -n demo-camel deployment/camel-kafka-bridge --tail=10 | grep "Sent to Kafka"
echo ""

# Step 5: Check Kafka topic
echo "Step 5: Checking Kafka topic (last 3 messages)..."
oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \
  --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 \
  -- bin/kafka-console-consumer.sh \
  --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
  --topic demo-messages \
  --from-beginning \
  --max-messages 3 \
  --timeout-ms 10000 2>/dev/null | tail -3 | jq .

echo ""
echo "=========================================="
echo "E2E Test Complete!"
echo "=========================================="
```

このスクリプトを保存して実行：

```bash
chmod +x e2e-test.sh
./e2e-test.sh
```

---

## トラブルシューティング

### WebUIが表示されない

```bash
# Pod状態確認
oc get pods -n demo-webui

# ログ確認
oc logs -n demo-webui deployment/webui
```

### メッセージが送信できない

```bash
# Tuxedo C health check
curl https://tuxedo-msgsvc-demo-tuxedo-c.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com/health

# WebUI環境変数確認
oc get deployment webui -n demo-webui -o jsonpath='{.spec.template.spec.containers[0].env}'
```

### Camelがポーリングしない

```bash
# Camelログリアルタイム表示
oc logs -n demo-camel deployment/camel-kafka-bridge -f

# Camel Route確認
oc exec -n demo-camel deployment/camel-kafka-bridge -- curl localhost:8080/q/health
```

### Kafkaにメッセージが届かない

```bash
# Kafka topic確認
oc get kafkatopic -n demo-kafka

# Kafka broker確認
oc get pods -n demo-kafka | grep kafka
```

---

## 成功基準

すべての条件を満たせばE2Eテスト成功です：

- ✅ WebUIからメッセージ送信成功
- ✅ Tuxedo Cでメッセージ受信・enqueue成功
- ✅ Camelでdequeue・変換成功
- ✅ Kafkaトピックにメッセージ到達成功

---

## 次のステップ

### パフォーマンステスト

```bash
# 100メッセージ連続送信
for i in {1..100}; do
  curl -s -X POST "$WEBUI_URL/api/send" \
    -H "Content-Type: text/plain" \
    -d "Load Test Message $i"
  sleep 0.1
done
```

### 負荷テスト

```bash
# Apache Benchで負荷テスト
ab -n 1000 -c 10 -p /dev/null -T text/plain \
  "$WEBUI_URL/api/send"
```

---

お疲れ様でした！完全なE2E統合が動作しています 🎉
