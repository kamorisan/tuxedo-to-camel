# コンポーネント仕様書

## 1. Quarkus Web UI

### 概要
ユーザーがメッセージを入力してTuxedoシステムへ送信するためのシンプルなWebインターフェース。

### 技術スタック
- **Framework**: Quarkus 3.x
- **Template Engine**: Qute
- **REST Client**: RESTEasy Reactive
- **Language**: Java 17
- **Build Tool**: Maven

### ディレクトリ構成
```
components/webui/
├── src/
│   ├── main/
│   │   ├── java/
│   │   │   └── com/example/webui/
│   │   │       ├── MessageResource.java       # REST API
│   │   │       ├── TuxedoClient.java          # Tuxedo HTTP Client
│   │   │       └── MessageService.java        # ビジネスロジック
│   │   ├── resources/
│   │   │   ├── templates/
│   │   │   │   └── index.html                 # メイン画面
│   │   │   └── application.properties         # 設定
│   │   └── docker/
│   │       └── Dockerfile.jvm
│   └── test/
│       └── java/
├── pom.xml
└── README.md
```

### API仕様

#### `GET /`
メッセージ入力画面を表示

**Response:**
- Content-Type: `text/html`
- Body: HTML form

**HTML構成:**
```html
<!DOCTYPE html>
<html>
<head>
    <title>Tuxedo Message Sender</title>
    <style>
        /* シンプルなスタイリング */
    </style>
</head>
<body>
    <h1>Send Message to Tuxedo</h1>
    <form id="messageForm">
        <label for="message">Message:</label>
        <input type="text" id="message" name="message" required>
        <button type="submit">Send</button>
    </form>
    <div id="result"></div>
    
    <script>
        // AJAX送信処理
        document.getElementById('messageForm').addEventListener('submit', async (e) => {
            e.preventDefault();
            const message = document.getElementById('message').value;
            
            try {
                const response = await fetch('/api/send', {
                    method: 'POST',
                    headers: {'Content-Type': 'text/plain'},
                    body: message
                });
                
                const result = await response.json();
                document.getElementById('result').innerHTML = 
                    `<p style="color: green;">Success: ${result.message}</p>`;
            } catch (error) {
                document.getElementById('result').innerHTML = 
                    `<p style="color: red;">Error: ${error.message}</p>`;
            }
        });
    </script>
</body>
</html>
```

#### `POST /api/send`
メッセージをTuxedoへ送信

**Request:**
- Content-Type: `text/plain`
- Body: メッセージ文字列（例: "Hello World!"）

**Response:**
- Content-Type: `application/json`
- Status: 200 OK
```json
{
  "status": "success",
  "message": "Message sent to Tuxedo",
  "timestamp": "2026-06-04T10:30:00Z"
}
```

**Error Response:**
- Status: 500 Internal Server Error
```json
{
  "status": "error",
  "message": "Failed to connect to Tuxedo service",
  "timestamp": "2026-06-04T10:30:00Z"
}
```

### Java実装例

#### MessageResource.java
```java
package com.example.webui;

import jakarta.inject.Inject;
import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import org.jboss.logging.Logger;

import java.time.Instant;
import java.util.Map;

@Path("/api")
@Produces(MediaType.APPLICATION_JSON)
public class MessageResource {

    private static final Logger LOG = Logger.getLogger(MessageResource.class);

    @Inject
    MessageService messageService;

    @POST
    @Path("/send")
    @Consumes(MediaType.TEXT_PLAIN)
    public Response sendMessage(String message) {
        LOG.infof("[WebUI] Received: %s", message);
        
        try {
            messageService.sendToTuxedo(message);
            LOG.infof("[WebUI] Sent to Tuxedo: %s", message);
            
            return Response.ok(Map.of(
                "status", "success",
                "message", "Message sent to Tuxedo",
                "timestamp", Instant.now().toString()
            )).build();
            
        } catch (Exception e) {
            LOG.errorf(e, "[WebUI] Failed to send message: %s", message);
            
            return Response.serverError().entity(Map.of(
                "status", "error",
                "message", "Failed to connect to Tuxedo service: " + e.getMessage(),
                "timestamp", Instant.now().toString()
            )).build();
        }
    }
}
```

