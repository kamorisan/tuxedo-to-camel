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
 * #ident	"@(#) gp/sec/include/e_sec_pki.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_PKI_H
#define E_SEC_PKI_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_PKI_MAJOR_VERSION	1
#define ED_SEC_PKI_MINOR_VERSION	0

#define EE_SEC_PKI_SUCCESS		EE_SEC_SUCCESS
#define EE_SEC_PKI_FAILURE		EE_SEC_FAILURE
#define EE_SEC_PKI_INVAL		EE_SEC_INVAL
#define EE_SEC_PKI_SYSTEM		EE_SEC_SYSTEM
#define EE_SEC_PKI_STATUS		EE_SEC_STATUS
#define EE_SEC_PKI_NOSPACE		EE_SEC_NOSPACE
#define EE_SEC_PKI_NOT_SUPPORTED	EE_SEC_NOT_SUPPORTED
#define EE_SEC_PKI_UNKNOWN		EE_SEC_UNKNOWN
#define EE_SEC_PKI_FIXED		EE_SEC_FIXED
#define EE_SEC_PKI_RANGE		EE_SEC_RANGE
#define EE_SEC_PKI_NOT_PRIV		EE_SEC_NOT_PRIV
#define EE_SEC_PKI_NOT_FOUND		EE_SEC_NOT_FOUND
#define EE_SEC_PKI_TRUSTED		EE_SEC_TRUSTED

/* Interfaces defined in this module */
#define ED_PKI_INTF_ID "engine/security/pk_initialization"

typedef struct sec_pki_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_pki_init) _((struct sec_pki_1_Vtbl *,
		_TCADEF, const char *, const char *,
		const unsigned char *, TM32U, TM32U));
} sec_pki_1, *sec_pki_1_ptr;

#endif /* E_SEC_PKI_H */
