#!/bin/bash
# Tuxedo to Kafka Demo - Full Deployment Script for OpenShift
#
# This script deploys the complete demo environment:
# - AMQ Streams Operator (Kafka)
# - Tuxedo C Implementation
# - Apache Camel Integration
# - Quarkus WebUI
#
# Prerequisites:
# - oc CLI installed and logged in to OpenShift cluster
# - Cluster admin or sufficient permissions

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Configuration
GIT_REPO="${GIT_REPO:-https://github.com/kamorisan/tuxedo-to-camel.git}"
GIT_BRANCH="${GIT_BRANCH:-main}"

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

function log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

function log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

function log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

function log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

function check_prerequisites() {
    log_info "Checking prerequisites..."

    # Check oc CLI
    if ! command -v oc &> /dev/null; then
        log_error "oc CLI not found. Please install OpenShift CLI."
        exit 1
    fi

    # Check if logged in
    if ! oc whoami &> /dev/null; then
        log_error "Not logged in to OpenShift. Please run: oc login"
        exit 1
    fi

    log_success "Prerequisites check passed"
}

function deploy_amq_streams_operator() {
    log_info "Deploying AMQ Streams Operator..."

    # Create operator namespace if not exists
    oc get namespace openshift-operators &> /dev/null || oc create namespace openshift-operators

    # Create OperatorGroup for all namespaces (if not exists)
    cat <<EOF | oc apply -f -
apiVersion: operators.coreos.com/v1
kind: OperatorGroup
metadata:
  name: global-operators
  namespace: openshift-operators
spec: {}
EOF

    # Create Subscription for AMQ Streams
    cat <<EOF | oc apply -f -
apiVersion: operators.coreos.com/v1alpha1
kind: Subscription
metadata:
  name: amq-streams
  namespace: openshift-operators
spec:
  channel: stable
  name: amq-streams
  source: redhat-operators
  sourceNamespace: openshift-marketplace
  installPlanApproval: Automatic
EOF

    log_info "Waiting for AMQ Streams Operator to be ready (max 3 minutes)..."
    for i in {1..36}; do
        if oc get csv -n openshift-operators | grep -q "amqstreams.*Succeeded"; then
            log_success "AMQ Streams Operator installed successfully"
            return 0
        fi
        echo -n "."
        sleep 5
    done

    log_warn "AMQ Streams Operator installation timeout - continuing anyway"
}

function deploy_kafka() {
    log_info "Deploying Kafka cluster..."

    # Create namespace
    oc new-project demo-kafka 2>/dev/null || oc project demo-kafka

    # Deploy Kafka cluster
    cat <<EOF | oc apply -f -
apiVersion: kafka.strimzi.io/v1beta2
kind: Kafka
metadata:
  name: demo-kafka
  namespace: demo-kafka
spec:
  kafka:
    version: 3.7.0
    replicas: 1
    listeners:
      - name: plain
        port: 9092
        type: internal
        tls: false
      - name: tls
        port: 9093
        type: internal
        tls: true
    config:
      offsets.topic.replication.factor: 1
      transaction.state.log.replication.factor: 1
      transaction.state.log.min.isr: 1
      default.replication.factor: 1
      min.insync.replicas: 1
      inter.broker.protocol.version: "3.7"
    storage:
      type: ephemeral
  zookeeper:
    replicas: 1
    storage:
      type: ephemeral
  entityOperator:
    topicOperator: {}
    userOperator: {}
EOF

    # Wait for Kafka to be ready
    log_info "Waiting for Kafka cluster to be ready (max 5 minutes)..."
    oc wait kafka/demo-kafka --for=condition=Ready --timeout=300s -n demo-kafka || log_warn "Kafka timeout - continuing"

    # Create topic
    cat <<EOF | oc apply -f -
apiVersion: kafka.strimzi.io/v1beta2
kind: KafkaTopic
metadata:
  name: demo-messages
  namespace: demo-kafka
  labels:
    strimzi.io/cluster: demo-kafka
spec:
  partitions: 3
  replicas: 1
  config:
    retention.ms: 604800000
    segment.bytes: 1073741824
EOF

    log_success "Kafka cluster deployed"
}

