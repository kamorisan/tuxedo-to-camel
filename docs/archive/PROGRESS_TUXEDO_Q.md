# Tuxedo /Q Integration Progress

**ブランチ**: feature/tuxedo-q-integration  
**開始日**: 2026-06-04  
**目標**: AMQ BrokerをTuxedo /Qに置き換え

---

## アーキテクチャ変更

### Before (完了済み)
```
WebUI → Tuxedo Mock (Python) → AMQ Broker → Camel → Kafka
```

### After (目標)
```
WebUI → Tuxedo (Python) → Tuxedo /Q → Camel (REST polling) → Kafka
```

---

## Phase 1: QSPACE環境構築 (完了)

### ✅ 完了
- [x] ubbconfig作成 (QSPACE定義)
- [x] 起動スクリプト作成 (start-tuxedo-with-q.sh)
- [x] Containerfile更新（ubbconfig, pythonライブラリコピー）
- [x] フォールバックモード実装（Tuxedo binaries不在時対応）

### 📁 作成ファイル
```
components/tuxedo/
├── config/
│   └── ubbconfig                  # Tuxedo設定（QSPACE, TMQUEUE）
├── python/
│   └── tuxedo_queue.py            # Python ctypes wrapper (ATMI Queue API)
└── scripts/
    ├── start-tuxedo-with-q.sh     # 統合起動スクリプト
    └── tuxedo-q-server.py         # REST API server (enqueue/dequeue)
```

---

## Phase 2: Python実装 (完了)

### ✅ 完了
- [x] Python ctypes wrapper実装 (tuxedo_queue.py)
- [x] TPQCTL構造体定義
- [x] tpenqueue/tpdequeue関数バインディング
- [x] REST API server実装 (tuxedo-q-server.py)
  - POST /enqueue - メッセージ投入
  - POST /MSGSVC - 後方互換エンドポイント
  - GET /dequeue - メッセージ取得
  - GET /health - ヘルスチェック

### 技術詳細
**実装方式**: Python ctypes
- `libtux.so` 直接呼び出し
- TPQCTL構造体マッピング
- tpinit/tpterm でTuxedoコンテキスト管理
- BlockingSender不要（/Q内部管理）

**フォールバックモード**:
- Tuxedo binaries不在時はロギングのみ
- REST APIは常に起動
- エラーハンドリング実装済み

---

## Phase 3: Camel統合 (次のステップ)

### タスク
- [ ] Camel Route変更
  - AMQPコンシューマ削除
  - REST polling追加 (timer → HTTP GET /dequeue)
  - Nullチェック実装
- [ ] エラーハンドリング
  - 空キュー対応
  - Tuxedoダウン時リトライ
- [ ] テスト

## Phase 4: エンドツーエンドテスト (予定)

### 検証項目
- [ ] WebUI → Tuxedo /Q (enqueue)
- [ ] Camel → Tuxedo /Q (dequeue)
- [ ] Camel → Kafka
- [ ] Kafka Consoleで確認

---

## 推定進捗

| Phase | 予定時間 | 実績時間 | 状況 |
|-------|---------|---------|------|
| Phase 1: 環境構築 | 4-6h | 2h | ✅ 完了 |
| Phase 2: Python実装 | 6-8h | 3h | ✅ 完了 |
| Phase 3: Camel統合 | 3-4h | - | ⚪ 次のステップ |
| Phase 4: E2Eテスト | 2-3h | - | ⚪ 未着手 |
| **合計** | **15-21h** | **5h** | **30%** |

---

## 技術メモ

### Tuxedo /Q 構成要素
- **QSPACE**: キュー名前空間
- **TMQUEUE**: キューサーバープロセス
- **DEMO_QUEUE**: 今回使用するキュー名

### ATMI Queue API
```c
#include <atmi.h>

// Enqueue
int tpenqueue(char *qspace, TPQCTL *ctl, char *data, long len, long flags);

// Dequeue  
int tpdequeue(char *qspace, TPQCTL *ctl, char **data, long *len, long flags);
```

### TPQCTL構造体（簡略版）
```c
struct tpqctl_t {
    long flags;           /* Operation flags */
    long deq_time;        /* Absolute/relative time for dequeuing */
    long priority;        /* Enqueue priority */
    long diagnostic;      /* Error diagnostic */
    char msgid[32];       /* Message ID */
    char corrid[32];      /* Correlation ID */
    char replyqueue[128]; /* Reply queue name */
    char failurequeue[128]; /* Failure queue name */
};
```

---

**最終更新**: 2026-06-04 14:10
