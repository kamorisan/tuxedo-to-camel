#!/bin/bash
#
# Kafka Cluster Deployment Script for OpenShift (AMQ Streams)
# Usage: ./deploy-kafka.sh <namespace> <cluster-name> <console-hostname>
#

set -e

NAMESPACE=${1:-demo-kafka}
CLUSTER_NAME=${2:-my-kafka}
CONSOLE_HOSTNAME=${3}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
KAFKA_DIR="$SCRIPT_DIR/../kafka"

echo "========================================="
echo "Kafka Cluster Deployment (AMQ Streams)"
echo "========================================="
echo "Namespace: $NAMESPACE"
echo "Cluster Name: $CLUSTER_NAME"

# Auto-detect cluster domain if console hostname not provided
if [ -z "$CONSOLE_HOSTNAME" ]; then
    CLUSTER_DOMAIN=$(oc whoami --show-console 2>/dev/null | sed 's|https://console-openshift-console.apps.||' | sed 's|/||' || echo "")
    if [ -n "$CLUSTER_DOMAIN" ]; then
        CONSOLE_HOSTNAME="kafka-console.apps.$CLUSTER_DOMAIN"
        echo "Console Hostname: $CONSOLE_HOSTNAME (auto-detected)"
    else
        echo "Error: Could not auto-detect cluster domain. Please provide console hostname."
        echo "Usage: $0 <namespace> <cluster-name> <console-hostname>"
        exit 1
    fi
else
    echo "Console Hostname: $CONSOLE_HOSTNAME"
fi

echo ""

# Check if oc is available
if ! command -v oc &> /dev/null; then
    echo "Error: 'oc' command not found. Please install OpenShift CLI."
    exit 1
fi

# Check if logged in
if ! oc whoami &> /dev/null; then
    echo "Error: Not logged in to OpenShift. Please run 'oc login' first."
    exit 1
fi

echo "Step 1: Installing AMQ Streams Operator..."
oc apply -f "$KAFKA_DIR/00-operator.yaml"

echo "Waiting for AMQ Streams Operator installation..."
sleep 10

RETRY=0
MAX_RETRY=30
while [ $RETRY -lt $MAX_RETRY ]; do
    if oc get csv -n openshift-operators 2>/dev/null | grep -q "amqstreams.*Succeeded"; then
        echo "✓ AMQ Streams Operator installed successfully"
        break
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY)"
    sleep 10
    RETRY=$((RETRY+1))
done

if [ $RETRY -eq $MAX_RETRY ]; then
    echo "Error: AMQ Streams Operator installation timed out"
    exit 1
fi

echo ""
echo "Step 2: Installing AMQ Streams Console Operator..."
oc apply -f "$KAFKA_DIR/01-console-operator.yaml"

echo "Waiting for Console Operator installation..."
sleep 10

RETRY=0
MAX_RETRY=30
while [ $RETRY -lt $MAX_RETRY ]; do
    if oc get csv -n openshift-operators 2>/dev/null | grep -q "amq-streams-console.*Succeeded"; then
        echo "✓ AMQ Streams Console Operator installed successfully"
        break
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY)"
    sleep 10
    RETRY=$((RETRY+1))
done

if [ $RETRY -eq $MAX_RETRY ]; then
    echo "Warning: Console Operator installation timed out, but continuing..."
fi

echo ""
echo "Step 3: Creating namespace '$NAMESPACE'..."
sed "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" "$KAFKA_DIR/02-namespace.yaml" | oc apply -f -

echo ""
echo "Step 4: Deploying Kafka cluster '$CLUSTER_NAME'..."
sed -e "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" \
    -e "s/KAFKA_CLUSTER_NAME/$CLUSTER_NAME/g" \
    "$KAFKA_DIR/03-kafka-cluster.yaml" | oc apply -f -

echo "Waiting for Kafka cluster to be ready..."
sleep 20

RETRY=0
MAX_RETRY=60
while [ $RETRY -lt $MAX_RETRY ]; do
    POD_COUNT=$(oc get pods -n "$NAMESPACE" 2>/dev/null | grep -c "Running" || echo "0")
    if [ "$POD_COUNT" -ge 2 ]; then
        echo "✓ Kafka cluster Pods are running"
        break
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY) - Running pods: $POD_COUNT"
    sleep 5
    RETRY=$((RETRY+1))
done

if [ $RETRY -eq $MAX_RETRY ]; then
    echo "Warning: Kafka cluster not fully ready after timeout, but continuing..."
fi

echo ""
echo "Step 5: Deploying Kafka Console..."
sed -e "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" \
    -e "s/KAFKA_CLUSTER_NAME/$CLUSTER_NAME/g" \
    -e "s/CONSOLE_HOSTNAME_PLACEHOLDER/$CONSOLE_HOSTNAME/g" \
    "$KAFKA_DIR/05-console.yaml" | oc apply -f -

echo "Waiting for Kafka Console to be ready..."
sleep 15

RETRY=0
MAX_RETRY=30
while [ $RETRY -lt $MAX_RETRY ]; do
    if oc get pods -n "$NAMESPACE" 2>/dev/null | grep -q "console.*Running"; then
        echo "✓ Kafka Console is ready"
        break
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY)"
    sleep 5
    RETRY=$((RETRY+1))
done

echo ""
echo "========================================="
echo "✓ Kafka Cluster Deployment Complete!"
echo "========================================="
echo ""
echo "Resources:"
echo "  Namespace: $NAMESPACE"
echo "  Cluster Name: $CLUSTER_NAME"
echo "  Bootstrap Service: $CLUSTER_NAME-kafka-bootstrap:9092"
echo ""
echo "Pods:"
oc get pods -n "$NAMESPACE" 2>/dev/null || true
echo ""
echo "Services:"
oc get svc -n "$NAMESPACE" 2>/dev/null | grep kafka || true
echo ""
echo "Kafka Console:"
CONSOLE_URL=$(oc get route -n "$NAMESPACE" 2>/dev/null | grep console | awk '{print $2}' | head -1)
if [ -n "$CONSOLE_URL" ]; then
    echo "  URL: https://$CONSOLE_URL"
else
    echo "  Route not yet available"
fi
echo ""
echo "Next steps:"
echo "  1. Check cluster status: oc get kafka -n $NAMESPACE"
echo "  2. View Pod logs: oc logs -n $NAMESPACE <pod-name>"
echo "  3. Create topics: see $KAFKA_DIR/04-topic-example.yaml"
echo "  4. Access console: https://$CONSOLE_HOSTNAME"
echo ""