function deploy_tuxedo_c() {
    log_info "Deploying Tuxedo C Implementation..."

    # Create namespace
    oc new-project demo-tuxedo-c 2>/dev/null || oc project demo-tuxedo-c

    # Create BuildConfig for base image
    cat <<EOF | oc apply -f -
apiVersion: build.openshift.io/v1
kind: BuildConfig
metadata:
  name: tuxedo-base
  namespace: demo-tuxedo-c
spec:
  source:
    type: Git
    git:
      uri: ${GIT_REPO}
      ref: ${GIT_BRANCH}
    contextDir: components/tuxedo-base-sdk
  strategy:
    type: Docker
    dockerStrategy:
      dockerfilePath: Dockerfile.standalone
  output:
    to:
      kind: ImageStreamTag
      name: tuxedo-sdk:latest
EOF

    # Create ImageStream for base
    oc create imagestream tuxedo-sdk -n demo-tuxedo-c 2>/dev/null || true

    # Build base image
    log_info "Building Tuxedo SDK base image (this may take 5-7 minutes)..."
    oc start-build tuxedo-base -n demo-tuxedo-c --follow || log_warn "Base build failed - check logs"

    # Create BuildConfig for Tuxedo C application
    cat <<EOF | oc apply -f -
apiVersion: build.openshift.io/v1
kind: BuildConfig
metadata:
  name: tuxedo-msgsvc
  namespace: demo-tuxedo-c
spec:
  source:
    type: Git
    git:
      uri: ${GIT_REPO}
      ref: ${GIT_BRANCH}
    contextDir: components/tuxedo
  strategy:
    type: Docker
    dockerStrategy:
      dockerfilePath: Containerfile.c
      from:
        kind: ImageStreamTag
        name: tuxedo-sdk:latest
  output:
    to:
      kind: ImageStreamTag
      name: tuxedo-msgsvc:latest
EOF

    # Create ImageStream for app
    oc create imagestream tuxedo-msgsvc -n demo-tuxedo-c 2>/dev/null || true

    # Build app image
    log_info "Building Tuxedo C application image..."
    oc start-build tuxedo-msgsvc -n demo-tuxedo-c --follow || log_warn "App build failed - check logs"

    # Create Deployment
    cat <<EOF | oc apply -f -
apiVersion: apps/v1
kind: Deployment
metadata:
  name: tuxedo-msgsvc
  namespace: demo-tuxedo-c
  labels:
    app: tuxedo-msgsvc
spec:
  replicas: 1
  selector:
    matchLabels:
      app: tuxedo-msgsvc
  template:
    metadata:
      labels:
        app: tuxedo-msgsvc
    spec:
      containers:
      - name: tuxedo-msgsvc
        image: image-registry.openshift-image-registry.svc:5000/demo-tuxedo-c/tuxedo-msgsvc:latest
        ports:
        - containerPort: 8080
          name: http
        livenessProbe:
          httpGet:
            path: /health
            port: 8080
          initialDelaySeconds: 10
          periodSeconds: 30
        readinessProbe:
          httpGet:
            path: /health
            port: 8080
          initialDelaySeconds: 5
          periodSeconds: 10
EOF

    # Create Service
    cat <<EOF | oc apply -f -
apiVersion: v1
kind: Service
metadata:
  name: tuxedo-msgsvc
  namespace: demo-tuxedo-c
  labels:
    app: tuxedo-msgsvc
spec:
  ports:
  - name: http
    port: 8080
    targetPort: 8080
  selector:
    app: tuxedo-msgsvc
EOF

    # Create Route
    cat <<EOF | oc apply -f -
apiVersion: route.openshift.io/v1
kind: Route
metadata:
  name: tuxedo-msgsvc
  namespace: demo-tuxedo-c
spec:
  to:
    kind: Service
    name: tuxedo-msgsvc
  port:
    targetPort: http
  tls:
    termination: edge
    insecureEdgeTerminationPolicy: Redirect
EOF

    log_success "Tuxedo C deployed"
}

