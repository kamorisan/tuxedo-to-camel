#!/bin/bash
#
# Camel Deployment Script
# Deploys Camel with Tuxedo /Q REST polling integration
#

set -e

# Default values
NAMESPACE="${1:-demo-camel}"
KAFKA_NAMESPACE="${2:-demo-kafka}"
GIT_REPO="${3:-https://github.com/kamorisan/tuxedo-to-camel.git}"
GIT_BRANCH="${4:-feature/tuxedo-q-integration}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CAMEL_DIR="$SCRIPT_DIR/../camel"

KAFKA_BROKERS="demo-kafka-kafka-bootstrap.${KAFKA_NAMESPACE}.svc.cluster.local:9092"

echo "=========================================="
echo "Camel Deployment"
echo "=========================================="
echo "Namespace:       $NAMESPACE"
echo "Kafka Brokers:   $KAFKA_BROKERS"
echo "Git Repo:        $GIT_REPO"
echo "Git Branch:      $GIT_BRANCH"
echo "=========================================="
echo ""

# Function to replace placeholders
apply_template() {
    local template=$1
    local temp_file=$(mktemp)
    
    sed -e "s|NAMESPACE_PLACEHOLDER|$NAMESPACE|g" \
        -e "s|KAFKA_BROKERS_PLACEHOLDER|$KAFKA_BROKERS|g" \
        -e "s|GIT_REPO_PLACEHOLDER|$GIT_REPO|g" \
        -e "s|GIT_BRANCH_PLACEHOLDER|$GIT_BRANCH|g" \
        "$template" > "$temp_file"
    
    oc apply -f "$temp_file"
    rm "$temp_file"
}

# Check if oc is available
if ! command -v oc &> /dev/null; then
    echo "Error: 'oc' command not found."
    exit 1
fi

if ! oc whoami &> /dev/null; then
    echo "Error: Not logged in to OpenShift."
    exit 1
fi

echo "[1/5] Creating namespace..."
apply_template "$CAMEL_DIR/01-namespace.yaml"
sleep 2

echo "[2/5] Creating ImageStream..."
apply_template "$CAMEL_DIR/02-imagestream.yaml"

echo "[3/5] Creating BuildConfig..."
apply_template "$CAMEL_DIR/03-buildconfig.yaml"

echo "[4/5] Starting build..."
oc start-build camel-kafka-bridge -n "$NAMESPACE" --follow || echo "Build started in background"

echo "[5/5] Creating Deployment and Service..."
apply_template "$CAMEL_DIR/04-deployment.yaml"
apply_template "$CAMEL_DIR/05-service.yaml"

echo ""
echo "=========================================="
echo "Camel Deployment Complete!"
echo "=========================================="
echo ""
echo "Check status:"
echo "  oc get pods -n $NAMESPACE"
echo ""
echo "View logs:"
echo "  oc logs -n $NAMESPACE -l app=camel-kafka-bridge --tail=50"
echo ""
