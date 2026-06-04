# Tuxedo QSPACE Implementation Guide

Complete guide to implement actual Tuxedo QSPACE instead of in-memory fallback queue.

## Current Status

### What We Have Now ✅
- ✅ Python implementation with fallback queue
- ✅ C implementation with fallback queue
- ✅ ubbconfig configuration file
- ✅ REST API server (Python & C)
- ✅ Deployment kit

### What's Missing ❌
- ❌ Tuxedo SDK (headers and build tools)
- ❌ Actual QSPACE server (TMQUEUE) running
- ❌ Persistent queue storage

---

## Architecture Comparison

### Current (Fallback Mode)
```
┌─────────────────────────────┐
│ Tuxedo Container            │
│                             │
│ ┌─────────────────────────┐ │
│ │ REST API Server         │ │
│ │  - tpenqueue() FAILS    │ │
│ │  - tpdequeue() FAILS    │ │
│ └────────┬────────────────┘ │
│          ↓                   │
│ ┌─────────────────────────┐ │
│ │ Fallback Queue          │ │
│ │ (Python list / C list)  │ │
│ │ - In-memory only        │ │
│ │ - Pod restart = LOST    │ │
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

### Target (Real QSPACE)
```
┌─────────────────────────────┐
│ Tuxedo Container            │
│                             │
│ ┌─────────────────────────┐ │
│ │ tmboot (Tuxedo Boot)    │ │
│ └────────┬────────────────┘ │
│          ↓                   │
│ ┌─────────────────────────┐ │
│ │ TMQUEUE Server          │ │
│ │ - QSPACE: QSPACE        │ │
│ │ - Queue: DEMO_QUEUE     │ │
│ │ - Persistent storage    │ │
│ └────────┬────────────────┘ │
│          ↑                   │
│ ┌─────────────────────────┐ │
│ │ REST API Server         │ │
│ │  - tpenqueue() ✅       │ │
│ │  - tpdequeue() ✅       │ │
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

---

## Prerequisites

### 1. Tuxedo SDK Installation

**What You Need**:
- Oracle Tuxedo 22.1.1.0.0 **SDK** (not just runtime)
- Include files:
  - `/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/atmi.h`
  - `/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/tmqueue.h`
  - `/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/fml.h`
  - `/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/fml32.h`
- Build tools:
  - `tmloadcf` - Load configuration
  - `tmboot` - Boot Tuxedo domain
  - `tmshutdown` - Shutdown Tuxedo domain
  - `tmadmin` - Administration tool
  - `buildserver` - Server build tool
  - `buildclient` - Client build tool

**How to Obtain**:

#### Option A: Oracle Download
1. Go to: https://www.oracle.com/middleware/technologies/tuxedo-downloads.html
2. Download: Oracle Tuxedo 22.1.1.0.0 for Linux x86-64
3. Requires Oracle account
4. License: Check Oracle Tuxedo licensing terms

#### Option B: Container Image with SDK
```bash
# If you have access to a full Tuxedo SDK image
docker pull container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0
```

#### Option C: Extract from Existing Installation
If you have Tuxedo installed elsewhere:
```bash
# Copy SDK files
scp -r user@tuxedo-server:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include ./
scp -r user@tuxedo-server:/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin ./
```

---

## Implementation Steps

### Step 1: Update Base Image

**Current**: `oracle/tuxedo:22.1.1.0.0` (Runtime only)  
**Need**: Tuxedo SDK image or manual SDK installation

**Option 1: Use SDK Image**
```dockerfile
# components/tuxedo-base/Dockerfile.sdk
FROM container-registry.oracle.com/middleware/tuxedo_dev:22.1.1.0.0

# Or if using downloaded installer
FROM oraclelinux:8

# Install Tuxedo SDK
COPY tuxedo221100_64_linux_x86_64.zip /tmp/
RUN unzip /tmp/tuxedo221100_64_linux_x86_64.zip && \
    ./tuxedo221100_64_linux_x86_64.bin -silent -responseFile response.txt
```

**Option 2: Add SDK to Runtime Image**
```dockerfile
FROM oracle/tuxedo:22.1.1.0.0

USER root

# Copy SDK files from local extraction
COPY --chown=oracle:oracle include/ /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/
COPY --chown=oracle:oracle bin/tmloadcf /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
COPY --chown=oracle:oracle bin/tmboot /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/
COPY --chown=oracle:oracle bin/tmadmin /u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/

USER oracle
```

### Step 2: Verify SDK Installation

```bash
# Inside container
which tmloadcf
which tmboot
ls /u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/atmi.h
```

Expected output:
```
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmloadcf
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/bin/tmboot
/u01/oracle/tuxHome/tuxedo22.1.1.0.0/include/atmi.h
```