function deploy_camel() {
    log_info "Deploying Apache Camel Integration..."

    # Create namespace
    oc new-project demo-camel 2>/dev/null || oc project demo-camel

    # Create BuildConfig
    cat <<EOF | oc apply -f -
apiVersion: build.openshift.io/v1
kind: BuildConfig
metadata:
  name: camel-kafka-bridge
  namespace: demo-camel
spec:
  source:
    type: Git
    git:
      uri: ${GIT_REPO}
      ref: ${GIT_BRANCH}
    contextDir: components/camel
  strategy:
    type: Source
    sourceStrategy:
      from:
        kind: ImageStreamTag
        namespace: openshift
        name: java:openjdk-21-ubi8
  output:
    to:
      kind: ImageStreamTag
      name: camel-kafka-bridge:latest
EOF

    # Create ImageStream
    oc create imagestream camel-kafka-bridge -n demo-camel 2>/dev/null || true

    # Build
    log_info "Building Camel application (this may take 2-3 minutes)..."
    oc start-build camel-kafka-bridge -n demo-camel --follow || log_warn "Camel build failed - check logs"

    # Create Deployment
    cat <<EOF | oc apply -f -
apiVersion: apps/v1
kind: Deployment
metadata:
  name: camel-kafka-bridge
  namespace: demo-camel
  labels:
    app: camel-kafka-bridge
spec:
  replicas: 1
  selector:
    matchLabels:
      app: camel-kafka-bridge
  template:
    metadata:
      labels:
        app: camel-kafka-bridge
    spec:
      containers:
      - name: camel-kafka-bridge
        image: image-registry.openshift-image-registry.svc:5000/demo-camel/camel-kafka-bridge:latest
        ports:
        - containerPort: 8080
          name: http
        env:
        - name: KAFKA_BROKERS
          value: demo-kafka-kafka-bootstrap.demo-kafka.svc.cluster.local:9092
EOF

    log_success "Camel deployed"
}

function deploy_webui() {
    log_info "Deploying WebUI..."

    # Create namespace
    oc new-project demo-webui 2>/dev/null || oc project demo-webui

    # Create BuildConfig
    cat <<EOF | oc apply -f -
apiVersion: build.openshift.io/v1
kind: BuildConfig
metadata:
  name: webui
  namespace: demo-webui
spec:
  source:
    type: Git
    git:
      uri: ${GIT_REPO}
      ref: ${GIT_BRANCH}
    contextDir: components/webui
  strategy:
    type: Source
    sourceStrategy:
      from:
        kind: ImageStreamTag
        namespace: openshift
        name: java:openjdk-21-ubi8
  output:
    to:
      kind: ImageStreamTag
      name: webui:latest
EOF

    # Create ImageStream
    oc create imagestream webui -n demo-webui 2>/dev/null || true

    # Build
    log_info "Building WebUI application (this may take 2-3 minutes)..."
    oc start-build webui -n demo-webui --follow || log_warn "WebUI build failed - check logs"

    # Create Deployment
    cat <<EOF | oc apply -f -
apiVersion: apps/v1
kind: Deployment
metadata:
  name: webui
  namespace: demo-webui
  labels:
    app: webui
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
        image: image-registry.openshift-image-registry.svc:5000/demo-webui/webui:latest
        ports:
        - containerPort: 8080
          name: http
        env:
        - name: TUXEDO_URL
          value: http://tuxedo-msgsvc.demo-tuxedo-c.svc.cluster.local:8080
        - name: JAVA_OPTS
          value: "-Dquarkus.http.host=0.0.0.0 -Djava.util.logging.manager=org.jboss.logmanager.LogManager"
EOF

    # Create Service
    cat <<EOF | oc apply -f -
apiVersion: v1
kind: Service
metadata:
  name: webui
  namespace: demo-webui
  labels:
    app: webui
spec:
  ports:
  - name: http
    port: 8080
    targetPort: 8080
  selector:
    app: webui
EOF

    # Create Route
    cat <<EOF | oc apply -f -
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
    targetPort: http
  tls:
    termination: edge
    insecureEdgeTerminationPolicy: Redirect
EOF

    log_success "WebUI deployed"
}

