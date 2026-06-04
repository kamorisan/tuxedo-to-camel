# Tuxedo SDK Base Image

Custom base image with Oracle Tuxedo SDK for C implementation builds.

---

## Quick Start

### Step 1: Extract SDK from ZIP

You have the Tuxedo installer at:
```
/Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip
```

Run the extraction script:
```bash
cd components/tuxedo-base-sdk
./extract-sdk.sh
```

This will:
1. Create a temp Oracle Linux container
2. Install Tuxedo with all components
3. Extract SDK files to `sdk/` directory
4. Cleanup temp container

**Time**: ~3-5 minutes  
**Disk**: ~500MB extracted

### Step 2: Build SDK Base Image

```bash
docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .
```

### Step 3: Tag and Push (Optional)

```bash
docker tag tuxedo-sdk:22.1.1.0.0 <your-registry>/tuxedo-sdk:22.1.1.0.0
docker push <your-registry>/tuxedo-sdk:22.1.1.0.0
```

### Step 4: Use in OpenShift

Update `deploy-kit/tuxedo-q-c/templates/03-base-buildconfig.yaml`:
```yaml
from:
  kind: DockerImage
  name: '<your-registry>/tuxedo-sdk:22.1.1.0.0'
```

---

## What's Included

### SDK Files (`sdk/`)

After extraction:

```
sdk/
├── include/               # C header files
│   ├── atmi.h            # ATMI API
│   ├── tmqueue.h         # Queue API
│   ├── fml.h / fml32.h   # Field Manipulation Language
│   ├── userlog.h         # Logging API
│   └── ... (~100+ headers)
└── bin/                   # Build tools
    ├── tmloadcf          # Load Tuxedo config
    ├── tmboot            # Boot Tuxedo domain
    ├── tmshutdown        # Shutdown domain
    ├── tmadmin           # Administration tool
    ├── qmadmin           # Queue admin
    ├── buildclient       # Build ATMI clients
    └── buildserver       # Build ATMI servers
```

### Docker Image

The `Dockerfile.simple` creates an image with:
- Base: `oracle/tuxedo:22.1.1.0.0` (runtime)
- Added: SDK headers and tools
- Added: gcc, make, libmicrohttpd-devel
- Size: ~1.2 GB (vs 800 MB runtime-only)

---

## Usage Scenarios

### Scenario 1: Local C Development

Use SDK directly without Docker:
```bash
export TUXDIR=$(pwd)/sdk
export PATH=$TUXDIR/bin:$PATH

# Compile C implementation
cd ../../tuxedo/
gcc -I../tuxedo-base-sdk/sdk/include \
    src/tuxedo_q_server.c \
    -L/path/to/libtux -ltux -lbuft -lfml -lmicrohttpd \
    -o tuxedo_q_server
```

### Scenario 2: OpenShift Build

1. Build and push SDK base image
2. Update BuildConfig to use SDK image
3. C implementation builds automatically

### Scenario 3: QSPACE Development

Use `tmloadcf` and `tmboot` from SDK:
```bash
cd ../../tuxedo/config/
../../../tuxedo-base-sdk/sdk/bin/tmloadcf -y ubbconfig
../../../tuxedo-base-sdk/sdk/bin/tmboot -y
```

---

## Troubleshooting

### Extraction Script Fails

**Check Docker**:
```bash
docker --version
docker ps
```

**Check ZIP file**:
```bash
ls -lh /Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip
unzip -l /Users/kamori/vscode/customer/japan-post/tuxedo221100_64_linux_01_x86.zip | head
```

**Manual extraction**:
See [EXTRACT_SDK.md](EXTRACT_SDK.md) for alternative methods.

### Build Fails: SDK Directory Not Found

```bash
ls -la sdk/include/atmi.h
ls -la sdk/bin/tmloadcf
```

If missing, re-run `./extract-sdk.sh`

### Docker Build Fails

**Error**: `COPY failed: file not found`

**Solution**: Run extraction first:
```bash
./extract-sdk.sh
ls -la sdk/
docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .
```

---

## Alternative: Manual Extraction

If automated script fails, see detailed guide:
[EXTRACT_SDK.md](EXTRACT_SDK.md)

Covers:
- Method 1: Install in temp container (step-by-step)
- Method 2: Automated script
- Method 3: JAR extraction (headers only)
- Method 4: Copy from existing server

---

## Verification

After building SDK image:

```bash
# Test SDK availability
docker run --rm tuxedo-sdk:22.1.1.0.0 ls -la /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/atmi.h

# Test compilation
docker run --rm tuxedo-sdk:22.1.1.0.0 bash -c '
echo "#include <atmi.h>" | gcc -x c -I/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include -c - -o /dev/null && echo "Headers OK"
'

# Test tools
docker run --rm tuxedo-sdk:22.1.1.0.0 which tmloadcf
docker run --rm tuxedo-sdk:22.1.1.0.0 which tmboot
```

Expected output:
```
Headers OK
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmloadcf
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmboot
```

---

## Next Steps

1. ✅ Extract SDK: `./extract-sdk.sh`
2. ✅ Build image: `docker build -f Dockerfile.simple -t tuxedo-sdk:22.1.1.0.0 .`
3. ✅ Test C compilation with SDK
4. ⬜ Deploy to OpenShift with C implementation
5. ⬜ Configure real QSPACE

---

## Files

- `extract-sdk.sh` - Automated SDK extraction
- `Dockerfile.simple` - Build SDK base image
- `EXTRACT_SDK.md` - Detailed extraction guide
- `sdk/` - Extracted SDK files (created by script)

---

## Support

- C Implementation: [../tuxedo/README_C_IMPLEMENTATION.md](../tuxedo/README_C_IMPLEMENTATION.md)
- Deploy Kit: [../../deploy-kit/tuxedo-q-c/README.md](../../deploy-kit/tuxedo-q-c/README.md)
- QSPACE Guide: [../../QSPACE_IMPLEMENTATION_GUIDE.md](../../QSPACE_IMPLEMENTATION_GUIDE.md)
