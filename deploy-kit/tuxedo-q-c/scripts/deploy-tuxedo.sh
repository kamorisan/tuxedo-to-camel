#!/bin/bash
#
# Tuxedo /Q Deployment Script
# Deploys Tuxedo with /Q integration to OpenShift
#

set -e

# Default values
NAMESPACE="${1:-demo-tuxedo}"
GIT_REPO="${2:-https://github.com/kamorisan/tuxedo-to-camel.git}"
GIT_BRANCH="${3:-feature/tuxedo-q-integration}"
TUXEDO_BASE_IMAGE="${4:-container-registry.oracle.com/middleware/tuxedo:22.1.1.0.0}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_DIR="$SCRIPT_DIR/../templates"

echo "=========================================="
echo "Tuxedo /Q Deployment"
echo "=========================================="
echo "Namespace:     $NAMESPACE"
echo "Git Repo:      $GIT_REPO"
echo "Git Branch:    $GIT_BRANCH"
echo "Base Image:    $TUXEDO_BASE_IMAGE"
echo "=========================================="
echo ""

# Function to replace placeholders
apply_template() {
    local template=$1
    local temp_file=$(mktemp)
    
    sed -e "s|NAMESPACE_PLACEHOLDER|$NAMESPACE|g" \
        -e "s|GIT_REPO_PLACEHOLDER|$GIT_REPO|g" \
        -e "s|GIT_BRANCH_PLACEHOLDER|$GIT_BRANCH|g" \
        "$template" > "$temp_file"
    
    oc apply -f "$temp_file"
    rm "$temp_file"
}

# Check if oc is available
if ! command -v oc &> /dev/null; then
    echo "Error: 'oc' command not found. Please install OpenShift CLI."
    exit 1
fi

# Check if logged in to OpenShift
if ! oc whoami &> /dev/null; then
    echo "Error: Not logged in to OpenShift. Please run 'oc login' first."
    exit 1
fi

echo "[1/8] Creating namespace..."
apply_template "$TEMPLATE_DIR/01-namespace.yaml"
sleep 2

echo "[2/8] Creating Tuxedo base ImageStream..."
apply_template "$TEMPLATE_DIR/02-base-imagestream.yaml"

echo "[3/8] Importing Tuxedo base image..."
oc import-image tuxedo-base:22.1.1.0.0 \
  --from="$TUXEDO_BASE_IMAGE" \
  --confirm \
  -n "$NAMESPACE" || echo "Note: Image import may require Oracle credentials"

echo "[4/8] Creating Tuxedo base BuildConfig..."
apply_template "$TEMPLATE_DIR/03-base-buildconfig.yaml"

echo "[5/8] Creating Tuxedo msgsvc ImageStream..."
apply_template "$TEMPLATE_DIR/04-msgsvc-imagestream.yaml"

echo "[6/8] Creating Tuxedo msgsvc BuildConfig..."
apply_template "$TEMPLATE_DIR/05-msgsvc-buildconfig.yaml"

echo "[7/8] Starting build..."
oc start-build tuxedo-msgsvc -n "$NAMESPACE" --follow || echo "Build started in background"

echo "[8/8] Creating Deployment, Service, and Route..."
apply_template "$TEMPLATE_DIR/06-deployment.yaml"
apply_template "$TEMPLATE_DIR/07-service.yaml"
apply_template "$TEMPLATE_DIR/08-route.yaml"

echo ""
echo "=========================================="
echo "Deployment Complete!"
echo "=========================================="
echo ""
echo "Check status:"
echo "  oc get pods -n $NAMESPACE"
echo "  oc get route -n $NAMESPACE"
echo ""
echo "View logs:"
echo "  oc logs -n $NAMESPACE -l app=tuxedo-msgsvc --tail=50"
echo ""
echo "Test endpoints:"
echo "  Tuxedo Route: \$(oc get route tuxedo-msgsvc -n $NAMESPACE -o jsonpath='{.spec.host}')"
echo "  Health: curl http://\$(oc get route tuxedo-msgsvc -n $NAMESPACE -o jsonpath='{.spec.host}')/health"
echo ""
