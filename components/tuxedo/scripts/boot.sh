#!/bin/bash
#
# boot.sh - Boot Tuxedo application
#

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

cd $APPDIR

echo "[Tuxedo] Booting Tuxedo application..."
echo "[Tuxedo] APPDIR: $APPDIR"
echo "[Tuxedo] TUXDIR: $TUXDIR"

# Create TLOG if it doesn't exist
if [ ! -f TLOG ]; then
    echo "[Tuxedo] Creating TLOG..."
    dd if=/dev/zero of=TLOG bs=1024 count=10240
fi

# Load UBB configuration
if [ ! -f tuxconfig ]; then
    echo "[Tuxedo] Loading UBB configuration..."
    tmloadcf -y ubbconfig
else
    echo "[Tuxedo] Reloading UBB configuration..."
    tmloadcf -y ubbconfig
fi

# Boot Tuxedo
echo "[Tuxedo] Starting Tuxedo services..."
tmboot -y

# Show status
sleep 2
echo "[Tuxedo] Application status:"
tmadmin -r << EOF
psr
psc
quit
EOF

echo "[Tuxedo] Boot complete"