### Step 3: QSPACE Configuration

**Current ubbconfig** (components/tuxedo/config/ubbconfig):
```
*RESOURCES
IPCKEY          123456
DOMAINID        tuxdemo
MASTER          simpapp
MODEL           SHM
LDBAL           N

*MACHINES
"tuxedo-msgsvc" LMID=simpapp
                APPDIR="/u01/oracle/user_projects/tuxdemo"
                TUXCONFIG="/u01/oracle/user_projects/tuxdemo/tuxconfig"
                TUXDIR="/u01/oracle/tuxHome/tuxedo22.1.1.0.0"

*GROUPS
GROUP1          LMID=simpapp GRPNO=1

*SERVERS
TMQUEUE         SRVGRP=GROUP1 SRVID=1
                MIN=1 MAX=1 RESTART=Y GRACE=0
                CLOPT="-s QSPACE:DEMO_QUEUE --"

*SERVICES
QSPACE
```

**Add Persistent Storage** (for production):
```
*RESOURCES
...

*GROUPS
GROUP1          LMID=simpapp GRPNO=1 TMSNAME=TMS_QM OPENINFO="TUXEDO/QM:DEMO_QUEUE:/u01/oracle/user_projects/tuxdemo/queuedata"

*SERVERS
TMQUEUE         SRVGRP=GROUP1 SRVID=1
                MIN=1 MAX=1 RESTART=Y GRACE=0
                CLOPT="-s QSPACE:DEMO_QUEUE -- -m 1 -x -e 100"

*MACHINES
...
                QMCONFIG="/u01/oracle/user_projects/tuxdemo/qdevice.conf"
```

**Create qdevice.conf**:
```bash
cat > /u01/oracle/user_projects/tuxdemo/qdevice.conf << 'QCONF'
#QMCONFIG

DEMO_QUEUE:
    IPCKEY      123457
    DIRECTORY   /u01/oracle/user_projects/tuxdemo/queuedata
    MAXMSG      1000
    MAXPROC     50
QCONF
```

### Step 4: Boot QSPACE

**Test Boot**:
```bash
cd /u01/oracle/user_projects/tuxdemo

# Set environment
export TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0
export TUXCONFIG=/u01/oracle/user_projects/tuxdemo/tuxconfig
export PATH=$TUXDIR/bin:$PATH
export LD_LIBRARY_PATH=$TUXDIR/lib:$LD_LIBRARY_PATH

# Load configuration
tmloadcf -y ubbconfig

# Boot Tuxedo
tmboot -y

# Check status
tmadmin
> psr
> psc
> q
```

Expected output:
```
Booting all admin and server processes in /u01/oracle/user_projects/tuxdemo/tuxconfig

INFO: Oracle Tuxedo, Version 22.1.1.0.0, 64-bit
Booting server processes ...
        Boot of TMQUEUE (SRVID=1) succeeded

1 process started.
```

### Step 5: Test QSPACE Operations

**Using tmadmin**:
```bash
tmadmin
> psr
Prog Name      Queue Name  Grp Name      ID RqDone Load Done Current Service
---------      ----------  --------      -- ------ --------- ---------------
TMQUEUE        00001.00001 GROUP1         1      0         0 ( IDLE )

> qstats DEMO_QUEUE
Queue Name: DEMO_QUEUE
  Current Entries: 0
  Maximum Entries: 100
  ...
```

**Using qmadmin**:
```bash
qmadmin
> stats QSPACE DEMO_QUEUE
Queue Space: QSPACE
Queue Name: DEMO_QUEUE
  Current Messages: 0
  High Water Mark: 0
```

### Step 6: Update Startup Scripts

**components/tuxedo/scripts/start-tuxedo-with-qspace.sh**:
```bash
#!/bin/bash
set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

export TUXDIR TUXCONFIG=$APPDIR/tuxconfig
export PATH=$TUXDIR/bin:$PATH
export LD_LIBRARY_PATH=$TUXDIR/lib:${LD_LIBRARY_PATH}

cd $APPDIR

echo "[Tuxedo-Startup] Loading ubbconfig..."
tmloadcf -y $APPDIR/ubbconfig

echo "[Tuxedo-Startup] Booting Tuxedo domain with QSPACE..."
tmboot -y

echo "[Tuxedo-Startup] Checking QSPACE status..."
tmadmin -r << ADMEOF
psr
psc
qstats DEMO_QUEUE
q
ADMEOF

echo "[Tuxedo-Startup] QSPACE ready, starting REST API server..."
exec $APPDIR/tuxedo_q_server
```

### Step 7: Test End-to-End

