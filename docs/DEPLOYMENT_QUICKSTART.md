# デプロイクイックスタート

新しいOpenShift環境に5ステップでデモをデプロイ！

---

## 🚀 5ステップデプロイ

### Step 1: OpenShiftにログイン

```bash
oc login https://api.your-cluster.example.com:6443
```

### Step 2: リポジトリクローン

```bash
git clone https://github.com/kamorisan/tuxedo-to-camel.git
cd tuxedo-to-camel
```

### Step 3: デプロイ実行

```bash
cd deploy-kit
./deploy-all.sh
```

### Step 4: 完了待ち（15-20分）

コーヒーブレイク ☕

スクリプトが自動的に以下をデプロイ：
- ✅ AMQ Streams Operator
- ✅ Kafka Cluster
- ✅ Tuxedo C Implementation
- ✅ Apache Camel
- ✅ Quarkus WebUI

### Step 5: WebUIにアクセス

スクリプト完了後に表示されるURLをブラウザで開く：

```
🎉 Deployment Complete!

Access the demo at:
  🌐 WebUI: https://webui-demo-webui.apps.cluster-xxxxx.opentlc.com
```

---

## ✅ 動作確認

### 1. メッセージ送信

WebUIで：
1. メッセージ入力
2. 「Tuxedoへ送信」クリック
3. 成功メッセージ確認

### 2. Kafkaで確認

```bash
oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \
  --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 \
  -- bin/kafka-console-consumer.sh \
  --bootstrap-server demo-kafka-kafka-bootstrap:9092 \
  --topic demo-messages \
  --from-beginning \
  --max-messages 5
```

送信したメッセージがJSON形式で表示されます！

---

## 🔧 カスタマイズ

### 自分のフォークを使う

```bash
export GIT_REPO="https://github.com/YOUR_ORG/tuxedo-to-camel.git"
export GIT_BRANCH="main"
./deploy-all.sh
```

---

## 🧹 クリーンアップ

```bash
./cleanup.sh
```

全リソースが削除されます。

---

## 📊 デプロイされる構成

```
┌─────────────────────────────────────────┐
│ OpenShift Cluster                       │
├─────────────────────────────────────────┤
│                                         │
│ ┌──────────┐  ┌──────────┐  ┌────────┐│
│ │ WebUI    │→ │ Tuxedo C │→ │ Camel  ││
│ │(Quarkus) │  │ (C impl) │  │(Quarkus││
│ └──────────┘  └──────────┘  └───┬────┘│
│                                  │     │
│                              ┌───▼────┐│
│                              │ Kafka  ││
│                              └────────┘│
└─────────────────────────────────────────┘

Namespaces:
  - demo-webui
  - demo-tuxedo-c
  - demo-camel
  - demo-kafka
```

---

## 💡 トラブルシューティング

### ビルドが遅い

初回は15-20分かかります。2回目以降はキャッシュが効いて速くなります。

### Podが起動しない

```bash
# 状態確認
oc get pods --all-namespaces | grep demo

# ログ確認
oc logs -n demo-tuxedo-c deployment/tuxedo-msgsvc
```

### 詳細ガイド

[deploy-kit/README.md](deploy-kit/README.md) を参照

---

## 📚 関連ドキュメント

- [deploy-kit/README.md](deploy-kit/README.md) - 詳細デプロイガイド
- [E2E_TEST_GUIDE.md](E2E_TEST_GUIDE.md) - E2Eテスト手順
- [QUICKSTART_LOCAL.md](QUICKSTART_LOCAL.md) - ローカル実行ガイド

---

**所要時間**: 15-20分  
**前提条件**: oc CLI + OpenShiftアクセス  
**難易度**: ⭐ (とても簡単)
