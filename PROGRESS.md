# Tuxedo to Camel Demo - 実装進捗

最終更新: 2026-06-04

## プロジェクト概要

WebアプリケーションからTuxedoを経由してApache CamelでメッセージをKafkaへ送信するまでの流れを可視化するデモシステム。

**メッセージフロー:**
```
[Web UI (Quarkus)] 
  ↓ HTTP POST
[WSL (Tuxedo WebService Listener) :8080]
  ↓ Tuxedo ATMI
[MSGSVC (Tuxedo Service)]
  ↓ AMQP (qpid-proton)
[Red Hat AMQ Queue: TUXEDO.OUT]
  ↓ AMQP
[Apache Camel]
  ↓ JSON変換
[Kafka Topic: demo-messages]
```

---

## ✅ 完了した実装

### 1. Quarkus WebUI（完了 100%）

**場所:** `components/webui/`

**実装内容:**
- ✅ REST API (`MessageResource.java`) - メッセージ送信エンドポイント
- ✅ HTML UI (`index.html`) - ユーザー入力画面
- ✅ Health Check (`MyLivenessCheck.java`)
- ✅ application.properties設定
- ✅ Dockerfile (Java 21)
- ✅ ローカル動作確認完了
- ✅ OpenShiftデプロイ成功
  - Namespace: `demo-webui`
  - URL: https://webui-demo-webui.apps.cluster-4sgmc.4sgmc.sandbox356.opentlc.com
  - HTTPSアクセス有効（HTTPからリダイレクト）
- ✅ GitHubからのビルド設定（BuildConfig with ubi8-openjdk-21）

**技術スタック:**
- Quarkus 3.15.1
- Java 21
- RESTEasy Reactive
- SmallRye Health

**エンドポイント:**
- `GET /` - メッセージ入力画面
- `POST /api/send` - メッセージ送信API
- `GET /health/live` - Liveness probe
- `GET /health/ready` - Readiness probe

**状態:** 本番稼働可能

---

### 2. Tuxedo Message Service（実装完了 100%、ビルド待ち）

**場所:** `components/tuxedo/`

**実装内容:**

#### ソースコード
- ✅ `src/MSGSVC.c` - Tuxedoサービス本体
  - tpsvrinit: AMQクライアント初期化
  - MSGSVC: メッセージ受信→AMQ送信
  - tpsvrdone: クリーンアップ
  - ログ出力: `[Tuxedo] Received: {message}`, `[Tuxedo] Message queued to AMQ: {message}`

- ✅ `src/amqclient.c` / `src/amqclient.h` - AMQP Cライブラリ
  - qpid-proton Cライブラリのラッパー
  - amq_client_init: 初期化
  - amq_client_connect: AMQ接続
  - amq_client_send: メッセージ送信
  - amq_client_disconnect / destroy: クリーンアップ

#### 設定ファイル
- ✅ `config/ubbconfig` - Tuxedo UBB設定
  - MSGSVC: メッセージサービス
  - WSL: WebService Listener (port 8080)
  - ドメイン: tuxdemo
  - IPC Key: 112233

#### ビルド
- ✅ `Makefile` - Tuxedo buildserver用
  - buildserverコマンドでMSGSVC実行ファイル作成
  - qpid-protonライブラリリンク

#### スクリプト
- ✅ `scripts/boot.sh` - Tuxedo起動
  - TLOG作成
  - tmloadcf (UBB読み込み)
  - tmboot (サービス起動)
  
- ✅ `scripts/shutdown.sh` - Tuxedo停止
  - tmshutdown

- ✅ `scripts/entrypoint.sh` - コンテナエントリーポイント
  - シグナルハンドリング
  - ヘルスチェック
  - プロセス監視

#### コンテナ化
- ✅ `Containerfile` - podman用イメージ定義
  - ベース: oracle/tuxedo:22.1.1.0.0
  - qpid-proton C library追加
  - ソースビルド
  - ポート8080公開

#### ドキュメント
- ✅ `README.md` - ビルド・デプロイ手順書

**技術スタック:**
- Oracle Tuxedo 22.1.1.0.0
- C言語
- qpid-proton (AMQP 1.0クライアント)
- Tuxedo ATMI
- Tuxedo WSL (WebService Listener)

**環境変数:**
- `AMQ_HOST`: AMQブローカーホスト名（デフォルト: amq-broker.demo-tuxedo.svc.cluster.local）
- `AMQ_PORT`: AMQブローカーポート（デフォルト: 5672）
- `AMQ_QUEUE`: AMQキュー名（デフォルト: TUXEDO.OUT）

**状態:** 実装完了、ビルド・テスト待ち

---

### 3. 設計ドキュメント（完了）

**場所:** `design/`

