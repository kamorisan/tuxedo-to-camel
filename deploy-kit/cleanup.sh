#!/bin/bash
# Tuxedo to Kafka Demo - Cleanup Script
#
# This script removes all demo resources from OpenShift

set -e

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

echo "==========================================="
echo "Tuxedo to Kafka Demo - Cleanup"
echo "==========================================="
echo ""

# Ask for confirmation
read -p "This will delete all demo resources. Continue? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    log_warn "Cleanup cancelled"
    exit 0
fi

log_info "Deleting demo namespaces..."

# Delete namespaces
for ns in demo-webui demo-camel demo-tuxedo-c demo-kafka; do
    if oc get namespace $ns &> /dev/null; then
        log_info "Deleting namespace: $ns"
        oc delete namespace $ns --wait=false
    else
        log_warn "Namespace $ns not found, skipping"
    fi
done

log_info "Waiting for namespaces to be deleted (max 2 minutes)..."
sleep 5

for i in {1..24}; do
    REMAINING=$(oc get namespaces | grep -E "demo-(webui|camel|tuxedo-c|kafka)" | wc -l || echo "0")
    if [ "$REMAINING" -eq 0 ]; then
        log_success "All namespaces deleted"
        break
    fi
    echo -n "."
    sleep 5
done

echo ""

# Optionally delete AMQ Streams Operator
echo ""
read -p "Delete AMQ Streams Operator? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    log_info "Deleting AMQ Streams Operator..."
    oc delete subscription amq-streams -n openshift-operators 2>/dev/null || log_warn "Subscription not found"
    oc delete csv -n openshift-operators -l operators.coreos.com/amq-streams.openshift-operators 2>/dev/null || log_warn "CSV not found"
    log_success "AMQ Streams Operator deleted"
fi

echo ""
echo "==========================================="
echo "🎉 Cleanup Complete!"
echo "==========================================="
echo ""
echo "Deleted namespaces:"
echo "  - demo-webui"
echo "  - demo-camel"
echo "  - demo-tuxedo-c"
echo "  - demo-kafka"
echo ""
