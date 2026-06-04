/*
 * Tuxedo /Q REST API Server (C implementation)
 * Provides HTTP endpoints for enqueue/dequeue operations using ATMI API
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <atmi.h>
#include <tmqueue.h>
#include <microhttpd.h>

#define PORT 8080
#define MAX_MSG_SIZE 10240
#define QSPACE "QSPACE"

/* Global queue name */
static char queue_name[128] = "DEMO_QUEUE";

/* In-memory fallback queue for when QSPACE is not available */
typedef struct message_node {
    char *data;
    struct message_node *next;
} message_node_t;

static message_node_t *queue_head = NULL;
static message_node_t *queue_tail = NULL;
static int queue_size = 0;
static int tuxedo_available = 0;

/* Function prototypes */
static int enqueue_fallback(const char *message);
static char* dequeue_fallback(void);
static int enqueue_tuxedo(const char *message);
static char* dequeue_tuxedo(void);
static int handle_request(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls);

/* Get current timestamp */
static void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = gmtime(&now);
    strftime(buffer, size, "%Y-%m-%dT%H:%M:%SZ", t);
}

/* Initialize Tuxedo context */
static int init_tuxedo(void) {
    if (tpinit(NULL) == -1) {
        fprintf(stderr, "[TuxedoQ-Server] Warning: tpinit failed (code %d), using fallback queue\n", tperrno);
        tuxedo_available = 0;
        return -1;
    }
    
    printf("[TuxedoQ-Server] Tuxedo context initialized successfully\n");
    tuxedo_available = 1;
    return 0;
}

/* Enqueue to in-memory fallback queue */
static int enqueue_fallback(const char *message) {
    message_node_t *node = (message_node_t*)malloc(sizeof(message_node_t));
    if (!node) {
        fprintf(stderr, "[TuxedoQ-Server] Memory allocation failed\n");
        return -1;
    }
    
    node->data = strdup(message);
    node->next = NULL;
    
    if (queue_tail) {
        queue_tail->next = node;
    } else {
        queue_head = node;
    }
    queue_tail = node;
    queue_size++;
    
    printf("[TuxedoQ-Server] Enqueued to in-memory fallback queue (size: %d)\n", queue_size);
    return 0;
}

/* Dequeue from in-memory fallback queue */
static char* dequeue_fallback(void) {
    if (!queue_head) {
        return NULL;
    }
    
    message_node_t *node = queue_head;
    char *data = node->data;
    
    queue_head = node->next;
    if (!queue_head) {
        queue_tail = NULL;
    }
    queue_size--;
    
    free(node);
    
    printf("[TuxedoQ-Server] Dequeued from in-memory fallback queue (remaining: %d)\n", queue_size);
    return data;
}

