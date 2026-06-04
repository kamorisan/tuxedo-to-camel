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
 * #ident	"@(#) gp/sec/include/e_sec_cert.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_CERT_H
#define E_SEC_CERT_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_CERT_MAJOR_VERSION	1
#define ED_SEC_CERT_MINOR_VERSION	0

#define EE_SEC_CERT_SUCCESS		EE_SEC_SUCCESS
#define EE_SEC_CERT_FAILURE		EE_SEC_FAILURE
#define EE_SEC_CERT_INVAL		EE_SEC_INVAL
#define EE_SEC_CERT_SYSTEM		EE_SEC_SYSTEM
#define EE_SEC_CERT_STATUS		EE_SEC_STATUS
#define EE_SEC_CERT_NOSPACE		EE_SEC_NOSPACE
#define EE_SEC_CERT_NOT_SUPPORTED	EE_SEC_NOT_SUPPORTED
#define EE_SEC_CERT_UNKNOWN		EE_SEC_UNKNOWN
#define EE_SEC_CERT_FIXED		EE_SEC_FIXED
#define EE_SEC_CERT_RANGE		EE_SEC_RANGE
#define EE_SEC_CERT_NOT_PRIV		EE_SEC_NOT_PRIV
#define EE_SEC_CERT_NOT_FOUND		EE_SEC_NOT_FOUND
#define EE_SEC_CERT_TRUSTED		EE_SEC_TRUSTED

/* Interfaces defined in this module */
#define ED_CERT_INTF_ID "engine/security/certificate_lookup"

typedef struct sec_cert_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_cert_by_name) _((struct sec_cert_1_Vtbl *,
		_TCADEF, ET_SEC_CERTDESC *, TM32U, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_cert_by_dsn) _((struct sec_cert_1_Vtbl *,
		_TCADEF, ET_SEC_CERTDESC *, TM32U,
		const unsigned char *, TM32U,
		const unsigned char *, TM32U, TM32U));
} sec_cert_1, *sec_cert_1_ptr;

#endif /* E_SEC_CERT_H */
