# Tuxedo to Camel Demo - アーキテクチャ設計

## 1. 概要

本デモは、WebアプリケーションからTuxedoを経由してApache CamelでメッセージをKafkaへ送信するまでの流れを可視化するシステムです。

### 目的
- レガシーシステム（Tuxedo）からモダンなメッセージング基盤（Kafka）への移行パターンを実演
- OpenShift上でのコンテナベースの統合パターンを示す
- 各コンポーネント間の疎結合な連携を実現

## 2. システム構成図

```
┌─────────────────────────────────────────────────────────────────────┐
│                          OpenShift Cluster                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────┐      ┌──────────────┐      ┌──────────────┐     │
│  │ Namespace:   │      │ Namespace:   │      │ Namespace:   │     │
│  │ demo-webui   │      │ demo-tuxedo  │      │ demo-camel   │     │
│  ├──────────────┤      ├──────────────┤      ├──────────────┤     │
│  │              │      │              │      │              │     │
│  │  Quarkus     │ HTTP │  Tuxedo      │ AMQ  │  Apache      │     │
│  │  Web App ────┼─────>│  Service ────┼─────>│  Camel   ────┼──┐  │
│  │              │      │              │      │              │  │  │
│  │  (入力画面)   │      │  (メッセージ  │      │  (変換・     │  │  │
│  │              │      │   中継)      │      │   ルーティング)│  │  │
│  └──────────────┘      └──────────────┘      └──────────────┘  │  │
│                                                                 │  │
│  ┌──────────────────────────────────────────────────────────────┘  │
│  │                                                                 │
│  │  ┌──────────────┐                                              │
│  │  │ Namespace:   │                                              │
│  │  │ demo-kafka   │                                              │
│  │  ├──────────────┤                                              │
│  │  │              │                                              │
│  └─>│  Kafka       │                                              │
│     │  Broker      │                                              │
│     │              │                                              │
│     │  (メッセージ  │                                              │
│     │   永続化)    │                                              │
│     └──────────────┘                                              │
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ Tuxedo /Q (Tuxedo Queue) - Shared Service                  │  │
│  │ - demo-tuxedo と demo-camel 間で使用                         │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## 3. メッセージフロー

### 3.1 データフロー

```
[ユーザー入力]
    │
    │ 1. HTTP POST (平文メッセージ)
    ▼
[Quarkus Web UI]
    │
    │ 2. HTTP POST /api/send
    │    Body: "Hello World!"
    ▼
[Tuxedo Service]
    │
    │ 3. Tuxedo /Q へ送信
    │    Queue: TUXEDO.OUT
    │    Message: "Hello World!"
    ▼
[Tuxedo /Q]
    │
    │ 4. Queue から取得
    ▼
[Apache Camel]
    │
    │ 5. メッセージ変換
    │    JSON構造化:
    │    {
    │      "id": "uuid",
    │      "timestamp": "ISO8601",
    │      "message": "Hello World!"
    │    }
    ▼
[Kafka Broker]
    │
    │ 6. Topic: demo-messages
    ▼
[Kafka Console] (確認用)
```

### 3.2 処理フェーズ

| フェーズ | コンポーネント | 処理内容 | ログ出力 |
|---------|--------------|---------|---------|
| 入力 | Quarkus WebUI | ユーザーからメッセージ受信 | `[WebUI] Received: {message}` |
| 転送 | Quarkus WebUI | TuxedoへHTTP POST | `[WebUI] Sent to Tuxedo: {message}` |
| 受信 | Tuxedo Service | メッセージ受信・検証 | `[Tuxedo] Received: {message}` |
| キュー送信 | Tuxedo Service | Tuxedo /Qキューへ送信 | `[Tuxedo] Queued to /Q: {message}` |
| キュー受信 | Apache Camel | Tuxedo /Qキューから取得 | `[Camel] Consumed from queue: {message}` |
| 変換 | Apache Camel | JSON構造化 | `[Camel] Transformed: {json}` |
| 送信 | Apache Camel | Kafkaへ送信 | `[Camel] Sent to Kafka topic: {topic}` |
| 永続化 | Kafka | メッセージ保存 | `[Kafka] Stored message offset: {offset}` |

## 4. コンポーネント詳細

### 4.1 Quarkus Web UI (demo-webui)

**技術スタック:**
- Quarkus 3.x
- RESTEasy Reactive
- Qute Template Engine (シンプルなHTML UI)

**機能:**
- メッセージ入力フォーム
- Tuxedoへの送信ボタン
- 送信結果の表示

**エンドポイント:**
- `GET /` - 入力画面
- `POST /api/send` - メッセージ送信

**コンテナイメージ:**
- ベース: `registry.access.redhat.com/ubi9/openjdk-17`
- ビルド: Maven / Quarkus Native (オプション)

### 4.2 Tuxedo Service (demo-tuxedo)

**技術スタック:**
- C言語
- Oracle Tuxedo フレームワーク
- Tuxedo /Q (QSPACE)

**機能:**
- Tuxedoサービスとしてメッセージ受信
- ビジネスロジック処理（ログ記録）
- Tuxedo /Qキューへメッセージ送信

**Tuxedoサービス:**
- サービス名: `MSGSVC`
- バッファタイプ: STRING

**Tuxedo /Q設定:**
- Queue Space: `QSPACE`
- Queue Name: `TUXEDO.OUT`
- Protocol: ATMI (tpenqueue/tpdequeue)

**コンテナイメージ:**
- ベース: Oracle Tuxedo Runtime Container
- または UBI9 + Tuxedo Runtime

### 4.3 Apache Camel (demo-camel)

**技術スタック:**
- Apache Camel 4.x
- Camel Quarkus
- Tuxedo /Q Integration
- Camel Kafka Component

**機能:**
- Tuxedo /Qキューからメッセージ消費
- メッセージ変換（平文 → JSON）
- Kafkaトピックへ送信

**Camel Route:**
```java
from("tuxedo:queue:TUXEDO.OUT")
    .log("[Camel] Consumed from queue: ${body}")
    .process(exchange -> {
        String message = exchange.getIn().getBody(String.class);
        String json = String.format(
            "{\"id\":\"%s\",\"timestamp\":\"%s\",\"message\":\"%s\"}",
            UUID.randomUUID(),
            Instant.now().toString(),
            message
        );
        exchange.getIn().setBody(json);
    })
    .log("[Camel] Transformed: ${body}")
    .to("kafka:demo-messages")
    .log("[Camel] Sent to Kafka topic: demo-messages");
