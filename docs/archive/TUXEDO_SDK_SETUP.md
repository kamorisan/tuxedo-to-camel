# Oracle Tuxedo SDK Setup Guide

Complete guide to obtain and install Oracle Tuxedo SDK for C implementation.

---

## What is Tuxedo SDK?

Oracle Tuxedo SDK (Software Development Kit) includes:

### Development Files
- **Header files** (`.h`):
  - `atmi.h` - ATMI (Application-to-Transaction Monitor Interface)
  - `tmqueue.h` - Queue management APIs
  - `fml.h` / `fml32.h` - Field Manipulation Language
  - `userlog.h` - Logging APIs
  - `xa.h` - XA transaction APIs

### Build Tools
- `buildclient` - Build ATMI client programs
- `buildserver` - Build ATMI server programs
- `tmloadcf` - Load Tuxedo configuration
- `tmboot` - Boot Tuxedo domain
- `tmshutdown` - Shutdown Tuxedo domain
- `tmadmin` - Administration tool
- `qmadmin` - Queue management tool

### Libraries
- `libtux.so` - Core Tuxedo library
- `libbuft.so` - Typed buffer library
- `libfml.so` / `libfml32.so` - FML libraries
- `libengine.so` - Transaction engine

---

## Why SDK is Required

### Current Situation

**Runtime Image** (`oracle/tuxedo:22.1.1.0.0`):
```
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/
├── bin/              # Runtime binaries only
│   ├── TMQFORWARD
│   └── TMQUEUE
├── lib/              # Shared libraries only
│   ├── libtux.so
│   └── libbuft.so
└── locale/
```

**Missing**:
- ❌ `include/` directory (header files)
- ❌ Build tools (tmloadcf, tmboot, buildserver)
- ❌ Development utilities

### For C Implementation

**Python version**:
```python
# Uses ctypes - NO headers needed
from ctypes import CDLL
libtux = CDLL("/u01/oracle/tuxHome/tuxedo22.1.1.0.0/lib/libtux.so")
```

**C version**:
```c
// Requires headers at COMPILE time
#include <atmi.h>      // ← Need SDK
#include <tmqueue.h>   // ← Need SDK

int main() {
    tpinit(NULL);      // Link against libtux.so at runtime
}
```

**Compilation**:
```bash
gcc -I$TUXDIR/include tuxedo_q_server.c \
    -L$TUXDIR/lib -ltux -lbuft -o tuxedo_q_server
```

Without SDK: **Build fails with "atmi.h: No such file or directory"**

---

## Option 1: Oracle Official Download (Recommended)

### Step 1: Access Oracle Technology Network

**URL**: https://www.oracle.com/middleware/technologies/tuxedo-downloads.html

### Step 2: Sign In / Register

- Requires Oracle account (free)
- Accept Oracle Technology Network License Agreement

### Step 3: Download Tuxedo 22.1.1.0.0

**Product**: Oracle Tuxedo 22.1.1.0.0  
**Platform**: Linux x86-64  
**File**: `tuxedo221100_64_linux_x86_64.zip` (~1.5 GB)

**What's Included**:
- ✅ Runtime binaries
- ✅ SDK headers
- ✅ Build tools
- ✅ Documentation
- ✅ Sample code

### Step 4: Install SDK

```bash
# Extract
unzip tuxedo221100_64_linux_x86_64.zip

# Run installer
./tuxedo221100_64_linux_x86_64.bin -silent -responseFile response.txt
```

**response.txt** example:
```
[ENGINE]
Response File Version=1.0.0.0.0

[GENERIC]
ORACLE_HOME=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
INSTALL_TYPE=Complete
```

### Step 5: Verify Installation

```bash
export TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

# Check headers
ls $TUXDIR/include/atmi.h
ls $TUXDIR/include/tmqueue.h

# Check build tools
which tmloadcf
which tmboot
which buildserver

# Check version
tmadmin -v
```

Expected output:
```
Oracle Tuxedo, Version 22.1.1.0.0, 64-bit
```

---

## Option 2: Oracle Container Registry

### Prerequisites

- Docker or Podman
- Oracle Container Registry account
- Accept license terms

### Step 1: Login to Oracle Container Registry

**URL**: https://container-registry.oracle.com