#### MessageService.java
```java
package com.example.webui;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.eclipse.microprofile.rest.client.inject.RestClient;

@ApplicationScoped
public class MessageService {

    @Inject
    @RestClient
    TuxedoClient tuxedoClient;

    public void sendToTuxedo(String message) {
        tuxedoClient.receive(message);
    }
}
```

#### TuxedoClient.java
```java
package com.example.webui;

import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.core.MediaType;
import org.eclipse.microprofile.rest.client.inject.RegisterRestClient;

@Path("/api")
@RegisterRestClient(configKey = "tuxedo-api")
public interface TuxedoClient {

    @POST
    @Path("/receive")
    @Consumes(MediaType.TEXT_PLAIN)
    void receive(String message);
}
```

### 設定ファイル

#### application.properties
```properties
# Application
quarkus.application.name=webui
quarkus.http.port=8080

# Tuxedo Client
quarkus.rest-client.tuxedo-api.url=http://tuxedo-service.demo-tuxedo.svc.cluster.local:8080
quarkus.rest-client.tuxedo-api.scope=jakarta.inject.Singleton

# Logging
quarkus.log.console.format=%d{HH:mm:ss} %-5p [%c{2.}] (%t) %s%e%n
quarkus.log.level=INFO
quarkus.log.category."com.example.webui".level=DEBUG

# Health
quarkus.smallrye-health.root-path=/health
```

### コンテナイメージ

#### Dockerfile.jvm
```dockerfile
FROM registry.access.redhat.com/ubi9/openjdk-17:latest

ENV LANGUAGE='en_US:en'

COPY --chown=185 target/quarkus-app/lib/ /deployments/lib/
COPY --chown=185 target/quarkus-app/*.jar /deployments/
COPY --chown=185 target/quarkus-app/app/ /deployments/app/
COPY --chown=185 target/quarkus-app/quarkus/ /deployments/quarkus/

EXPOSE 8080
USER 185

ENV JAVA_OPTS="-Dquarkus.http.host=0.0.0.0 -Djava.util.logging.manager=org.jboss.logmanager.LogManager"
ENV JAVA_APP_JAR="/deployments/quarkus-run.jar"
```

---

## 2. Tuxedo Service

### 概要
Tuxedoシミュレーションサービス。メッセージを受信し、AMQキューへ転送する。

### 技術スタック
- **Language**: C言語
- **Tuxedo Framework**: Oracle Tuxedo
- **AMQ Client**: AMQP C Client Library
- **Build Tool**: Makefile

### ディレクトリ構成
```
components/tuxedo/
├── src/
│   ├── app.py                    # メインアプリケーション
│   ├── amq_client.py             # AMQ接続クライアント
│   └── config.py                 # 設定管理
├── requirements.txt
├── Dockerfile
└── README.md
```

### API仕様

#### `POST /api/receive`
WebUIからメッセージを受信しAMQへ転送

**Request:**
- Content-Type: `text/plain`
- Body: メッセージ文字列

**Response:**
- Status: 200 OK
```json
{
  "status": "success",
  "message": "Message queued to AMQ"
}
```

**Error Response:**
- Status: 500 Internal Server Error
```json
{
  "status": "error",
  "message": "AMQ connection failed"
}
```

#### `GET /health`
ヘルスチェックエンドポイント

**Response:**
```json
{
  "status": "healthy",
  "amq_connected": true
}
```

### Python実装例

