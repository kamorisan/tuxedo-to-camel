/*	
 *	Copyright (c) 1999 BEA Systems, Inc.
 *	All rights reserved
 *
 *	THIS IS UNPUBLISHED PROPRIETARY
 *	SOURCE CODE OF BEA Systems, Inc.
 *	The copyright notice above does not
 *	evidence any actual or intended
 *	publication of such source code.
 */
/* #ident	"@(#) gp/include/e_engine.h	$Revision: 1.2 $" */
 
#ifndef E_ENGINE_H
#define E_ENGINE_H

/* Include machine environment definitions */
#include <tmenv.h>

/* Engine Version:
	MAJORVERSION changes when there is an incompatible version
	MINORVERSION changes with each Engine release
*/
#define ED_ENGINE_MAJORVERSION	1
#define ED_ENGINE_MINORVERSION	0

typedef struct {
	unsigned short m_major_version;
	unsigned short m_minor_version;
	unsigned short m_patch_level;
} ET_ENGINE_VERSION;

/*
 * _TCARG:  Used for passing the argument to any internal tuxedo library
 *      function.
 */
#define _TCARG  _tm_cntxt

/*
 * _TCADEF:  Used in the declaration and the prototype of the function
 */
typedef char ** _TMctx;
#define _TCADEF _TMctx _tm_cntxt

/*
 * Maximum TUXEDO buffer type and subtype length
 */
#define ED_TYPELEN	8
#define ED_STYPELEN	16

/* Engine Components:
		Note: SP is reserved for Service Provider
*/
#define ED_ENG		0	/* Engine */
#define	ED_GP		1	/* General Purpose */
#define	ED_THR		2	/* Threads and Threads Synch. (mutex) */
#define	ED_SEC		3	/* Security */
#define	ED_PIF		4	/* Plug-in Framework */
#define	ED_DL		5	/* Dynamic Linking */
#define	ED_REG		6	/* Registry */
#define	ED_QS		7	/* Queuing and Event Services */
#define	ED_CRYP		8	/* Cryptography */
#define	ED_CTX		9	/* Context handling */
#define	ED_FS		10	/* File System */
#define	ED_ID		11	/* User, Group, process identifiers */
#define	ED_IPC		12	/* Inter-Process Communication */
#define	ED_LIC		13	/* Licensing */
#define	ED_MEM		14	/* Memory Management */
#define	ED_NET		15	/* Network Handling */
#define	ED_PROC		16	/* Process */
#define	ED_SIG		17	/* Signals */
#define	ED_STR		18	/* Strings */
#define	ED_VTOC		19	/* VTOC - Engine's File System */
#define	ED_XDR		20	/* XDR encoding */
#define ED_SORT		21	/* Generic sort (also known as usort) */

/*
 *  Definitions for the base error range for engine components
 */
#define ED_ENG_ERRORRANGE   (-1000)
#define	EE_ENG_BASE         (-500)                         /* specific Engine errors */

#define EE_ENG_CBASE        (ED_ENG  * ED_ENG_ERRORRANGE)  /* Common error base */
#define EE_GP_BASE          (ED_GP   * ED_ENG_ERRORRANGE)
#define EE_THR_BASE         (ED_THR  * ED_ENG_ERRORRANGE)
#define EE_SEC_BASE         (ED_SEC  * ED_ENG_ERRORRANGE)
#define EE_PIF_BASE         (ED_PIF  * ED_ENG_ERRORRANGE)
#define EE_DL_BASE          (ED_DL   * ED_ENG_ERRORRANGE)
#define EE_REG_BASE         (ED_REG  * ED_ENG_ERRORRANGE)
#define EE_QS_BASE          (ED_QS   * ED_ENG_ERRORRANGE)
#define EE_CRYP_BASE        (ED_CRYP * ED_ENG_ERRORRANGE)
#define EE_CTX_BASE         (ED_CTX  * ED_ENG_ERRORRANGE)
#define EE_FS_BASE          (ED_FS   * ED_ENG_ERRORRANGE)
#define EE_ID_BASE          (ED_ID   * ED_ENG_ERRORRANGE)
#define EE_IPC_BASE         (ED_IPC  * ED_ENG_ERRORRANGE)
#define EE_LIC_BASE         (ED_LIC  * ED_ENG_ERRORRANGE)
#define EE_MEM_BASE         (ED_MEM  * ED_ENG_ERRORRANGE)
#define EE_NET_BASE         (ED_NET  * ED_ENG_ERRORRANGE)
#define EE_PROC_BASE        (ED_PROC * ED_ENG_ERRORRANGE)
#define EE_SIG_BASE         (ED_SIG  * ED_ENG_ERRORRANGE)
#define EE_SORT_BASE        (ED_SORT * ED_ENG_ERRORRANGE)
#define EE_STR_BASE         (ED_STR  * ED_ENG_ERRORRANGE)
#define EE_VTOC_BASE        (ED_VTOC * ED_ENG_ERRORRANGE)
#define EE_XDR_BASE         (ED_XDR  * ED_ENG_ERRORRANGE)