Navigate to: **Middleware** → **tuxedo_dev**

Accept license agreement.

### Step 2: Docker Login

```bash
docker login container-registry.oracle.com
Username: <your-oracle-email>
Password: <oracle-account-password>
```

### Step 3: Pull SDK Image

```bash
# Full SDK image (development)
docker pull container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0
```

**Image size**: ~2.5 GB  
**Includes**: Full SDK + runtime

### Step 4: Extract SDK Files

```bash
# Create container
docker create --name tux-sdk container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0

# Extract SDK
docker cp tux-sdk:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include ./sdk/include
docker cp tux-sdk:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin ./sdk/bin

# Cleanup
docker rm tux-sdk
```

### Step 5: Use in Dockerfile

```dockerfile
FROM oracle/tuxedo:22.1.1.0.0

# Copy SDK files
COPY --chown=oracle:oracle sdk/include /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/
COPY --chown=oracle:oracle sdk/bin/tmloadcf /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
COPY --chown=oracle:oracle sdk/bin/tmboot /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
```

Or use the SDK image directly:
```dockerfile
FROM container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0

# Your build steps
```

---

## Option 3: Extract from Existing Installation

If you have access to a Tuxedo server with SDK already installed:

### Step 1: Locate SDK

```bash
# On Tuxedo server
echo $TUXDIR
# Example: /u01/oracle/tuxHome/tuxedo22.1.1.0.0

ls $TUXDIR/include/
ls $TUXDIR/bin/
```

### Step 2: Create Archive

```bash
# Create tarball with SDK files
cd $TUXDIR
tar czf tuxedo-sdk-22.1.1.tar.gz \
    include/ \
    bin/tmloadcf \
    bin/tmboot \
    bin/tmshutdown \
    bin/tmadmin \
    bin/buildserver \
    bin/buildclient
```

### Step 3: Transfer to Build Environment

```bash
# SCP to local machine
scp user@tuxedo-server:/tmp/tuxedo-sdk-22.1.1.tar.gz ./

# Extract
tar xzf tuxedo-sdk-22.1.1.tar.gz
```

### Step 4: Use in Container Build

```dockerfile
FROM oracle/tuxedo:22.1.1.0.0

USER root

# Copy SDK
COPY --chown=oracle:oracle include/ /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/
COPY --chown=oracle:oracle bin/* /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/

USER oracle
```

---

## Option 4: Trial / Developer License

Oracle provides trial versions for development:

### Step 1: Download Trial

**URL**: https://www.oracle.com/downloads/middleware-downloads.html

**Tuxedo Trial**: 30-day evaluation license

### Step 2: Install

Same as Option 1 (Official Download)

**License**: Trial license (30 days)  
**Use case**: Development, testing, PoC

---

## Integration with OpenShift Build

### Method 1: Custom Base Image

**Create**: `components/tuxedo-base-sdk/Dockerfile`

```dockerfile
FROM oracle/tuxedo:22.1.1.0.0

USER root

# Install build tools
RUN yum -y install gcc make libmicrohttpd-devel && yum clean all

# Copy SDK files (from Option 1, 2, or 3)
COPY --chown=oracle:oracle sdk/include /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/
COPY --chown=oracle:oracle sdk/bin/tmloadcf /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
COPY --chown=oracle:oracle sdk/bin/tmboot /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
COPY --chown=oracle:oracle sdk/bin/tmadmin /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/

USER oracle

# Set environment
ENV TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
ENV PATH=$TUXDIR/bin:$PATH
```

**Build and Push**:
```bash
cd components/tuxedo-base-sdk/
docker build -t your-registry/tuxedo-sdk:22.1.1.0.0 .
docker push your-registry/tuxedo-sdk:22.1.1.0.0
```

**Update BuildConfig**:
```yaml
# deploy-kit/tuxedo-q-c/templates/05-msgsvc-buildconfig.yaml
strategy:
  dockerStrategy:
    from:
      kind: DockerImage
      name: 'your-registry/tuxedo-sdk:22.1.1.0.0'
```

### Method 2: Use Oracle Registry Image

**Update**: `deploy-kit/tuxedo-q-c/templates/03-tuxedo-base.yaml`

