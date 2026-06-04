/*	Copyright 1996 BEA Systems, Inc.	*/
/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	BEA Systems, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	Copyright (c) 1993 Unix System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF Unix System Laboratories, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TIDLXDR_H
#define TIDLXDR_H
/* #ident       "@(#) trpc/rpc/tidlxdr.h        $Revision: 1.5 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static  char    h_tidlxdr[] = "@(#) trpc/rpc/tidlxdr.h  $Revision: 1.5 $";
#endif


#ifndef xdr_TM32I
#ifdef _TMLONG64
#define xdr_TM32I	xdr_int
#define xdr_TM32U	xdr_u_int
#else
#define xdr_TM32I	xdr_long
#define xdr_TM32U	xdr_u_long
#endif
#endif

#define xdr_small(xdrs, val)	xdr_opaque(xdrs, (char _TM_FAR *)val, 1)
#define xdr_usmall(xdrs, val)	xdr_opaque(xdrs, (char _TM_FAR *)val, 1)
#define xdr_byte(xdrs, val)	xdr_opaque(xdrs, (char _TM_FAR *)val, 1)

#ifndef TIDLBASE_H
#define idl_fvoid_p_t idl_void_p_t
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(M_I86MM) || defined(__MEDIUM__)
/* fix for medium model - compiler extension puts data segment into node */
#define _TM_MMFIX	_TIDLnode &= 0xffff
#else
#define _TM_MMFIX
#endif

#if defined(_IBMR2) && !defined(_ALL_SOURCE)
typedef	unsigned int	u_int;
#endif
/* some machines incorrectly redefine malloc in rpc/types.h */
#if !defined(_TM_MAC) && !defined(malloc)
#define malloc ________malloc
#define _mallocredef
#endif
#ifdef _as400_
#include "types.h"
#else
#include <rpc/types.h>
#endif
#if defined(_mallocredef)
#undef malloc
#undef _mallocredef
#endif
#ifdef _as400_
#include "xdr.h"
#else
#include <rpc/xdr.h>
#endif


extern int _TM_FAR _TM_CDECL _xdr_hyper _((XDR _TM_FAR *xdrs, idl_hyper_int _TM_FAR *objp));
extern int _TM_FAR _TM_CDECL _xdr_u_hyper _((XDR _TM_FAR *xdrs, idl_uhyper_int _TM_FAR *objp));

#ifdef FML_H
extern long _TMDLLENTRY _xdr_Fbfr _((XDR _TM_FAR *, char _TM_FAR *, FBFR _TM_FAR *, char _TM_FAR *));
struct Fbfr {long l1;}; /* dummy */
#endif

#ifdef FML32_H
extern long _TMDLLENTRY _xdr_Fbfr32 _((char **, XDR _TM_FAR *, long, FBFR32 _TM_FAR *, long));
struct Fbfr32 {long l1;}; /* dummy */
#endif

typedef struct {
	uuid_t _TM_FAR *uuid;			/* uuid for interface */
	idl_long_int idlversion;		/* compiler version */
	idl_long_int interfmajor;		/* interface major */
	idl_long_int interfminor;		/* interface minor */
	idl_long_int opcode;			/* operation number */
	XDR _TM_FAR *marsh;			/* xdrs structure */
	idl_ulong_int comm_status;		/* error_status_t */
	idl_ulong_int fault_status;		/* error_status_t */
	idl_ulong_int _TM_FAR *app_comm_status;	/* applic error_status_t */
	idl_ulong_int _TM_FAR *app_fault_status;/* applic error_status_t */
	char _TM_FAR *buffer;			/* (un)marshalling buffer */
	idl_long_int bufferlen;			/* used length in buffer */
	idl_long_int buffermax;			/* allocated length */
	long xmitlen;				/* length to transmit */
	idl_long_int processtype;		/* client/server */
#define _TMCLIENT	1
#define _TMSERVER	2
	idl_long_int actiontype;		/* marshall/unmarshall */
#define _TMREGISTER	0	/* not used in this struct */
#define _TMMARSHALL	1
#define _TMUNMARSHALL	2
	idl_long_int trantype;			/* transaction type or 0 */
#define _TMTRANSMANDATORY	1
#define _TMTRANSOPTIONAL	2
	idl_boolean medium;	/* 1 => medium memory model on dos/os2 */
#if defined(M_I86MM) || defined(__MEDIUM__)
#define _TMMEDIUM 1
#else
#define _TMMEDIUM 0
#endif
	} _TIDL_t;