```

**コンテナイメージ:**
- ベース: `registry.access.redhat.com/ubi9/openjdk-17`

### 4.4 Tuxedo /Q (Shared)

**デプロイ方式:**
- Tuxedo QSPACE サーバー
- Tuxedo /Q メッセージキュー

**設定:**
- Queue Space: `QSPACE`
- Queue: `TUXEDO.OUT`
- Protocol: ATMI (tpenqueue/tpdequeue)
- Persistent: false (デモ用、インメモリフォールバック)

**アクセス:**
- demo-tuxedo → Tuxedo /Q (Producer)
- demo-camel → Tuxedo /Q (Consumer)

### 4.5 Kafka (demo-kafka)

**デプロイ方式:**
- AMQ Streams Operator (Strimzi)
- Ansible でOperator自動インストール

**設定:**
- Cluster Name: `demo-cluster`
- Topic: `demo-messages`
- Partitions: 1
- Replication Factor: 1 (シングルブローカー)

**確認ツール:**
- Kafka Console Operator (UI)
- または `kafka-console-consumer` Pod

## 5. Namespace設計

| Namespace | コンポーネント | 用途 |
|-----------|--------------|------|
| demo-webui | Quarkus Web App | ユーザーインターフェース |
| demo-tuxedo | Tuxedo Service, Tuxedo /Q | レガシーシステム層 |
| demo-tuxedo-c | Tuxedo C Implementation, Tuxedo /Q | レガシーシステム層 (C版) |
| demo-camel | Apache Camel | 統合・変換層 |
| demo-kafka | Kafka Cluster, Kafka Console | メッセージング基盤 |

**Namespace間通信:**
- Service間はOpenShift内部DNS使用
  - 例: `http://tuxedo-service.demo-tuxedo.svc.cluster.local:8080`
- NetworkPolicyは基本的に全許可（デモ用）

## 6. デプロイメント戦略

### 6.1 Ansibleによる自動デプロイ

**ディレクトリ構成:**
```
ansible/
├── playbooks/
│   ├── 00-setup-operators.yml      # Operator導入
│   ├── 01-create-namespaces.yml    # Namespace作成
│   ├── 02-deploy-kafka.yml         # Kafka構築
│   ├── 03-deploy-amq.yml           # AMQ構築
│   ├── 04-deploy-tuxedo.yml        # Tuxedo Service
│   ├── 05-deploy-camel.yml         # Apache Camel
│   ├── 06-deploy-webui.yml         # Web UI
│   └── 99-cleanup.yml              # 全削除
├── roles/
│   ├── kafka-operator/
│   ├── amq-broker/
│   ├── tuxedo-service/
│   ├── camel-integration/
│   └── webui/
├── inventory/
│   └── hosts.yml
└── group_vars/
    └── all.yml
```

### 6.2 デプロイ順序

1. **Operator導入** (`00-setup-operators.yml`)
   - AMQ Streams Operator (Kafka)
   - AMQ Broker Operator (オプション)
   - Kafka Console Operator

2. **Namespace作成** (`01-create-namespaces.yml`)
   - demo-webui
   - demo-tuxedo
   - demo-camel
   - demo-kafka

