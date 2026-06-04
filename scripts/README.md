# Scripts - ローカル実行用

お手元でTuxedo C実装を簡単に試せるスクリプト集です。

---

## クイックスタート

### 1. サーバー起動

```bash
./scripts/run-local.sh
```

初回は7-10分、2回目以降は1-2分で起動します。

### 2. テスト実行（別ターミナル）

```bash
./scripts/test-local.sh
```

5つの自動テストが実行されます。

---

## スクリプト一覧

| スクリプト | 用途 | 所要時間 |
|-----------|------|---------|
| `run-local.sh` | Tuxedo Cサーバー起動 | 初回7-10分 |
| `test-local.sh` | 自動テスト実行 | 10秒 |

---

## 使用例

### 基本的な使い方

**ターミナル1（サーバー起動）**:
```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel
./scripts/run-local.sh
```

**ターミナル2（テスト実行）**:
```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel
./scripts/test-local.sh
```

### 手動テスト

```bash
# Health check
curl http://localhost:8080/health

# Enqueue
curl -X POST http://localhost:8080/enqueue -d "Hello!"

# Dequeue
curl http://localhost:8080/dequeue
```

---

## トラブルシューティング

### "Podman not found"

```bash
brew install podman
podman machine init
podman machine start
```

### "Port 8080 is in use"

スクリプトが自動的にポート8081を使います。
テストスクリプトも自動検出します。

### "SDK not found"

```bash
# SDK抽出が必要
cd components/tuxedo-base-sdk
./extract-sdk.sh
```

---

## 次のステップ

- 詳細なガイド: `QUICKSTART_LOCAL.md`
- OpenShiftデプロイ: `deploy-kit/tuxedo-q-c/`
- 開発ガイド: `README_C_IMPLEMENTATION.md`
