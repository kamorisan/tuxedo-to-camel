#!/bin/bash
# Tuxedo C Implementation - ローカル実行スクリプト

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "==========================================="
echo "Tuxedo C Implementation - Local Run"
echo "==========================================="
echo ""

# Check Podman
if ! command -v podman &> /dev/null; then
    echo "❌ Podman not found. Please install:"
    echo "   brew install podman"
    exit 1
fi

# Check Podman machine
if ! podman machine list 2>/dev/null | grep -q "Running"; then
    echo "⚠️  Podman machine not running. Starting..."
    podman machine start
    sleep 5
fi

# Check SDK
if [ ! -d "$REPO_ROOT/components/tuxedo-base-sdk/sdk/include" ]; then
    echo "❌ SDK not found at: $REPO_ROOT/components/tuxedo-base-sdk/sdk/"
    echo "   Please extract SDK first. See: SDK_EXTRACTION_RESULTS.md"
    exit 1
fi

echo "✅ Prerequisites check passed"
echo ""

# Build base image
echo "📦 Step 1/3: Building base image (tuxedo-sdk:local)"
echo "   This may take 5-7 minutes on first run..."
echo ""

cd "$REPO_ROOT/components/tuxedo-base-sdk"

if podman images | grep -q "tuxedo-sdk.*local"; then
    echo "   ℹ️  Base image exists. Skip rebuild? (y/n)"
    read -r SKIP_BASE
    if [[ $SKIP_BASE != "y" ]]; then
        podman build --platform linux/amd64 -f Dockerfile.standalone -t tuxedo-sdk:local .
    else
        echo "   ⏭️  Skipped"
    fi
else
    podman build --platform linux/amd64 -f Dockerfile.standalone -t tuxedo-sdk:local .
fi

echo ""
echo "✅ Base image ready"
echo ""

# Build C implementation
echo "🔨 Step 2/3: Building C implementation (tuxedo-c:local)"
echo ""

cd "$REPO_ROOT/components/tuxedo"
podman build --platform linux/amd64 -f Containerfile.c -t tuxedo-c:local .

echo ""
echo "✅ C implementation built"
echo ""

# Run
echo "🚀 Step 3/3: Starting Tuxedo C server"
echo ""
echo "==========================================="
echo "Server will start on http://localhost:8080"
echo "==========================================="
echo ""
echo "Test commands (run in another terminal):"
echo "  curl http://localhost:8080/health"
echo "  curl -X POST http://localhost:8080/enqueue -d \"Test\""
echo "  curl http://localhost:8080/dequeue"
echo ""
echo "Or run: ./scripts/test-local.sh"
echo ""
echo "Press Ctrl+C to stop"
echo "==========================================="
echo ""

# Check if port is in use
if lsof -Pi :8080 -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo "⚠️  Port 8080 is in use. Using port 8081 instead..."
    PORT=8081
else
    PORT=8080
fi

podman run --rm --platform linux/amd64 -p $PORT:8080 tuxedo-c:local
