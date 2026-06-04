#!/usr/bin/env python3
"""
MSGSVC Mock - HTTP server that simulates Tuxedo MSGSVC
Receives POST requests, logs messages, and forwards to AMQ via AMQP
"""

import http.server
import socketserver
import os
import sys
import threading
from datetime import datetime

# Try to import proton for AMQP support
try:
    from proton import Message
    from proton.reactor import Container
    from proton.handlers import MessagingHandler
    AMQP_AVAILABLE = True
except ImportError:
    AMQP_AVAILABLE = False
    print('[Tuxedo-Mock] Warning: proton library not available, AMQ forwarding disabled', flush=True)

PORT = 8080
AMQ_HOST = os.getenv('AMQ_HOST', 'amq-broker-hdls-svc.demo-amq.svc.cluster.local')
AMQ_PORT = os.getenv('AMQ_PORT', '5672')
AMQ_QUEUE = os.getenv('AMQ_QUEUE', 'TUXEDO.OUT')

class AMQPSender(MessagingHandler):
    """AMQP message sender using proton reactor pattern"""
    def __init__(self, url, queue):
        super(AMQPSender, self).__init__()
        self.url = url
        self.queue = queue
        self.sender = None
        self.container = None
        self.pending_messages = []
        self.lock = threading.Lock()

    def on_start(self, event):
        """Called when the reactor starts"""
        conn = event.container.connect(self.url)
        self.sender = event.container.create_sender(conn, self.queue)
        print(f'[Tuxedo-Mock] AMQP connection established to {self.url}', flush=True)

    def on_sendable(self, event):
        """Called when the sender is ready to send messages"""
        with self.lock:
            while self.pending_messages and event.sender.credit:
                message_body = self.pending_messages.pop(0)
                msg = Message(body=message_body)
                event.sender.send(msg)
                print(f'[Tuxedo-Mock] Sent to AMQ: {message_body}', flush=True)

    def queue_message(self, message_body):
        """Queue a message for sending"""
        with self.lock:
            self.pending_messages.append(message_body)
        # Wake up the reactor to process the message
        if self.container:
            self.container.wakeup()

    def on_accepted(self, event):
        """Called when a message is accepted by the broker"""
        print(f'[Tuxedo-Mock] Message accepted by AMQ broker', flush=True)

    def on_rejected(self, event):
        """Called when a message is rejected"""
        print(f'[Tuxedo-Mock] Message rejected by AMQ broker', flush=True)

    def on_transport_error(self, event):
        """Called when there's a transport error"""
        print(f'[Tuxedo-Mock] AMQP transport error: {event.transport.condition}', flush=True)

# Global AMQP sender instance
amqp_sender = None
amqp_container = None

class MockTuxedoHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        try:
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length).decode('utf-8')

            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f'[{timestamp}] [Tuxedo-Mock] Received: {body}', flush=True)

            # Forward to AMQ
            if AMQP_AVAILABLE and amqp_sender:
                try:
                    amqp_sender.queue_message(body)
                    status_msg = "forwarded to AMQ"
                except Exception as e:
                    print(f'[Tuxedo-Mock] Failed to queue message: {e}', flush=True)
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
            print(f'[Tuxedo-Mock] Error: {e}', flush=True)
            self.send_response(500)
            self.end_headers()

    def do_GET(self):
        # Health check endpoint
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        amqp_status = "enabled" if AMQP_AVAILABLE else "disabled"
        self.wfile.write(f'Tuxedo Mock Server OK (AMQP: {amqp_status})'.encode('utf-8'))

    def log_message(self, format, *args):
        pass  # Suppress default logging

if __name__ == '__main__':
    print('[Tuxedo-Mock] Starting MSGSVC mock server', flush=True)
    print(f'[Tuxedo-Mock] Listening on port {PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_HOST: {AMQ_HOST}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_PORT: {AMQ_PORT}', flush=True)
    print(f'[Tuxedo-Mock] AMQ_QUEUE: {AMQ_QUEUE}', flush=True)
    print(f'[Tuxedo-Mock] AMQP Support: {"Available" if AMQP_AVAILABLE else "Not Available"}', flush=True)

    # Initialize AMQP sender if available
    if AMQP_AVAILABLE:
        amqp_url = f'{AMQ_HOST}:{AMQ_PORT}'
        amqp_sender = AMQPSender(amqp_url, AMQ_QUEUE)

        # Start AMQP container in a separate thread
        def run_amqp():
            global amqp_container
            amqp_container = Container(amqp_sender)
            amqp_sender.container = amqp_container
            try:
                amqp_container.run()
            except Exception as e:
                print(f'[Tuxedo-Mock] AMQP container error: {e}', flush=True)

        amqp_thread = threading.Thread(target=run_amqp, daemon=True)
        amqp_thread.start()
        print('[Tuxedo-Mock] AMQP container started in background', flush=True)

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
