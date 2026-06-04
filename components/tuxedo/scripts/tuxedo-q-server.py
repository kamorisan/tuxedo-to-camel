#!/usr/bin/env python3
"""
Tuxedo /Q REST API Server
Provides HTTP endpoints for enqueue/dequeue operations
"""

import http.server
import socketserver
import json
import os
import sys
from datetime import datetime

# Add parent directory to path for tuxedo_queue import
sys.path.insert(0, '/u01/oracle/user_projects/tuxdemo/python')

try:
    from tuxedo_queue import TuxedoQueue
    TUXEDO_Q_AVAILABLE = True
except Exception as e:
    print(f'[TuxedoQ-Server] Warning: TuxedoQueue not available: {e}', flush=True)
    TUXEDO_Q_AVAILABLE = False

PORT = 8080
QUEUE_NAME = os.getenv('TUXEDO_QUEUE', 'DEMO_QUEUE')

# Global TuxedoQueue instance
tuxedo_queue = None

if TUXEDO_Q_AVAILABLE:
    try:
        tuxedo_queue = TuxedoQueue()
    except Exception as e:
        print(f'[TuxedoQ-Server] Failed to initialize TuxedoQueue: {e}', flush=True)


class TuxedoQHandler(http.server.BaseHTTPRequestHandler):
    """HTTP request handler for Tuxedo Queue operations"""
    
    def do_POST(self):
        """Handle POST requests - enqueue and legacy MSGSVC"""
        path = self.path
        
        if path == '/enqueue':
            self._handle_enqueue()
        elif path == '/MSGSVC':
            # Legacy endpoint - enqueue to Tuxedo /Q instead of AMQ
            self._handle_enqueue()
        else:
            self.send_error(404, f'Not Found: {path}')
    
    def do_GET(self):
        """Handle GET requests - dequeue and health check"""
        path = self.path
        
        if path == '/dequeue':
            self._handle_dequeue()
        elif path == '/' or path == '/health':
            self._handle_health()
        else:
            self.send_error(404, f'Not Found: {path}')
    
    def _handle_enqueue(self):
        """Enqueue message to Tuxedo /Q"""
        try:
            # Read request body
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length).decode('utf-8')
            
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f'[{timestamp}] [TuxedoQ-Server] Received for enqueue: {body[:100]}...', flush=True)
            
            if not TUXEDO_Q_AVAILABLE or not tuxedo_queue:
                # Fallback: just log
                print(f'[TuxedoQ-Server] Tuxedo /Q not available, logging only', flush=True)
                response = {
                    'status': 'success',
                    'message': 'Message logged (Tuxedo /Q not available)',
                    'queue': QUEUE_NAME
                }
            else:
                # Enqueue to Tuxedo /Q
                success = tuxedo_queue.enqueue(QUEUE_NAME, body)
                
                if success:
                    response = {
                        'status': 'success',
                        'message': 'Message enqueued to Tuxedo /Q',
                        'queue': QUEUE_NAME
                    }
                else:
                    response = {
                        'status': 'error',
                        'message': 'Failed to enqueue message',
                        'queue': QUEUE_NAME
                    }
            
            # Send response
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps(response).encode('utf-8'))
            
        except Exception as e:
            print(f'[TuxedoQ-Server] Enqueue error: {e}', flush=True)
            import traceback
            traceback.print_exc()
            self.send_error(500, str(e))
    
    def _handle_dequeue(self):
        """Dequeue message from Tuxedo /Q"""
        try:
            if not TUXEDO_Q_AVAILABLE or not tuxedo_queue:
                # No Tuxedo /Q - return empty
                self.send_response(200)
                self.send_header('Content-Type', 'application/json')
                self.end_headers()
                self.wfile.write(json.dumps({'message': None}).encode('utf-8'))
                return
            
            # Dequeue from Tuxedo /Q (non-blocking)
            message = tuxedo_queue.dequeue(QUEUE_NAME, wait=False)
            
            response = {
                'message': message,
                'queue': QUEUE_NAME,
                'timestamp': datetime.now().isoformat()
            }
            
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps(response).encode('utf-8'))
            
        except Exception as e:
            print(f'[TuxedoQ-Server] Dequeue error: {e}', flush=True)
            import traceback
            traceback.print_exc()
            self.send_error(500, str(e))
    
    def _handle_health(self):
        """Health check endpoint"""
        status = {
            'status': 'healthy',
            'tuxedo_q_available': TUXEDO_Q_AVAILABLE and tuxedo_queue is not None,
            'queue': QUEUE_NAME,
            'timestamp': datetime.now().isoformat()
        }
        
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(status).encode('utf-8'))
    
    def log_message(self, format, *args):
        pass  # Suppress default logging


if __name__ == '__main__':
    print('[TuxedoQ-Server] Starting Tuxedo /Q REST API Server', flush=True)
    print(f'[TuxedoQ-Server] Listening on port {PORT}', flush=True)
    print(f'[TuxedoQ-Server] Queue name: {QUEUE_NAME}', flush=True)
    print(f'[TuxedoQ-Server] Tuxedo /Q available: {TUXEDO_Q_AVAILABLE}', flush=True)
    print('[TuxedoQ-Server] Endpoints:', flush=True)
    print('  POST /enqueue   - Enqueue message', flush=True)
    print('  POST /MSGSVC    - Legacy endpoint (enqueue)', flush=True)
    print('  GET  /dequeue   - Dequeue message', flush=True)
    print('  GET  /health    - Health check', flush=True)
    
    try:
        with socketserver.TCPServer(('', PORT), TuxedoQHandler) as httpd:
            print(f'[TuxedoQ-Server] Server ready on port {PORT}', flush=True)
            httpd.serve_forever()
    except KeyboardInterrupt:
        print('[TuxedoQ-Server] Shutting down', flush=True)
        sys.exit(0)
    except Exception as e:
        print(f'[TuxedoQ-Server] Fatal error: {e}', flush=True)
        import traceback
        traceback.print_exc()
        sys.exit(1)
