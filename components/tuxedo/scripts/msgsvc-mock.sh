#!/bin/bash
#
# MSGSVC Mock - Simple HTTP server that forwards messages to AMQ
# This replaces the compiled Tuxedo service for demo purposes
#

AMQ_HOST=${AMQ_HOST:-amq-broker.demo-amq.svc.cluster.local}
AMQ_PORT=${AMQ_PORT:-5672}
AMQ_QUEUE=${AMQ_QUEUE:-TUXEDO.OUT}

echo "[Tuxedo-Mock] Starting MSGSVC mock server on port 8080"
echo "[Tuxedo-Mock] AMQ_HOST: $AMQ_HOST"
echo "[Tuxedo-Mock] AMQ_PORT: $AMQ_PORT"
echo "[Tuxedo-Mock] AMQ_QUEUE: $AMQ_QUEUE"

# Simple HTTP server using netcat or Python
# For now, use a simple Python HTTP server that forwards to AMQ
python3 -c "
import http.server
import socketserver
import os
import sys
from datetime import datetime

PORT = 8080

class MockTuxedoHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length).decode('utf-8')

        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        print(f'[{timestamp}] [Tuxedo-Mock] Received: {body}')

        # TODO: Forward to AMQ (for now, just log)
        print(f'[{timestamp}] [Tuxedo-Mock] Would send to AMQ: {body}')

        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        response = '{\"status\":\"success\",\"message\":\"Message received by Tuxedo mock\"}'
        self.wfile.write(response.encode('utf-8'))

    def log_message(self, format, *args):
        pass  # Suppress default logging

try:
    with socketserver.TCPServer(('', PORT), MockTuxedoHandler) as httpd:
        print(f'[Tuxedo-Mock] Server ready on port {PORT}')
        httpd.serve_forever()
except KeyboardInterrupt:
    print('[Tuxedo-Mock] Shutting down')
    sys.exit(0)
"
