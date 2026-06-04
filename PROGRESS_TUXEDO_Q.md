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

## Phase 1: QSPACE環境構築 (進行中)

### ✅ 完了
- [x] ubbconfig作成 (QSPACE定義)
- [x] 起動スクリプト作成 (start-tuxedo-q.sh)
- [x] 設定ファイル構造

### 📁 作成ファイル
```
components/tuxedo/
├── config/
│   └── ubbconfig              # Tuxedo設定（QSPACE, TMQUEUE）
└── scripts/
    └── start-tuxedo-q.sh      # Tuxedo起動スクリプト
```

### ⏳ 次のステップ
- [ ] Containerfile更新（ubbconfigコピー）
- [ ] Tuxedo起動テスト
- [ ] QSPACEサーバー起動確認

---

## Phase 2: C言語サンプル (予定)

### タスク
- [ ] enqueueサンプル作成
- [ ] dequeueサンプル作成
- [ ] ビルドとテスト

### 技術検討
**課題**: Tuxedo Runtimeにinclude/がない
- オプションA: ヘッダーファイルのみ入手
- オプションB: Python ctypes実装へ切り替え（推奨）

---

## Phase 3: Python実装 (予定)

### アプローチ
1. **ctypes経由でATMI呼び出し**
   - `libtux.so` を直接呼び出し
   - TPQCTL構造体をctypesで定義
   
2. **REST APIエンドポイント**
   - POST /enqueue - メッセージ投入
   - GET /dequeue - メッセージ取得

---

## Phase 4: Camel統合 (予定)

### 変更内容
- AMQPコンシューマ削除
- REST polling追加
- タイマーベースでdequeue呼び出し

---

## 推定進捗

| Phase | 予定時間 | 実績時間 | 状況 |
|-------|---------|---------|------|
| Phase 1: 環境構築 | 4-6h | 0.5h | 🟡 進行中 |
| Phase 2: C実装調査 | 2-3h | - | ⚪ 未着手 |
| Phase 3: Python実装 | 6-8h | - | ⚪ 未着手 |
| Phase 4: Camel統合 | 3-4h | - | ⚪ 未着手 |
| **合計** | **15-21h** | **0.5h** | **2%** |

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
