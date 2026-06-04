# Tuxedo to Camel Demo - 設計ドキュメント

このディレクトリには、Tuxedoシステムを経由してKafkaへメッセージを送信するデモシステムの設計ドキュメントが含まれています。

## ドキュメント一覧

### 1. [アーキテクチャ設計](./architecture.md)
システム全体のアーキテクチャ、コンポーネント構成、メッセージフロー、インフラ設計を記載しています。

**主な内容:**
- システム構成図
- メッセージフロー詳細
- コンポーネント概要
- Namespace設計
- デプロイメント戦略
- モニタリング・ログ設計

**対象読者:** アーキテクト、プロジェクトマネージャー、技術リード

### 2. [デプロイメントガイド](./deployment-guide.md)
実際にOpenShift環境へデプロイする手順、運用方法、トラブルシューティングを記載しています。

**主な内容:**
- クイックスタートガイド
- Ansible設定方法
- 段階的デプロイ手順
- 動作確認方法
- トラブルシューティング
- クリーンアップ手順

**対象読者:** 運用エンジニア、DevOpsエンジニア、実装担当者

### 3. [コンポーネント仕様書](./component-specs.md)
各コンポーネントの詳細な技術仕様、API定義、実装例を記載しています。

**主な内容:**
- Quarkus Web UI 仕様
- Tuxedo Service 仕様
- Apache Camel Integration 仕様
- インフラコンポーネント設定
- OpenShift Deployment定義
- メッセージフォーマット詳細

**対象読者:** 開発エンジニア、実装担当者

## デモシステム概要

### 目的
レガシーシステム（Tuxedo）からモダンなメッセージング基盤（Kafka）への移行パターンを実演し、OpenShift上でのコンテナベース統合アーキテクチャを示します。

### システムフロー

```
[ユーザー] 
    ↓ (Web入力)
[Quarkus WebUI] (demo-webui namespace)
    ↓ (HTTP POST)
[Tuxedo Service] (demo-tuxedo namespace)
    ↓ (AMQ Queue)
[Red Hat AMQ] (demo-tuxedo namespace)
    ↓ (AMQP)
[Apache Camel] (demo-camel namespace)
    ↓ (Kafka Producer)
[Kafka Broker] (demo-kafka namespace)
    ↓
[Kafka Console] (確認用)
```

### 主な技術スタック

| コンポーネント | 技術 |
|--------------|------|
| WebUI | Quarkus 3.x, Java 17 |
| Tuxedo Service | Python 3.11, Flask |
| Integration | Apache Camel 4.x, Camel Quarkus |
| Message Queue | Red Hat AMQ (ActiveMQ Artemis) |
| Message Broker | Kafka (AMQ Streams / Strimzi) |
| Orchestration | OpenShift 4.12+ |
| IaC | Ansible 2.14+ |

## 使い方

### 初めて読む方へ

1. **まず [architecture.md](./architecture.md) を読む**
   - システム全体像を理解
   - 各コンポーネントの役割を把握

2. **[deployment-guide.md](./deployment-guide.md) でデプロイを試す**
   - 環境準備
   - 実際のデプロイ手順を実行
   - デモ動作確認

3. **[component-specs.md](./component-specs.md) で詳細を学ぶ**
   - 実装の詳細を理解
   - カスタマイズや拡張の参考に

### 実装を始める方へ

1. **component-specs.md の該当セクションを参照**
   - API仕様
   - 実装サンプルコード
   - 設定ファイル例

2. **deployment-guide.md でデプロイ設定を確認**
   - ConfigMap/Secret設定
   - Deployment YAML
   - 環境変数

3. **architecture.md で全体設計を再確認**
   - 他コンポーネントとの連携
   - メッセージフロー
   - エラーハンドリング

## 主要な設計決定事項

### 1. Namespace分離
- **決定:** 各コンポーネントを独立したNamespaceに配置
- **理由:** 
  - 責任範囲の明確化
  - リソース分離とセキュリティ境界
  - 段階的デプロイ・削除の容易性

### 2. メッセージ形式
- **決定:** 
  - Web → Tuxedo → AMQ: Plain text
  - Camel → Kafka: JSON
