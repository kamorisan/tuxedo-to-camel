#!/bin/bash
#
# shutdown.sh - Shutdown Tuxedo application
#

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo

cd $APPDIR

echo "[Tuxedo] Shutting down Tuxedo application..."

# Shutdown Tuxedo
tmshutdown -y

echo "[Tuxedo] Shutdown complete"
