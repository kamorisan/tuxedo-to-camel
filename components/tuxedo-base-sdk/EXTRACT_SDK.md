# Tuxedo SDK Extraction Guide

How to extract SDK files from `tuxedo221100_64_linux_01_x86.zip` for use in Docker builds.

---

## Method 1: Install in Temp Container (Recommended)

### Step 1: Create Response Files

Create installation response files:

**response.txt**:
```ini
[ENGINE]
Response File Version=1.0.0.0.0

[GENERIC]
DECLINE_SECURITY_UPDATES=true
SECURITY_UPDATES_VIA_MYORACLESUPPORT=false
ORACLE_HOME=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
INSTALL_TYPE=Complete
```

**oraInst.loc**:
```ini
inventory_loc=/u01/oracle/oraInventory
inst_group=oracle
```

### Step 2: Run Installer in Container

```bash
# Start Oracle Linux container
docker run -it --name tux-installer \
  -v $(pwd)/tuxedo221100_64_linux_01_x86.zip:/tmp/tuxedo.zip \
  oraclelinux:8 bash

# Inside container:
useradd -m oracle
mkdir -p /u01/oracle
chown -R oracle:oracle /u01/oracle

# Install as oracle user
su - oracle
cd /tmp
unzip -q tuxedo.zip
cd Disk1/install

# Create response files
cat > /tmp/response.txt << 'EOF'
[ENGINE]
Response File Version=1.0.0.0.0

[GENERIC]
DECLINE_SECURITY_UPDATES=true
SECURITY_UPDATES_VIA_MYORACLESUPPORT=false
ORACLE_HOME=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
INSTALL_TYPE=Complete
EOF

cat > /tmp/oraInst.loc << 'EOF'
inventory_loc=/u01/oracle/oraInventory
inst_group=oracle
EOF

# Run installer
./runInstaller.sh -silent \
  -responseFile /tmp/response.txt \
  -invPtrLoc /tmp/oraInst.loc

# Wait for installation to complete
# Check: tail -f /u01/oracle/oraInventory/logs/*.log
```

### Step 3: Extract SDK Files

In another terminal (while container is still running):

```bash
# Copy SDK files from container
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include ./sdk/include
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmloadcf ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmboot ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmshutdown ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmadmin ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/qmadmin ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/buildclient ./sdk/bin/
docker cp tux-installer:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/buildserver ./sdk/bin/

# Stop and remove container
docker stop tux-installer
docker rm tux-installer
```

### Step 4: Verify Extracted Files

```bash
cd components/tuxedo-base-sdk
tree sdk/

# Expected:
# sdk/
# ├── include/
# │   ├── atmi.h
# │   ├── tmqueue.h
# │   ├── fml.h
# │   ├── fml32.h
# │   └── ...
# └── bin/
#     ├── tmloadcf
#     ├── tmboot
#     ├── tmadmin
#     └── ...
```

---

## Method 2: Automated Script

Create script to automate extraction:

**extract-sdk.sh**:
```bash
#!/bin/bash
set -e

TUXEDO_ZIP="$1"
OUTPUT_DIR="sdk"

if [ -z "$TUXEDO_ZIP" ] || [ ! -f "$TUXEDO_ZIP" ]; then
    echo "Usage: $0 <path-to-tuxedo-zip>"
    exit 1
fi

echo "Starting Tuxedo SDK extraction..."

# Create temp container
CONTAINER_ID=$(docker create oraclelinux:8 bash)

# Copy installer
docker cp "$TUXEDO_ZIP" "$CONTAINER_ID:/tmp/tuxedo.zip"

# Start and install
docker start "$CONTAINER_ID"
docker exec -u root "$CONTAINER_ID" bash -c "
    yum -y install unzip
    useradd -m oracle
    mkdir -p /u01/oracle
    chown -R oracle:oracle /u01/oracle
"

docker exec -u oracle "$CONTAINER_ID" bash -c "
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
        -invPtrLoc /tmp/oraInst.loc
"

echo "Waiting for installation to complete..."
sleep 30

# Extract SDK
mkdir -p "$OUTPUT_DIR"
docker cp "$CONTAINER_ID:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include" "$OUTPUT_DIR/"
mkdir -p "$OUTPUT_DIR/bin"

for tool in tmloadcf tmboot tmshutdown tmadmin qmadmin buildclient buildserver; do
    docker cp "$CONTAINER_ID:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/$tool" "$OUTPUT_DIR/bin/" || true
done

# Cleanup
docker stop "$CONTAINER_ID"
docker rm "$CONTAINER_ID"

echo "SDK extracted to $OUTPUT_DIR/"
ls -la "$OUTPUT_DIR/include/"
ls -la "$OUTPUT_DIR/bin/"
```

