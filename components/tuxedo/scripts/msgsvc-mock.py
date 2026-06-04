#!/usr/bin/env python3
"""
MSGSVC Mock - Simple HTTP server that simulates Tuxedo MSGSVC
Receives POST requests and logs messages (AMQ forwarding TODO)
"""

import http.server
import socketserver
import os
import sys
from datetime import datetime

PORT = 8080
AMQ_HOST = os.getenv('AMQ_HOST', 'amq-broker.demo-amq.svc.cluster.local')
AMQ_PORT = os.getenv('AMQ_PORT', '5672')
AMQ_QUEUE = os.getenv('AMQ_QUEUE', 'TUXEDO.OUT')

class MockTuxedoHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        try:
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length).decode('utf-8')

            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f'[{timestamp}] [Tuxedo-Mock] Received: {body}', flush=True)
            print(f'[{timestamp}] [Tuxedo-Mock] Would send to AMQ {AMQ_HOST}:{AMQ_PORT}/{AMQ_QUEUE}', flush=True)

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            response = '{"status":"success","message":"Message received by Tuxedo mock"}'
            self.wfile.write(response.encode('utf-8'))
        except Exception as e:
            print(f'[Tuxedo-Mock] Error: {e}', flush=True)
            self.send_response(500)
            self.end_headers()

    def do_GET(self):
        # Health check endpoint
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b'Tuxedo Mock Server OK')

    def log_message(self, format, *args):
        pass  # Suppress default logging

if __name__ == '__main__':
    print('[Tuxedo-Mock] Starting MSGSVC mock server', flush=True)
    print(f'[Tuxedo-Mock] Listening on port {PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_HOST: {AMQ_HOST}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_PORT: {AMQ_PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_QUEUE: {AMQ_QUEUE}', flush=True)

    try:
        with socketserver.TCPServer(('', PORT), MockTuxedoHandler) as httpd:
            print(f'[Tuxedo-Mock] Server ready on port {PORT}', flush=True)
            httpd.serve_forever()
    except KeyboardInterrupt:
        print('[Tuxedo-Mock] Shutting down', flush=True)
        sys.exit(0)
    except Exception as e:
        print(f'[Tuxedo-Mock] Fatal error: {e}', flush=True)
        sys.exit(1)
