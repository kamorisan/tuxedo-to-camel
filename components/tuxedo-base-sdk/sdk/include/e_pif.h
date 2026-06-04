/*	Copyright (c) 1999 BEA Systems, Inc.
 *	All rights reserved
 *
 *	THIS IS UNPUBLISHED PROPRIETARY
 *	SOURCE CODE OF BEA Systems, Inc.
 *	The copyright notice above does not
 *	evidence any actual or intended
 *	publication of such source code.
 *
 * #ident	"@(#) gp/pif/e_pif.h	$Revision: 1.3 $"
*/
 
#ifndef e_pif_h
#define e_pif_h

#include <e_engine.h>

#define EE_PIF_NORESOURCE	(EE_PIF_BASE-0)
#define EE_PIF_NOT_REGISTERED	(EE_PIF_BASE-1)
#define EE_PIF_VERSION_MISMATCH	(EE_PIF_BASE-2)
#define EE_PIF_BAD_CONFIG	(EE_PIF_BASE-3)
#define EE_PIF_NOTFOUND		(EE_PIF_BASE-4)
#define EE_PIF_RESERVED1	(EE_PIF_BASE-5)
#define EE_PIF_RESERVED2	(EE_PIF_BASE-6)
#define EE_PIF_RESERVED3	(EE_PIF_BASE-7)
#define EE_PIF_RESERVED4	(EE_PIF_BASE-8)
#define EE_PIF_RESERVED5	(EE_PIF_BASE-9)

typedef struct _e_pif_iversion {
    unsigned char i_major_version;
    unsigned char i_minor_version;
} _e_pif_iversion, *p_e_pif_iversion;

typedef struct _e_pif_interception_data {
    _e_pif_iversion m_pif_version;
    void * const *fanout_interception_seq;
    unsigned long fanout_interception_seq_len;
    void *next_ip;
} _e_pif_interception_data, *p_e_pif_interception_data;

typedef struct _e_pif_data {
    _e_pif_iversion m_pif_version;
    const void *data;
    unsigned long datalen;
    const char * const *regdata;
} _e_pif_data, *p_e_pif_data;

struct _e_pif_instance_handles;

typedef TM32I (_TMDLLENTRY *p_ep_pif_destroy)
    (_TCADEF, const struct _e_pif_instance_handles *, TM32U);
typedef TM32I (_TMDLLENTRY *p_ep_pif_copy)
    (_TCADEF, void *, const struct _e_pif_interception_data *,
    struct _e_pif_instance_handles *, TM32U);

typedef struct _e_pif_plugin_info {
    _e_pif_iversion m_pif_version;
    _e_pif_iversion m_i_version;
    char *m_impl_id;
    char *m_interface_id;
    int m_vtbl_size;
    char *m_vendor;
    char *m_productname;
    char *m_vendorversion;
#define EF_PIF_SINGLETON	0x080000
#define EF_PIF_STACK		0x100000
#define EF_PIF_FANOUT		0x200000
#define EF_PIF_SERIALIZED	0x400000
    TM32U m_flags;
    p_ep_pif_destroy destroy;
    p_ep_pif_copy copy;
} _e_pif_plugin_info, *p_e_pif_plugin_info;

typedef struct _e_pif_plugin_info_public {
    _e_pif_iversion m_pif_version;
    _e_pif_iversion m_i_version;
    char *m_impl_id;
    char *m_interface_id;
    char *m_vendor;
    char *m_productname;
    char *m_vendorversion;
    TM32U m_flags;
} _e_pif_plugin_info_public, *p_e_pif_plugin_info_public;

/* The following is to make calling _e_pif_getinfo easier for users */
typedef const _e_pif_plugin_info_public * const *_e_pif_plugin_info_public_vector;

typedef struct _e_pif_instance_handles {
    _e_pif_iversion m_pif_version;
    void *pVtbl;
    void *pPrivData;
    _e_pif_plugin_info *pPluginInfo;
} _e_pif_instance_handles, *p_e_pif_instance_handles;

typedef int (_TMDLLENTRY *p_ep_dl_instantiate)(
			_TCADEF, const char *, const char *,
			const _e_pif_iversion *,
			const _e_pif_data *,
			const _e_pif_interception_data *,
			_e_pif_instance_handles *, TM32U);

/* flags to dupref */
#define EF_PIF_DEEP_COPY	0x010000

/* flags to realize */
#define EF_PIF_EXACT_MATCH	0x020000

/* flags to instantiate */
#define EF_PIF_QUERY		0x040000

/* reserved flags */
#define EF_PIF_RESERVED1	0x080000
#define EF_PIF_RESERVED2	0x100000

#if defined(WIN32)
# if defined(IncludedByPluginDotH)
#  define PluginExportDLL __declspec(dllexport)
# else
#  define PluginExportDLL __declspec(dllimport)
# endif
#else
# define PluginExportDLL extern
#endif

PluginExportDLL TM32I _TMDLLENTRY _e_pif_realize(_TCADEF,
	const char *, const char *, const struct _e_pif_iversion *,
	const void *, long, void **, TM32U);

PluginExportDLL TM32I _TMDLLENTRY _e_pif_dupref(_TCADEF, void *, void **, TM32U);
PluginExportDLL TM32I _TMDLLENTRY _e_pif_release(_TCADEF, void *);
PluginExportDLL TM32I _TMDLLENTRY _e_pif_iscompatible(_TCADEF, void *, const _e_pif_iversion *);
/*NOTE: update fs, move const in following*/
PluginExportDLL TM32I _TMDLLENTRY _e_pif_interception_seq(
	_TCADEF, void *, const _e_pif_interception_data **, TM32U);
PluginExportDLL TM32I _TMDLLENTRY _e_pif_this(_TCADEF, void *, const void *, void **);
PluginExportDLL TM32I _TMDLLENTRY _e_pif_getinfo(
	_TCADEF, void *, unsigned long *,
	const _e_pif_plugin_info_public * const **, TM32U);

#undef PluginExportDLL

#endif /* e_pif_h */
