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
 * #ident	"@(#) gp/sec/include/e_sec_val.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_VAL_H
#define E_SEC_VAL_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_VAL_MAJOR_VERSION	1
#define ED_SEC_VAL_MINOR_VERSION	0

#define EE_SEC_VAL_SUCCESS		EE_SEC_SUCCESS
#define EE_SEC_VAL_FAILURE		EE_SEC_FAILURE
#define EE_SEC_VAL_INVAL		EE_SEC_INVAL
#define EE_SEC_VAL_SYSTEM		EE_SEC_SYSTEM
#define EE_SEC_VAL_STATUS		EE_SEC_STATUS
#define EE_SEC_VAL_NOSPACE		EE_SEC_NOSPACE
#define EE_SEC_VAL_NOT_SUPPORTED	EE_SEC_NOT_SUPPORTED
#define EE_SEC_VAL_UNKNOWN		EE_SEC_UNKNOWN
#define EE_SEC_VAL_FIXED		EE_SEC_FIXED
#define EE_SEC_VAL_RANGE		EE_SEC_RANGE
#define EE_SEC_VAL_NOT_PRIV		EE_SEC_NOT_PRIV
#define EE_SEC_VAL_NOT_FOUND		EE_SEC_NOT_FOUND
#define EE_SEC_VAL_TRUSTED		EE_SEC_TRUSTED

/* Interfaces defined in this module */
#define ED_VAL_INTF_ID "engine/security/certificate_validation"

typedef struct sec_val_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_val_cert) _((struct sec_val_1_Vtbl *,
		_TCADEF, ET_SEC_CERTDESC *, TM32U, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_val_trust) _((struct sec_val_1_Vtbl *,
		_TCADEF, ET_SEC_CERTDESC *, TM32U, TM32U));
} sec_val_1, *sec_val_1_ptr;

#endif /* E_SEC_VAL_H */
