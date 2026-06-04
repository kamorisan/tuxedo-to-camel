/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1993 Unix System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF Unix System Laboratories, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TIDLBASE_H
#define TIDLBASE_H
/* #ident	"@(#) trpc/rpc/tidlbase.h	$Revision: 1.3 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tidlbase[] = "@(#) trpc/rpc/tidlbase.h	$Revision: 1.3 $";
#endif

/* define the following for a 4.2.1 add-on */
#ifdef R421ADDON
#ifdef _TMLONG64
typedef	int		TM32I;
typedef	unsigned int	TM32U;
#else
typedef	long		TM32I;
typedef	unsigned long	TM32U;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef applec
#ifndef true
#define true	1
#endif
#ifndef false
#define false	0
#endif
#endif

#define idl_boolean	unsigned char
#define idl_false	false
#define idl_true	true
#define idl_char	unsigned char
#define idl_byte	unsigned char
#if defined(mips) || defined(_IBMR2) || defined(_U370)
/* signed char is not portable - include on necessary platforms */
#define idl_small_int	signed char
#else
#define idl_small_int	char
#endif
#define idl_short_int	short
#define idl_long_int	TM32I	/* 32-bit signed long */
#define idl_usmall_int	unsigned char
#define idl_ushort_int	unsigned short
#define idl_ulong_int	TM32U	/* 32-bit unsigned long */


#ifdef _TML_ENDIAN
struct _tidl_hyper_int	{TM32U low; TM32I high;};
struct _tidl_uhyper_int	{TM32U low; TM32U high;};
#else
struct _tidl_hyper_int	{TM32I high; TM32U low;};
struct _tidl_uhyper_int	{TM32U high; TM32U low;};
#endif

#ifdef _TMLONG64
#define	idl_hyper_int	long
#define	idl_uhyper_int	unsigned long
#else
#define idl_hyper_int	struct _tidl_hyper_int
#define idl_uhyper_int	struct _tidl_uhyper_int
#endif

#define idl_short_float	float
#define idl_long_float	double

#if (defined(ultrix) || defined(uts)) && !defined(__STDC__)
typedef char *idl_void_p_t;
typedef char _TM_FAR *idl_fvoid_p_t;	/* DOS/Windows */
#else
typedef void *idl_void_p_t;
typedef void _TM_FAR *idl_fvoid_p_t;	/* DOS/Windows */
#endif


/* Standard stub functions */
#define dce_c_error_string_len 128
extern void _TMDLLENTRY dce_error_inq_text _((unsigned long status_to_convert, unsigned char _TM_FAR *error_text, int _TM_FAR *status));

extern idl_fvoid_p_t _TM_FAR _TM_CDECL rpc_ss_allocate _((idl_ulong_int size));
extern idl_fvoid_p_t _TM_FAR _TM_CDECL rpc_ss_client_allocate _((idl_ulong_int size));
#if defined(WIN32) && defined(_TMDCE)

extern idl_fvoid_p_t _TMDLLENTRY rpc_sm_allocate _((idl_ulong_int size, idl_ulong_int _TM_FAR *status));
extern void _TMDLLENTRY rpc_sm_client_free _((idl_fvoid_p_t node_to_free, idl_ulong_int _TM_FAR *stat));
extern void _TMDLLENTRY rpc_ss_client_free _((idl_fvoid_p_t node_to_free));
extern void _TMDLLENTRY rpc_sm_free _((idl_fvoid_p_t node_to_free, idl_ulong_int _TM_FAR *status));

#else

extern idl_fvoid_p_t _TM_FAR _TM_CDECL rpc_sm_allocate _((idl_ulong_int size, idl_ulong_int _TM_FAR *status));
extern void _TM_FAR _TM_CDECL rpc_sm_client_free _((idl_fvoid_p_t node_to_free, idl_ulong_int _TM_FAR *stat));
extern void _TM_FAR _TM_CDECL rpc_ss_client_free _((idl_fvoid_p_t node_to_free));
extern void _TM_FAR _TM_CDECL rpc_sm_free _((idl_fvoid_p_t node_to_free, idl_ulong_int _TM_FAR *status));
#endif
extern void _TM_FAR _TM_CDECL rpc_ss_free _((idl_fvoid_p_t node_to_free));
extern void _TMDLLENTRY rpc_sm_disable_allocate _((idl_ulong_int _TM_FAR *status));
extern void _TMDLLENTRY rpc_ss_disable_allocate _((void));
extern void _TMDLLENTRY rpc_sm_enable_allocate _((idl_ulong_int _TM_FAR *status));
extern void _TMDLLENTRY rpc_ss_enable_allocate _((void));
extern void _TMDLLENTRY rpc_sm_set_client_alloc_free _((
	idl_fvoid_p_t (_TM_FAR _TM_CDECL *p_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL *p_free)(idl_fvoid_p_t ptr), idl_ulong_int _TM_FAR *status));
extern void _TMDLLENTRY rpc_ss_set_client_alloc_free _((
	idl_fvoid_p_t (_TM_FAR _TM_CDECL *p_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL *p_free)(idl_fvoid_p_t ptr)));
extern void _TMDLLENTRY rpc_sm_swap_client_alloc_free _((
	idl_fvoid_p_t (_TM_FAR _TM_CDECL *p_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL *p_free)(idl_fvoid_p_t ptr),
	idl_fvoid_p_t (_TM_FAR _TM_CDECL * _TM_FAR*p_p_old_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL * _TM_FAR *p_p_old_free)(idl_fvoid_p_t ptr),
	idl_ulong_int _TM_FAR *status));
extern void _TMDLLENTRY rpc_ss_swap_client_alloc_free _((
	idl_fvoid_p_t (_TM_FAR _TM_CDECL *p_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL *p_free)(idl_fvoid_p_t ptr),
	idl_fvoid_p_t (_TM_FAR _TM_CDECL * _TM_FAR *p_p_old_allocate)(idl_ulong_int size),
	void (_TM_FAR _TM_CDECL * _TM_FAR *p_p_old_free)(idl_fvoid_p_t ptr)));

#ifdef __cplusplus
}
#endif

#endif
