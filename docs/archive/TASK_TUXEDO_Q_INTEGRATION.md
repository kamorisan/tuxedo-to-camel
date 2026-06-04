# Task: Tuxedo /Q 統合

## 概要

AMQ Broker の代わりに Tuxedo /Q (Queue) を使用したメッセージング

**現在:** Tuxedo Mock → AMQ Broker → Camel  
**変更後:** Tuxedo /Q ← メッセージ投入 → Camel が /Q から取得

## アーキテクチャ変更

```
┌─────────┐     HTTP      ┌──────────────┐
│ WebUI   │─────────────>│ Tuxedo       │
└─────────┘               │ (HTTP受信)   │
                          └──────┬───────┘
                                 │ enqueue
                                 ↓
                          ┌──────────────┐
                          │ Tuxedo /Q    │
                          │ (QSPACE)     │
                          └──────┬───────┘
                                 │ dequeue
                                 ↓
                          ┌──────────────┐
                          │ Camel        │
                          │ (JCA/JMS?)   │
                          └──────┬───────┘
                                 │
                                 ↓
                          ┌──────────────┐
                          │ Kafka        │
                          └──────────────┘
```

---

## 技術的検討事項

### 1. Tuxedo /Q とは

**Tuxedo Queue (TMQUEUE):**
- Tuxedo組み込みの永続キュー
- QSPACE (Queue Space) で管理
- ATMI API で操作: `tpenqueue()`, `tpdequeue()`
- トランザクション対応

### 2. Camel から Tuxedo /Q へのアクセス方法

#### オプション A: Tuxedo JCA Adapter (商用)
- Oracle WebLogic Tuxedo Connector
- **課題**: ライセンス、複雑な設定

#### オプション B: JMS Bridge (カスタム)
- Tuxedo側でJMSブリッジサービス実装
- **課題**: 追加開発必要

#### オプション C: REST API (推奨)
- Tuxedo側でREST APIサーバー
  - POST /enqueue - キュー投入
  - GET /dequeue - キュー取得
- Camel で HTTP Polling
- **メリット**: シンプル、実装容易

#### オプション D: Tuxedo WTC/WLS (複雑)
- WebLogic Server経由
- **課題**: 大規模環境設定必要

---

## タスクブレイクダウン

### Phase 1: Tuxedo /Q 環境構築 (推定: 4-6時間)

#### 1.1 QSPACE 設定 (2時間)
- [ ] ubbconfig に QSPACE定義追加
  ```
  *SERVERS
  QSPACE   SRVGRP=GROUP1 SRVID=1
           CLOPT="-s QSPACE:MYQUEUE"
  ```
- [ ] tmloadcf でコンフィグロード
- [ ] tmboot で QSPACE起動確認

#### 1.2 Queueテスト (1-2時間)
- [ ] サンプルenqueueプログラム作成
  ```c
  tpenqueue("MYQUEUE", &qctl, buffer, len, 0);
  ```
- [ ] サンプルdequeueプログラム作成
  ```c
  tpdequeue("MYQUEUE", &qctl, &buffer, &len, 0);
  ```
- [ ] 動作確認

#### 1.3 Python bindings調査 (1-2時間)
- [ ] PythonからTuxedo /Q 呼び出し可能か調査
  - ctypes でlibatmi.so 呼び出し
  - または subprocess で tmadmin 経由
- [ ] 実装方法決定

---

### Phase 2: Tuxedo側実装 (推定: 6-8時間)

#### オプション A: Python実装 (推奨 - 3-4時間)

**2.1 ctypes でATMI呼び出し**
```python
from ctypes import *

libtux = CDLL("/u01/oracle/tuxHome/tuxedo22.1.1.0.0/lib/libtux.so")
libtux.tpenqueue.argtypes = [c_char_p, c_void_p, c_char_p, c_long, c_long]
libtux.tpenqueue.restype = c_int

def enqueue_message(queue_name, message):
    # QCTL構造体作成
    # tpenqueue() 呼び出し
    pass
```

**課題:**
- TPQCTL構造体の定義
- バッファ管理
- エラーハンドリング

**2.2 REST API実装**
```python
@app.route('/dequeue', methods=['GET'])
def dequeue():
    # tpdequeue() でメッセージ取得
    # JSON で返却
```

#### オプション B: C実装 (8-12時間)
- [ ] QSPACEサーバープログラム
- [ ] enqueue/dequeue ラッパーサービス
- [ ] HTTP/REST インターフェース

---

### Phase 3: Camel統合 (推定: 3-4時間)

