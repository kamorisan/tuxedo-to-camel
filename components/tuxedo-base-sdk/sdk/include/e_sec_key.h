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
 * #ident	"@(#) gp/sec/include/e_sec_key.h	$Revision: 1.3 $"
 */
 
#ifndef E_SEC_KEY_H
#define E_SEC_KEY_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_KEY_MAJOR_VERSION	1
#define ED_SEC_KEY_MINOR_VERSION	0

#define EE_SEC_KEY_SUCCESS		EE_SEC_SUCCESS
#define EE_SEC_KEY_FAILURE		EE_SEC_FAILURE
#define EE_SEC_KEY_INVAL		EE_SEC_INVAL
#define EE_SEC_KEY_SYSTEM		EE_SEC_SYSTEM
#define EE_SEC_KEY_STATUS		EE_SEC_STATUS
#define EE_SEC_KEY_NOSPACE		EE_SEC_NOSPACE
#define EE_SEC_KEY_NOT_SUPPORTED	EE_SEC_NOT_SUPPORTED
#define EE_SEC_KEY_UNKNOWN		EE_SEC_UNKNOWN
#define EE_SEC_KEY_FIXED		EE_SEC_FIXED
#define EE_SEC_KEY_RANGE		EE_SEC_RANGE
#define EE_SEC_KEY_NOT_PRIV		EE_SEC_NOT_PRIV
#define EE_SEC_KEY_NOT_FOUND		EE_SEC_NOT_FOUND
#define EE_SEC_KEY_TRUSTED		EE_SEC_TRUSTED

typedef void *ET_SEC_KEY;
#define ED_SEC_NO_KEY (ET_SEC_KEY) NULL

/* Interfaces defined in this module */
#define ED_KEY_INTF_ID "engine/security/key_management"

typedef struct sec_key_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_key_open_private_key) _((struct sec_key_1_Vtbl *,
		_TCADEF, ET_SEC_KEY *, ET_SEC_CERTDESC *, TM32U,
		const char *, const unsigned char *, TM32U,
		TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_open_public_key) _((struct sec_key_1_Vtbl *,
		_TCADEF, ET_SEC_KEY *, ET_SEC_CERTDESC *, TM32U,
		TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_copy) _((struct sec_key_1_Vtbl *,
		_TCADEF, ET_SEC_KEY *, ET_SEC_KEY, ET_SEC_CERTDESC *,
		TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_close) _((struct sec_key_1_Vtbl *,
		_TCADEF, ET_SEC_KEY *, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_getinfo) _((struct sec_key_1_Vtbl *,
		_TCADEF, void *, TM32U *,
		ET_SEC_KEY, const char *, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_setinfo) _((struct sec_key_1_Vtbl *,
		_TCADEF, ET_SEC_KEY, const char *, const void *, TM32U, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_digest) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, const unsigned char *, TM32U, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_encrypt_digest) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_decrypt_digest) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_encrypt_session_key) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_decrypt_session_key) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U));
	TM32I (_TMDLLENTRY *_ec_sec_key_sign_msg) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U, TM32I));
	TM32I (_TMDLLENTRY *_ec_sec_key_verify_sign) _((struct sec_key_1_Vtbl *,
		_TCADEF, unsigned char *, TM32U *, const unsigned char *,
		TM32U, ET_SEC_KEY, TM32U, TM32I));
} sec_key_1, *sec_key_1_ptr;

/* Get and set info values */
#define ED_SEC_KEY_DIGEST_ALG	"DIGEST_ALG" /* Digest object ID */
#define ED_SEC_KEY_PUB_BITS	"PKENCRYPT_BITS" /* Bits in encryption algorithm */
#define ED_SEC_KEY_PUB_ALG	"PKENCRYPT_ALG" /* Encryption object ID */
#define ED_SEC_KEY_PUB_PARAM	"PKENCRYPT_PARAM" /* Encryption parameters */
#define ED_SEC_KEY_PRINCIPAL	"PRINCIPAL" /* Associated principal */
#define ED_SEC_KEY_SIG_BITS	"SIGNATURE_BITS" /* Bits in signature algorithm */
#define ED_SEC_KEY_SIG_ALG	"SIGNATURE_ALG" /* Signature object ID */
#define ED_SEC_KEY_SIG_PARAM	"SIGNATURE_PARAM" /* Signature parameters */
#define ED_SEC_KEY_ENG_TYPE	"ENG_TYPE" /* Key type */
#define ED_SEC_KEY_X509_CERT	"CERTIFICATE" /* BER X509 certificate */
#define ED_SEC_KEY_SYM_BITS	"ENCRYPT_BITS" /* Symetric key bits */
#define ED_SEC_KEY_SYM_ALG	"ENCRYPT_ALG" /* Object ID of symetric key */
#define ED_SEC_KEY_SYM_PARAM	"ENCRYPT_PARAM" /* symetric key parameters */
#define ED_SEC_KEY_X509_CERT_CHAIN	"CERTIFICATE_CHAIN" /* BER X509 certificate chain */
#define ED_SEC_KEY_RAW	"RAW_PRIVATE" /* raw private key */

#endif /* E_SEC_KEY_H */
