#!/bin/bash
#
# AMQ Broker Deployment Script for OpenShift
# Usage: ./deploy-amq.sh <namespace> [queue-name]
#

set -e

NAMESPACE=${1:-demo-amq}
QUEUE_NAME=${2:-EXAMPLE.QUEUE}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
AMQ_DIR="$SCRIPT_DIR/../amq"

echo "========================================="
echo "AMQ Broker Deployment"
echo "========================================="
echo "Namespace: $NAMESPACE"
echo "Queue Name: $QUEUE_NAME"
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

echo "Step 1: Installing AMQ Broker Operator..."
oc apply -f "$AMQ_DIR/00-operator.yaml"

echo "Waiting for AMQ Broker Operator installation..."
sleep 10

RETRY=0
MAX_RETRY=30
while [ $RETRY -lt $MAX_RETRY ]; do
    if oc get csv -n openshift-operators | grep -q "amq-broker.*Succeeded"; then
        echo "✓ AMQ Broker Operator installed successfully"
        break
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY)"
    sleep 10
    RETRY=$((RETRY+1))
done

if [ $RETRY -eq $MAX_RETRY ]; then
    echo "Error: AMQ Broker Operator installation timed out"
    exit 1
fi

echo ""
echo "Step 2: Creating namespace '$NAMESPACE'..."
sed "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" "$AMQ_DIR/01-namespace.yaml" | oc apply -f -

echo ""
echo "Step 3: Deploying AMQ Broker..."
sed "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" "$AMQ_DIR/02-broker.yaml" | oc apply -f -

echo "Waiting for AMQ Broker Pod to be ready..."
sleep 15

RETRY=0
MAX_RETRY=60
while [ $RETRY -lt $MAX_RETRY ]; do
    if oc get pods -n "$NAMESPACE" 2>/dev/null | grep -q "amq-broker-ss-0.*Running"; then
        if oc get pods -n "$NAMESPACE" -o jsonpath='{.items[?(@.metadata.name=="amq-broker-ss-0")].status.containerStatuses[0].ready}' | grep -q "true"; then
            echo "✓ AMQ Broker Pod is ready"
            break
        fi
    fi
    echo "  Waiting... ($((RETRY+1))/$MAX_RETRY)"
    sleep 5
    RETRY=$((RETRY+1))
done

if [ $RETRY -eq $MAX_RETRY ]; then
    echo "Warning: AMQ Broker Pod not ready after timeout, but continuing..."
fi

echo ""
echo "Step 4: Creating queue '$QUEUE_NAME'..."
sed -e "s/NAMESPACE_PLACEHOLDER/$NAMESPACE/g" \
    -e "s/EXAMPLE.QUEUE/$QUEUE_NAME/g" \
    -e "s/example-queue-address/${QUEUE_NAME,,}-address/g" \
    "$AMQ_DIR/03-address-example.yaml" | oc apply -f -

echo ""
echo "========================================="
echo "✓ AMQ Broker Deployment Complete!"
echo "========================================="
echo ""
echo "Resources:"
echo "  Namespace: $NAMESPACE"
echo "  Broker Pod: amq-broker-ss-0"
echo "  Queue: $QUEUE_NAME"
echo ""
echo "Services:"
oc get svc -n "$NAMESPACE" | grep amq-broker || true
echo ""
echo "Console Route:"
oc get route -n "$NAMESPACE" 2>/dev/null | grep console || echo "  (Not exposed)"
echo ""
echo "Next steps:"
echo "  1. Check Pod status: oc get pods -n $NAMESPACE"
echo "  2. View logs: oc logs -n $NAMESPACE amq-broker-ss-0"
echo "  3. Access console: oc get route -n $NAMESPACE"
echo ""