#### 3.1 Camel Route変更 (2時間)

**現在:**
```java
from("amqp:queue:TUXEDO.OUT")
  .to("kafka:demo-messages")
```

**変更後（REST Polling）:**
```java
from("timer:tuxedo-poll?period=1000")
  .to("http://tuxedo-msgsvc:8080/dequeue")
  .choice()
    .when(body().isNotNull())
      .unmarshal().json()
      .to("kafka:demo-messages")
```

#### 3.2 エラーハンドリング (1時間)
- [ ] キューが空の場合の処理
- [ ] Tuxedoダウン時のリトライ
- [ ] デッドレターキュー設定

#### 3.3 テスト (1時間)
- [ ] メッセージフロー確認
- [ ] パフォーマンステスト

---

### Phase 4: メッセージ投入側実装 (推定: 2-3時間)

#### 4.1 WebUI → Tuxedo enqueue (1-2時間)

**現在:** Tuxedo Mock が AMQPで送信  
**変更後:** Tuxedo Mock が /Q に enqueue

```python
# msgsvc-mock.py
def handle_post(message):
    # tpenqueue() または REST API 呼び出し
    enqueue_to_tuxedo_q("MYQUEUE", message)
```

#### 4.2 統合テスト (1時間)
- [ ] WebUI → enqueue → /Q → Camel dequeue → Kafka
- [ ] 全フロー動作確認

---

## 推定作業時間

| Phase | 作業時間 | 実装方式 |
|-------|---------|---------|
| Phase 1: /Q環境構築 | 4-6時間 | QSPACE設定 |
| Phase 2: Tuxedo実装 | 3-4時間 (Python) / 8-12時間 (C) | enqueue/dequeue |
| Phase 3: Camel統合 | 3-4時間 | REST polling |
| Phase 4: 投入側実装 | 2-3時間 | WebUI連携 |
| **合計 (Python)** | **12-17時間** | ⭐⭐⭐ |
| **合計 (C)** | **17-25時間** | ⭐⭐⭐⭐⭐ |

---

## 技術的課題

### 高リスク
1. **PythonからTuxedo ATMI呼び出し**
   - ctypes で可能だが複雑
   - 構造体マッピング
   - メモリ管理

2. **QSPACE の永続化設定**
   - デモでは ephemeral で良いが設定必要

### 中リスク
3. **Camel Polling パフォーマンス**
   - REST polling は効率悪い
   - WebSocketや Server-Sent Events も検討

4. **トランザクション管理**
   - /Qはトランザクション対応だが、Camel連携で活かせるか

---

## 推奨実装方式

### Phase-by-Phase アプローチ

**Phase 1 (短期 - 4-6時間):**
- QSPACE環境構築
- サンプルC プログラムで動作確認
- **成果物**: 動作するTuxedo /Q

**Phase 2 (中期 - 8-12時間):**
- Python ctypes 実装調査・PoC
- または、シンプルなC実装
- REST API エンドポイント実装
- **成果物**: enqueue/dequeue REST API

**Phase 3 (短期 - 3-4時間):**
- Camel Route 変更
- 統合テスト
- **成果物**: 動作する統合システム

---

## 成功基準

- [ ] Tuxedo /Q が起動・動作
- [ ] REST API でenqueue/dequeue可能
- [ ] Camel が /Q からメッセージ取得
- [ ] Kafka にメッセージ配信
- [ ] エンドツーエンドフロー動作確認

---

## 次のステップ判断

### オプション 1: Tuxedo /Q 統合を実施
- **推奨**: "本物のTuxedo"感が増す
- **工数**: 12-17時間（Python）
- **価値**: Tuxedo特有機能のデモ

### オプション 2: 現状維持（AMQ使用）
- **推奨**: デモとしては既に完成
- **工数**: 0時間
- **価値**: 時間を他のタスクに

### オプション 3: C実装への移行
- **推奨**: 要件次第
- **工数**: 14-25時間（/Q含む）
- **価値**: 本番相当の実装

---

## 判断軸

| 要素 | AMQ (現状) | /Q (Python) | /Q (C言語) |
|------|-----------|------------|-----------|
| デモ完成度 | ✅ 100% | ✅ 100% | ✅ 100% |
| Tuxedoらしさ | ⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 実装工数 | 0h | 12-17h | 20-30h |
| メンテナンス性 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| パフォーマンス | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| 本番適用可能性 | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ |

**推奨**: デモ目的なら現状維持、Tuxedo機能デモなら /Q (Python)、本番想定なら /Q (C)
