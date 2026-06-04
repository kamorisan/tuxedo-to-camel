#!/bin/bash
#
# Complete Tuxedo /Q Integration Deployment
# Deploys all components: Tuxedo, WebUI, Camel
#

set -e

# Default values
TUXEDO_NS="${1:-demo-tuxedo}"
WEBUI_NS="${2:-demo-webui}"
CAMEL_NS="${3:-demo-camel}"
KAFKA_NS="${4:-demo-kafka}"
GIT_REPO="${5:-https://github.com/kamorisan/tuxedo-to-camel.git}"
GIT_BRANCH="${6:-feature/tuxedo-q-integration}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "Tuxedo /Q Complete Deployment"
echo "=========================================="
echo "Tuxedo Namespace:  $TUXEDO_NS"
echo "WebUI Namespace:   $WEBUI_NS"
echo "Camel Namespace:   $CAMEL_NS"
echo "Kafka Namespace:   $KAFKA_NS"
echo "Git Repo:          $GIT_REPO"
echo "Git Branch:        $GIT_BRANCH"
echo "=========================================="
echo ""

read -p "Continue with deployment? (y/n) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Deployment cancelled."
    exit 0
fi

echo ""
echo "=========================================="
echo "Step 1: Deploying Tuxedo /Q"
echo "=========================================="
"$SCRIPT_DIR/deploy-tuxedo.sh" "$TUXEDO_NS" "$GIT_REPO" "$GIT_BRANCH"

echo ""
echo "Waiting for Tuxedo to be ready..."
oc wait --for=condition=available --timeout=300s deployment/tuxedo-msgsvc -n "$TUXEDO_NS" || echo "Timeout waiting for Tuxedo"

echo ""
echo "=========================================="
echo "Step 2: Deploying WebUI"
echo "=========================================="
"$SCRIPT_DIR/deploy-webui.sh" "$WEBUI_NS" "$TUXEDO_NS" "$GIT_REPO" "$GIT_BRANCH"

echo ""
echo "Waiting for WebUI to be ready..."
oc wait --for=condition=available --timeout=300s deployment/webui -n "$WEBUI_NS" || echo "Timeout waiting for WebUI"

echo ""
echo "=========================================="
echo "Step 3: Deploying Camel"
echo "=========================================="
"$SCRIPT_DIR/deploy-camel.sh" "$CAMEL_NS" "$KAFKA_NS" "$GIT_REPO" "$GIT_BRANCH"

echo ""
echo "Waiting for Camel to be ready..."
oc wait --for=condition=available --timeout=300s deployment/camel-kafka-bridge -n "$CAMEL_NS" || echo "Timeout waiting for Camel"

echo ""
echo "=========================================="
echo "Deployment Summary"
echo "=========================================="
echo ""
echo "Tuxedo /Q:"
echo "  Namespace: $TUXEDO_NS"
echo "  URL: http://$(oc get route tuxedo-msgsvc -n $TUXEDO_NS -o jsonpath='{.spec.host}' 2>/dev/null || echo '<pending>')"
echo ""
echo "WebUI:"
echo "  Namespace: $WEBUI_NS"
echo "  URL: https://$(oc get route webui -n $WEBUI_NS -o jsonpath='{.spec.host}' 2>/dev/null || echo '<pending>')"
echo ""
echo "Camel:"
echo "  Namespace: $CAMEL_NS"
echo "  Status: $(oc get deployment camel-kafka-bridge -n $CAMEL_NS -o jsonpath='{.status.conditions[?(@.type=="Available")].status}' 2>/dev/null || echo 'Unknown')"
echo ""
echo "=========================================="
echo "Next Steps:"
echo "=========================================="
echo ""
echo "1. Test WebUI:"
echo "   Open: https://$(oc get route webui -n $WEBUI_NS -o jsonpath='{.spec.host}' 2>/dev/null)"
echo ""
echo "2. Send test message:"
echo "   curl -X POST https://$(oc get route webui -n $WEBUI_NS -o jsonpath='{.spec.host}' 2>/dev/null)/api/send \\"
echo "     -H 'Content-Type: text/plain' \\"
echo "     -d 'Test message from Tuxedo /Q' -k"
echo ""
echo "3. Check logs:"
echo "   Tuxedo: oc logs -n $TUXEDO_NS -l app=tuxedo-msgsvc --tail=20"
echo "   Camel:  oc logs -n $CAMEL_NS -l app=camel-kafka-bridge --tail=20"
echo ""
