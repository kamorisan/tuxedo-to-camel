#!/bin/bash
#
# entrypoint.sh - Container entrypoint script for Tuxedo
#

set -e

APPDIR=/u01/oracle/user_projects/tuxdemo
TUXDIR=/u01/oracle/tuxHome/tuxedo22.1.1.0.0

cd $APPDIR

# Trap signals for graceful shutdown
trap 'echo "[Tuxedo] Received SIGTERM, shutting down..."; ./shutdown.sh; exit 0' SIGTERM SIGINT

# Function to boot Tuxedo
boot_tuxedo() {
    echo "[Tuxedo] Starting Tuxedo Message Service..."
    echo "[Tuxedo] APPDIR: $APPDIR"
    echo "[Tuxedo] TUXDIR: $TUXDIR"
    echo "[Tuxedo] AMQ_HOST: $AMQ_HOST"
    echo "[Tuxedo] AMQ_PORT: $AMQ_PORT"
    echo "[Tuxedo] AMQ_QUEUE: $AMQ_QUEUE"

    # Create TLOG if it doesn't exist
    if [ ! -f TLOG ]; then
        echo "[Tuxedo] Creating TLOG..."
        dd if=/dev/zero of=TLOG bs=1024 count=10240
    fi

    # Load UBB configuration
    echo "[Tuxedo] Loading UBB configuration..."
    tmloadcf -y ubbconfig

    # Boot Tuxedo
    echo "[Tuxedo] Booting Tuxedo services..."
    tmboot -y

    # Wait for services to start
    sleep 3

    # Show status
    echo "[Tuxedo] Application status:"
    tmadmin -r << EOF
psr
psc
quit
EOF

    echo "[Tuxedo] Tuxedo Message Service is ready"
    echo "[Tuxedo] WSL listening on port 8080"
}

# Function to keep container running
keep_running() {
    echo "[Tuxedo] Container running. Press Ctrl+C to stop."

    # Monitor Tuxedo processes
    while true; do
        sleep 30

        # Check if Tuxedo is still running
        if ! tmadmin -r << EOF | grep -q "Master"
psr
quit
EOF
        then
            echo "[Tuxedo] ERROR: Tuxedo services stopped unexpectedly"
            exit 1
        fi
    done
}

# Main execution
case "${1:-run}" in
    run)
        boot_tuxedo
        keep_running
        ;;
    boot)
        boot_tuxedo
        ;;
    shutdown)
        ./shutdown.sh
        ;;
    status)
        tmadmin -r << EOF
psr
psc
quit
EOF
        ;;
    *)
        echo "Usage: $0 {run|boot|shutdown|status}"
        exit 1
        ;;
esac