#### app.py
```python
from flask import Flask, request, jsonify
import logging
from amq_client import AMQClient
from config import Config

# Logging設定
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - [%(name)s] - %(message)s'
)
logger = logging.getLogger(__name__)

app = Flask(__name__)
amq_client = AMQClient(
    host=Config.AMQ_HOST,
    port=Config.AMQ_PORT,
    queue=Config.AMQ_QUEUE
)

@app.route('/api/receive', methods=['POST'])
def receive_message():
    message = request.data.decode('utf-8')
    logger.info(f"[Tuxedo] Received: {message}")
    
    try:
        # Tuxedo内部処理（シミュレーション）
        # 実際のTuxedoではここでサービス呼び出しなどが入る
        processed_message = message.strip()
        
        # AMQキューへ送信
        amq_client.send(processed_message)
        logger.info(f"[Tuxedo] Queued to AMQ: {processed_message}")
        
        return jsonify({
            "status": "success",
            "message": "Message queued to AMQ"
        }), 200
        
    except Exception as e:
        logger.error(f"[Tuxedo] Error: {str(e)}")
        return jsonify({
            "status": "error",
            "message": str(e)
        }), 500

@app.route('/health', methods=['GET'])
def health():
    amq_connected = amq_client.is_connected()
    status = "healthy" if amq_connected else "unhealthy"
    
    return jsonify({
        "status": status,
        "amq_connected": amq_connected
    }), 200 if amq_connected else 503

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
```

#### amq_client.py
```python
from proton import Message
from proton.handlers import MessagingHandler
from proton.reactor import Container
import logging
import threading

logger = logging.getLogger(__name__)

class AMQSender(MessagingHandler):
    def __init__(self, url, queue):
        super(AMQSender, self).__init__()
        self.url = url
        self.queue = queue
        self.sender = None
        self.message_queue = []
        self.connected = False
        
    def on_start(self, event):
        conn = event.container.connect(self.url)
        self.sender = event.container.create_sender(conn, self.queue)
        
    def on_connection_opened(self, event):
        self.connected = True
        logger.info(f"[AMQ] Connected to {self.url}")
        
    def on_sendable(self, event):
        while self.message_queue and event.sender.credit:
            msg_text = self.message_queue.pop(0)
            msg = Message(body=msg_text)
            event.sender.send(msg)
            logger.debug(f"[AMQ] Sent: {msg_text}")
    
    def send(self, message):
        self.message_queue.append(message)

class AMQClient:
    def __init__(self, host, port, queue):
        self.url = f"amqp://{host}:{port}"
        self.queue = queue
        self.handler = AMQSender(self.url, self.queue)
        
        # バックグラウンドでContainer起動
        self.container_thread = threading.Thread(
            target=self._run_container,
            daemon=True
        )
        self.container_thread.start()
        
    def _run_container(self):
        try:
            Container(self.handler).run()
        except Exception as e:
            logger.error(f"[AMQ] Container error: {e}")
    
    def send(self, message):
        self.handler.send(message)
        
    def is_connected(self):
        return self.handler.connected
```

#### config.py
```python
import os

class Config:
    AMQ_HOST = os.getenv('AMQ_HOST', 'amq-broker.demo-tuxedo.svc.cluster.local')
    AMQ_PORT = int(os.getenv('AMQ_PORT', '5672'))
    AMQ_QUEUE = os.getenv('AMQ_QUEUE', 'TUXEDO.OUT')
    LOG_LEVEL = os.getenv('LOG_LEVEL', 'INFO')
```

#### requirements.txt
```
Flask==3.0.0
python-qpid-proton==0.38.0
gunicorn==21.2.0
```

### コンテナイメージ

#### Dockerfile
```dockerfile
FROM registry.access.redhat.com/ubi9/python-311:latest

USER 0

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY src/ .

USER 1001

EXPOSE 8080

CMD ["gunicorn", "--bind", "0.0.0.0:8080", "--workers", "2", "app:app"]
```

---

## 3. Apache Camel Integration

### 概要
AMQキューからメッセージを消費し、JSON変換してKafkaへ送信する統合サービス。

