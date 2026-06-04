# Tuxedo SDK 使用方法まとめ

お手元の `tuxedo221100_64_linux_01_x86.zip` を使ってC版実装をビルドする方法。

---

## 現在の状況

✅ **手元にあるファイル**:
```
/Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip (275MB)
```

このZIPファイルには完全なTuxedo SDK (22.1.1.0.0) が含まれています。

✅ **必要なものは揃っています！**

---

## 次にやること

### ステップ1: Dockerを起動

```bash
# Docker Desktopを起動
open -a Docker

# 起動確認
docker ps
```

### ステップ2: SDKを抽出

```bash
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo-base-sdk

# 自動抽出スクリプト実行 (3-5分)
./extract-sdk.sh
```

**処理内容**:
1. Oracle Linux 8コンテナを起動
2. Tuxedoインストーラーを実行
3. SDK (headers + build tools) を抽出
4. `sdk/` ディレクトリに保存
5. コンテナをクリーンアップ

**抽出されるファイル**:
```
sdk/
├── include/          # C header files
│   ├── atmi.h       # ← C実装で必要
│   ├── tmqueue.h    # ← C実装で必要
│   ├── fml.h
│   └── ... (100+ files)
└── bin/             # Build tools
    ├── tmloadcf     # ← QSPACE起動で必要
    ├── tmboot       # ← QSPACE起動で必要
    ├── tmadmin
    └── ...
```

### ステップ3: SDK base imageをビルド

```bash
# SDKを含むDockerイメージ作成
docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .

# 確認
docker images | grep tuxedo-sdk
```

### ステップ4: C実装をローカルでビルドテスト

```bash
cd ../tuxedo

# コンパイルテスト (SDK使用)
docker run --rm -v $(pwd):/workspace -w /workspace \
  tuxedo-sdk:22.1.1.0.0 \
  gcc -I/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include \
      src/tuxedo_q_server.c \
      -L/u01/oracle/tuxHome/tuxedo22.1.1.0.0/lib \
      -ltux -lbuft -lfml -lmicrohttpd \
      -o tuxedo_q_server

# 成功すればバイナリが生成される
ls -lh tuxedo_q_server
```

---

## OpenShiftへのデプロイ

### Option A: SDK imageをレジストリにプッシュ

```bash
# タグ付け
docker tag tuxedo-sdk:22.1.1.0.0 <your-registry>/tuxedo-sdk:22.1.1.0.0

# プッシュ
docker push <your-registry>/tuxedo-sdk:22.1.1.0.0
```

**deploy-kit/tuxedo-q-c/templates/03-base-buildconfig.yaml を更新**:
```yaml
from:
  kind: DockerImage
  name: '<your-registry>/tuxedo-sdk:22.1.1.0.0'
```

### Option B: OpenShift内でビルド

1. SDKファイル (`sdk/`) をgitにコミット
2. OpenShift BuildConfigでSDK base imageをビルド
3. その後C実装をビルド

---

## 完全な実行例

```bash
# 1. Docker起動
open -a Docker
sleep 10

# 2. SDK抽出
cd /Users/kamori/vscode/customer/japan-post/tuxedo-to-camel/components/tuxedo-base-sdk
./extract-sdk.sh

# 3. Base imageビルド
docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .

# 4. C実装ビルドテスト
cd ../tuxedo
docker run --rm -v $(pwd):/workspace -w /workspace \
  tuxedo-sdk:22.1.1.0.0 \
  make -f Makefile.c

# 5. 動作確認
docker run --rm -p 8080:8080 -v $(pwd):/workspace -w /workspace \
  tuxedo-sdk:22.1.1.0.0 \
  ./tuxedo_q_server

# 別ターミナルでテスト
curl -X POST http://localhost:8080/enqueue -d "Test message"
curl http://localhost:8080/dequeue
```

---

## トラブルシューティング

### Dockerが起動しない

```bash
# Docker Desktopを再起動
killall Docker && open -a Docker
```

### 抽出スクリプトが失敗

**手動抽出**を試す ([EXTRACT_SDK.md](components/tuxedo-base-sdk/EXTRACT_SDK.md) 参照):
```bash
# 一時コンテナ起動
docker run -it --name tux-install \
  -v /Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip:/tmp/tux.zip \
  oraclelinux:8 bash

# コンテナ内で手動インストール
# (詳細はEXTRACT_SDK.md参照)
```

### ビルドエラー: atmi.h not found

SDKが正しく抽出されているか確認:
```bash
ls -la components/tuxedo-base-sdk/sdk/include/atmi.h
```

なければ再抽出:
```bash
cd components/tuxedo-base-sdk
rm -rf sdk/
./extract-sdk.sh
```

---

## まとめ

| ステップ | コマンド | 所要時間 |
|---------|---------|---------|
| 1. Docker起動 | `open -a Docker` | 30秒 |
| 2. SDK抽出 | `./extract-sdk.sh` | 3-5分 |
| 3. Image build | `docker build ...` | 1-2分 |
| 4. C実装ビルド | `make -f Makefile.c` | 10秒 |
| **合計** | | **5-8分** |

**結果**:
- ✅ C実装がコンパイル可能
- ✅ QSPACE起動可能 (tmloadcf, tmboot使用)
- ✅ OpenShiftデプロイ準備完了

---

## 参考ドキュメント

- [components/tuxedo-base-sdk/README.md](components/tuxedo-base-sdk/README.md) - クイックスタート
- [components/tuxedo-base-sdk/EXTRACT_SDK.md](components/tuxedo-base-sdk/EXTRACT_SDK.md) - 詳細抽出ガイド (4つの方法)
- [deploy-kit/tuxedo-q-c/README.md](deploy-kit/tuxedo-q-c/README.md) - C版デプロイガイド
- [QSPACE_IMPLEMENTATION_GUIDE.md](QSPACE_IMPLEMENTATION_GUIDE.md) - QSPACE設定ガイド

---

## 次のステップ

1. ✅ ZIPファイル確認 - **完了** (`tuxedo221100_64_linux_01_x86.zip` 存在確認済み)
2. ✅ 抽出ツール作成 - **完了** (`extract-sdk.sh` 作成済み)
3. ⬜ **Docker起動** - 次にやること
4. ⬜ SDK抽出実行
5. ⬜ C実装ビルドテスト
6. ⬜ OpenShiftデプロイ

**今すぐできること**: Docker Desktopを起動して `./extract-sdk.sh` を実行！
