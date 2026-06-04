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
 * #ident	"@(#) gp/sec/include/e_sec_atz.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_ATZ_H
#define E_SEC_ATZ_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_ATZ_MAJOR_VERSION	1
#define ED_SEC_ATZ_MINOR_VERSION	0

/* Interfaces defined in this module */
#define ED_ATZ_INTF_ID "engine/security/authorization"

typedef struct sec_atz_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_atz_authorize_preop) _((
		struct sec_atz_1_Vtbl *,
        	_TCADEF,
		ET_SEC_HANDLE *,
        	TM32I,
        	const char *,
        	const unsigned char *,
        	TM32U,
        	void **,
        	TM32U *,
        	TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_atz_authorize_postop) _((
		struct sec_atz_1_Vtbl *,
        	_TCADEF,
		ET_SEC_HANDLE *,
        	TM32I,
        	const char *,
        	const unsigned char *,
        	TM32U,
        	void **,
        	TM32U *,
        	TM32U,
        	TM32U));
} sec_atz_1, *sec_atz_1_ptr;

#endif /* E_SEC_ATZ_H */
