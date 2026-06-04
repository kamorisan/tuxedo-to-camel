#!/bin/bash
# Start Tuxedo with QSPACE and REST API server

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

export TUXDIR TUXCONFIG=$APPDIR/tuxconfig
export PATH=$TUXDIR/bin:$PATH
export LD_LIBRARY_PATH=$TUXDIR/lib:$LD_LIBRARY_PATH
export TUXEDO_QUEUE=${TUXEDO_QUEUE:-DEMO_QUEUE}

cd $APPDIR

echo "[Tuxedo-Startup] =========================================="
echo "[Tuxedo-Startup] Starting Tuxedo with QSPACE"
echo "[Tuxedo-Startup] =========================================="

# Load Tuxedo configuration
echo "[Tuxedo-Startup] Loading ubbconfig..."
tmloadcf -y $APPDIR/ubbconfig

# Boot Tuxedo domain
echo "[Tuxedo-Startup] Booting Tuxedo domain..."
tmboot -y

# Check status
echo "[Tuxedo-Startup] Checking Tuxedo status..."
tmadmin -r << ADMEOF
psr
psc
q
ADMEOF

echo "[Tuxedo-Startup] =========================================="
echo "[Tuxedo-Startup] Tuxedo QSPACE started successfully"
echo "[Tuxedo-Startup] =========================================="

# Start REST API server in foreground
echo "[Tuxedo-Startup] Starting REST API server on port 8080..."
exec python3 $APPDIR/tuxedo-q-server.py
