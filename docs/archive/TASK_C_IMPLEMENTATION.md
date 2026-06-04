# Task: Tuxedo Mock のC言語実装への移行

## 概要

現在のPython実装をTuxedo C言語 (ATMI API) で実装し直す

## 前提条件

### 必要なもの
- ✅ Tuxedo Runtime (既存)
- ⚠️ Tuxedo SDK (include/, buildserver) - **現在不足**
- ✅ qpid-proton-c ライブラリ
- 🆕 HTTPサーバーライブラリ (libmicrohttpd または自前実装)

### 課題
1. **Tuxedo Runtime には include/ がない**
   - 現在のランタイムはビルドツールが含まれていない
   - ヘッダーファイル（atmi.h等）が必要
   - 解決策：
     - オプションA: 完全なTuxedo SDKインストール
     - オプションB: ヘッダーファイルのみを別途入手
     - オプションC: Python実装を継続（推奨）

2. **開発・ビルド環境**
   - buildserver コマンドが必要
   - C コンパイラとリンカ設定
   - OpenShift BuildConfig での C ビルド対応

## タスクブレイクダウン

### Phase 1: 環境準備 (推定: 4-6時間)

#### 1.1 Tuxedo SDK 入手 (2時間)
- [ ] Oracle Tuxedo SDK ダウンロード
- [ ] インストーラ実行または手動展開
- [ ] include/ ディレクトリ確認
- [ ] 必要なヘッダーファイル：
  - `atmi.h` - ATMI基本API
  - `fml32.h` - FMLバッファ
  - `tmqueue.h` - /Q機能（後のタスク用）

#### 1.2 ビルド環境構築 (2-3時間)
- [ ] Containerfile更新：C開発ツール追加
  ```dockerfile
  RUN yum -y install gcc make
  ```
- [ ] buildserver コマンド動作確認
- [ ] サンプルATMIプログラムのビルドテスト

#### 1.3 依存ライブラリ (1時間)
- [ ] qpid-proton-c インストール確認（既存）
- [ ] HTTPサーバーライブラリ選定・インストール
  - libmicrohttpd (推奨)
  - または mongoose (組込み型)

---

### Phase 2: C実装 (推定: 8-12時間)

#### 2.1 HTTPサーバー実装 (3-4時間)
- [ ] libmicrohttpd を使用したHTTPサーバー
- [ ] POST /MSGSVC エンドポイント
- [ ] リクエストボディ読み取り
- [ ] JSON レスポンス生成

**ファイル:** `components/tuxedo/src/msgsvc.c`

```c
#include <microhttpd.h>
#include <atmi.h>
#include <proton/message.h>

#define PORT 8080

int handle_post(void *cls, struct MHD_Connection *conn, 
                const char *url, const char *method,
                const char *version, const char *upload_data,
                size_t *upload_data_size, void **con_cls);
```

#### 2.2 AMQP送信実装 (3-4時間)
- [ ] qpid-proton C API 使用
- [ ] pn_messenger または pn_reactor パターン
- [ ] 接続管理・再接続ロジック
- [ ] エラーハンドリング

```c
#include <proton/messenger.h>

int send_to_amq(const char *message, const char *amq_url);
```

#### 2.3 統合とビルド (2-3時間)
- [ ] Makefile または buildserver スクリプト作成
- [ ] リンクオプション設定
  ```bash
  buildserver -o msgsvc -f msgsvc.c -l qpid-proton-core -l microhttpd
  ```
- [ ] Containerfile ビルドステップ更新
- [ ] 動作テスト

#### 2.4 ロギング・設定 (1時間)
- [ ] userlog() を使用したTuxedoログ
- [ ] 環境変数読み取り (AMQ_HOST等)
- [ ] デバッグモード実装

---

### Phase 3: デプロイ・テスト (推定: 2-3時間)

#### 3.1 OpenShift対応 (1-2時間)
- [ ] BuildConfig更新（C build対応）
- [ ] Binary Build でビルド済みバイナリ転送
- [ ] または Source Build でOpenShift上でコンパイル

#### 3.2 統合テスト (1時間)
- [ ] WebUI → C実装Tuxedo → AMQ → Camel → Kafka
- [ ] パフォーマンステスト
- [ ] エラーケーステスト

---

## 推定作業時間

| Phase | 作業時間 | 難易度 |
|-------|---------|--------|
| Phase 1: 環境準備 | 4-6時間 | ⭐⭐⭐ |
| Phase 2: C実装 | 8-12時間 | ⭐⭐⭐⭐ |
| Phase 3: デプロイ | 2-3時間 | ⭐⭐ |
| **合計** | **14-21時間** | **⭐⭐⭐⭐** |

---

## リスクと課題

### 高リスク
1. **Tuxedo SDK入手困難**
   - Oracleライセンス必要
   - インストーラの互換性問題

2. **ビルド環境の複雑さ**
   - OpenShiftでのCビルドはJavaより複雑
   - ライブラリ依存関係の解決

### 中リスク
3. **デバッグ困難**
   - C言語特有のメモリ管理
   - セグメンテーションフォルト対応

4. **メンテナンス性低下**
   - Pythonより可読性低下
   - チームのCスキル依存

---

## 代替案：Python実装継続（推奨）

### メリット
- ✅ 既に動作している
- ✅ 開発・メンテナンスが容易
- ✅ デバッグが簡単
- ✅ デモ目的には十分

### デメリット
- ❌ "本物のTuxedo"感がない
- ❌ パフォーマンスはCより劣る（実用上は問題なし）

---

## 推奨アプローチ

**段階的移行:**

1. **現在（完了）**: Python Mock - デモ・PoC用
2. **次フェーズ**: Tuxedo /Q 統合（次のタスク）- Pythonのまま
3. **将来**: C実装 - 本番相当が必要になった場合

**理由:**
- デモシステムとしてはPythonで十分
- まずはTuxedo /Q統合で"Tuxedoらしさ"を追加
- C実装は要件が明確になってから