/* Engine Common Errors */

#define EE_SUCCESS      0		          /* Engine Function ended successfully */

#define EE_WOULDBLOCK	(EE_ENG_CBASE-1)  /* EF_NOBLOCK specifiedg but module blocks */
#define EE_TIMEOUT      (EE_ENG_CBASE-2)  /* failure to complete within allocated time */
#define EE_NOMEMORY     (EE_ENG_CBASE-3)  /* memory allocation failure,  malloc, calloc */
#define EE_INVAL        (EE_ENG_CBASE-4)  /* invalid args */
#define EE_SYSTEM       (EE_ENG_CBASE-5)  /* error making system call */
#define EE_NOSPACE      (EE_ENG_CBASE-6)  /* no space left on device */
#define EE_NOPERM       (EE_ENG_CBASE-7)  /* no permission */
#define EE_OS           (EE_ENG_CBASE-8)  /* OS specific error */
#define EE_NOENT        (EE_ENG_CBASE-9)  /* no entry found */
#define EE_TOOBIG       (EE_ENG_CBASE-10) /* data is too big for buffer */
#define EE_PROTO        (EE_ENG_CBASE-11) /* protocol error */
#define EE_EXISTS       (EE_ENG_CBASE-12) /* matching entry exists */
#define EE_MATCH        EE_EXISTS         /* matching entry exists */
#define EE_NOSHARE      (EE_ENG_CBASE-13) /* sharing violation     */

/* Engine Errors -  the first 500 errors are the common error space*/

#define EE_ENGINE_VERSION_MISMATCH		EE_ENG_BASE
#define	EE_ENGINE_WRONG_MAJOR			(EE_ENG_BASE-1)
#define EE_ENGINE_WRONG_MINOR			(EE_ENG_BASE-2)
#define EE_ENGINE_WRONG_PATCHLEVEL		(EE_ENG_BASE-3)


/*
 * Generic Engine flags
 *
 * The low sixteen bits (0x0000ffff) are reserved for generic engine flags
 * that may be passed to plug-in implementations.  The high sixteen bits
 * (0xffff0000) are used by various providers, and may overlap with each
 * other.  So, for example, the security plug-ins may define bits in the
 * high range that overlap with bits defined by the registry plug-ins
 */
#define EF_NOFLAGS      ((TM32U) 0x00000000)      /* No flags set */
#define EF_SP_RESERVED  ((TM32U) 0x00000001)	/*
						 * Reserved flag for service
						 * providers
						 */
#define EF_NOBLOCK      ((TM32U) 0x00000002)      /* Do not block */


/* Engine specific flags */
#define EF_ENGINE_MAJOR	((TM32U) 0x00010000)
#define EF_ENGINE_MINOR	((TM32U) 0x00020000)
#define EF_ENGINE_PATCH ((TM32U) 0x00040000)


/* Generic engine functions */
extern TM32I _e_engine_isvalid _((_TCADEF, const ET_ENGINE_VERSION *, TM32U));
extern TM32I _e_engine_getversion _((_TCADEF, ET_ENGINE_VERSION *, TM32U));
extern TM32I _e_engine_isavailable _((_TCADEF, TM32I, TM32U));
extern TM32I _e_thread_isavailable _((_TCADEF, TM32U));

#endif /* E_ENGINE_H */ 