Usage:
```bash
chmod +x extract-sdk.sh
./extract-sdk.sh /path/to/tuxedo221100_64_linux_01_x86.zip
```

---

## Method 3: Quick JAR Extraction (Headers Only)

If you only need headers for C compilation:

```bash
# Extract tuxedoServer JAR (contains headers)
cd /path/to/extracted/Disk1/stage/Components/tuxedoServer/22.1.1.0.0/1/DataFiles/

# Find and extract header JARs
for jar in *.jar; do
    unzip -l "$jar" | grep -E "\\.h$" && unzip -q "$jar" "*.h" -d extracted/
done

# Copy headers
mkdir -p sdk/include
find extracted -name "*.h" -exec cp {} sdk/include/ \;
```

This gives you headers but NOT build tools (tmloadcf, tmboot, etc.)

---

## Method 4: Use Existing Installation

If you have access to a server with Tuxedo already installed:

```bash
# On Tuxedo server
cd $TUXDIR
tar czf tuxedo-sdk.tar.gz include/ bin/

# Transfer to local
scp user@tuxedo-server:$TUXDIR/tuxedo-sdk.tar.gz ./

# Extract
mkdir -p sdk
tar xzf tuxedo-sdk.tar.gz -C sdk/
```

---

## Next Steps

After extraction, you can:

### Build SDK Base Image

```bash
cd components/tuxedo-base-sdk
docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .
docker push <your-registry>/tuxedo-sdk:22.1.1.0.0
```

### Use in C Implementation

Update `deploy-kit/tuxedo-q-c/templates/03-base-buildconfig.yaml`:
```yaml
from:
  kind: DockerImage
  name: '<your-registry>/tuxedo-sdk:22.1.1.0.0'
```

---

## File Checklist

After extraction, verify:

### Headers (Required for C compilation)
- [ ] `sdk/include/atmi.h`
- [ ] `sdk/include/tmqueue.h`
- [ ] `sdk/include/fml.h`
- [ ] `sdk/include/fml32.h`
- [ ] `sdk/include/userlog.h`
- [ ] `sdk/include/xa.h`

### Build Tools (Required for QSPACE)
- [ ] `sdk/bin/tmloadcf`
- [ ] `sdk/bin/tmboot`
- [ ] `sdk/bin/tmshutdown`
- [ ] `sdk/bin/tmadmin`
- [ ] `sdk/bin/qmadmin`
- [ ] `sdk/bin/buildclient`
- [ ] `sdk/bin/buildserver`

---

## Troubleshooting

### Installer Fails

Check logs:
```bash
docker exec tux-installer tail -100 /u01/oracle/oraInventory/logs/installActions*.log
```

### Permission Denied

Ensure oracle user owns files:
```bash
docker exec -u root tux-installer chown -R oracle:oracle /u01/oracle
```

### Missing Dependencies

Install required packages:
```bash
docker exec -u root tux-installer yum -y install glibc libgcc libstdc++
```

---

## Summary

**Recommended**: Method 1 (Install in temp container)  
**Fastest**: Method 3 (JAR extraction for headers only)  
**Easiest**: Method 4 (If you have existing installation)

Choose based on your needs:
- **C compilation only**: Method 3 (headers)
- **Full SDK + QSPACE**: Method 1 or 2
- **Quick test**: Method 4