3. **基盤構築** (`02-deploy-kafka.yml`, `03-deploy-amq.yml`)
   - Kafka Cluster作成
   - Kafka Topic作成
   - AMQ Broker作成
   - Queue作成

4. **アプリケーション** (`04-06`)
   - Tuxedo Service
   - Apache Camel
   - Web UI

### 6.3 コンテナイメージ管理

**ビルド戦略:**
- Option 1: GitHub Actions でビルド → Quay.io/Docker Hub
- Option 2: OpenShift BuildConfig (S2I)

**推奨: Option 1**
- リポジトリ構成:
  ```
  components/
  ├── webui/
  │   ├── Dockerfile
  │   └── src/
  ├── tuxedo/
  │   ├── Dockerfile
  │   └── src/
  └── camel/
      ├── Dockerfile
      └── src/
  ```

- GitHub Actions:
  ```
  .github/workflows/
  ├── build-webui.yml
  ├── build-tuxedo.yml
  └── build-camel.yml
  ```

**イメージタグ戦略:**
- `latest` - 最新ビルド
- `v1.0.0` - リリースタグ
- `main-{git-sha}` - コミット単位

## 7. 設定管理

### 7.1 ConfigMap

各コンポーネントの設定をConfigMapで管理:

**demo-tuxedo/tuxedo-config:**
```yaml
AMQ_HOST: amq-broker.demo-tuxedo.svc.cluster.local
AMQ_PORT: 5672
AMQ_QUEUE: TUXEDO.OUT
LOG_LEVEL: INFO
```

**demo-camel/camel-config:**
```yaml
AMQ_URL: amqp://amq-broker.demo-tuxedo.svc.cluster.local:5672
KAFKA_BOOTSTRAP: demo-cluster-kafka-bootstrap.demo-kafka.svc.cluster.local:9092
KAFKA_TOPIC: demo-messages
```

**demo-webui/webui-config:**
```yaml
TUXEDO_URL: http://tuxedo-service.demo-tuxedo.svc.cluster.local:8080
```

### 7.2 Secret

認証情報（必要に応じて）:
```yaml
apiVersion: v1
kind: Secret
metadata:
  name: amq-credentials
type: Opaque
data:
  username: YWRtaW4=  # admin
  password: YWRtaW4=  # admin
```

## 8. モニタリング・ログ

### 8.1 ログ収集

各Podの標準出力にログ出力:
```bash
# WebUI
oc logs -f deployment/webui -n demo-webui

# Tuxedo
oc logs -f deployment/tuxedo-service -n demo-tuxedo

# Camel
oc logs -f deployment/camel-integration -n demo-camel

# Kafka
oc logs -f demo-cluster-kafka-0 -n demo-kafka
```

### 8.2 Kafka確認

Kafka Console Operatorで確認:
```bash
# Console URL取得
oc get route kafka-console -n demo-kafka
```

または CLI:
```bash
# Consumer起動
oc run kafka-consumer -ti --image=quay.io/strimzi/kafka:latest-kafka-3.6.0 \
  --rm=true --restart=Never -n demo-kafka -- bin/kafka-console-consumer.sh \
  --bootstrap-server demo-cluster-kafka-bootstrap:9092 \
  --topic demo-messages --from-beginning
```

## 9. テストシナリオ

### 9.1 正常系

1. Web UIにアクセス
2. メッセージ入力: "Hello from Tuxedo!"
3. 送信ボタンクリック
4. 各コンポーネントのログ確認:
   ```
   [WebUI] Received: Hello from Tuxedo!
   [WebUI] Sent to Tuxedo: Hello from Tuxedo!
   [Tuxedo] Received: Hello from Tuxedo!
   [Tuxedo] Queued to AMQ: Hello from Tuxedo!
   [Camel] Consumed from queue: Hello from Tuxedo!
   [Camel] Transformed: {"id":"xxx","timestamp":"2026-06-04T...","message":"Hello from Tuxedo!"}
   [Camel] Sent to Kafka topic: demo-messages
   ```
5. Kafka Consoleでメッセージ確認

### 9.2 異常系（オプション）

- Tuxedo Service停止時のWebUI挙動
- AMQ接続断時のTuxedo挙動
- Kafka停止時のCamel挙動

## 10. クリーンアップ

```bash
# Ansible playbook実行
ansible-playbook ansible/playbooks/99-cleanup.yml

# または手動
oc delete project demo-webui demo-tuxedo demo-camel demo-kafka
oc delete subscription amq-streams -n openshift-operators
oc delete csv amqstreams.v2.x.x -n openshift-operators
```

## 11. 今後の拡張案

- メッセージ追跡ID（Correlation ID）の導入
- OpenTelemetry による分散トレーシング
- エラーハンドリング・リトライロジック
- Dead Letter Queue
- マルチインスタンス化（スケールアウト）
- セキュリティ強化（mTLS、認証）
