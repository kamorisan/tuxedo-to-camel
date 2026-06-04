/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*      Copyright (c) 1990 Unix System Laboratories, Inc.
        All rights reserved

        THIS IS UNPUBLISHED PROPRIETARY
        SOURCE CODE OF Unix System Laboratories, Inc.
        The copyright notice above does not
        evidence any actual or intended
        publication of such source code.
*/
#ifndef TMAEXIT_H
#define TMAEXIT_H
/* #ident	"@(#) tuxedo/include/tmaexit.h	$Revision: 1.6 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tmaexit[] = "@(#) tuxedo/include/tmaexit.h	$Revision: 1.6 $";
#endif

/*
 *	DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.
 *
 *	Warning: This header file should not be changed in any
 *	way, doing so will destroy the compatibility with TUXEDO programs
 *	and libraries.
 */

#include <atmi.h>

/*
 * TPBREAK has this value because it needs to be in the series of TPFAIL
 * and TPSUCCESS which is defined in atmi.h.
 */
#define TPBREAK		0x01000000	/* tpreturn - break out of loop */

/* Options used in main() abstraction */
#define TMRECORD	0x001		/* log service start & stop times */
#define TMCOBOL		0x002		/* cobol server */
#define TMONETIME	0x004		/* handle one service request */
#define TMNOBLOCKING	0x008		/* don't block on receive */
#define TMNOSIGRSTRT	0x010		/* don't restart on signal */
#define TMDRAIN		0x020		/* receive until queue empty */
#define TMRECORDINTF	0x040		/* log interface name instead of service name for -r */

#if defined(__cplusplus)
extern "C" {
#endif


/* server main abstraction */
/* extern void	_tmsetup _((int *, char **)); */ /* prototype moved to atmi.h */
extern void	_tm_ps_setup _((int *, char **));
extern int	_tmmain _((int, char **));
extern int	_tmrunserver _((int options));
extern void	_tmcleanup _((void));

#if defined(__cplusplus)
}
#endif


#endif
