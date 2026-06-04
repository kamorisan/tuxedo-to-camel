#!/usr/bin/env python3
"""
MSGSVC Mock - HTTP server that simulates Tuxedo MSGSVC
Receives POST requests, logs messages, and forwards to AMQ via AMQP
"""

import http.server
import socketserver
import os
import sys
from datetime import datetime

# Try to import proton for AMQP support
try:
    from proton import Message
    from proton.utils import BlockingSender, BlockingConnection
    AMQP_AVAILABLE = True
except ImportError:
    AMQP_AVAILABLE = False
    print('[Tuxedo-Mock] Warning: proton library not available, AMQ forwarding disabled', flush=True)

PORT = 8080
AMQ_HOST = os.getenv('AMQ_HOST', 'amq-broker-hdls-svc.demo-amq.svc.cluster.local')
AMQ_PORT = os.getenv('AMQ_PORT', '5672')
AMQ_QUEUE = os.getenv('AMQ_QUEUE', 'TUXEDO.OUT')

# Global AMQP connection and sender
amqp_connection = None
amqp_sender = None

def init_amqp():
    """Initialize AMQP connection and sender"""
    global amqp_connection, amqp_sender

    if not AMQP_AVAILABLE:
        return False

    try:
        url = f'{AMQ_HOST}:{AMQ_PORT}'
        print(f'[Tuxedo-Mock] Connecting to AMQP broker at {url}...', flush=True)

        amqp_connection = BlockingConnection(url, timeout=10)
        amqp_sender = amqp_connection.create_sender(AMQ_QUEUE)

        print(f'[Tuxedo-Mock] AMQP connection established to {url}', flush=True)
        print(f'[Tuxedo-Mock] AMQP sender created for queue: {AMQ_QUEUE}', flush=True)
        return True
    except Exception as e:
        print(f'[Tuxedo-Mock] Failed to initialize AMQP: {e}', flush=True)
        amqp_connection = None
        amqp_sender = None
        return False

def send_to_amq(message_body):
    """Send a message to AMQ"""
    global amqp_connection, amqp_sender

    if not amqp_sender:
        print('[Tuxedo-Mock] AMQP sender not initialized, attempting reconnect...', flush=True)
        if not init_amqp():
            return False

    try:
        msg = Message(body=message_body)
        amqp_sender.send(msg, timeout=5)
        print(f'[Tuxedo-Mock] ✓ Sent to AMQ: {message_body[:100]}...', flush=True)
        return True
    except Exception as e:
        print(f'[Tuxedo-Mock] ✗ Failed to send to AMQ: {e}', flush=True)
        # Try to reconnect
        amqp_sender = None
        amqp_connection = None
        return False

class MockTuxedoHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        try:
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length).decode('utf-8')

            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f'[{timestamp}] [Tuxedo-Mock] Received: {body}', flush=True)

            # Forward to AMQ
            if AMQP_AVAILABLE:
                try:
                    success = send_to_amq(body)
                    status_msg = "forwarded to AMQ" if success else "AMQ forward failed"
                except Exception as e:
                    print(f'[Tuxedo-Mock] Exception in send_to_amq: {e}', flush=True)
                    status_msg = "AMQ forward failed"
            else:
                status_msg = "logged (AMQ forwarding disabled)"
                print(f'[{timestamp}] [Tuxedo-Mock] Would send to AMQ {AMQ_HOST}:{AMQ_PORT}/{AMQ_QUEUE}', flush=True)

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            response = f'{{"status":"success","message":"Message received and {status_msg}"}}'
            self.wfile.write(response.encode('utf-8'))
        except Exception as e:
            print(f'[Tuxedo-Mock] Error in do_POST: {e}', flush=True)
            import traceback
            traceback.print_exc()
            self.send_response(500)
            self.end_headers()

    def do_GET(self):
        # Health check endpoint
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        amqp_status = "enabled" if AMQP_AVAILABLE else "disabled"
        connection_status = "connected" if amqp_sender else "disconnected"
        self.wfile.write(f'Tuxedo Mock Server OK (AMQP: {amqp_status}, Connection: {connection_status})'.encode('utf-8'))

    def log_message(self, format, *args):
        pass  # Suppress default logging

if __name__ == '__main__':
    print('[Tuxedo-Mock] Starting MSGSVC mock server', flush=True)
    print(f'[Tuxedo-Mock] Listening on port {PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_HOST: {AMQ_HOST}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_PORT: {AMQ_PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_QUEUE: {AMQ_QUEUE}', flush=True)
    print(f'[Tuxedo-Mock] AMQP Support: {"Available" if AMQP_AVAILABLE else "Not Available"}', flush=True)

    # Initialize AMQP connection
    if AMQP_AVAILABLE:
        init_amqp()

    try:
        with socketserver.TCPServer(('', PORT), MockTuxedoHandler) as httpd:
            print(f'[Tuxedo-Mock] Server ready on port {PORT}', flush=True)
            httpd.serve_forever()
    except KeyboardInterrupt:
        print('[Tuxedo-Mock] Shutting down', flush=True)
        if amqp_connection:
            amqp_connection.close()
        sys.exit(0)
    except Exception as e:
        print(f'[Tuxedo-Mock] Fatal error: {e}', flush=True)
        import traceback
        traceback.print_exc()
        sys.exit(1)
