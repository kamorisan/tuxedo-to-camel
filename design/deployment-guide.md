# デプロイメントガイド

## 前提条件

### 必要な環境
- OpenShift Cluster (4.12以降)
- `oc` CLI インストール済み
- Ansible 2.14以降
- GitHub アカウント
- コンテナレジストリ（Quay.io または Docker Hub）

### OpenShift接続確認
```bash
# ログイン確認
oc whoami
oc version

# クラスター情報
oc cluster-info
```

## クイックスタート

### 1. リポジトリクローン
```bash
git clone https://github.com/kamorisan/tuxedo-to-camel.git
cd tuxedo-to-camel
```

### 2. Ansible環境設定
```bash
# Python仮想環境作成（推奨）
python3 -m venv venv
source venv/bin/activate

# Ansible依存関係インストール
pip install ansible kubernetes openshift

# Ansible Collection インストール
ansible-galaxy collection install kubernetes.core
ansible-galaxy collection install community.general
```

### 3. 変数設定
```bash
# group_vars/all.yml を編集
vi ansible/group_vars/all.yml
```

設定例:
```yaml
---
# OpenShift接続情報
openshift_api_url: "https://api.your-cluster.example.com:6443"
openshift_token: "{{ lookup('env', 'OC_TOKEN') }}"  # または直接指定

# コンテナイメージレジストリ
container_registry: "quay.io"
registry_namespace: "your-username"

# Namespace
namespaces:
  - demo-webui
  - demo-tuxedo
  - demo-camel
  - demo-kafka

# Operator設定
install_amq_streams_operator: true
amq_streams_version: "2.6.0"
install_kafka_console: true

# Kafka設定
kafka_cluster_name: "demo-cluster"
kafka_topic_name: "demo-messages"
kafka_partitions: 1
kafka_replicas: 1

# AMQ設定
amq_broker_name: "amq-broker"
amq_queue_name: "TUXEDO.OUT"

# イメージタグ
image_tag: "latest"
```

### 4. デプロイ実行

**全コンポーネント一括デプロイ:**
```bash
cd ansible
ansible-playbook playbooks/site.yml
```

**段階的デプロイ:**
```bash
# Step 1: Operatorインストール
ansible-playbook playbooks/00-setup-operators.yml

# Step 2: Namespaceチェック
oc get projects | grep demo-

# Step 3: Kafka構築
ansible-playbook playbooks/02-deploy-kafka.yml

# Kafka準備完了待機
oc wait kafka/demo-cluster --for=condition=Ready --timeout=300s -n demo-kafka

# Step 4: AMQ構築
ansible-playbook playbooks/03-deploy-amq.yml

# Step 5: アプリケーションデプロイ
ansible-playbook playbooks/04-deploy-tuxedo.yml
ansible-playbook playbooks/05-deploy-camel.yml
ansible-playbook playbooks/06-deploy-webui.yml
```

### 5. 動作確認

**Pod状態確認:**
```bash
# 全Namespace確認
for ns in demo-webui demo-tuxedo demo-camel demo-kafka; do
  echo "=== $ns ==="
  oc get pods -n $ns
done
```

期待される出力:
```
=== demo-webui ===
NAME                     READY   STATUS    RESTARTS   AGE
webui-xxxxxxxxxx-xxxxx   1/1     Running   0          2m

=== demo-tuxedo ===
NAME                              READY   STATUS    RESTARTS   AGE
tuxedo-service-xxxxxxxxxx-xxxxx   1/1     Running   0          3m
amq-broker-ss-0                   1/1     Running   0          4m

=== demo-camel ===
NAME                                 READY   STATUS    RESTARTS   AGE
camel-integration-xxxxxxxxxx-xxxxx   1/1     Running   0          2m

=== demo-kafka ===
NAME                                        READY   STATUS    RESTARTS   AGE
demo-cluster-kafka-0                        1/1     Running   0          5m
demo-cluster-zookeeper-0                    1/1     Running   0          6m
demo-cluster-entity-operator-xxxxx-xxxxx    2/2     Running   0          4m
```

**Service確認:**
```bash
oc get svc -n demo-webui
oc get svc -n demo-tuxedo
oc get svc -n demo-camel
oc get svc -n demo-kafka
```

**Route確認（外部アクセス用）:**
```bash
# WebUI URL取得
WEBUI_URL=$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')
echo "Web UI: http://${WEBUI_URL}"

# Kafka Console URL取得
KAFKA_CONSOLE_URL=$(oc get route kafka-console -n demo-kafka -o jsonpath='{.spec.host}')
echo "Kafka Console: http://${KAFKA_CONSOLE_URL}"
```

