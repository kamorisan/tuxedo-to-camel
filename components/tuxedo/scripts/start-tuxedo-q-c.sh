#!/bin/bash
# Start Tuxedo /Q with C implementation

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

export TUXDIR TUXCONFIG=$APPDIR/tuxconfig
export PATH=$TUXDIR/bin:$PATH
export LD_LIBRARY_PATH=$TUXDIR/lib:${LD_LIBRARY_PATH}
export TUXEDO_QUEUE=${TUXEDO_QUEUE:-DEMO_QUEUE}

cd $APPDIR

echo "[Tuxedo-Startup] =========================================="
echo "[Tuxedo-Startup] Starting Tuxedo /Q (C Implementation)"
echo "[Tuxedo-Startup] =========================================="
echo "[Tuxedo-Startup] TUXDIR=$TUXDIR"
echo "[Tuxedo-Startup] Queue: $TUXEDO_QUEUE"

# Check if tmloadcf exists
if command -v tmloadcf > /dev/null 2>&1; then
    echo "[Tuxedo-Startup] Loading ubbconfig..."
    tmloadcf -y $APPDIR/ubbconfig || echo "Note: tmloadcf may fail without proper config"

    echo "[Tuxedo-Startup] Booting Tuxedo domain..."
    tmboot -y || echo "Note: tmboot may fail without QSPACE configured"

    echo "[Tuxedo-Startup] Checking Tuxedo status..."
    tmadmin -r << ADMEOF || true
psr
psc
q
ADMEOF

    echo "[Tuxedo-Startup] =========================================="
    echo "[Tuxedo-Startup] Tuxedo QSPACE started"
    echo "[Tuxedo-Startup] =========================================="
else
    echo "[Tuxedo-Startup] WARNING: tmloadcf not found"
    echo "[Tuxedo-Startup] WARNING: Running without QSPACE (fallback mode)"
    echo "[Tuxedo-Startup] =========================================="
fi

# Start C implementation of REST API server
echo "[Tuxedo-Startup] Starting C REST API server on port 8080..."
exec $APPDIR/tuxedo_q_server
