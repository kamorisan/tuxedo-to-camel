/*
 * amqclient.h - AMQ Client Library Header
 *
 * Simple wrapper around qpid-proton for sending messages to AMQ
 */

#ifndef AMQCLIENT_H
#define AMQCLIENT_H

/* AMQ Client structure (opaque) */
typedef struct amq_client_s amq_client_t;

/*
 * amq_client_init - Initialize AMQ client
 * @host: AMQ broker hostname
 * @port: AMQ broker port
 * @queue: Queue name to send messages to
 *
 * Returns: Pointer to amq_client_t on success, NULL on failure
 */
amq_client_t *amq_client_init(const char *host, int port, const char *queue);

/*
 * amq_client_connect - Connect to AMQ broker
 * @client: AMQ client handle
 *
 * Returns: 0 on success, -1 on failure
 */
int amq_client_connect(amq_client_t *client);

/*
 * amq_client_send - Send message to AMQ queue
 * @client: AMQ client handle
 * @message: Message data
 * @length: Message length
 *
 * Returns: 0 on success, -1 on failure
 */
int amq_client_send(amq_client_t *client, const char *message, int length);

/*
 * amq_client_disconnect - Disconnect from AMQ broker
 * @client: AMQ client handle
 *
 * Returns: 0 on success, -1 on failure
 */
int amq_client_disconnect(amq_client_t *client);

/*
 * amq_client_destroy - Destroy AMQ client and free resources
 * @client: AMQ client handle
 */
void amq_client_destroy(amq_client_t *client);

#endif /* AMQCLIENT_H */
