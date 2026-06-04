# Tuxedo to Kafka Demo

WebUIからTuxedo /Qを経由してKafkaまでメッセージを送信するデモシステムです。

---

## 🎯 デモ概要

このデモは、レガシーシステム（Oracle Tuxedo）とモダンなメッセージング基盤（Apache Kafka）を統合するアーキテクチャを実演します。

### データフロー

```
Web UI → Tuxedo Service → Tuxedo /Q → Apache Camel → Kafka Broker
```

### 主な特徴

- ✅ **Tuxedo C実装** - ATMI APIを使用した本格的なC言語実装
- ✅ **Fallbackキュー** - インメモリキューによる動作デモ
- ✅ **完全E2E統合** - ブラウザからKafkaまでシームレス
- ✅ **OpenShift対応** - コンテナベースで簡単デプロイ
- ✅ **自動化スクリプト** - 1コマンドで完全環境構築

---

## 🚀 クイックスタート

### OpenShiftへデプロイ

新しいOpenShift環境に完全なデモ環境をデプロイ：

```bash
git clone https://github.com/kamorisan/tuxedo-to-camel.git
cd tuxedo-to-camel

# OpenShiftにログイン
oc login https://api.your-cluster.example.com:6443

# 1コマンドでデプロイ（15-20分）
cd deploy-kit
./deploy-all.sh
```

詳細: [DEPLOYMENT_QUICKSTART.md](docs/DEPLOYMENT_QUICKSTART.md)

### ローカルで試す（Mac/Linux + Podman）

```bash
cd tuxedo-to-camel

# ビルド＆起動（初回7-10分）
./scripts/run-local.sh

# 別ターミナルでテスト
./scripts/test-local.sh
```

詳細: [QUICKSTART_LOCAL.md](docs/QUICKSTART_LOCAL.md)

---

## 📋 コンポーネント

| コンポーネント | 技術スタック | 役割 |
|--------------|------------|------|
| **WebUI** | Quarkus 3.x | メッセージ入力画面 |
| **Tuxedo C** | C + ATMI API | メッセージキュー処理 |
| **Camel** | Camel Quarkus 4.x | Tuxedo /Q → Kafka 統合 |
| **Kafka** | AMQ Streams (Strimzi) | メッセージ永続化 |

---

## 📂 ディレクトリ構成

```
tuxedo-to-camel/
├── components/
│   ├── tuxedo/              # Tuxedo C実装
│   │   ├── src/tuxedo_q_server.c
│   │   ├── Containerfile.c
│   │   └── Makefile.c-headers-only
│   ├── tuxedo-base-sdk/     # Tuxedo SDKベースイメージ
│   ├── camel/               # Apache Camel統合
│   └── webui/               # Quarkus WebUI
├── deploy-kit/
│   ├── deploy-all.sh        # 完全自動デプロイ
│   ├── cleanup.sh           # 環境クリーンアップ
│   └── README.md            # デプロイ詳細ガイド
├── scripts/
│   ├── run-local.sh         # ローカル実行スクリプト
│   └── test-local.sh        # ローカルテストスクリプト
└── docs/
    ├── DEPLOYMENT_QUICKSTART.md  # デプロイクイックガイド
    ├── QUICKSTART_LOCAL.md       # ローカル実行ガイド
    └── E2E_TEST_GUIDE.md         # E2Eテストガイド
```

---

## 📖 ドキュメント

### デプロイ

- **[DEPLOYMENT_QUICKSTART.md](docs/DEPLOYMENT_QUICKSTART.md)** - 5ステップでOpenShiftにデプロイ
- **[deploy-kit/README.md](deploy-kit/README.md)** - 詳細デプロイガイド

### ローカル開発

- **[QUICKSTART_LOCAL.md](docs/QUICKSTART_LOCAL.md)** - Mac/Linuxでローカル実行
- **[scripts/README.md](scripts/README.md)** - スクリプト使用方法

### テスト

- **[E2E_TEST_GUIDE.md](docs/E2E_TEST_GUIDE.md)** - E2E統合テスト手順

### 技術詳細

- **[components/tuxedo/README_C_IMPLEMENTATION.md](components/tuxedo/README_C_IMPLEMENTATION.md)** - C実装の詳細
- **[design/architecture.md](design/architecture.md)** - アーキテクチャ設計

---

## ✅ 動作確認済み環境

- **OpenShift**: 4.12+
- **ローカル環境**:
  - macOS (M1/M2/M3) + Podman
  - Linux + Podman/Docker
- **ブラウザ**: Chrome, Firefox, Safari

---

## 🛠️ 開発

### ブランチ

- `main` - 安定版
- `feature/tuxedo-q-c-implementation` - Tuxedo C実装（最新）

### 主要機能

✅ **実装済み**:
- Tuxedo C実装（libmicrohttpd + ATMI stub）
- Fallback in-memoryキュー
- WebUI（Quarkus + REST）
- Camel統合（HTTP polling）
- Kafka連携

🔄 **将来の拡張**:
- 実Tuxedo QSPACEとの統合
- FML32バッファ対応
- トランザクション処理

---

## 📊 アーキテクチャ

```
┌─────────────────────────────────────────────────────┐
│ OpenShift Cluster                                   │
├─────────────────────────────────────────────────────┤
│                                                     │
│  ┌──────────┐    ┌──────────┐    ┌──────────┐     │
│  │ WebUI    │───▶│ Tuxedo C │───▶│ Camel    │     │
│  │(Quarkus) │    │ (C+ATMI) │    │(Quarkus) │     │
│  └──────────┘    └──────────┘    └─────┬────┘     │
│                                         │          │
│                                    ┌────▼─────┐    │
│                                    │ Kafka    │    │
│                                    │(Strimzi) │    │
│                                    └──────────┘    │
└─────────────────────────────────────────────────────┘

Namespaces:
  demo-webui, demo-tuxedo-c, demo-camel, demo-kafka
```

---

## 🤝 コントリビューション

Issue、Pull Requestを歓迎します！

---

## 📄 ライセンス

MIT License

---

## 📧 サポート

- **Issues**: https://github.com/kamorisan/tuxedo-to-camel/issues
- **Discussions**: https://github.com/kamorisan/tuxedo-to-camel/discussions

---

**Last Updated**: 2026-06-04  
**Version**: 1.0.0 (Tuxedo C Implementation Complete)