### 6. デモ実行

1. **WebUIにアクセス**
   ```bash
   open http://${WEBUI_URL}
   ```

2. **メッセージ送信**
   - テキストボックスに "Hello Tuxedo Demo!" を入力
   - "Send" ボタンをクリック

3. **ログ確認**
   
   別ターミナルで各コンポーネントのログを並行表示:
   ```bash
   # Terminal 1: WebUI
   oc logs -f deployment/webui -n demo-webui
   
   # Terminal 2: Tuxedo
   oc logs -f deployment/tuxedo-service -n demo-tuxedo
   
   # Terminal 3: Camel
   oc logs -f deployment/camel-integration -n demo-camel
   
   # Terminal 4: Kafka Consumer
   oc run kafka-consumer -ti --image=quay.io/strimzi/kafka:latest-kafka-3.6.0 \
     --rm=true --restart=Never -n demo-kafka -- \
     bin/kafka-console-consumer.sh \
     --bootstrap-server demo-cluster-kafka-bootstrap:9092 \
     --topic demo-messages --from-beginning
   ```

4. **Kafka Consoleで確認**
   ```bash
   open http://${KAFKA_CONSOLE_URL}
   ```
   - Topic: `demo-messages` を選択
   - メッセージ一覧を確認

## トラブルシューティング

### Podが起動しない

```bash
# Pod詳細確認
oc describe pod <pod-name> -n <namespace>

# イベント確認
oc get events -n <namespace> --sort-by='.lastTimestamp'

# ログ確認
oc logs <pod-name> -n <namespace>
```

### ImagePullBackOff エラー

```bash
# イメージPull Secret作成（プライベートレジストリの場合）
oc create secret docker-registry registry-secret \
  --docker-server=quay.io \
  --docker-username=<username> \
  --docker-password=<password> \
  --docker-email=<email> \
  -n <namespace>

# ServiceAccountにSecretリンク
oc secrets link default registry-secret --for=pull -n <namespace>
```

### Kafka接続エラー

```bash
# Kafka状態確認
oc get kafka demo-cluster -n demo-kafka -o yaml

# Kafka Bootstrap Service確認
oc get svc demo-cluster-kafka-bootstrap -n demo-kafka

# Kafka Listener確認
oc exec -it demo-cluster-kafka-0 -n demo-kafka -- \
  bin/kafka-broker-api-versions.sh \
  --bootstrap-server localhost:9092
```

### AMQ接続エラー

```bash
# AMQ Broker状態確認
oc get statefulset amq-broker-ss -n demo-tuxedo
oc logs amq-broker-ss-0 -n demo-tuxedo

# Queue確認（Broker内）
oc exec -it amq-broker-ss-0 -n demo-tuxedo -- \
  /opt/amq/bin/artemis queue stat --url tcp://localhost:61616
```

### Camelルートエラー

```bash
# Camel Route状態確認
oc logs deployment/camel-integration -n demo-camel | grep -i "route"

# AMQ接続テスト
oc exec -it deployment/camel-integration -n demo-camel -- \
  curl -v telnet://amq-broker.demo-tuxedo.svc.cluster.local:5672
```

## クリーンアップ

### 全削除
```bash
cd ansible
ansible-playbook playbooks/99-cleanup.yml
```

### 手動削除
```bash
# Namespace削除
oc delete project demo-webui demo-tuxedo demo-camel demo-kafka

# Operator削除（注意: 他のプロジェクトで使用していないか確認）
oc delete subscription amq-streams -n openshift-operators
oc delete csv $(oc get csv -n openshift-operators | grep amq-streams | awk '{print $1}') -n openshift-operators

# CRD削除（完全削除の場合のみ）
oc get crd | grep kafka.strimzi.io | awk '{print $1}' | xargs oc delete crd
```

### 部分削除
```bash
# アプリケーションのみ削除（Operatorは残す）
oc delete all --all -n demo-webui
oc delete all --all -n demo-tuxedo
oc delete all --all -n demo-camel
oc delete kafka demo-cluster -n demo-kafka
```

## 再デプロイ

### 既存環境での更新