function verify_deployment() {
    log_info "Verifying deployment..."

    echo ""
    echo "==========================================="
    echo "Deployment Status"
    echo "==========================================="

    # Kafka
    echo ""
    log_info "Kafka Status:"
    oc get kafka -n demo-kafka

    # Tuxedo C
    echo ""
    log_info "Tuxedo C Status:"
    oc get pods -n demo-tuxedo-c -l app=tuxedo-msgsvc

    # Camel
    echo ""
    log_info "Camel Status:"
    oc get pods -n demo-camel -l app=camel-kafka-bridge

    # WebUI
    echo ""
    log_info "WebUI Status:"
    oc get pods -n demo-webui -l app=webui

    # Routes
    echo ""
    log_info "Routes:"
    WEBUI_ROUTE=$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}' 2>/dev/null || echo "Not ready")
    TUXEDO_ROUTE=$(oc get route tuxedo-msgsvc -n demo-tuxedo-c -o jsonpath='{.spec.host}' 2>/dev/null || echo "Not ready")

    echo "  WebUI:    https://$WEBUI_ROUTE"
    echo "  Tuxedo C: https://$TUXEDO_ROUTE"
}

function print_summary() {
    echo ""
    echo "==========================================="
    echo "🎉 Deployment Complete!"
    echo "==========================================="
    echo ""

    WEBUI_ROUTE=$(oc get route webui -n demo-webui -o jsonpath='{.spec.host}' 2>/dev/null || echo "Not ready")

    echo "Access the demo at:"
    echo "  🌐 WebUI: https://$WEBUI_ROUTE"
    echo ""
    echo "Test the E2E flow:"
    echo "  1. Open WebUI in browser"
    echo "  2. Enter a message"
    echo "  3. Click 'Tuxedoへ送信'"
    echo "  4. Message flows: WebUI → Tuxedo C → Tuxedo /Q → Camel → Kafka"
    echo ""
    echo "Verify Kafka messages:"
    echo "  oc run kafka-consumer -n demo-kafka --rm -i --restart=Never \\"
    echo "    --image=quay.io/strimzi/kafka:latest-kafka-4.1.0 \\"
    echo "    -- bin/kafka-console-consumer.sh \\"
    echo "    --bootstrap-server demo-kafka-kafka-bootstrap:9092 \\"
    echo "    --topic demo-messages \\"
    echo "    --from-beginning"
    echo ""
    echo "Namespaces created:"
    echo "  - demo-kafka (Kafka cluster)"
    echo "  - demo-tuxedo-c (Tuxedo C implementation)"
    echo "  - demo-camel (Apache Camel integration)"
    echo "  - demo-webui (Quarkus WebUI)"
    echo ""
    echo "To clean up:"
    echo "  ./deploy-kit/cleanup.sh"
    echo ""
}

# Main execution
main() {
    echo "==========================================="
    echo "Tuxedo to Kafka Demo - Full Deployment"
    echo "==========================================="
    echo ""
    echo "Git Repository: $GIT_REPO"
    echo "Git Branch:     $GIT_BRANCH"
    echo ""

    check_prerequisites

    # Deploy components
    deploy_amq_streams_operator
    deploy_kafka
    deploy_tuxedo_c
    deploy_camel
    deploy_webui

    # Wait for pods to be ready
    log_info "Waiting for all pods to be ready (max 2 minutes)..."
    sleep 30

    oc wait --for=condition=Ready pod -l app=tuxedo-msgsvc -n demo-tuxedo-c --timeout=120s 2>/dev/null || log_warn "Tuxedo C not ready yet"
    oc wait --for=condition=Ready pod -l app=camel-kafka-bridge -n demo-camel --timeout=120s 2>/dev/null || log_warn "Camel not ready yet"
    oc wait --for=condition=Ready pod -l app=webui -n demo-webui --timeout=120s 2>/dev/null || log_warn "WebUI not ready yet"

    verify_deployment
    print_summary
}

# Run main
main "$@"
