/*
 * Minimal atmi.h stub for Tuxedo ATMI
 * This is a simplified version containing only the definitions needed for MSGSVC
 */

#ifndef _ATMI_H
#define _ATMI_H

/* TPSVCINFO structure */
typedef struct {
    char *data;         /* Pointer to data portion of buffer */
    long len;           /* Length of data */
    long flags;         /* Describes the buffer */
    int cd;             /* Connection descriptor */
    long appkey;        /* Application authentication client key */
    char name[32];      /* Service name invoked */
} TPSVCINFO;

/* Return codes */
#define TPSUCCESS   0
#define TPFAIL      0x0001

/* Function prototypes */
extern int tpsvrinit(int argc, char **argv);
extern void tpsvrdone(void);
extern void tpreturn(int rval, long rcode, char *data, long len, long flags);
extern int tpinit(void *tpinfo);
extern int tpterm(void);

#endif /* _ATMI_H */
