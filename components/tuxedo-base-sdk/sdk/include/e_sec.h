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
 * #ident	"@(#) gp/sec/include/e_sec.h	$Revision: 1.5 $"
 */
 
#ifndef E_SEC_H
#define E_SEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tmenv.h>
#include <gssapi.h>

#define ED_SEC_MAJOR_VERSION	1
#define ED_SEC_MINOR_VERSION	0

#define EE_SEC_SUCCESS		0
#define EE_SEC_FAILURE		(EE_SEC_BASE-1)
#define EE_SEC_INVAL		(EE_SEC_BASE-2)
#define EE_SEC_SYSTEM		(EE_SEC_BASE-3)
#define EE_SEC_STATUS		(EE_SEC_BASE-4)
#define EE_SEC_NOSPACE		(EE_SEC_BASE-5)
#define EE_SEC_NOT_SUPPORTED	(EE_SEC_BASE-6)
#define EE_SEC_UNKNOWN		(EE_SEC_BASE-7)
#define EE_SEC_FIXED		(EE_SEC_BASE-8)
#define EE_SEC_RANGE		(EE_SEC_BASE-9)
#define EE_SEC_NOT_PRIV		(EE_SEC_BASE-10)
#define EE_SEC_NOT_FOUND	(EE_SEC_BASE-11)
#define EE_SEC_TRUSTED		(EE_SEC_BASE-12)
#define EE_SEC_PERM		(EE_SEC_BASE-13)
#define EE_SEC_EXPIRED		(EE_SEC_BASE-14)
#define EE_SEC_LIMIT		(EE_SEC_BASE-15)
#define EE_SEC_NOTFOUND		(EE_SEC_BASE-16)
#define EE_SEC_DENIED		(EE_SEC_BASE-17)
#define EE_SEC_ABSTAIN		(EE_SEC_BASE-18)
#define EE_SEC_COMPLETE		(EE_SEC_BASE-19)
#define EE_SEC_CONTINUE		(EE_SEC_BASE-20)
#define EE_SEC_PERMIT		(EE_SEC_SUCCESS)

typedef void *ET_SEC_PRIVATE;
#define ED_SEC_NO_PRIVATE (ET_SEC_PRIVATE) NULL

typedef enum {
	ED_SEC_LEAF,		/* Certificate needs verification */
	ED_SEC_SELF_SIGNED,	/* Certificate is self signed */
	ED_SEC_IMPLICITLY_TRUSTED	/* Cert is implicitly trusted */
} ET_SEC_TRUST;

typedef struct et_sec_certdesc_t ET_SEC_CERTDESC;

typedef void (* ET_SEC_DESTRUCTOR) _((_TCADEF, ET_SEC_CERTDESC *,
	TM32U, TM32U));
#define ED_SEC_NO_DESTRUCTOR (ET_SEC_DESTRUCTOR) NULL

typedef TM32I (* ET_SEC_COPY) _((_TCADEF, ET_SEC_CERTDESC *, ET_SEC_CERTDESC *,
	TM32U, TM32U));
#define ED_SEC_DEFAULT_COPY (ET_SEC_COPY) NULL

typedef void *ET_SEC_CERTOBJ;
#define ED_SEC_NO_CERTOBJ (ET_SEC_CERTOBJ) NULL

struct et_sec_certdesc_t {
	unsigned short	cds_major_version;	/* ED_SEC_MAJOR_VERSION */
	unsigned short	cds_minor_version;	/* ED_SEC_MINOR_VERSION */
	char		*cds_principal_name;	/* Easy associated name */
	ET_SEC_DESTRUCTOR cds_destroy_principal_name;
						/* Principal name destructor */
	ET_SEC_COPY	cds_copy_principal_name;/* Principal name copyor */
	unsigned char	*cds_cert;		/* BER of certificate */
	TM32U		cds_cert_size;		/* Size of cds_cert */
	ET_SEC_DESTRUCTOR cds_destroy_cert;	/* Certificate destructor */
	ET_SEC_COPY	cds_copy_cert;		/* Certificate copyor */
	TM32U		cds_status;		/* Status of certificate */
	TM32U		cds_usage;		/* Valid uses for cert */
	ET_SEC_TRUST	cds_trust;		/* Trust state of certificate */
	ET_SEC_CERTDESC	*cds_issuer_cd;		/* Issuer certificate desc */
	unsigned char	*cds_objid; /* BER encoding object ID of cds_private */
	TM32U		cds_objid_size;		/* Data in cds_objid */
	ET_SEC_DESTRUCTOR cds_destroy_objid;	/* object ID destructor */
	ET_SEC_COPY	cds_copy_objid;		/* object ID copyor */
	ET_SEC_PRIVATE	cds_private;		/* Private data */
	ET_SEC_DESTRUCTOR cds_destroy_private;	/* private data destructor */
	ET_SEC_COPY	cds_copy_private;	/* private data copyor */
	ET_SEC_CERTOBJ	cds_co;			/* Reserved for use by BEA */
};

