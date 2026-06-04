#!/bin/bash
# Test Tuxedo /Q C Server

set -e

echo "=========================================="
echo "Tuxedo /Q C Server - Quick Test"
echo "=========================================="

if [ ! -f tuxedo_q_server ]; then
    echo "ERROR: tuxedo_q_server binary not found"
    echo "Run compilation first: make -f Makefile.c-headers-only"
    exit 1
fi

echo ""
echo "[1/4] Starting server in background..."
./tuxedo_q_server &
SERVER_PID=$!
sleep 3

echo "[2/4] Testing health endpoint..."
curl -s http://localhost:8080/health | jq . || curl -s http://localhost:8080/health

echo ""
echo "[3/4] Testing enqueue..."
curl -s -X POST http://localhost:8080/enqueue -d "Test message from script" | jq . || curl -s -X POST http://localhost:8080/enqueue -d "Test message from script"

echo ""
echo "[4/4] Testing dequeue..."
curl -s http://localhost:8080/dequeue | jq . || curl -s http://localhost:8080/dequeue

echo ""
echo "=========================================="
echo "Cleaning up..."
kill $SERVER_PID 2>/dev/null || true
echo "Test complete!"
echo "=========================================="