**コンテナイメージ更新:**
```bash
# 新しいイメージビルド後
oc set image deployment/webui webui=quay.io/your-username/webui:v1.1.0 -n demo-webui
oc set image deployment/tuxedo-service tuxedo=quay.io/your-username/tuxedo:v1.1.0 -n demo-tuxedo
oc set image deployment/camel-integration camel=quay.io/your-username/camel:v1.1.0 -n demo-camel

# Rollout状態確認
oc rollout status deployment/webui -n demo-webui
```

**設定変更:**
```bash
# ConfigMap更新
oc edit configmap tuxedo-config -n demo-tuxedo

# Pod再起動（ConfigMap変更を反映）
oc rollout restart deployment/tuxedo-service -n demo-tuxedo
```

**Ansibleで再デプロイ:**
```bash
# 特定コンポーネントのみ
ansible-playbook playbooks/04-deploy-tuxedo.yml --extra-vars "image_tag=v1.1.0"

# 全体再デプロイ
ansible-playbook playbooks/site.yml --skip-tags "operators,namespaces"
```

## パフォーマンステスト

### 負荷テスト

**WebUIへの連続送信:**
```bash
WEBUI_URL=$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}')

for i in {1..100}; do
  curl -X POST http://${WEBUI_URL}/api/send \
    -H "Content-Type: text/plain" \
    -d "Test message $i"
  echo "Sent message $i"
  sleep 0.1
done
```

**Kafkaメッセージ数確認:**
```bash
oc exec -it demo-cluster-kafka-0 -n demo-kafka -- \
  bin/kafka-run-class.sh kafka.tools.GetOffsetShell \
  --broker-list localhost:9092 \
  --topic demo-messages
```

### メトリクス確認

**Pod リソース使用状況:**
```bash
oc adm top pods -n demo-webui
oc adm top pods -n demo-tuxedo
oc adm top pods -n demo-camel
oc adm top pods -n demo-kafka
```

**Kafka lag確認:**
```bash
oc exec -it demo-cluster-kafka-0 -n demo-kafka -- \
  bin/kafka-consumer-groups.sh \
  --bootstrap-server localhost:9092 \
  --describe --group camel-consumer-group
```

## ベストプラクティス

### セキュリティ

1. **Secret管理**
   - 本番環境では外部Vault使用推奨（HashiCorp Vault、OpenShift Vault）
   - 環境変数からトークン読み込み
   ```bash
   export OC_TOKEN=$(oc whoami -t)
   ansible-playbook playbooks/site.yml
   ```

2. **NetworkPolicy適用**
   ```bash
   # 最小権限の原則
   oc apply -f network-policies/ -n demo-tuxedo
   ```

3. **RBAC設定**
   - 専用ServiceAccount使用
   - 必要最小限のRole付与

### 可用性

1. **Replica設定**
   ```yaml
   # 本番環境用（各Deployment）
   spec:
     replicas: 2  # 最小2以上
   ```

2. **Resource Limits**
   ```yaml
   resources:
     requests:
       memory: "256Mi"
       cpu: "100m"
     limits:
       memory: "512Mi"
       cpu: "500m"
   ```

3. **Health Check**
   ```yaml
   livenessProbe:
     httpGet:
       path: /health/live
       port: 8080
     initialDelaySeconds: 30
   readinessProbe:
     httpGet:
       path: /health/ready
       port: 8080
     initialDelaySeconds: 10
   ```

### 運用

1. **ログ集約**
   - OpenShift Logging Operator導入
   - Elasticsearch / Kibana活用

2. **監視**
   - Prometheus / Grafana
   - カスタムメトリクス公開

3. **バックアップ**
   - Kafka Topic定期バックアップ
   - ConfigMap/Secret バックアップ

## 参考情報

### ドキュメント
- [Red Hat AMQ Streams Documentation](https://access.redhat.com/documentation/en-us/red_hat_amq_streams/)
- [Apache Camel on Quarkus](https://camel.apache.org/camel-quarkus/latest/)
- [Quarkus Guides](https://quarkus.io/guides/)

### リポジトリ構成
```
tuxedo-to-camel/
├── README.md
├── design/
│   ├── architecture.md          # このファイル
│   └── deployment-guide.md      # このガイド
├── components/
│   ├── webui/                   # Quarkus WebUI
│   ├── tuxedo/                  # Tuxedo Service
│   └── camel/                   # Apache Camel
├── ansible/
│   ├── playbooks/
│   ├── roles/
│   ├── inventory/
│   └── group_vars/
├── openshift/
│   ├── templates/
│   └── network-policies/
└── .github/
    └── workflows/               # CI/CD
```
