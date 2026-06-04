/*
 *	Copyright (c) 1999 BEA Systems, Inc.
 *	All rights reserved
 *
 *	THIS IS UNPUBLISHED PROPRIETARY
 *	SOURCE CODE OF BEA Systems, Inc.
 *	The copyright notice above does not
 *	evidence any actual or intended
 *	publication of such source code.
 *
 * #ident	"@(#) gp/sec/include/e_sec_aud.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_AUD_H
#define E_SEC_AUD_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_AUD_MAJOR_VERSION	1
#define ED_SEC_AUD_MINOR_VERSION	0

/* Interfaces defined in this module */
#define ED_AUD_INTF_ID "engine/security/auditing"

typedef struct sec_aud_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_aud_audit_preop) _((
		struct sec_aud_1_Vtbl *,
        	_TCADEF,
		ET_SEC_HANDLE *,
        	TM32I,
        	const char *,
        	const unsigned char *,
        	TM32U,
        	const void *,
        	TM32U,
        	TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_aud_audit_postop) _((
		struct sec_aud_1_Vtbl *,
        	_TCADEF,
		ET_SEC_HANDLE *,
        	TM32I,
        	const char *,
        	const unsigned char *,
        	TM32U,
        	const void *,
        	TM32U,
        	TM32U,
        	TM32U));
} sec_aud_1, *sec_aud_1_ptr;

#endif /* E_SEC_AUD_H */
