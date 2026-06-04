#!/bin/bash
#
# WebUI Deployment Script
# Deploys WebUI for Tuxedo /Q integration
#

set -e

# Default values
NAMESPACE="${1:-demo-webui}"
TUXEDO_NAMESPACE="${2:-demo-tuxedo}"
GIT_REPO="${3:-https://github.com/kamorisan/tuxedo-to-camel.git}"
GIT_BRANCH="${4:-feature/tuxedo-q-integration}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WEBUI_DIR="$SCRIPT_DIR/../webui"

TUXEDO_URL="http://tuxedo-msgsvc.${TUXEDO_NAMESPACE}.svc.cluster.local:8080"

echo "=========================================="
echo "WebUI Deployment"
echo "=========================================="
echo "Namespace:       $NAMESPACE"
echo "Tuxedo URL:      $TUXEDO_URL"
echo "Git Repo:        $GIT_REPO"
echo "Git Branch:      $GIT_BRANCH"
echo "=========================================="
echo ""

# Function to replace placeholders
apply_template() {
    local template=$1
    local temp_file=$(mktemp)
    
    sed -e "s|NAMESPACE_PLACEHOLDER|$NAMESPACE|g" \
        -e "s|TUXEDO_URL_PLACEHOLDER|$TUXEDO_URL|g" \
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
apply_template "$WEBUI_DIR/01-namespace.yaml"
sleep 2

echo "[2/5] Creating BuildConfig..."
apply_template "$WEBUI_DIR/05-buildconfig.yaml"

echo "[3/5] Starting build..."
oc start-build webui -n "$NAMESPACE" --follow || echo "Build started in background"

echo "[4/5] Creating Deployment and Service..."
apply_template "$WEBUI_DIR/02-deployment.yaml"
apply_template "$WEBUI_DIR/03-service.yaml"

echo "[5/5] Creating Route..."
apply_template "$WEBUI_DIR/04-route.yaml"

echo ""
echo "=========================================="
echo "WebUI Deployment Complete!"
echo "=========================================="
echo ""
echo "WebUI URL: https://$(oc get route webui -n $NAMESPACE -o jsonpath='{.spec.host}' 2>/dev/null || echo '<pending>')"
echo ""
echo "Check status:"
echo "  oc get pods -n $NAMESPACE"
echo ""