- ✅ `README.md` - ドキュメント索引
- ✅ `architecture.md` - アーキテクチャ設計
  - Tuxedo技術スタックをC言語に更新
- ✅ `deployment-guide.md` - デプロイメントガイド
- ✅ `component-specs.md` - コンポーネント仕様書

---

## 🚧 未完了・次回作業

### 1. Tuxedo Message Serviceのビルド・テスト

**優先度:** 高

**必要な作業:**

#### Step 1: Oracle Tuxedo 22.1.1.0.0ベースイメージのビルド

**利用可能なリソース:**
- Tuxedoインストーラー: `/Users/kamori/temp/Oracle_Tuxedo_22c/tuxedo221100_64_linux_01_x86.zip`
- 展開済みディレクトリ: `/Users/kamori/temp/Oracle_Tuxedo_22c/`
- Dockerfileテンプレート: `/Users/kamori/vscode/customer/japan-post/docker-images/OracleTuxedo/core/dockerfiles/22.1.1.0.0/`

**方法A: 公式ビルドスクリプト使用（推奨）**
```bash
cd /Users/kamori/vscode/customer/japan-post/docker-images/OracleTuxedo/core/dockerfiles

# ビルドスクリプト実行（要: ServerJRE 8イメージ）
./buildContainerImage.sh -v 22.1.1.0.0 -i tuxedo221100_64_Linux_01_x86.zip -s

# タグ付け
podman tag oracle/tuxedo:22.1.1.0.0 oracle/tuxedo:latest
```

**方法B: 簡略化（UBIベース直接ビルド）**
- oracle/serverjre:8ベースイメージなしでビルド
- registry.access.redhat.com/ubi8/ubi:latest から開始
- Java 8を手動インストール
- Tuxedoインストール

**推奨:** 方法Aだが、ServerJRE 8イメージが必要
- ServerJREインストーラー入手先: Oracle公式サイト
- または方法Bで回避

**所要時間:** 20-30分

#### Step 2: Tuxedo MSGSVCイメージビルド

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo

# ビルド
podman build -f Containerfile -t tuxedo-msgsvc:latest .
```

**所要時間:** 5-10分

#### Step 3: ローカル動作確認

**必要なコンポーネント:**
1. AMQ Broker（Red Hat AMQ Artemis）
2. Tuxedo MSGSVCコンテナ
3. WebUI（既にデプロイ済み）

**テスト手順:**
```bash
# 1. AMQ Broker起動（別途必要）
podman run -d --name amq-broker \
  -p 5672:5672 \
  quay.io/artemiscloud/activemq-artemis-broker:latest

# 2. Tuxedo MSGSVC起動
podman run -d --name tuxedo-msgsvc \
  -p 8080:8080 \
  -e AMQ_HOST=localhost \
  -e AMQ_PORT=5672 \
  -e AMQ_QUEUE=TUXEDO.OUT \
  tuxedo-msgsvc:latest

# 3. ログ確認
podman logs -f tuxedo-msgsvc

# 4. メッセージ送信テスト
curl -X POST http://localhost:8080/MSGSVC \
  -H "Content-Type: text/plain" \
  -d "Hello from Tuxedo!"

# 5. Tuxedoステータス確認
podman exec tuxedo-msgsvc tmadmin -r << EOF
psr
psc
quit
EOF
```

**期待される動作:**
- Tuxedoサービス起動成功
- WSLがport 8080でリッスン
- メッセージ受信ログ: `[Tuxedo] Received: Hello from Tuxedo!`
- AMQ送信ログ: `[Tuxedo] Message queued to AMQ: Hello from Tuxedo!`

---

### 2. Apache Camel実装

**場所:** `components/camel/`（未作成）

**必要な実装:**
- Camel Quarkusプロジェクト
- AMQキュー消費ルート
- JSON変換プロセッサー
- Kafkaプロデューサー

**参考:** `design/component-specs.md` の Section 4.3

---

### 3. Red Hat AMQデプロイ

**場所:** `openshift/amq/`（未作成）

**必要なリソース:**
- AMQ Broker Operator
- AMQ Broker CR
- Queue定義: `TUXEDO.OUT`

---

### 4. Kafkaデプロイ

**場所:** `openshift/kafka/`（未作成）

**必要なリソース:**
- AMQ Streams Operator
- Kafka Cluster CR
- Kafka Topic CR: `demo-messages`
- Kafka Console（確認用）

---

### 5. OpenShiftマニフェスト作成

#### Tuxedo Service用

**場所:** `openshift/tuxedo/`（未作成）

**必要なファイル:**
- `01-namespace.yaml` - demo-tuxedo
- `02-buildconfig.yaml` - GitHubソースからビルド
- `03-imagestream.yaml`
- `04-deployment.yaml` - MSGSVC Deployment
- `05-service.yaml` - ClusterIP Service (port 8080)
- `06-route.yaml` - 外部アクセス用Route

**環境変数設定（Deployment）:**
```yaml
env:
  - name: AMQ_HOST
    value: "amq-broker.demo-tuxedo.svc.cluster.local"
  - name: AMQ_PORT
    value: "5672"
  - name: AMQ_QUEUE
    value: "TUXEDO.OUT"
