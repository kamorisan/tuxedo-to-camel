/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1984 AT&T; 1991 USL
	All rights reserved
*/
#ifndef USIGNAL_H
#define USIGNAL_H

/* #ident	"@(#) gp/libgp/Usignal.h	$Revision: 1.6 $" */
#ifndef TMENV_H
#include <tmenv.h>
#endif

#ifndef NOWHAT
static	char	h_Usignal[] = "@(#) gp/libgp/Usignal.h	$Revision: 1.6 $";
#endif

/*
 *	DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.
 *
 *	Warning: This header file should not be changed in any way.
 *	Doing so will destroy the compatibility with TUXEDO programs
 *	and libraries.
 */

/*
 *	macros for signal deferral/reinstatement
 */


#ifndef USIGTYP
#if !defined(__cplusplus) && defined(uts)
#define USIGTYP int
#else
#define USIGTYP void
#endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif

extern void Usiginit _((void));
extern USIGTYP (*Usignal _((int, USIGTYP (*)(int)))) _((int));
#if defined(SA_SIGINFO) && !defined(WIN32)
extern void (*Usignal_siginfo(int, void (*)(int, siginfo_t *, void *))) (int, siginfo_t *, void *);
#endif

extern void _e_sig_dosigs _((void));
extern int UGDEFERLEVEL _((void));

#if defined(__cplusplus)
}
#endif


#if defined(_TM_NETWARE) 

extern int *_e_sig_get_sigdefer_addr _((void));
extern int *_e_sig_get_sigspending_addr _((void));
#define _e_sig_defer		(*_e_sig_get_sigdefer_addr())
#define _e_sig_pending		(*_e_sig_get_sigspending_addr())

#define GET_SIGDEFER()		(GPP->_GPP_e_sig_defer)
#define SET_SIGDEFER(v)		GPP->_GPP_e_sig_defer = (v)
#define GET_SIGSPENDING()	(GPP->_GPT_e_sig_pending)
#define SET_SIGSPENDING(v)	GPP->_GPP_e_sig_pending = (v)

#elif defined (_GPTHREADS)

#define _e_sig_defer		(GPP->_GPP_e_sig_defer)
#define _e_sig_pending		(GPP->_GPP_e_sig_pending)
#define GET_SIGDEFER()		(GPP->_GPP_e_sig_defer)
#define SET_SIGDEFER(v)		GPP->_GPP_e_sig_defer = (v)
#define GET_SIGSPENDING()	(GPP->_GPP_e_sig_pending)
#define SET_SIGSPENDING(v)	GPP->_GPP_e_sig_pending = (v)



#else /* NOT _TM_NETWARE */

/* _TMIGP extern int _e_sig_defer; */
/* _TMIGP extern int _e_sig_pending; */

#if defined(__cplusplus)
extern "C" {
#endif
extern int _TMDLLENTRY *_e_sigdefer_addr _((void));
extern int  _TMDLLENTRY *_e_sigpending_addr _((void));
#if defined(__cplusplus)
}
#endif

#define _e_sig_defer            (*_e_sigdefer_addr())
#define _e_sig_pending          (*_e_sigpending_addr())
#define GET_SIGDEFER()		(GPP->_GPP_e_sig_defer = _e_sig_defer)
#define SET_SIGDEFER(v)		GPP->_GPP_e_sig_defer = _e_sig_defer = (v)
#define GET_SIGSPENDING()	(GPP->_GPP_e_sig_pending = _e_sig_pending)
#define SET_SIGSPENDING(v)	GPP->_GPP_e_sig_pending = _e_sig_pending = (v)

#endif /* _TM_NETWARE ,_GPTHREADS*/

	/* macro to defer signals ... note that deferrals stack */
#define UDEFERSIGS() {_e_sig_defer++;}

/* macro to reinstate signals ... note that reinstatement really unstacks */
#define URESUMESIGS() {if(_e_sig_defer>0)_e_sig_defer--; if((_e_sig_defer<=0)&&_e_sig_pending) _e_sig_dosigs();}

	/* macro to pop deferral stack completely ...
		ensures that sigs will be processed */
#define UENSURESIGS() {_e_sig_defer = 0; if(_e_sig_pending) _e_sig_dosigs();}


	/* the following macro is useful for setjmp/longjmp situations */

		/* macro to set defer level */
#define USDEFERLEVEL(newlev) {_e_sig_defer=(newlev<0)?0:newlev; if ((_e_sig_defer<=0)&&_e_sig_pending) _e_sig_dosigs();}

#endif
