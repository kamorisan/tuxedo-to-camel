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
#ifndef tbase_v0_0_included
#define tbase_v0_0_included 1
/* #ident	"@(#) trpc/rpc/tbase.h	$Revision: 1.3 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tbase[] = "@(#) trpc/rpc/tbase.h	$Revision: 1.3 $";
#endif

#include <rpc/tidlbase.h>

typedef idl_usmall_int	unsigned8;
typedef idl_ushort_int	unsigned16;
typedef idl_ulong_int	unsigned32;
typedef idl_small_int	signed8;
typedef idl_short_int	signed16;
typedef idl_long_int	signed32;

typedef unsigned32	boolean32;

typedef idl_ulong_int	error_status_t;
#define error_status_ok (0)


/*
** WIN32 case: Redefine uuid_t to match DCE
*/
#ifdef WIN32
#ifdef uuid_t
#undef uuid_t
#endif
#endif

typedef struct uuid_t {
	unsigned32	time_low;
	unsigned16	time_mid;
	unsigned16	time_hi_and_version;
	unsigned8	clock_seq_hi_and_reserved;
	unsigned8	clock_seq_low;
	idl_byte	node[6];
} uuid_t, _TM_FAR *uuid_p_t;

/* not included: protocol tower, ndr format flag type definitions,
 * ndr_format_t, ndr_context_handle, auth protocol ids.
 */

typedef idl_byte	ISO_LATIN_1;

typedef struct ISO_MULTI_LINGUAL {
	idl_byte	row;
	idl_byte	column;
} ISO_MULTI_LINGUAL;

typedef struct ISO_UCS {
	idl_byte	group;
	idl_byte	plane;
	idl_byte	row;
	idl_byte	column;
} ISO_UCS;

#if !defined(WIN32) 
typedef idl_ushort_int _TM_FAR *handle_t;
#endif

#endif