### 技術スタック
- **Framework**: Camel Quarkus 3.x
- **Camel Version**: 4.x
- **Components**: camel-amqp, camel-kafka, camel-jackson
- **Language**: Java 17

### ディレクトリ構成
```
components/camel/
├── src/
│   ├── main/
│   │   ├── java/
│   │   │   └── com/example/camel/
│   │   │       ├── routes/
│   │   │       │   └── TuxedoToKafkaRoute.java
│   │   │       └── processor/
│   │   │           └── MessageTransformer.java
│   │   └── resources/
│   │       └── application.properties
│   └── test/
├── pom.xml
└── README.md
```

### Camel Route実装

#### TuxedoToKafkaRoute.java
```java
package com.example.camel.routes;

import com.example.camel.processor.MessageTransformer;
import jakarta.enterprise.context.ApplicationScoped;
import org.apache.camel.builder.RouteBuilder;

@ApplicationScoped
public class TuxedoToKafkaRoute extends RouteBuilder {

    @Override
    public void configure() throws Exception {
        
        from("amqp:queue:{{amq.queue.name}}")
            .routeId("tuxedo-to-kafka")
            .log("[Camel] Consumed from queue: ${body}")
            
            // メッセージ変換
            .process(new MessageTransformer())
            .log("[Camel] Transformed: ${body}")
            
            // Kafkaへ送信
            .to("kafka:{{kafka.topic.name}}?brokers={{kafka.bootstrap.servers}}")
            .log("[Camel] Sent to Kafka topic: {{kafka.topic.name}}");
    }
}
```

#### MessageTransformer.java
```java
package com.example.camel.processor;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;
import org.apache.camel.Exchange;
import org.apache.camel.Processor;

import java.time.Instant;
import java.util.UUID;

public class MessageTransformer implements Processor {

    private final ObjectMapper objectMapper = new ObjectMapper();

    @Override
    public void process(Exchange exchange) throws Exception {
        String originalMessage = exchange.getIn().getBody(String.class);
        
        // JSON構造化
        ObjectNode json = objectMapper.createObjectNode();
        json.put("id", UUID.randomUUID().toString());
        json.put("timestamp", Instant.now().toString());
        json.put("message", originalMessage);
        
        String jsonString = objectMapper.writeValueAsString(json);
        exchange.getIn().setBody(jsonString);
    }
}
```

### 設定ファイル

#### application.properties
```properties
# Application
quarkus.application.name=camel-integration
camel.context.name=camel-integration

# AMQ Configuration
amq.url=amqp://amq-broker.demo-tuxedo.svc.cluster.local:5672
amq.queue.name=TUXEDO.OUT

# Kafka Configuration
kafka.bootstrap.servers=demo-cluster-kafka-bootstrap.demo-kafka.svc.cluster.local:9092
kafka.topic.name=demo-messages

# Camel AMQP Component
camel.component.amqp.connection-factory=#amqpConnectionFactory

# Logging
quarkus.log.console.format=%d{HH:mm:ss} %-5p [%c{2.}] (%t) %s%e%n
quarkus.log.level=INFO
quarkus.log.category."com.example.camel".level=DEBUG
quarkus.log.category."org.apache.camel".level=INFO

# Health & Metrics
quarkus.camel.health.enabled=true
quarkus.smallrye-health.root-path=/health
```

#### AMQP ConnectionFactory Bean
```java
package com.example.camel.config;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.enterprise.inject.Produces;
import jakarta.inject.Named;
import org.apache.qpid.jms.JmsConnectionFactory;

@ApplicationScoped
public class AMQConfig {

    @Produces
    @Named("amqpConnectionFactory")
    public JmsConnectionFactory createConnectionFactory() {
        String amqUrl = System.getenv().getOrDefault(
            "AMQ_URL", 
            "amqp://amq-broker.demo-tuxedo.svc.cluster.local:5672"
        );
        return new JmsConnectionFactory(amqUrl);
    }
}
```

