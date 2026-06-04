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
 * #ident	"@(#) gp/sec/include/e_sec_map.h	$Revision: 1.2 $"
 */
 
#ifndef E_SEC_MAP_H
#define E_SEC_MAP_H

#include <tmenv.h>
#include <e_engine.h>
#include <e_sec.h>

#define ED_SEC_MAP_MAJOR_VERSION	1
#define ED_SEC_MAP_MINOR_VERSION	0

#define EE_SEC_MAP_SUCCESS		EE_SEC_SUCCESS
#define EE_SEC_MAP_FAILURE		EE_SEC_FAILURE
#define EE_SEC_MAP_INVAL		EE_SEC_INVAL
#define EE_SEC_MAP_SYSTEM		EE_SEC_SYSTEM
#define EE_SEC_MAP_STATUS		EE_SEC_STATUS
#define EE_SEC_MAP_NOSPACE		EE_SEC_NOSPACE
#define EE_SEC_MAP_NOT_SUPPORTED	EE_SEC_NOT_SUPPORTED
#define EE_SEC_MAP_UNKNOWN		EE_SEC_UNKNOWN
#define EE_SEC_MAP_FIXED		EE_SEC_FIXED
#define EE_SEC_MAP_RANGE		EE_SEC_RANGE
#define EE_SEC_MAP_NOT_PRIV		EE_SEC_NOT_PRIV
#define EE_SEC_MAP_NOT_FOUND		EE_SEC_NOT_FOUND
#define EE_SEC_MAP_TRUSTED		EE_SEC_TRUSTED:w

/* Interfaces defined in this module */
#define ED_MAP_INTF_ID "engine/security/map_proof"

typedef struct et_sec_mapdesc_t ET_SEC_MAPDESC;

struct et_sec_mapdesc_t {
	unsigned short	map_major_version;	/* Major version of struct */
	unsigned short	map_minor_version;	/* Minor version of struct */
	char		*map_principal_name;	/* Principal name to map */
	TM32U		map_principal_name_asize;
						/* Allocated size */
						/* Principal name destructor */
	unsigned char	*map_proof_material;	/* Proof material to map */
	TM32U		map_proof_material_size;	/* Size in proof */
	TM32U		map_proof_material_asize;	/* Allocated size */
	char		*map_location;		/* location of principal */
	TM32U		map_location_asize;	/* Allocated size */
};

typedef struct sec_map_1_Vtbl {
	TM32I (_TMDLLENTRY *_ec_sec_map_proof) _((struct sec_map_1_Vtbl *,
		_TCADEF, ET_SEC_MAPDESC *, ET_SEC_MAPDESC *,
		TM32U, TM32U));
} sec_map_1, *sec_map_1_ptr;

#endif /* E_SEC_MAP_H */
