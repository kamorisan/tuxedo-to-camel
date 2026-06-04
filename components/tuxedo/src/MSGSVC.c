/*
 * MSGSVC.c - Tuxedo Message Service
 *
 * This service receives messages via Tuxedo ATMI and forwards them to
 * Red Hat AMQ (ActiveMQ Artemis) queue.
 *
 * Flow: WebUI -> WSL -> MSGSVC -> AMQ -> Camel -> Kafka
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <atmi.h>      /* TUXEDO Header File */
#include <userlog.h>   /* TUXEDO Header File */
#include "amqclient.h" /* AMQ Client functions */

/* Global AMQ client handle */
static amq_client_t *amq_client = NULL;

/*
 * tpsvrinit - Server initialization function
 * Called when the server is booted, before processing requests
 */
#if defined(__STDC__) || defined(__cplusplus)
int tpsvrinit(int argc, char *argv[])
#else
int tpsvrinit(argc, argv)
int argc;
char **argv;
#endif
{
    char *amq_host;
    char *amq_port_str;
    int amq_port;
    char *amq_queue;

    /* Get AMQ configuration from environment variables */
    amq_host = getenv("AMQ_HOST");
    if (amq_host == NULL) {
        amq_host = "amq-broker.demo-tuxedo.svc.cluster.local";
    }

    amq_port_str = getenv("AMQ_PORT");
    if (amq_port_str == NULL) {
        amq_port = 5672;
    } else {
        amq_port = atoi(amq_port_str);
    }

    amq_queue = getenv("AMQ_QUEUE");
    if (amq_queue == NULL) {
        amq_queue = "TUXEDO.OUT";
    }

    userlog("[Tuxedo] MSGSVC initializing...");
    userlog("[Tuxedo] AMQ Host: %s", amq_host);
    userlog("[Tuxedo] AMQ Port: %d", amq_port);
    userlog("[Tuxedo] AMQ Queue: %s", amq_queue);

    /* Initialize AMQ client */
    amq_client = amq_client_init(amq_host, amq_port, amq_queue);
    if (amq_client == NULL) {
        userlog("[Tuxedo] ERROR: Failed to initialize AMQ client");
        return -1;
    }

    /* Connect to AMQ */
    if (amq_client_connect(amq_client) != 0) {
        userlog("[Tuxedo] ERROR: Failed to connect to AMQ");
        amq_client_destroy(amq_client);
        amq_client = NULL;
        return -1;
    }

    userlog("[Tuxedo] MSGSVC initialized successfully");
    return 0;
}

/*
 * tpsvrdone - Server shutdown function
 * Called at server shutdown time
 */
#if defined(__STDC__) || defined(__cplusplus)
void tpsvrdone(void)
#else
void tpsvrdone()
#endif
{
    userlog("[Tuxedo] MSGSVC shutting down...");

    if (amq_client != NULL) {
        amq_client_disconnect(amq_client);
        amq_client_destroy(amq_client);
        amq_client = NULL;
    }

    userlog("[Tuxedo] MSGSVC shutdown complete");
}

/*
 * MSGSVC - Main service function
 * Receives message from WSL and forwards to AMQ
 */
#ifdef __cplusplus
extern "C"
#endif
void
#if defined(__STDC__) || defined(__cplusplus)
MSGSVC(TPSVCINFO *rqst)
#else
MSGSVC(rqst)
TPSVCINFO *rqst;
#endif
{
    char *message;
    int msg_len;
    int ret;
    time_t now;
    char timestamp[32];

    /* Get current timestamp */
    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    /* Extract message from request buffer */
    message = rqst->data;
    msg_len = rqst->len;

    /* Ensure null-terminated string */
    if (message[msg_len-1] != '\0') {
        msg_len--;
    }

    userlog("[Tuxedo] Received message: %s", message);

    /* Check if AMQ client is initialized */
    if (amq_client == NULL) {
        userlog("[Tuxedo] ERROR: AMQ client not initialized");
        tpreturn(TPFAIL, 0, rqst->data, 0, 0);
        return;
    }

    /* Send message to AMQ queue */
    ret = amq_client_send(amq_client, message, msg_len);
    if (ret != 0) {
        userlog("[Tuxedo] ERROR: Failed to send message to AMQ");
        tpreturn(TPFAIL, 0, rqst->data, 0, 0);
        return;
    }

    userlog("[Tuxedo] Message queued to AMQ: %s", message);

    /* Return success to caller */
    tpreturn(TPSUCCESS, 0, rqst->data, 0, 0);
}