/* (un)marshalling flags */
#define _TMFULLPTR	1
#define _TMREFNEW	2
#define _TMREFOLD	3
#define _TMALLOC	4

/* System/T Stub support Functions */
extern void _TMDLLENTRY _TIDL_free_node _((void));
extern int _TMDLLENTRY _TIDLaborted _((void));
extern void _TMDLLENTRY _TIDL_client_error _((_TIDL_t _TM_FAR *idl));
extern void _TMDLLENTRY _TIDL_tperror _((_TIDL_t _TM_FAR *idl));
extern void _TMDLLENTRY _TIDLinit _((_TIDL_t _TM_FAR *idl));
extern idl_long_int _TMDLLENTRY _TIDLrealloc _((_TIDL_t _TM_FAR *idl));

extern char _TM_FAR * _TMDLLENTRY _TIDLallocate _((idl_ulong_int size, int processtype));
extern void _TMDLLENTRY _TIDLfree _((char _TM_FAR *ptr, _TIDL_t _TM_FAR *idl));
extern idl_long_int _TMDLLENTRY _TIDLsetnode _((char _TM_FAR *,
	int, int _TM_FAR *));
extern int _TMDLLENTRY _TIDL_m _((char _TM_FAR *ptr, int ptrtype,
	_TIDL_t _TM_FAR *_TIDL));
int _TMDLLENTRY _TIDL_u _((char _TM_FAR *_TM_FAR *refptr, int type,
	_TIDL_t _TM_FAR *_TIDL));
char _TM_FAR * _TMDLLENTRY _TIDLgetnodeptr _((unsigned long,
	unsigned long, int _TM_FAR *, int _TM_FAR *, int));
unsigned32 _TMDLLENTRY _TIDLstrlen _((char _TM_FAR *ptr, unsigned32 size));

#ifdef _TMTEST
#define xdr_long checkxdr_long
#define xdr_u_long checkxdr_u_long
#define xdr_short checkxdr_short
#define xdr_u_short checkxdr_u_short
#define xdr_char checkxdr_char
#define xdr_u_char checkxdr_u_char
#define xdr_opaque checkxdr_opaque
#define xdr_double checkxdr_double
#define xdr_float checkxdr_float
#define xdr_enum checkxdr_enum
#define _xdr_hyper check_xdr_hyper
#define _xdr_u_hyper check_xdr_u_hyper

extern int checkxdr_long _((XDR *xdrs, long *lp));
extern int checkxdr_u_long _((XDR *xdrs, unsigned long *ulp));
extern int checkxdr_short _((XDR *xdrs, short *sp));
extern int checkxdr_u_short _((XDR *xdrs, unsigned short *usp));
extern int checkxdr_char _((XDR *xdrs, char *cp));
extern int checkxdr_u_char _((XDR *xdrs, unsigned char *cp));
extern int checkxdr_opaque _((XDR *xdrs, char *cp, unsigned int size));
extern int checkxdr_enum _((XDR *xdrs, enum_t *ep));
extern int checkxdr_float _((XDR *xdrs, float *fp));
extern int checkxdr_double _((XDR *xdrs, double *dp));
extern int check_xdr_hyper _((XDR *xdrs, idl_hyper_int *objp));
extern int check_xdr_u_hyper _((XDR *xdrs, idl_uhyper_int *objp));

/* end _TMTEST */
#endif

#ifdef __cplusplus
}
#endif

#endif
