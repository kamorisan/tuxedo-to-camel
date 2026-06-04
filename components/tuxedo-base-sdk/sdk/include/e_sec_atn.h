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
 * #ident	"@(#) gp/sec/include/e_sec_atn.h	$Revision: 1.4 $"
 */
 
#ifndef E_SEC_ATN_H
#define E_SEC_ATN_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>
#include <gssapi.h>

#define ED_SEC_ATN_MAJOR_VERSION	1
#define ED_SEC_ATN_MINOR_VERSION	0

/* supporting structures not defined by gssapi */
typedef gss_ctx_id_t ET_SEC_CONTEXT;
typedef gss_cred_id_t ET_SEC_CRED;
typedef gss_OID_set ET_SEC_OID_SET;
typedef gss_OID ET_SEC_OID;
typedef gss_channel_bindings_t ET_SEC_CHAN_BINDING;

extern gss_OID _e_ed_atn_acquire_cred_nametype _((void));
extern gss_OID _e_ed_atn_init_context_nametype _((void));

#define ED_ATN_ACQUIRE_CRED_NAMETYPE _e_ed_atn_acquire_cred_nametype()
#define ED_ATN_INIT_CONTEXT_NAMETYPE _e_ed_atn_init_context_nametype()

/* Interfaces defined in this module */
#define ED_ATN_INTF_ID "engine/security/authentication"

typedef struct sec_atn_1_Vtbl {
	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_indicate_mechs) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_OID_set *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_acquire_cred) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_name_t,
		OM_uint32,
		gss_OID_set,
		int,
		gss_cred_id_t *,
		gss_OID_set *,
		OM_uint32 *,
		void *,
		OM_uint32,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_release_cred) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_cred_id_t *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_init_sec_context) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_cred_id_t,
		gss_ctx_id_t *,
		gss_name_t,
		gss_OID,
		int,
		OM_uint32,
		gss_channel_bindings_t,
		gss_buffer_t,
		gss_OID *,
		gss_buffer_t,
		int *,
		OM_uint32 *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_accept_sec_context) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_ctx_id_t *,
		gss_cred_id_t,
		gss_buffer_t,
		gss_channel_bindings_t,
		gss_name_t *,
		gss_OID *,
		gss_buffer_t,
		int *,
		OM_uint32 *,
		gss_cred_id_t  *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_delete_sec_context) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_ctx_id_t *,
		gss_buffer_t,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_import_name) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_buffer_t,
		gss_OID,
		gss_name_t *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_display_name) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_name_t,
		gss_buffer_t,
		gss_OID *,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_release_name) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_name_t,
		TM32U));

	OM_uint32 (_TMDLLENTRY *_ec_sec_atn_gss_release_buffer) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		OM_uint32 *,
		gss_buffer_t,
		TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_atn_authorization_token) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		ET_SEC_CONTEXT,
		unsigned char *,
		TM32U *,
		TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_atn_auditing_token) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		ET_SEC_CONTEXT,
		unsigned char *,
		TM32U *,
		TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_atn_token_property) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		const unsigned char *,
		TM32U,
		const char *,
		char *,
		TM32U *,
		TM32U));

	TM32I (_TMDLLENTRY *_ec_sec_atn_impersonate_user) _((
		struct sec_atn_1_Vtbl *,
		_TCADEF,
		const char *,
		const unsigned char *,
		TM32U,
		const unsigned char *,
		TM32U,
		unsigned char *,
		TM32U *,
		unsigned char *,
		TM32U *,
		TM32U));
} sec_atn_1, *sec_atn_1_ptr;

typedef struct sec_authn_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_impersonate_user) _((
	    struct sec_authn_1_Vtbl *, _TCADEF, const char *,
	    const unsigned char *, TM32U, const unsigned char *, TM32U,
	    unsigned char * , TM32U *, unsigned char *, TM32U *, TM32U));
} sec_authn_1, *sec_authn_1_ptr;


#endif /* E_SEC_ATN_H */
