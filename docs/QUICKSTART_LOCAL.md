# Tuxedo C Implementation - ローカル実行ガイド

お手元のMacで簡単にTuxedo C実装を試せます。

---

## 前提条件

- ✅ Podman インストール済み
- ✅ Git リポジトリクローン済み
- ✅ Tuxedo SDK抽出済み (`components/tuxedo-base-sdk/sdk/`)

---

## クイックスタート（3ステップ）

### Step 1: Base Imageビルド

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo-base-sdk

# SDKを含むBase Imageをビルド（初回のみ、5-7分）
podman build --platform linux/amd64 -f Dockerfile.standalone -t tuxedo-sdk:local .
```

### Step 2: Tuxedo C実装ビルド

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo

# C実装をビルド（1-2分）
podman build --platform linux/amd64 -f Containerfile.c -t tuxedo-c:local .
```

### Step 3: 実行＆テスト

```bash
# サーバー起動
podman run --rm --platform linux/amd64 -p 8080:8080 tuxedo-c:local
```

別ターミナルでテスト：
```bash
# Health check
curl http://localhost:8080/health | jq .

# Enqueue
curl -X POST http://localhost:8080/enqueue -d "Hello from local!"

# Dequeue
curl http://localhost:8080/dequeue | jq .
```

---

## 簡単実行スクリプト

全自動でビルド＆実行するスクリプトを用意しました。

### 使い方

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel

# ビルドと起動（初回は7-10分）
./scripts/run-local.sh

# テスト実行（別ターミナル）
./scripts/test-local.sh
```

---

## トラブルシューティング

### Podmanが起動していない

```bash
# Podman machineを起動
podman machine start
```

### ポート8080が使用中

```bash
# 別のポートを使用
podman run --rm --platform linux/amd64 -p 8081:8080 tuxedo-c:local

# テストも同じポートで
curl http://localhost:8081/health
```

### Base Imageのビルドが遅い

初回のみ時間がかかります（libmicrohttpdをソースビルドするため）。
2回目以降はキャッシュが効いて高速です。

---

## ログ確認

### サーバーログ

```bash
# フォアグラウンドで起動してログを見る
podman run --rm --platform linux/amd64 -p 8080:8080 tuxedo-c:local

# 出力例：
# [Tuxedo-Startup] Starting Tuxedo /Q (C Implementation)
# [Tuxedo-Startup] TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
# [Tuxedo-Startup] Starting C REST API server on port 8080...
# [STUB] tpinit
```

### デバッグモード

```bash
# 詳細ログ付きで起動
podman run --rm --platform linux/amd64 -p 8080:8080 -e DEBUG=1 tuxedo-c:local
```

---

## クリーンアップ

### コンテナ停止

```bash
# 実行中のコンテナを確認
podman ps

# 停止（Ctrl+Cで停止、または）
podman stop <container-id>
```

### イメージ削除

```bash
# イメージ一覧
podman images | grep tuxedo

# 削除
podman rmi tuxedo-c:local
podman rmi tuxedo-sdk:local
```

### 完全クリーンアップ

```bash
# 全て削除して再ビルド
podman rmi -f tuxedo-c:local tuxedo-sdk:local
cd components/tuxedo-base-sdk
podman build --platform linux/amd64 --no-cache -f Dockerfile.standalone -t tuxedo-sdk:local .
```

---

## パフォーマンステスト

### 負荷テスト

```bash
# Apache Bench（インストール必要）
brew install apache-bench

# 1000リクエスト、10並行
ab -n 1000 -c 10 -p /dev/null http://localhost:8080/health

# または、簡易ループテスト
for i in {1..100}; do 
  curl -s -X POST http://localhost:8080/enqueue -d "Message $i"
done

for i in {1..100}; do 
  curl -s http://localhost:8080/dequeue | jq -r .message
done
```

### メモリ使用量確認

```bash
# コンテナの統計情報
podman stats

# 出力例：
# CONTAINER ID  NAME         CPU %  MEM USAGE / LIMIT  MEM %
# abc123...     tuxedo-c     0.5%   8MB / 2GB          0.4%
```

---

## 開発モード

### ソースを編集しながら実行

```bash
# ボリュームマウントで開発
podman run --rm --platform linux/amd64 -p 8080:8080 \
  -v $(pwd)/components/tuxedo:/workspace \
  tuxedo-sdk:local \
  bash -c "cd /workspace && make -f Makefile.c-headers-only && ./tuxedo_q_server"
```

### リアルタイムリビルド

```bash
# ソース変更を監視して自動リビルド（要fswatch）
brew install fswatch

fswatch -o components/tuxedo/src/ | while read; do
  echo "Rebuilding..."
  cd components/tuxedo
  podman build --platform linux/amd64 -f Containerfile.c -t tuxedo-c:local .
  echo "Done! Restart the container."
done
```

---

## 次のステップ

### OpenShiftと同じ環境で試す

```bash
# OpenShiftのイメージを取得
oc login <your-cluster>
podman pull image-registry.openshift-image-registry.svc:5000/demo-tuxedo-c/tuxedo-msgsvc:latest
podman run --rm --platform linux/amd64 -p 8080:8080 <image-id>
```

### Camelと連携

次のガイド参照：
- `CAMEL_INTEGRATION_LOCAL.md` - Camelをローカル実行
- `E2E_TEST_LOCAL.md` - 完全なE2Eテスト

---

## FAQ

**Q: Macで動きますか？**
A: はい。`--platform linux/amd64`でx86エミュレーションで動作します。

**Q: M1/M2/M3 Macで遅くないですか？**
A: エミュレーションですが、開発・テストには十分な速度です。

**Q: Docker Desktopでも動きますか？**
A: はい。`podman`を`docker`に置き換えて同じコマンドが使えます。

**Q: Windowsでも動きますか？**
A: WSL2 + Podman/Dockerで同様に動作します。

**Q: 本番デプロイできますか？**
A: OpenShiftやKubernetesへのデプロイガイドは`deploy-kit/tuxedo-q-c/`を参照。

---

## サポート

- 問題が発生した場合: [GitHub Issues](https://github.com/kamorisan/tuxedo-to-camel/issues)
- ドキュメント: `README_C_IMPLEMENTATION.md`
- OpenShiftデプロイ: `deploy-kit/tuxedo-q-c/README.md`

---

## まとめ

**最短実行**:
```bash
cd components/tuxedo-base-sdk
podman build --platform linux/amd64 -f Dockerfile.standalone -t tuxedo-sdk:local .

cd ../tuxedo
podman build --platform linux/amd64 -f Containerfile.c -t tuxedo-c:local .
podman run --rm --platform linux/amd64 -p 8080:8080 tuxedo-c:local

# 別ターミナル
curl http://localhost:8080/health
```

**所要時間**: 初回 7-10分、2回目以降 1-2分

お疲れ様でした！楽しんでください 🎉