```

---

### 6. 統合テスト

**テストフロー:**
1. WebUI (demo-webui) からメッセージ送信
2. Tuxedo WSL (demo-tuxedo) が受信
3. MSGSVC がAMQキューへ送信
4. Camel (demo-camel) がキューから消費
5. Camel がJSON変換
6. Kafka (demo-kafka) へ送信
7. Kafka Consoleで確認

**期待されるログ:**
```
[WebUI] Received: Hello World!
[WebUI] Sent to Tuxedo: Hello World!
[Tuxedo] Received: Hello World!
[Tuxedo] Message queued to AMQ: Hello World!
[Camel] Consumed from queue: Hello World!
[Camel] Transformed: {"id":"xxx","timestamp":"...","message":"Hello World!"}
[Camel] Sent to Kafka topic: demo-messages
```

---

## 📁 ディレクトリ構成

```
tuxedo-to-camel/
├── design/                      # 設計ドキュメント ✅
│   ├── README.md
│   ├── architecture.md
│   ├── deployment-guide.md
│   └── component-specs.md
├── components/
│   ├── webui/                   # Quarkus WebUI ✅
│   │   ├── src/
│   │   ├── Dockerfile
│   │   ├── pom.xml
│   │   └── README.md
│   ├── tuxedo/                  # Tuxedo Service ✅ (ビルド待ち)
│   │   ├── src/
│   │   │   ├── MSGSVC.c
│   │   │   ├── amqclient.c
│   │   │   └── amqclient.h
│   │   ├── config/
│   │   │   └── ubbconfig
│   │   ├── scripts/
│   │   │   ├── boot.sh
│   │   │   ├── shutdown.sh
│   │   │   └── entrypoint.sh
│   │   ├── Containerfile
│   │   ├── Makefile
│   │   └── README.md
│   ├── camel/                   # Apache Camel 🚧 未作成
│   └── amq/                     # AMQ設定 🚧 未作成
├── openshift/
│   ├── webui/                   # WebUI manifests ✅
│   │   ├── 01-namespace.yaml
│   │   ├── 02-deployment.yaml
│   │   ├── 03-service.yaml
│   │   ├── 04-route.yaml
│   │   └── 05-buildconfig.yaml
│   ├── tuxedo/                  # Tuxedo manifests 🚧 未作成
│   ├── camel/                   # Camel manifests 🚧 未作成
│   ├── amq/                     # AMQ manifests 🚧 未作成
│   └── kafka/                   # Kafka manifests 🚧 未作成
├── ansible/                     # Ansible自動化 🚧 未作成
└── PROGRESS.md                  # このファイル
```

---

## 🔧 技術スタック

### 実装済み
- **WebUI**: Quarkus 3.15.1, Java 21, RESTEasy Reactive
- **Tuxedo**: Oracle Tuxedo 22.1.1.0.0, C言語, qpid-proton
- **OpenShift**: 4.20.23, BuildConfig, Route (HTTPS)
- **Container**: podman, Containerfile

### 次回実装予定
- **AMQ**: Red Hat AMQ Broker (ActiveMQ Artemis)
- **Camel**: Apache Camel 4.x, Camel Quarkus
- **Kafka**: AMQ Streams (Strimzi), Kafka 3.6

---

## 📝 備考

### Git管理
- ブランチ: `feature/implementation`
- 最新コミット: `22e7a51` - "Add Tuxedo Message Service implementation"
- リモート: https://github.com/kamorisan/tuxedo-to-camel.git

### 利用可能なリソース
- Tuxedoサンプル: `/Users/kamori/vscode/customer/japan-post/Tuxedo-samples/`
- Tuxedoインストーラー: `/Users/kamori/temp/Oracle_Tuxedo_22c/`
- Docker Images: `/Users/kamori/vscode/customer/japan-post/docker-images/`

### OpenShift環境
- クラスター: cluster-4sgmc.4sgmc.sandbox356.opentlc.com
- 認証: kube:admin
- 既存Namespace: demo-webui ✅

---

## 次回セッションの開始手順

1. **Gitブランチ確認**
   ```bash
   cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel
   git status
   git log --oneline -5
   ```

2. **このドキュメント確認**
   ```bash
   cat PROGRESS.md
   ```

3. **「🚧 未完了・次回作業」セクションから開始**

---

**最終更新者:** Claude Sonnet 4.5  
**次回作業:** Tuxedoベースイメージビルド → ローカルテスト → OpenShiftデプロイ