```yaml
apiVersion: image.openshift.io/v1
kind: ImageStream
metadata:
  name: tuxedo-base
spec:
  lookupPolicy:
    local: false
  tags:
    - name: '22.1.1.0.0'
      from:
        kind: DockerImage
        name: 'container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0'
      importPolicy:
        scheduled: true
      referencePolicy:
        type: Source
```

**OpenShift Secret** (for Oracle Container Registry):
```bash
oc create secret docker-registry oracle-registry \
  --docker-server=container-registry.oracle.com \
  --docker-username=<your-email> \
  --docker-password=<your-password> \
  -n demo-tuxedo-c

oc secrets link builder oracle-registry -n demo-tuxedo-c
```

---

## Verification Checklist

After SDK installation:

### Build Environment
- [ ] `atmi.h` exists at `$TUXDIR/include/atmi.h`
- [ ] `tmqueue.h` exists at `$TUXDIR/include/tmqueue.h`
- [ ] `fml.h` exists at `$TUXDIR/include/fml.h`
- [ ] `tmloadcf` in PATH
- [ ] `tmboot` in PATH
- [ ] `gcc` installed
- [ ] `make` installed

### Runtime Environment
- [ ] `libtux.so` exists at `$TUXDIR/lib/libtux.so`
- [ ] `libbuft.so` exists at `$TUXDIR/lib/libbuft.so`
- [ ] `LD_LIBRARY_PATH` includes `$TUXDIR/lib`

### Test Compilation

```bash
# Create test program
cat > test.c << 'EOF'
#include <stdio.h>
#include <atmi.h>
#include <tmqueue.h>

int main() {
    printf("ATMI header: OK\n");
    printf("TMQUEUE header: OK\n");
    return 0;
}
EOF

# Compile
gcc -I$TUXDIR/include test.c -o test

# Run
./test
```

Expected output:
```
ATMI header: OK
TMQUEUE header: OK
```

---

## License Considerations

### Oracle Tuxedo Licensing

**Production Use**:
- Requires commercial license from Oracle
- Contact Oracle sales for pricing

**Development/Testing**:
- Trial license (30 days)
- Developer license (limited use)

**OTN License**:
- Development only
- No production use
- No redistribution

### Recommendation

For this project:
1. **Development/PoC**: Use trial/developer license
2. **Production**: Contact Oracle for commercial license
3. **Evaluation**: Use Python version (no SDK required)

---

## Common Issues

### Issue 1: Download Requires Login

**Symptom**: Oracle website requires account

**Solution**: Create free Oracle account at oracle.com

### Issue 2: Large Download Size

**Symptom**: 1.5 GB download

**Solution**: 
- Use faster internet connection
- Use Option 3 (extract from existing server) if available

### Issue 3: Installation Fails

**Symptom**: Installer error

**Solution**: Check:
- Sufficient disk space (5 GB)
- Linux x86-64 platform
- Proper permissions

### Issue 4: Headers Not Found After Install

**Symptom**: `atmi.h: No such file or directory`

**Solution**: Verify installation type was "Complete" not "Runtime Only"

---

## Next Steps

After SDK setup:

1. ✅ Build C implementation
2. ✅ Test QSPACE operations
3. ✅ Deploy to OpenShift
4. ⬜ Configure persistent QSPACE
5. ⬜ Performance testing

---

## References

- **Oracle Tuxedo Downloads**: https://www.oracle.com/middleware/technologies/tuxedo-downloads.html
- **Oracle Container Registry**: https://container-registry.oracle.com
- **Tuxedo Documentation**: https://docs.oracle.com/en/middleware/fusion-middleware/tuxedo/
- **C Implementation Guide**: [README_C_IMPLEMENTATION.md](components/tuxedo/README_C_IMPLEMENTATION.md)
- **QSPACE Setup**: [QSPACE_IMPLEMENTATION_GUIDE.md](QSPACE_IMPLEMENTATION_GUIDE.md)

---

## Summary

| Option | Difficulty | Time | Use Case |
|--------|-----------|------|----------|
| Oracle Download | Medium | 1-2h | Recommended |
| Container Registry | Easy | 30min | If you have access |
| Extract from Server | Easy | 15min | If server available |
| Trial License | Medium | 1-2h | Development only |

**Recommended**: Option 1 (Oracle Download) or Option 2 (Container Registry)
