/*
 * amqclient.c - AMQ Client Library Implementation
 *
 * Uses qpid-proton C library for AMQP 1.0 protocol
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <proton/messenger.h>
#include <proton/message.h>
#include "amqclient.h"

#define MAX_URL_LEN 256

/* AMQ Client structure */
struct amq_client_s {
    char *host;
    int port;
    char *queue;
    char url[MAX_URL_LEN];
    pn_messenger_t *messenger;
    pn_message_t *message;
};

/*
 * amq_client_init - Initialize AMQ client
 */
amq_client_t *amq_client_init(const char *host, int port, const char *queue)
{
    amq_client_t *client;

    if (host == NULL || queue == NULL) {
        return NULL;
    }

    client = (amq_client_t *)malloc(sizeof(amq_client_t));
    if (client == NULL) {
        return NULL;
    }

    memset(client, 0, sizeof(amq_client_t));

    client->host = strdup(host);
    client->port = port;
    client->queue = strdup(queue);

    /* Build AMQP URL: amqp://host:port/queue */
    snprintf(client->url, MAX_URL_LEN, "amqp://%s:%d/%s", host, port, queue);

    /* Create messenger */
    client->messenger = pn_messenger(NULL);
    if (client->messenger == NULL) {
        free(client->host);
        free(client->queue);
        free(client);
        return NULL;
    }

    /* Create message */
    client->message = pn_message();
    if (client->message == NULL) {
        pn_messenger_free(client->messenger);
        free(client->host);
        free(client->queue);
        free(client);
        return NULL;
    }

    return client;
}

/*
 * amq_client_connect - Connect to AMQ broker
 */
int amq_client_connect(amq_client_t *client)
{
    int ret;

    if (client == NULL || client->messenger == NULL) {
        return -1;
    }

    /* Start messenger */
    ret = pn_messenger_start(client->messenger);
    if (ret != 0) {
        fprintf(stderr, "Failed to start messenger: %s\n",
                pn_error_text(pn_messenger_error(client->messenger)));
        return -1;
    }

    return 0;
}

/*
 * amq_client_send - Send message to AMQ queue
 */
int amq_client_send(amq_client_t *client, const char *message, int length)
{
    int ret;
    pn_data_t *body;

    if (client == NULL || client->messenger == NULL || client->message == NULL) {
        return -1;
    }

    if (message == NULL || length <= 0) {
        return -1;
    }

    /* Clear message */
    pn_message_clear(client->message);

    /* Set message address (destination queue) */
    pn_message_set_address(client->message, client->url);

    /* Set message body */
    body = pn_message_body(client->message);
    pn_data_clear(body);
    pn_data_put_string(body, pn_bytes(length, message));

    /* Send message */
    ret = pn_messenger_put(client->messenger, client->message);
    if (ret != 0) {
        fprintf(stderr, "Failed to put message: %s\n",
                pn_error_text(pn_messenger_error(client->messenger)));
        return -1;
    }

    /* Send pending messages */
    ret = pn_messenger_send(client->messenger, -1);
    if (ret < 0) {
        fprintf(stderr, "Failed to send message: %s\n",
                pn_error_text(pn_messenger_error(client->messenger)));
        return -1;
    }

    return 0;
}

/*
 * amq_client_disconnect - Disconnect from AMQ broker
 */
int amq_client_disconnect(amq_client_t *client)
{
    if (client == NULL || client->messenger == NULL) {
        return -1;
    }

    /* Stop messenger */
    pn_messenger_stop(client->messenger);

    return 0;
}

/*
 * amq_client_destroy - Destroy AMQ client
 */
void amq_client_destroy(amq_client_t *client)
{
    if (client == NULL) {
        return;
    }

    if (client->message != NULL) {
        pn_message_free(client->message);
    }

    if (client->messenger != NULL) {
        pn_messenger_free(client->messenger);
    }

    if (client->host != NULL) {
        free(client->host);
    }

    if (client->queue != NULL) {
        free(client->queue);
    }

    free(client);
}