/* Enqueue to Tuxedo /Q using ATMI API */
static int enqueue_tuxedo(const char *message) {
    TPQCTL qctl;
    char *buf;
    long len;
    
    /* Allocate Tuxedo buffer */
    len = strlen(message) + 1;
    buf = tpalloc("STRING", NULL, len);
    if (!buf) {
        fprintf(stderr, "[TuxedoQ-Server] tpalloc failed (code %d)\n", tperrno);
        return -1;
    }
    
    strcpy(buf, message);
    
    /* Initialize queue control structure */
    memset(&qctl, 0, sizeof(qctl));
    qctl.flags = TPQCORRID;
    
    /* Enqueue message */
    if (tpenqueue(QSPACE, queue_name, &qctl, buf, len, TPNOTRAN) == -1) {
        fprintf(stderr, "[TuxedoQ-Server] tpenqueue failed for queue %s (code %d)\n", 
                queue_name, tperrno);
        tpfree(buf);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    printf("[%s] [TuxedoQ-Server] ✓ Enqueued to %s: %.100s...\n", 
           timestamp, queue_name, message);
    
    tpfree(buf);
    return 0;
}

/* Dequeue from Tuxedo /Q using ATMI API */
static char* dequeue_tuxedo(void) {
    TPQCTL qctl;
    char *buf;
    long len;
    char *result = NULL;
    
    /* Allocate Tuxedo buffer */
    len = MAX_MSG_SIZE;
    buf = tpalloc("STRING", NULL, len);
    if (!buf) {
        fprintf(stderr, "[TuxedoQ-Server] tpalloc failed (code %d)\n", tperrno);
        return NULL;
    }
    
    /* Initialize queue control structure */
    memset(&qctl, 0, sizeof(qctl));
    qctl.flags = 0; /* Non-blocking */
    
    /* Dequeue message */
    if (tpdequeue(QSPACE, queue_name, &qctl, &buf, &len, TPNOTRAN) == -1) {
        if (tperrno != TPEDIAGNOSTIC && tperrno != TPENOENT) {
            fprintf(stderr, "[TuxedoQ-Server] tpdequeue failed (code %d)\n", tperrno);
        }
        tpfree(buf);
        return NULL;
    }
    
    /* Copy result */
    result = strdup(buf);
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    printf("[%s] [TuxedoQ-Server] ✓ Dequeued from %s: %.100s...\n", 
           timestamp, queue_name, result);
    
    tpfree(buf);
    return result;
}

/* HTTP request handler */
static int handle_request(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    
    static int dummy;
    const char *page;
    struct MHD_Response *response;
    int ret;
    char json_response[2048];
    char timestamp[64];
    
    /* Handle upload data for POST */
    if (strcmp(method, "POST") == 0) {
        if (*con_cls == NULL) {
            *con_cls = &dummy;
            return MHD_YES;
        }
        
        if (*upload_data_size != 0) {
            /* Process upload data */
            if (*upload_data_size > MAX_MSG_SIZE) {
                snprintf(json_response, sizeof(json_response),
                        "{\"status\": \"error\", \"message\": \"Message too large\"}");
                response = MHD_create_response_from_buffer(strlen(json_response),
                           (void*)json_response, MHD_RESPMEM_MUST_COPY);
                MHD_add_response_header(response, "Content-Type", "application/json");
                ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                MHD_destroy_response(response);
                return ret;
            }
            
            /* Handle /enqueue or /MSGSVC */
            if (strcmp(url, "/enqueue") == 0 || strcmp(url, "/MSGSVC") == 0) {
                char message[MAX_MSG_SIZE];
                snprintf(message, sizeof(message), "%.*s", (int)*upload_data_size, upload_data);
                
                get_timestamp(timestamp, sizeof(timestamp));
                printf("[%s] [TuxedoQ-Server] Received for enqueue: %.100s...\n", 
                       timestamp, message);
                
                int success = 0;
                
                /* Try Tuxedo /Q first */
                if (tuxedo_available) {
                    success = (enqueue_tuxedo(message) == 0);
                }
                
                /* Fallback to in-memory queue */
                if (!success) {
                    success = (enqueue_fallback(message) == 0);
                }
                
                if (success) {
                    snprintf(json_response, sizeof(json_response),
                            "{\"status\": \"success\", \"message\": \"Message enqueued\", \"queue\": \"%s\"}",
                            queue_name);
                } else {
                    snprintf(json_response, sizeof(json_response),
                            "{\"status\": \"error\", \"message\": \"Failed to enqueue message\", \"queue\": \"%s\"}",
                            queue_name);
                }
            } else {
                snprintf(json_response, sizeof(json_response),
                        "{\"status\": \"error\", \"message\": \"Unknown endpoint\"}");
            }
            
            *upload_data_size = 0;
            response = MHD_create_response_from_buffer(strlen(json_response),
                       (void*)json_response, MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        
        return MHD_YES;
    }
    
    /* Handle GET requests */
    if (strcmp(method, "GET") == 0) {
        if (strcmp(url, "/dequeue") == 0) {
            char *message = NULL;
            
            /* Try Tuxedo /Q first */
            if (tuxedo_available) {
                message = dequeue_tuxedo();
            }
            
            /* Fallback to in-memory queue */
            if (!message) {
                message = dequeue_fallback();
            }
            
            get_timestamp(timestamp, sizeof(timestamp));
            
            if (message) {
                /* Escape quotes in message */
                char escaped[MAX_MSG_SIZE * 2];
                int i, j = 0;
                for (i = 0; message[i] && j < sizeof(escaped) - 2; i++) {
                    if (message[i] == '"') {
                        escaped[j++] = '\\';
                    }
                    escaped[j++] = message[i];
                }
                escaped[j] = '\0';
                
                snprintf(json_response, sizeof(json_response),
                        "{\"message\": \"%s\", \"queue\": \"%s\", \"timestamp\": \"%s\"}",
                        escaped, queue_name, timestamp);
                free(message);
            } else {
                snprintf(json_response, sizeof(json_response),
                        "{\"message\": null, \"queue\": \"%s\", \"timestamp\": \"%s\"}",
                        queue_name, timestamp);
            }
            
            response = MHD_create_response_from_buffer(strlen(json_response),
                       (void*)json_response, MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        else if (strcmp(url, "/health") == 0 || strcmp(url, "/") == 0) {
            get_timestamp(timestamp, sizeof(timestamp));
            snprintf(json_response, sizeof(json_response),
                    "{\"status\": \"healthy\", \"tuxedo_q_available\": %s, \"queue\": \"%s\", \"timestamp\": \"%s\"}",
                    tuxedo_available ? "true" : "false", queue_name, timestamp);
            
            response = MHD_create_response_from_buffer(strlen(json_response),
                       (void*)json_response, MHD_RESPMEM_MUST_COPY);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
    }
    
    /* 404 Not Found */
    page = "{\"status\": \"error\", \"message\": \"Not found\"}";
    response = MHD_create_response_from_buffer(strlen(page),
               (void*)page, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", "application/json");
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char *argv[]) {
    struct MHD_Daemon *daemon;
    
    /* Get queue name from environment */
    const char *env_queue = getenv("TUXEDO_QUEUE");
    if (env_queue) {
        snprintf(queue_name, sizeof(queue_name), "%s", env_queue);
    }
    
    printf("[TuxedoQ-Server] ==========================================\n");
    printf("[TuxedoQ-Server] Starting Tuxedo /Q REST API Server (C)\n");
    printf("[TuxedoQ-Server] ==========================================\n");
    
    /* Initialize Tuxedo */
    init_tuxedo();
    
    printf("[TuxedoQ-Server] Listening on port %d\n", PORT);
    printf("[TuxedoQ-Server] Queue name: %s\n", queue_name);
    printf("[TuxedoQ-Server] Tuxedo /Q available: %s\n", 
           tuxedo_available ? "true" : "false (using fallback)");
    printf("[TuxedoQ-Server] Endpoints:\n");
    printf("[TuxedoQ-Server]   POST /enqueue   - Enqueue message\n");
    printf("[TuxedoQ-Server]   POST /MSGSVC    - Legacy endpoint (enqueue)\n");
    printf("[TuxedoQ-Server]   GET  /dequeue   - Dequeue message\n");
    printf("[TuxedoQ-Server]   GET  /health    - Health check\n");
    printf("[TuxedoQ-Server] Server ready on port %d\n", PORT);
    
    /* Start HTTP server */
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &handle_request, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "[TuxedoQ-Server] Failed to start HTTP server\n");
        return 1;
    }
    
    /* Wait for termination */
    printf("[TuxedoQ-Server] Press Ctrl+C to stop\n");
    pause();
    
    /* Cleanup */
    MHD_stop_daemon(daemon);
    if (tuxedo_available) {
        tpterm();
    }
    
    return 0;
}
