#!/bin/bash
# Quick compilation test without full image build

set -e

echo "==================================="
echo "C Implementation Compilation Test"
echo "==================================="

# Create stub file
echo "[1/3] Creating Tuxedo stub functions..."
cat > /tmp/tuxedo_stub.c << 'STUB'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Minimal Tuxedo API stubs */
int tperrno = 0;
long tpurcode = 0;

typedef struct {
    long flags;
} TPINIT;

int tpinit(TPINIT *tpinfo) {
    fprintf(stderr, "[STUB] tpinit\n");
    return 0;
}

int tpterm(void) {
    fprintf(stderr, "[STUB] tpterm\n");
    return 0;
}

char *tpalloc(char *type, char *subtype, long size) {
    return malloc(size);
}

void tpfree(char *ptr) {
    free(ptr);
}
STUB

echo "[2/3] Compiling (headers-only, no Tuxedo libs)..."
gcc -I../tuxedo-base-sdk/sdk/include \
    -DTUXEDO_STUB \
    -Wall -O2 \
    src/tuxedo_q_server.c \
    /tmp/tuxedo_stub.c \
    -lmicrohttpd -lpthread \
    -o tuxedo_q_server_test 2>&1 | head -20

if [ -f tuxedo_q_server_test ]; then
    echo "[3/3] ✅ Compilation successful!"
    ls -lh tuxedo_q_server_test
    file tuxedo_q_server_test
    echo ""
    echo "Binary ready for testing."
else
    echo "[3/3] ❌ Compilation failed"
    exit 1
fi