### Maven依存関係（pom.xml抜粋）

```xml
<dependencies>
    <!-- Camel Quarkus -->
    <dependency>
        <groupId>org.apache.camel.quarkus</groupId>
        <artifactId>camel-quarkus-core</artifactId>
    </dependency>
    
    <!-- AMQP Component -->
    <dependency>
        <groupId>org.apache.camel.quarkus</groupId>
        <artifactId>camel-quarkus-amqp</artifactId>
    </dependency>
    
    <!-- Kafka Component -->
    <dependency>
        <groupId>org.apache.camel.quarkus</groupId>
        <artifactId>camel-quarkus-kafka</artifactId>
    </dependency>
    
    <!-- Jackson for JSON -->
    <dependency>
        <groupId>org.apache.camel.quarkus</groupId>
        <artifactId>camel-quarkus-jackson</artifactId>
    </dependency>
    
    <!-- Health -->
    <dependency>
        <groupId>io.quarkus</groupId>
        <artifactId>quarkus-smallrye-health</artifactId>
    </dependency>
</dependencies>
```

### コンテナイメージ

Dockerfile は WebUI と同様（Quarkus JVM モード）

---

## 4. インフラコンポーネント

### Red Hat AMQ (ActiveMQ Artemis)

#### Operator based deployment
```yaml
apiVersion: broker.amq.io/v1beta1
kind: ActiveMQArtemis
metadata:
  name: amq-broker
  namespace: demo-tuxedo
spec:
  deploymentPlan:
    size: 1
    image: registry.redhat.io/amq7/amq-broker-rhel8:7.11
    requireLogin: false
    persistenceEnabled: false
  acceptors:
    - name: amqp
      protocols: amqp
      port: 5672
  addressSettings:
    addressSetting:
      - match: '#'
        deadLetterAddress: DLQ
        expiryAddress: ExpiryQueue
```

#### Queue作成
```yaml
apiVersion: broker.amq.io/v1beta1
kind: ActiveMQArtemisAddress
metadata:
  name: tuxedo-out-queue
  namespace: demo-tuxedo
spec:
  addressName: TUXEDO.OUT
  queueName: TUXEDO.OUT
  routingType: anycast
```

### Kafka Cluster

#### Kafka CR
```yaml
apiVersion: kafka.strimzi.io/v1beta2
kind: Kafka
metadata:
  name: demo-cluster
  namespace: demo-kafka
spec:
  kafka:
    version: 3.6.0
    replicas: 1
    listeners:
      - name: plain
        port: 9092
        type: internal
        tls: false
    config:
      offsets.topic.replication.factor: 1
      transaction.state.log.replication.factor: 1
      transaction.state.log.min.isr: 1
    storage:
      type: ephemeral
  zookeeper:
    replicas: 1
    storage:
      type: ephemeral
  entityOperator:
    topicOperator: {}
    userOperator: {}
```

#### Topic作成
```yaml
apiVersion: kafka.strimzi.io/v1beta2
kind: KafkaTopic
metadata:
  name: demo-messages
  namespace: demo-kafka
  labels:
    strimzi.io/cluster: demo-cluster
spec:
  partitions: 1
  replicas: 1
  config:
    retention.ms: 86400000  # 1 day
    segment.bytes: 1073741824
```

---

## 5. OpenShift Deployment定義

### WebUI Deployment
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: webui
  namespace: demo-webui
spec:
  replicas: 1
  selector:
    matchLabels:
      app: webui
  template:
    metadata:
      labels:
        app: webui
    spec:
      containers:
      - name: webui
        image: quay.io/your-username/webui:latest
        ports:
        - containerPort: 8080
          protocol: TCP
        env:
        - name: QUARKUS_REST_CLIENT_TUXEDO_API_URL
          value: "http://tuxedo-service.demo-tuxedo.svc.cluster.local:8080"
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
---
apiVersion: v1
kind: Service
metadata:
  name: webui
  namespace: demo-webui
