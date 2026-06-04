#!/bin/bash
#
# Start Tuxedo with QSPACE
#

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

export TUXDIR
export TUXCONFIG=$APPDIR/tuxconfig
export PATH=$TUXDIR/bin:$PATH
export LD_LIBRARY_PATH=$TUXDIR/lib:$LD_LIBRARY_PATH

cd $APPDIR

echo "[Tuxedo-Q] Loading configuration..."
# Convert ubbconfig to binary tuxconfig
tmloadcf -y $APPDIR/ubbconfig

echo "[Tuxedo-Q] Starting Tuxedo domain..."
# Boot Tuxedo
tmboot -y

echo "[Tuxedo-Q] Tuxedo started successfully"
echo "[Tuxedo-Q] Queue Space: DEMO_QUEUE"

# Check status
tmadmin -r << ADMEOF
psr
psc
q
ADMEOF

echo "[Tuxedo-Q] Ready for messages"

# Keep container running and show logs
tail -f $TUXDIR/udataobj/ULOG.*