/* cds_status flags */
#define EF_SEC_TAMPERED	((TM32U) 0x00000001)	/* Message tampered */
#define EF_SEC_TRUST	((TM32U) 0x00000002)	/* Trust invalid */
#define EF_SEC_INVALID	((TM32U) 0x00000004)	/* Certificate invalid */
#define EF_SEC_CRITICAL	((TM32U) 0x00000008)	/* Unrecognized extension */
#define EF_SEC_USAGE	((TM32U) 0x00000010)	/* Not usable for op */
#define EF_SEC_EXPIRED	((TM32U) 0x00000020)	/* Certificate is expired */

/* cds_usage flags */
#define EF_SEC_SIGNATURE	((TM32U) 0x00000001)
#define EF_SEC_VERIFY		((TM32U) 0x00000002)
#define EF_SEC_DECRYPT		((TM32U) 0x00000004)
#define EF_SEC_ENCRYPT		((TM32U) 0x00000008)
#define EF_SEC_ISSUER		((TM32U) 0x00000010)

/* ATN - rec_flags/ret_flags */
#define EF_SEC_REPLAY		((TM32U) 0x00000001)
#define EF_SEC_SEQUENCE		((TM32U) 0x00000002)
#define EF_SEC_INTEGRITY	((TM32U) 0x00000004)

/* destructor function field values */
#define ED_SEC_PRINCIPAL_NAME	((TM32U) 1)
#define ED_SEC_CERT		((TM32U) 2)
#define ED_SEC_OBJID		((TM32U) 3)
#define ED_SEC_PRIVATE		((TM32U) 4)

/* _ec_sec_map_proof modes */
#define ED_SEC_PK_INIT		((TM32U) 1)
#define ED_SEC_AAA_INIT		((TM32U) 2)
#define ED_SEC_AAA_SESSION      ((TM32U) 3)

/* ATN - lifetime_req/lifetime_rec */
#define ED_SEC_INDEFINITE	((TM32U) GSS_C_INDEFINITE)

/* ATN - cred_usage */
#define ED_SEC_INITIATE_ONLY	((TM32U) GSS_C_INITIATE)
#define ED_SEC_ACCEPT_ONLY	((TM32U) GSS_C_ACCEPT)
#define ED_SEC_BOTH		((TM32U) GSS_C_BOTH)

/* ATZ/AUD - operation_name */
/* taken from tmextern.h: ACLSERVICE is 0x1 through ACLSUBSCRIBE 0x10 */
/* NO LONGER RESTRICTED TO BIT VALUES */
#define ED_SEC_SERVICE_OP	((TM32U) 0x1)	/* ACLSERVICE */
#define ED_SEC_ENQUEUE_OP	((TM32U) 0x2)	/* ACLENQUEUE */
#define ED_SEC_DEQUEUE_OP	((TM32U) 0x4)	/* ACLDEQUEUE */
#define ED_SEC_POST_OP		((TM32U) 0x8)	/* ACLPOST */
#define ED_SEC_CONNECT_OP	((TM32U) 0x9)
/* OLD: ACLSUBSCRIBE 0x10 - not currently under authorization checks */
#define ED_SEC_IMPERSONATE_OP	((TM32U) 0x11)
#define ED_LOGON_OP		((TM32U) 0x12)
#define ED_LOGOFF_OP		((TM32U) 0x13)
#define ED_SEC_DECRYPT_OP	((TM32U) 0x14)
#define ED_SEC_SERVICE_SIGNATURE_POLICY_OP	((TM32U) 0x15)
#define ED_SEC_SERVICE_ENCRYPTION_POLICY_OP	((TM32U) 0x16)
#define ED_SEC_QUEUE_SIGNATURE_POLICY_OP	((TM32U) 0x17)
#define ED_SEC_QUEUE_ENCRYPTION_POLICY_OP	((TM32U) 0x18)
#define ED_SEC_EVENT_SIGNATURE_POLICY_OP	((TM32U) 0x19)
#define ED_SEC_EVENT_ENCRYPTION_POLICY_OP	((TM32U) 0x1a)
#define ED_SEC_SIGNATURE_OP	((TM32U) 0x1b)
#define ED_SEC_METHOD_OP	((TM32U) 0x1c)

/* ATZ/AUD - operation_status */
#define ED_SEC_OP_SUCCESS	((TM32U) 1)
#define ED_SEC_OP_FAILURE	((TM32U) 2)
#define ED_SEC_SEC_FAILURE	((TM32U) 3)
#define ED_SEC_OP_NOTATTEMPTED	((TM32U) 4)

/* engine interface exposed to plugin writers */
extern TM32I _TMDLLENTRY _e_sec_token_property _((_TCADEF, const unsigned char *, TM32U,
	const char *, char *, TM32U *, TM32U)); 

#define ET_SEC_HANDLE void *

#ifdef __cplusplus
};
#endif

#endif /* E_SEC_H */
