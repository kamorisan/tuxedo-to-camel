#!/usr/bin/env python3
"""
Tuxedo Queue (TMQUEUE) Python wrapper using ctypes
Provides enqueue/dequeue operations for Tuxedo /Q
"""

import os
import sys
from ctypes import *
from datetime import datetime

# Tuxedo library paths
TUXDIR = os.getenv('TUXDIR', '/u01/oracle/tuxHome/tuxedo22.1.1.0.0')
LIBTUX = f'{TUXDIR}/lib/libtux.so'

# TPQCTL structure (simplified)
class TPQCTL(Structure):
    _fields_ = [
        ("flags", c_long),          # Operation flags
        ("deq_time", c_long),       # Dequeue time
        ("priority", c_long),       # Message priority
        ("diagnostic", c_long),     # Error diagnostic
        ("msgid", c_char * 32),     # Message ID
        ("corrid", c_char * 32),    # Correlation ID
        ("replyqueue", c_char * 128),   # Reply queue
        ("failurequeue", c_char * 128), # Failure queue
        ("cltid", c_char * 64),     # Client ID
        ("urcode", c_long),         # User return code
        ("appkey", c_long),         # Application key
        ("delivery_qos", c_long),   # Delivery QoS
        ("reply_qos", c_long),      # Reply QoS
        ("exp_time", c_long),       # Expiration time
    ]

# ATMI constants
TPNOFLAGS = 0x00000000
TPNOTIME = 0x00000001
TPNOTRAN = 0x00000002

# Queue control flags
TPQCORRID = 0x00000001
TPQFAILUREQ = 0x00000002
TPQREPLYQ = 0x00000004
TPQPRIORITY = 0x00000008
TPQTOP = 0x00000010
TPQWAIT = 0x00000020

class TuxedoQueue:
    """Tuxedo Queue wrapper using ctypes"""
    
    def __init__(self, qspace='QSPACE'):
        self.qspace = qspace
        self.libtux = None
        self._initialize()
    
    def _initialize(self):
        """Initialize Tuxedo library and tpinit"""
        try:
            # Load Tuxedo library
            self.libtux = CDLL(LIBTUX)
            
            # Define function signatures
            # int tpenqueue(char *qspace, TPQCTL *ctl, char *data, long len, long flags)
            self.libtux.tpenqueue.argtypes = [c_char_p, POINTER(TPQCTL), c_char_p, c_long, c_long]
            self.libtux.tpenqueue.restype = c_int
            
            # int tpdequeue(char *qspace, TPQCTL *ctl, char **data, long *len, long flags)  
            self.libtux.tpdequeue.argtypes = [c_char_p, POINTER(TPQCTL), POINTER(c_char_p), POINTER(c_long), c_long]
            self.libtux.tpdequeue.restype = c_int
            
            # char *tpalloc(char *type, char *subtype, long size)
            self.libtux.tpalloc.argtypes = [c_char_p, c_char_p, c_long]
            self.libtux.tpalloc.restype = c_void_p
            
            # void tpfree(char *ptr)
            self.libtux.tpfree.argtypes = [c_void_p]
            self.libtux.tpfree.restype = None
            
            # int tpinit(TPINIT *tpinfo)
            self.libtux.tpinit.argtypes = [c_void_p]
            self.libtux.tpinit.restype = c_int
            
            # int tpterm()
            self.libtux.tpterm.argtypes = []
            self.libtux.tpterm.restype = c_int
            
            # Initialize Tuxedo context
            ret = self.libtux.tpinit(None)
            if ret == -1:
                print('[TuxedoQueue] Warning: tpinit failed, continuing anyway', flush=True)
            
            print(f'[TuxedoQueue] Initialized with QSPACE: {self.qspace}', flush=True)
            
        except Exception as e:
            print(f'[TuxedoQueue] Failed to initialize: {e}', flush=True)
            raise
    
    def enqueue(self, queue_name, message, priority=50):
        """
        Enqueue a message to Tuxedo queue
        
        Args:
            queue_name: Queue name (e.g., 'DEMO_QUEUE')
            message: Message string
            priority: Message priority (0-100, default 50)
            
        Returns:
            True if successful, False otherwise
        """
        try:
            # Prepare TPQCTL
            qctl = TPQCTL()
            qctl.flags = TPQPRIORITY
            qctl.priority = priority
            
            # Convert message to bytes
            msg_bytes = message.encode('utf-8')
            
            # Call tpenqueue
            ret = self.libtux.tpenqueue(
                self.qspace.encode('utf-8'),
                byref(qctl),
                msg_bytes,
                len(msg_bytes),
                TPNOTRAN
            )
            
            if ret == -1:
                print(f'[TuxedoQueue] tpenqueue failed for queue {queue_name}', flush=True)
                return False
            
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f'[{timestamp}] [TuxedoQueue] ✓ Enqueued to {queue_name}: {message[:100]}...', flush=True)
            return True
            
        except Exception as e:
            print(f'[TuxedoQueue] Enqueue error: {e}', flush=True)
            return False
    
    def dequeue(self, queue_name, wait=True):
        """
        Dequeue a message from Tuxedo queue
        
        Args:
            queue_name: Queue name (e.g., 'DEMO_QUEUE')  
            wait: Wait for message if queue is empty
            
        Returns:
            Message string if successful, None if queue empty or error
        """
        try:
            # Prepare TPQCTL
            qctl = TPQCTL()
            qctl.flags = TPQWAIT if wait else TPNOFLAGS
            
            # Allocate buffer for message
            buf_size = 1024 * 10  # 10KB
            buf_ptr = c_char_p()
            buf_len = c_long(buf_size)
            
            # Call tpdequeue
            ret = self.libtux.tpdequeue(
                self.qspace.encode('utf-8'),
                byref(qctl),
                byref(buf_ptr),
                byref(buf_len),
                TPNOTRAN
            )
            
            if ret == -1:
                # Queue might be empty (not necessarily an error)
                return None
            
            # Extract message
            if buf_ptr and buf_len.value > 0:
                message = buf_ptr[:buf_len.value].decode('utf-8', errors='ignore')
                
                # Free buffer
                if buf_ptr:
                    self.libtux.tpfree(buf_ptr)
                
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                print(f'[{timestamp}] [TuxedoQueue] ✓ Dequeued from {queue_name}: {message[:100]}...', flush=True)
                return message
            
            return None
            
        except Exception as e:
            print(f'[TuxedoQueue] Dequeue error: {e}', flush=True)
            return None
    
    def __del__(self):
        """Cleanup Tuxedo context"""
        try:
            if self.libtux:
                self.libtux.tpterm()
        except:
            pass


# Simple test if run directly
if __name__ == '__main__':
    print('[TuxedoQueue] Testing Tuxedo Queue operations...')
    
    try:
        tq = TuxedoQueue()
        
        # Test enqueue
        print('[TuxedoQueue] Testing enqueue...')
        success = tq.enqueue('DEMO_QUEUE', 'Test message from Python ctypes')
        print(f'[TuxedoQueue] Enqueue result: {success}')
        
        # Test dequeue
        print('[TuxedoQueue] Testing dequeue...')
        msg = tq.dequeue('DEMO_QUEUE', wait=False)
        print(f'[TuxedoQueue] Dequeue result: {msg}')
        
    except Exception as e:
        print(f'[TuxedoQueue] Test failed: {e}')
        import traceback
        traceback.print_exc()