**1. Enqueue via REST API**:
```bash
curl -X POST http://localhost:8080/enqueue \
  -d "Test message to real QSPACE"
```

**2. Check QSPACE**:
```bash
tmadmin
> qstats DEMO_QUEUE
# Should show: Current Entries: 1
```

**3. Dequeue via REST API**:
```bash
curl http://localhost:8080/dequeue
# Should return: {"message": "Test message to real QSPACE", ...}
```

**4. Verify QSPACE empty**:
```bash
tmadmin
> qstats DEMO_QUEUE
# Should show: Current Entries: 0
```

---

## Troubleshooting

### tmloadcf: command not found
**Cause**: Tuxedo SDK not installed  
**Solution**: Install Tuxedo SDK (see Prerequisites)

### tmboot fails: TUXCONFIG not found
**Cause**: tmloadcf not run yet  
**Solution**: Run `tmloadcf -y ubbconfig` first

### TMQUEUE fails to start
**Cause**: QSPACE configuration issue  
**Solution**: Check ubbconfig, ensure QSPACE service defined

### tpenqueue returns TPENOENT
**Cause**: QSPACE not running or queue name wrong  
**Solution**: 
```bash
tmadmin
> psr  # Check TMQUEUE is running
> qstats DEMO_QUEUE  # Verify queue exists
```

### Permission denied accessing queue files
**Cause**: Incorrect directory permissions  
**Solution**:
```bash
chown -R oracle:oracle /u01/oracle/user_projects/tuxdemo/queuedata
chmod 750 /u01/oracle/user_projects/tuxdemo/queuedata
```

---

## Performance Tuning

### QSPACE Parameters

**ubbconfig CLOPT**:
```
TMQUEUE CLOPT="-s QSPACE:DEMO_QUEUE -- -m 100 -x -e 1000"
  -m 100  : Max 100 concurrent messages
  -x      : Enable XA transactions
  -e 1000 : Error queue entries
```

**qdevice.conf**:
```
DEMO_QUEUE:
    MAXMSG      10000    # Max messages in queue
    MAXPROC     100      # Max concurrent processes
    MSGSIZE     32768    # Max message size (32KB)
```

---

## Migration from Fallback to QSPACE

### Zero-Downtime Migration

1. **Deploy new version with QSPACE**
2. **Run both in parallel** (fallback drains, QSPACE fills)
3. **Switch traffic** to QSPACE endpoints
4. **Remove fallback** code

### Data Migration

**Fallback queue → QSPACE**:
```python
# Drain fallback queue
while True:
    msg = dequeue_fallback()
    if not msg:
        break
    enqueue_tuxedo(msg)
```

---

## Next Steps

1. ✅ **Obtain Tuxedo SDK**
   - Download from Oracle
   - Extract headers and binaries

2. ✅ **Update Container Image**
   - Add SDK to base image
   - Rebuild tuxedo-msgsvc

3. ✅ **Test QSPACE Boot**
   - Run tmloadcf + tmboot
   - Verify TMQUEUE running

4. ✅ **Test REST API**
   - tpenqueue should succeed
   - tpdequeue should succeed
   - Fallback no longer used

5. ✅ **Configure Persistence**
   - Add qdevice.conf
   - Test Pod restart
   - Messages should persist

6. ✅ **Performance Testing**
   - Measure throughput
   - Tune QSPACE parameters

---

## Checklist

Current implementation (fallback):
- [x] Python ctypes implementation
- [x] C ATMI implementation
- [x] In-memory fallback queue
- [x] REST API endpoints
- [x] E2E testing

QSPACE implementation:
- [ ] Tuxedo SDK obtained
- [ ] Headers available (atmi.h, tmqueue.h)
- [ ] Build tools available (tmloadcf, tmboot)
- [ ] ubbconfig loaded successfully
- [ ] TMQUEUE server started
- [ ] tpenqueue succeeds
- [ ] tpdequeue succeeds
- [ ] Persistent storage configured
- [ ] Messages survive Pod restart
- [ ] Performance tuned

---

## Estimated Effort

| Task | Time | Difficulty |
|------|------|------------|
| Obtain Tuxedo SDK | 1-4h | Medium |
| Update base image | 1-2h | Low |
| Configure QSPACE | 1-2h | Medium |
| Test basic operations | 1h | Low |
| Configure persistence | 2-3h | Medium |
| Performance tuning | 2-4h | Medium |
| **Total** | **8-16h** | **Medium** |

---

## References

- Oracle Tuxedo Administration Guide
- Oracle Tuxedo ATMI Programming Guide
- TMQUEUE Server Reference
- Queue Space Administrator's Guide
- Current implementation: `components/tuxedo/src/tuxedo_q_server.c`
