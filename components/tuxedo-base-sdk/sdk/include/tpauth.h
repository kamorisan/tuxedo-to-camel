/*	Copyright (c) 1998 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/

/*	Copyright 1996 BEA Systems, Inc.	*/
/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	BEA Systems, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	Copyright (c) 1991 Unix System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF Unix System Laboratories, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TPAUTH_H
#define TPAUTH_H
/* #ident	"@(#) tuxedo/include/tpauth.h	$Revision: 1.5 $" */

#ifndef TMENV
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tpauth[] = "@(#) tuxedo/include/tpauth.h	$Revision: 1.5 $";
#endif

/* NO LONGER USED (backwards compatibility code uses hardcoded 8 */
#define TMMAXPASSLEN	8

/*
 * The following pound defines are used to BLIND users
 * who may currently be using the encryption interface illegally.
 * Doing this will break current code using the _tp interfaces.
 * Further, the new symbols are not obviously for encryption purposes,
 * and hence cannot easily be deduced by looking at the NM or strings
 * output of the library
 */
#define _tpcryptinit	_tm_j13
#define _tpcrypt	_tm_j019
#define _tprandkey	_tm_j0230
#define _tppwtokey	_tm_j017

/*
 *  This data structure's size must be a multiple of 8 bytes to meet
 *  needs of DES encryption.  Also, it is assumed to be XDR opaque.
 */

#define TMSEC_KEYLEN	8
struct tmsec_key_t {
	char sec_key[TMSEC_KEYLEN];  /* Arbitrary data; NOT assumed to be */
				     /* null terminated or ASCII          */
};
typedef struct tmsec_key_t TMSEC_KEY;
	

/*
 *  The TMSEC_SCHED and TMSEC_TICKET data structures are defined
 *  with extra space to allow for alternate implementations
 *  of Kerberos without header file changes.
 */

/*
 *  This data structure's size must be a multiple of 8 bytes to meet
 *  needs of DES encryption.  Also, it is assumed to be XDR opaque.
 */

#define TMSEC_SCHEDLEN 	2048
/*
 *  See libtux/des/tmcrypt.c for usage of DES schedule.
 *  DES_ITERATIONS and hard-coded value of 48 are not
 *  intented to be used outside tmcrypt.c
 */
#define DES_ITERATIONS	16

struct tmsec_sched_t {
	union {
		char data[TMSEC_SCHEDLEN];	/* General decl. to pad len */
		char t_des[DES_ITERATIONS][48];	/* DES-encryption specific */
		char t_simple[TMSEC_KEYLEN];	/* Simple-encryption specific */
	} sched;
};
typedef struct tmsec_sched_t TMSEC_SCHED;


/*
 *  This data structure's size must be a multiple of 8 bytes to meet
 *  needs of DES encryption.  Also, it is assumed to be XDR opaque.
 */

#define TMSEC_TICKETLEN	1536
struct tmsec_ticket_t {
	union {
		char data[TMSEC_TICKETLEN];  /* general decl. to pad len */
		struct {
			char 	  usrname[MAXTIDENT+2];
			TMSEC_KEY key;
		} tux;  /* TUXEDO-specific stub for Kerberos */
	} ticket;
};
typedef struct tmsec_ticket_t TMSEC_TICKET;

extern int	_tpadminkey _((TMSEC_KEY **));
extern int	_tpreadkey _((char *, TMSEC_KEY *));
extern int	_tpwritekey _((char *, TMSEC_KEY *));
extern int	_tpgetticket _((TPINIT *, TMSEC_KEY **, TMSEC_TICKET **, long));
extern int	_tpatn _((TPINIT *, long));
extern int	_tpsessionkey _((TMSEC_TICKET *, TMSEC_KEY **));
extern int	_tpvalidate _((TMSEC_TICKET *, TPINIT *));
extern int	_tpcryptinit _((TMSEC_KEY *, TMSEC_SCHED **));
extern int	_tpcrypt _((char *, char *, TMSEC_SCHED *, size_t, long));
extern int	_tprandkey _((TMSEC_KEY **));
extern int	_tppwtokey _((char *, TMSEC_KEY **));
extern int	_encr_protected _((char *in, char *out, int length, unsigned char *oldkey, int has_oldkey));
extern int	_decr_protected _((char *in, char *out, int length));

#endif
