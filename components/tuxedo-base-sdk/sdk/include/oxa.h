/*
	Copyright (c) 2005 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/

#ifndef OXAH
#define OXAH
/*	Copyright (c) 1989 AT&T; 1991 USL
	All rights reserved
*/
/* #ident	"@(#) gp/libgp/oxa.h	$Revision: 1.5 $" */
#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_oxa[] = "@(#) gp/libgp/oxa.h	$Revision: 1.5 $";
#endif

/*
 *	XA DEFINITIONS 
 *
 *	Warning: This TUXEDO header file should not be changed in any
 *	way.  Doing so will destroy the compatibility with TUXEDO programs
 *	and libraries.
 */
#ifndef TMREGISTER
/* XA switch flag settings */
#define TMREGISTER	0x00000001	/* RM must dynamically register */
#define TMUSEBRANCH	0x00000002	/* RM supports branch identifiers */
#define TMUSEASYNC	0x00000004	/* RM supports asynchronous operations */
#endif

#ifndef TM_OK
/* gtrid() return codes */
#define TM_OK	0		/* normal execution */
#define TMER_TMERR	-1	/* an error occurred in the TM */
#define TMER_INVAL	-2	/* invalid arguments were given */
#define TMER_PROTO	-3	/* routine invoked in an improper context */
#endif

/* XA return codes */
#define XA_HEURHAZ	8	/* the transaction may have been heuristically completed */
#define XA_HEURCOM	7	/* the transaction has been heuristically committed */
#define XA_HEURABT	6	/* the transaction has been heuristically aborted */
#define XA_HEURMIX	5	/* the transaction has been heuristically committed and aborted */
#define XA_RETRY	4	/* routine returned with no effect and may be re-issued */
#define XA_RDONLY	3	/* the transaction was read-only and has been committed */
#define XA_ABORTED	2	/* the transaction has been aborted */
#define XA_ABTONLY	1	/* the transaction has been marked abort-only */
#define XA_OK		0	/* normal execution */
#define XAER_ABTONLY	-1	/* the transaction has been marked abort-only */
#define XAER_ASYNC	-2	/* asynchronous operation already outstanding */
#define XAER_RMERR	-3	/* an error occurred on the transaction */
#define XAER_NOTA	-4	/* the global transaction identifier is not valid */
#define XAER_INVAL	-5	/* invalid arguments were given */
#define XAER_PROTO	-6	/* routine invoked in an improper context */

#ifndef TMFAIL
/* Flag definitions for all functions */
#define TMFAIL		0x20000000
#define TMSUCCESS	0x04000000
#endif

#ifndef TMASYNC
#define TMASYNC		0x80000000
#define TMBRANCHID	0x40000000
#define TMNOWAIT	0x10000000
#define TMRESUME	0x08000000
#define TMSUSPEND	0x02000000
#define TMNOFLAGS	0x00000000	/* to be used when no other flags used */
#endif

/* GTRID DATA STRUCTURES AND MACROS */

/* another declaration of this is in tuxedo/include/tmbase.h */
#ifndef _GTRID
#define _GTRID

/* transaction manager independent structure definition */
#define SIZEOFGTRID 32		/* size in bytes */
struct gtrid_t {
	TM32I traninfo[SIZEOFGTRID/sizeof(TM32I)];
};
typedef struct gtrid_t GTRID;
#endif

_TMIGP extern _TMCONST GTRID NULLGTRID;

#define GTRIDFMTSIZE	64	/* maximum size of formatted GTRID in bytes */


#if defined(__cplusplus)
extern "C" {
#endif

extern int	gtrid_cmp _((GTRID *, GTRID *, long));
extern void	gtrid_fmt _((GTRID *, char *, long));
extern long	gtrid_hash _((GTRID *, long, long, long));
extern int	gtrid_reg _((int, GTRID *, long *, long));
extern int	gtrid_rmid _((char *));
extern int	gtrid_unreg _((int, long));


/* XA SWITCH DATA STRUCTURE */

#define RMNAMESZ	32	/* length of resource manager name */

#ifndef OXASWITCH
#define OXASWITCH
struct oxa_switch_t {
    char name[RMNAMESZ];		/* name of resource manager */
    long flags;				/* resource manager specific options */
/* PR030213: severe problem on Win64. Add padding for correctly using struct oxa_switch_t in xasys.h*/
#if defined(_as400_) || defined(_TMPOINTER64)
    long version;			/* AS400 needs this in the alignment
					 * padding.
					 * NOTE also that we're not supporting 
					 * existing applications that uses
					 * oxa_switch - only used by /D
					 * The system with 64-bit Pointer
					 * and 32-bit long, need this padding
					 * as well.
					 */
#endif
    int (*open) _((char *, long));     	/* xa_open function pointer */
    int (*close) _((char *, long));    	/* xa_close function pointer */
    int (*start) _((GTRID *, long)); 	/* xa_start function pointer */
    int (*end) _((GTRID *, long));   	/* xa_end function pointer */
    int (*abort) _((GTRID *, long));   	/* xa_abort function pointer */
    int (*precom) _((GTRID *, long));  	/* xa_precom function pointer */
    int (*commit) _((GTRID *, long));  	/* xa_commit function pointer */
    int (*recover) _((GTRID *, long, long, long)); /* xa_recover function pointer */
    int (*forget) _((GTRID *, long));	/* xa_forget function pointer */
    int (*complete) _((int, int *, long)); /* xa_complete function pointer */
};
#endif

#if defined(__cplusplus)
}
#endif


#endif
