/*
 * tmqueue.h - Tuxedo Queue API header
 * Minimal implementation for TPQCTL structure
 * Based on Oracle Tuxedo documentation
 */

#ifndef _TMQUEUE_H
#define _TMQUEUE_H

#include <atmi.h>

/* TPQCTL flags */
#define TPNOFLAGS       0x00000000
#define TPQCORRID       0x00000001
#define TPQFAILUREQ     0x00000002
#define TPQBEFOREMSGID  0x00000004
#define TPQGETBYMSGID   0x00000008
#define TPQMSGID        0x00000010
#define TPQPRIORITY     0x00000020
#define TPQTOP          0x00000040
#define TPQWAIT         0x00000080
#define TPQREPLYQ       0x00000100
#define TPQTIME_ABS     0x00000200
#define TPQTIME_REL     0x00000400
#define TPQGETBYCORRID  0x00000800
#define TPQPEEK         0x00001000

/* TPQCTL diagnostic codes */
#define QMEINVAL        -1
#define QMEBADRMID      -2
#define QMENOTOPEN      -3
#define QMETRAN         -4
#define QMEBADMSGID     -5
#define QMESYSTEM       -6
#define QMEOS           -7
#define QMEABORTED      -8
#define QMENOTA         -9
#define QMEPROTO        -10
#define QMEBADQUEUE     -11
#define QMENOMSG        -12
#define QMEINUSE        -13
#define QMENOSPACE      -14

/* Queue control structure */
typedef struct tpqctl_t {
    long flags;              /* Control flags */
    long deq_time;          /* Dequeue time */
    long priority;          /* Message priority (1-100) */
    long diagnostic;        /* Diagnostic return code */
    char msgid[32];         /* Message identifier */
    char corrid[32];        /* Correlation identifier */
    char replyqueue[128];   /* Reply queue name */
    char failurequeue[128]; /* Failure queue name */
    long urcode;            /* User return code */
    long appkey;            /* Application authentication key */
    long delivery_qos;      /* Quality of service */
    long reply_qos;         /* Reply queue QoS */
    long exp_time;          /* Expiration time */
    char pad[64];           /* Reserved for future use */
} TPQCTL;

/* Queue API functions */
#ifdef __cplusplus
extern "C" {
#endif

int tpenqueue(char *qspace, char *qname, TPQCTL *ctl, char *data,
              long len, long flags);

int tpdequeue(char *qspace, char *qname, TPQCTL *ctl, char **data,
              long *len, long flags);

#ifdef __cplusplus
}
#endif

#endif /* _TMQUEUE_H */