spec:
  selector:
    app: webui
  ports:
  - port: 8080
    targetPort: 8080
---
apiVersion: route.openshift.io/v1
kind: Route
metadata:
  name: webui
  namespace: demo-webui
spec:
  to:
    kind: Service
    name: webui
  port:
    targetPort: 8080
```

### Tuxedo Deployment
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: tuxedo-service
  namespace: demo-tuxedo
spec:
  replicas: 1
  selector:
    matchLabels:
      app: tuxedo-service
  template:
    metadata:
      labels:
        app: tuxedo-service
    spec:
      containers:
      - name: tuxedo
        image: quay.io/your-username/tuxedo:latest
        ports:
        - containerPort: 8080
          protocol: TCP
        env:
        - name: AMQ_HOST
          value: "amq-broker-hdls-svc.demo-tuxedo.svc.cluster.local"
        - name: AMQ_PORT
          value: "5672"
        - name: AMQ_QUEUE
          value: "TUXEDO.OUT"
---
apiVersion: v1
kind: Service
metadata:
  name: tuxedo-service
  namespace: demo-tuxedo
spec:
  selector:
    app: tuxedo-service
  ports:
  - port: 8080
    targetPort: 8080
```

### Camel Deployment
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: camel-integration
  namespace: demo-camel
spec:
  replicas: 1
  selector:
    matchLabels:
      app: camel-integration
  template:
    metadata:
      labels:
        app: camel-integration
    spec:
      containers:
      - name: camel
        image: quay.io/your-username/camel:latest
        ports:
        - containerPort: 8080
          protocol: TCP
        env:
        - name: AMQ_URL
          value: "amqp://amq-broker-hdls-svc.demo-tuxedo.svc.cluster.local:5672"
        - name: KAFKA_BOOTSTRAP_SERVERS
          value: "demo-cluster-kafka-bootstrap.demo-kafka.svc.cluster.local:9092"
---
apiVersion: v1
kind: Service
metadata:
  name: camel-integration
  namespace: demo-camel
spec:
  selector:
    app: camel-integration
  ports:
  - port: 8080
    targetPort: 8080
```

## 6. メッセージフォーマット詳細

### WebUI → Tuxedo
- **Type**: Plain text
- **Example**: `Hello World!`

### Tuxedo → AMQ
- **Type**: Plain text
- **Queue**: `TUXEDO.OUT`
- **Example**: `Hello World!`

### AMQ → Camel
- **Type**: Plain text (from AMQ body)
- **Example**: `Hello World!`

### Camel → Kafka
- **Type**: JSON
- **Topic**: `demo-messages`
- **Example**:
```json
{
  "id": "550e8400-e29b-41d4-a716-446655440000",
  "timestamp": "2026-06-04T10:30:00.123Z",
  "message": "Hello World!"
}
```

## 7. ログフォーマット仕様

全コンポーネント共通:
```
[YYYY-MM-DD HH:mm:ss] LEVEL [Component] Message
```

### 各コンポーネントログ例

**WebUI:**
```
[2026-06-04 10:30:00] INFO [WebUI] Received: Hello World!
[2026-06-04 10:30:00] INFO [WebUI] Sent to Tuxedo: Hello World!
```

**Tuxedo:**
```
[2026-06-04 10:30:00] INFO [Tuxedo] Received: Hello World!
[2026-06-04 10:30:00] INFO [Tuxedo] Queued to AMQ: Hello World!
```

**Camel:**
```
[2026-06-04 10:30:00] INFO [Camel] Consumed from queue: Hello World!
[2026-06-04 10:30:00] INFO [Camel] Transformed: {"id":"...","timestamp":"...","message":"Hello World!"}
[2026-06-04 10:30:00] INFO [Camel] Sent to Kafka topic: demo-messages
```
