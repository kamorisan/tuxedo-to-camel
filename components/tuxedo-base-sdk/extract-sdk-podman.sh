#!/bin/bash
# Extract Tuxedo SDK from tuxedo221100_64_linux_01_x86.zip
# Creates sdk/include and sdk/bin directories

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TUXEDO_ZIP="${1:-/Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip}"
OUTPUT_DIR="$SCRIPT_DIR/sdk"

if [ ! -f "$TUXEDO_ZIP" ]; then
    echo "Error: Tuxedo ZIP not found: $TUXEDO_ZIP"
    echo "Usage: $0 [path-to-tuxedo-zip]"
    exit 1
fi

echo "=================================================="
echo "Tuxedo SDK Extraction Tool"
echo "=================================================="
echo "ZIP: $TUXEDO_ZIP"
echo "Output: $OUTPUT_DIR"
echo ""

# Create temp container
echo "[1/6] Creating temp container..."
CONTAINER_ID=$(podman run -d --name tux-sdk-extract oraclelinux:8 sleep 3600)
echo "Container ID: $CONTAINER_ID"

# Copy installer
echo "[2/6] Copying Tuxedo installer..."
podman cp "$TUXEDO_ZIP" "$CONTAINER_ID:/tmp/tuxedo.zip"

# Install dependencies
echo "[3/6] Installing dependencies..."
podman exec -u root "$CONTAINER_ID" bash -c "yum -y install unzip && yum clean all" > /dev/null

# Setup user and extract
echo "[4/6] Setting up oracle user..."
podman exec -u root "$CONTAINER_ID" bash -c "
    useradd -m oracle
    mkdir -p /u01/oracle /u01/oracle/oraInventory
    chown -R oracle:oracle /u01/oracle
" > /dev/null

# Extract and install
echo "[5/6] Installing Tuxedo (this may take 2-3 minutes)..."
podman exec -u oracle "$CONTAINER_ID" bash -c "
    cd /tmp
    unzip -q tuxedo.zip
    cd Disk1/install

    cat > /tmp/response.txt << 'RESP'
[ENGINE]
Response File Version=1.0.0.0.0

[GENERIC]
DECLINE_SECURITY_UPDATES=true
SECURITY_UPDATES_VIA_MYORACLESUPPORT=false
ORACLE_HOME=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
INSTALL_TYPE=Complete
RESP

    cat > /tmp/oraInst.loc << 'INST'
inventory_loc=/u01/oracle/oraInventory
inst_group=oracle
INST

    ./runInstaller.sh -silent \
        -responseFile /tmp/response.txt \
        -invPtrLoc /tmp/oraInst.loc > /tmp/install.log 2>&1

    # Wait for completion
    sleep 10
"

# Check installation
echo "Verifying installation..."
podman exec -u oracle "$CONTAINER_ID" bash -c "
    if [ -d /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include ]; then
        echo 'Installation successful'
    else
        echo 'Installation failed'
        cat /tmp/install.log
        exit 1
    fi
"

# Extract SDK
echo "[6/6] Extracting SDK files..."
rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/bin"

# Copy headers
podman cp "$CONTAINER_ID:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include" "$OUTPUT_DIR/"

# Copy build tools
for tool in tmloadcf tmboot tmshutdown tmadmin qmadmin buildclient buildserver; do
    echo "  Extracting: $tool"
    podman cp "$CONTAINER_ID:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/$tool" "$OUTPUT_DIR/bin/" 2>/dev/null || echo "    (not found, skipping)"
done

# Cleanup
echo "Cleaning up container..."
podman stop "$CONTAINER_ID" > /dev/null
podman rm "$CONTAINER_ID" > /dev/null

echo ""
echo "=================================================="
echo "SDK Extraction Complete!"
echo "=================================================="
echo ""
echo "Extracted files:"
echo ""
echo "Headers:"
ls -1 "$OUTPUT_DIR/include/" | head -10 | sed 's/^/  /'
echo "  ... ($(ls -1 "$OUTPUT_DIR/include/" | wc -l | tr -d ' ') total)"
echo ""
echo "Build Tools:"
ls -1 "$OUTPUT_DIR/bin/" | sed 's/^/  /'
echo ""
echo "Next steps:"
echo "1. Build SDK base image:"
echo "   cd $(dirname $SCRIPT_DIR)/tuxedo-base-sdk"
echo "   podman build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 ."
echo ""
echo "2. Or use SDK directly in C compilation"
echo ""