- **理由:**
  - デモのシンプルさ
  - Camelでの変換パターン実演
  - Kafka側での構造化データ利用

### 3. Red Hat AMQ (ActiveMQ Artemis) 選定
- **決定:** MQとして Red Hat AMQ を使用
- **理由:**
  - Red Hat サポート対象
  - OpenShift Operator 対応
  - AMQP 1.0 標準プロトコル対応
  - `/Q` (Queue) パターンとの親和性

### 4. Ansible による IaC
- **決定:** デプロイ自動化に Ansible を使用
- **理由:**
  - 宣言的な設定管理
  - べき等性の保証
  - OpenShift環境変更時の再現性
  - CI/CDパイプラインへの統合容易性

### 5. コンテナイメージビルド戦略
- **推奨:** GitHub Actions → Quay.io/Docker Hub
- **理由:**
  - ビルド環境の外部化
  - イメージバージョン管理
  - OpenShift非依存のビルドプロセス

## 設計原則

### 疎結合
- 各コンポーネントは独立してデプロイ・スケール可能
- Service間通信はOpenShift DNS経由
- メッセージキューによる非同期連携

### 可視性
- 全コンポーネントで統一されたログフォーマット
- 各処理段階でのログ出力
- Kafka Consoleでの最終確認

### シンプルさ
- デモとしての分かりやすさを優先
- 複雑なエラーハンドリング・リトライは最小限
- 認証・暗号化は基本設定（本番環境では強化が必要）

### 再現性
- Ansibleによる完全自動デプロイ
- 環境変数・ConfigMapでの設定外部化
- ステートレスなアプリケーション設計

## 今後の拡張方向

### Phase 2: 信頼性向上
- エラーハンドリング強化
- Dead Letter Queue 導入
- リトライロジック実装
- Circuit Breaker パターン

### Phase 3: 監視・運用
- OpenTelemetry 分散トレーシング
- Prometheus/Grafana メトリクス
- アラート設定
- ログ集約（ELK Stack）

### Phase 4: スケーラビリティ
- Horizontal Pod Autoscaler
- Kafka パーティション増加
- AMQ クラスタリング
- 負荷試験・性能チューニング

### Phase 5: セキュリティ強化
- mTLS 通信
- OAuth2/OIDC 認証
- NetworkPolicy 厳密化
- Secret管理（Vault連携）

## 参考資料

### OpenShift
- [Red Hat OpenShift Documentation](https://docs.openshift.com/)
- [OpenShift CLI (oc) Reference](https://docs.openshift.com/container-platform/latest/cli_reference/openshift_cli/getting-started-cli.html)

### Quarkus
- [Quarkus Guides](https://quarkus.io/guides/)
- [Quarkus REST Client Guide](https://quarkus.io/guides/rest-client)

### Apache Camel
- [Camel Quarkus Documentation](https://camel.apache.org/camel-quarkus/latest/)
- [Camel AMQP Component](https://camel.apache.org/components/latest/amqp-component.html)
- [Camel Kafka Component](https://camel.apache.org/components/latest/kafka-component.html)

### Red Hat AMQ
- [Red Hat AMQ Documentation](https://access.redhat.com/documentation/en-us/red_hat_amq/)
- [AMQ Broker on OpenShift](https://access.redhat.com/documentation/en-us/red_hat_amq_broker/)

### Kafka / AMQ Streams
- [Red Hat AMQ Streams Documentation](https://access.redhat.com/documentation/en-us/red_hat_amq_streams/)
- [Strimzi Documentation](https://strimzi.io/docs/operators/latest/overview.html)

### Ansible
- [Ansible Documentation](https://docs.ansible.com/)
- [kubernetes.core Collection](https://docs.ansible.com/ansible/latest/collections/kubernetes/core/)

## ライセンス

このデモプロジェクトはサンプルコードであり、教育・検証目的で使用してください。

## 貢献・フィードバック

GitHub Issues または Pull Requests でフィードバックをお待ちしています。

Repository: https://github.com/kamorisan/tuxedo-to-camel

---

**Document Version:** 1.0  
**Last Updated:** 2026-06-04  
**Authors:** Demo Development Team
