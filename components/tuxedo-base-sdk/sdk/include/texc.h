/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1993 Unix System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF Unix System Laboratories, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TEXC_H
#define TEXC_H
/* #ident	"@(#) trpc/rpc/texc.h	$Revision: 1.3 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_texc[] = "@(#) trpc/rpc/texc.h	$Revision: 1.3 $";
#endif
#include <userlog.h>
#include <rpc/tbase.h>

/*
 * Exception handling for TUXEDO System/T RPC Interface.
 * Note that exception and status values are interchangable - they are both
 * integer values.
 * The syntax for handling exceptions is as follows:
 *
 * TRY
 *    try_block
 * [CATCH (exception_name)
 *    handler_block]...
 * [CATCH_ALL
 *    handler_block]
 * ENDTRY
 *
 * The syntax for using an eplilogue is as follows:
 *
 * TRY
 *    try_block
 * [FINALLY
 *    final_block]
 * ENDTRY
 *
 * IMPLEMENTATION:
 * The exception contexts are kept on the stack -  when a TRY
 * is done, a pointer to the old exception context is stored on the stack
 * and a new context is set up in a new stack variable.
 * When popping the stack, if the old exception context is NULL, then
 * an exception was raised outside the scope of an exception handler.
 * In this case, a message will be written to the userlog and abort()
 * is called to exit the program.
 * setjmp/longjmp are used to save the context and jump to the context
 * when an exception occurs.
 */

#include <setjmp.h>
#include <rpc/tsetjmp.h>

#if defined(M_I86MM) || defined(__MEDIUM__)
/* fix for medium model */
#ifdef __BORLANDC__
#define _TM_MMCAST	(struct __jmp_buf _near *)
#else
#define _TM_MMCAST	(int _near *)
#endif
#else
#define _TM_MMCAST
#endif

#ifdef WIN32
#undef _TM_MMCAST
#define _TM_MMCAST	
#endif

typedef struct {
	long type;
	long value;
 } EXCEPTION;
#define _TMEXCSTATUS	0
#define _TMEXCADDRESS	1

/* initialize application exception */
#define EXCEPTION_INIT(exc) \
	(exc).type = (_TMEXCADDRESS, (exc).value = (long)&(exc))

/* map status to exception */
#define exc_set_status(e,s) \
	((e)->type = _TMEXCSTATUS, (e)->value = (s))

/* map exception to status */
#define exc_get_status(e,s) \
		(((e)->type == _TMEXCSTATUS) ? (*(s) = (e)->value, 0) : -1)

/* compare exceptions */
#define exc_matches(e1,e2) \
		((e1)->type == (e2)->type && (e1)->value == (e2)->value)

#if defined(__cplusplus)
extern "C"
#endif
void _TMDLLENTRY exc_report _((EXCEPTION _TM_FAR *exception));

typedef struct {
	trpc_jmpbuf	jmpbuf;
	EXCEPTION	exception;
} exc_context_t;

#ifndef _TM_MAC
#if !defined(__STDC__) && !defined(_TMDOWN) && !defined(volatile)
#define volatile
#endif
#endif

#if (defined(_TM_WIN) || defined(_TM_OS2) || defined(WIN32) || defined(_TM_MAC)) && !defined(_TMDLL)

#if defined(__cplusplus)
extern "C" {
#endif

extern exc_context_t _TM_FAR * _TM_FAR * _TMDLLENTRY _tmget_exc_context(void);
#define __curr_exc_context	(*_tmget_exc_context())
extern EXCEPTION _TM_FAR * _TMDLLENTRY _tmexception(idl_long_int status);

#if defined(__cplusplus)
}
#endif

#else
_TMITRPC extern exc_context_t _TM_FAR *__curr_exc_context;
#endif

extern
#ifdef __cplusplus
 "C"
#endif
void _TMDLLENTRY _TIDL_initexc _((void));

#define TRY							\
    {								\
	volatile exc_context_t _TM_FAR *__prev_exc_context;	\
	volatile exc_context_t __exc_context;			\
	volatile int __exc_occured = 0;				\
	volatile int __exc_handled = 0;				\
	_TIDL_initexc();					\
	__prev_exc_context = __curr_exc_context;		\
	__curr_exc_context = (exc_context_t _TM_FAR *)&__exc_context;	\
	__exc_occured = trpc_setjmp(_TM_MMCAST __curr_exc_context->jmpbuf);\
	if (__exc_occured)					\
		__curr_exc_context =				\
			(exc_context_t _TM_FAR *)__prev_exc_context;	\
	if (__exc_occured == 0) {
		/* APPLICATION TRY CODE GOES HERE */

#define CATCH(e)						\
	}							\
	else if (exc_matches(&(e),&__exc_context.exception)) {		\
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&__exc_context.exception;	\
		__exc_handled = 1; 
		/* APPLICATION CATCH(e) CODE GOES HERE */

#define CATCH_ALL						\
	}							\
	else {							\
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&__exc_context.exception;	\
		__exc_handled = 1;
		/* APPLICATION CATCH_ALL CODE GOES HERE */

#define FINALLY							\
	}							\
	{							\
		if (!__exc_occured)				\
			__curr_exc_context = (exc_context_t _TM_FAR *)	\
				__prev_exc_context;		\
		/* APPLICATION FINALLY CODE GOES HERE */

#define ENDTRY							\
	}							\
	if (!__exc_occured)					\
		__curr_exc_context = (exc_context_t _TM_FAR *)		\
			__prev_exc_context;			\
	else if (!__exc_handled) {				\
		RAISE(__exc_context.exception);			\
	}							\
	if (__prev_exc_context == (exc_context_t _TM_FAR *)0)		\
		__curr_exc_context = (exc_context_t _TM_FAR *)0;	\
    }

#define RAISE(e)						\
    {								\
	if (__curr_exc_context == (exc_context_t _TM_FAR *)0) {	\
		userlog("exception 0X%lX raised with no handler",(long)e.value); \
		abort();					\
	}							\
	__curr_exc_context->exception = e;			\
	trpc_longjmp(_TM_MMCAST __curr_exc_context->jmpbuf, 1);	\
    }

#define RERAISE  RAISE(*THIS_CATCH)

#define pthread_s_badparam	0X177DB032
#define pthread_s_cancel	0X177DB030
#define pthread_s_defer_q_full	0X177DB03F
#define pthread_s_existence	0X177DB035
#define pthread_s_in_use	0X177DB036
#define pthread_s_nostackmem	0X177DB03A
#define pthread_s_nostack	0X177DB03B
#define pthread_s_signal_q_full	0X177DB040
#define pthread_s_stackovf	0X177DB039
#define pthread_s_unimp		0X177DB03D
#define pthread_s_use_error	0X177DB037
#define exc_s_decovf	0X177DB01B
#define exc_s_exquota	0X177DB006
#define exc_s_fltdiv	0X177DB019
#define exc_s_fltovf	0X177DB018
#define exc_s_fltund	0X177DB01A
#define exc_s_illaddr	0X177DB005
#define exc_s_insfmem	0X177DB007
#define exc_s_intdiv	0X177DB017
#define exc_s_intovf	0X177DB016
#define exc_s_nopriv	0X177DB008
#define exc_s_privinst	0X177DB00C
#define exc_s_resaddr	0X177DB00B
#define exc_s_resoper	0X177DB00D
#define exc_s_subrng	0X177DB01C
#define exc_s_uninitexc	0X177DB003
#define exc_s_SIGBUS	0X177DB005
#define exc_s_SIGEMT	0X177DB010
#define exc_s_SIGFPE	0X177DB011
#define exc_s_SIGILL	0X177DB00A
#define exc_s_SIGIOT	0X177DB00F
#define exc_s_SIGPIPE	0X177DB013
#define exc_s_SIGSEGV	0X177DB005
#define exc_s_SIGSYS	0X177DB012
#define exc_s_SIGTRAP	0X177DB00E
#define exc_s_SIGXCPU	0X177DB014
#define exc_s_SIGXFSZ	0X177DB015

#ifndef _TMEXCEPTINIT
#define _TMEXCEPTINIT(name,status) _TMITRPC extern EXCEPTION name
#endif

#if (defined(_TM_OS2) || defined(_TM_WIN) || defined(WIN32) || defined(_TM_MAC)) && !defined(_TMDLL) && !defined(_TMEXCPTINIT)
#define pthread_e_badparam	(*_tmexception(pthread_s_badparam))
#define pthread_e_cancel	(*_tmexception(pthread_s_cancel))
#define pthread_e_defer_q_full	(*_tmexception(pthread_s_defer_q_full))
#define pthread_e_existence	(*_tmexception(pthread_s_existence))
#define pthread_e_in_use	(*_tmexception(pthread_s_in_use))
#define pthread_e_nostackmem	(*_tmexception(pthread_s_nostackmem))
#define pthread_e_nostack	(*_tmexception(pthread_s_nostack))
#define pthread_e_signal_q_full	(*_tmexception(pthread_s_signal_q_full))
#define pthread_e_stackovf	(*_tmexception(pthread_s_stackovf))
#define pthread_e_unimp		(*_tmexception(pthread_s_unimp))
#define pthread_e_use_error	(*_tmexception(pthread_s_use_error))
#define exc_e_decovf		(*_tmexception(exc_s_decovf))
#define exc_e_exquota		(*_tmexception(exc_s_exquota))
#define exc_e_fltdiv		(*_tmexception(exc_s_fltdiv))
#define exc_e_fltovf		(*_tmexception(exc_s_fltovf))
#define exc_e_fltund		(*_tmexception(exc_s_fltund))
#define exc_e_illaddr		(*_tmexception(exc_s_illaddr))
#define exc_e_insfmem		(*_tmexception(exc_s_insfmem))
#define exc_e_intdiv		(*_tmexception(exc_s_intdiv))
#define exc_e_intovf		(*_tmexception(exc_s_intovf))
#define exc_e_nopriv		(*_tmexception(exc_s_nopriv))
#define exc_e_privinst		(*_tmexception(exc_s_privinst))
#define exc_e_resaddr		(*_tmexception(exc_s_resaddr))
#define exc_e_resoper		(*_tmexception(exc_s_resoper))
#define exc_e_subrng		(*_tmexception(exc_s_subrng))
#define exc_e_uninitexc		(*_tmexception(exc_s_uninitexc))
#define exc_e_SIGBUS		(*_tmexception(exc_s_SIGBUS))
#define exc_e_SIGEMT		(*_tmexception(exc_s_SIGEMT))
#define exc_e_SIGFPE		(*_tmexception(exc_s_SIGFPE))
#define exc_e_SIGILL		(*_tmexception(exc_s_SIGILL))
#define exc_e_SIGIOT		(*_tmexception(exc_s_SIGIOT))
#define exc_e_SIGPIPE		(*_tmexception(exc_s_SIGPIPE))
#define exc_e_SIGSEGV		(*_tmexception(exc_s_SIGSEGV))
#define exc_e_SIGSYS		(*_tmexception(exc_s_SIGSYS))
#define exc_e_SIGTRAP		(*_tmexception(exc_s_SIGTRAP))
#define exc_e_SIGXCPU		(*_tmexception(exc_s_SIGXCPU))
#define exc_e_SIGXFSZ		(*_tmexception(exc_s_SIGXFSZ))
#else
_TMEXCEPTINIT(pthread_e_badparam,pthread_s_badparam);
_TMEXCEPTINIT(pthread_e_cancel,pthread_s_cancel);
_TMEXCEPTINIT(pthread_e_defer_q_full,pthread_s_defer_q_full);
_TMEXCEPTINIT(pthread_e_existence,pthread_s_existence);
_TMEXCEPTINIT(pthread_e_in_use,pthread_s_in_use);
_TMEXCEPTINIT(pthread_e_nostackmem,pthread_s_nostackmem);
_TMEXCEPTINIT(pthread_e_nostack,pthread_s_nostack);
_TMEXCEPTINIT(pthread_e_signal_q_full,pthread_s_signal_q_full);
_TMEXCEPTINIT(pthread_e_stackovf,pthread_s_stackovf);
_TMEXCEPTINIT(pthread_e_unimp,pthread_s_unimp);
_TMEXCEPTINIT(pthread_e_use_error,pthread_s_use_error);
_TMEXCEPTINIT(exc_e_decovf,exc_s_decovf);
_TMEXCEPTINIT(exc_e_exquota,exc_s_exquota);
_TMEXCEPTINIT(exc_e_fltdiv,exc_s_fltdiv);
_TMEXCEPTINIT(exc_e_fltovf,exc_s_fltovf);
_TMEXCEPTINIT(exc_e_fltund,exc_s_fltund);
_TMEXCEPTINIT(exc_e_illaddr,exc_s_illaddr);
_TMEXCEPTINIT(exc_e_insfmem,exc_s_insfmem);
_TMEXCEPTINIT(exc_e_intdiv,exc_s_intdiv);
_TMEXCEPTINIT(exc_e_intovf,exc_s_intovf);
_TMEXCEPTINIT(exc_e_nopriv,exc_s_nopriv);
_TMEXCEPTINIT(exc_e_privinst,exc_s_privinst);
_TMEXCEPTINIT(exc_e_resaddr,exc_s_resaddr);
_TMEXCEPTINIT(exc_e_resoper,exc_s_resoper);
_TMEXCEPTINIT(exc_e_subrng,exc_s_subrng);
_TMEXCEPTINIT(exc_e_uninitexc,exc_s_uninitexc);
_TMEXCEPTINIT(exc_e_SIGBUS,exc_s_SIGBUS);
_TMEXCEPTINIT(exc_e_SIGEMT,exc_s_SIGEMT);
_TMEXCEPTINIT(exc_e_SIGFPE,exc_s_SIGFPE);
_TMEXCEPTINIT(exc_e_SIGILL,exc_s_SIGILL);
_TMEXCEPTINIT(exc_e_SIGIOT,exc_s_SIGIOT);
_TMEXCEPTINIT(exc_e_SIGPIPE,exc_s_SIGPIPE);
_TMEXCEPTINIT(exc_e_SIGSEGV,exc_s_SIGSEGV);
_TMEXCEPTINIT(exc_e_SIGSYS,exc_s_SIGSYS);
_TMEXCEPTINIT(exc_e_SIGTRAP,exc_s_SIGTRAP);
_TMEXCEPTINIT(exc_e_SIGXCPU,exc_s_SIGXCPU);
_TMEXCEPTINIT(exc_e_SIGXFSZ,exc_s_SIGXFSZ);
#endif


/* renamed exceptions */
#define pthread_badparam_e	pthread_e_badparam
#define pthread_cancel_e	pthread_e_cancel
#define pthread_defer_q_full_e	pthread_e_defer_q_full
#define pthread_existence_e	pthread_e_existence
#define pthread_in_use_e	pthread_e_in_use
#define pthread_nostackmem_e	pthread_e_nostackmem
#define pthread_nostack_e	pthread_e_nostack
#define pthread_signal_q_full_e	pthread_e_signal_q_full
#define pthread_stackovf_e	pthread_e_stackovf
#define pthread_unimp_e		pthread_e_unimp
#define pthread_use_error_e	pthread_e_use_error
#define exc_decovf_e  exc_e_decovf
#define exc_exquota_e  exc_e_exquota
#define exc_fltdiv_e  exc_e_fltdiv
#define exc_fltovf_e  exc_e_fltovf
#define exc_fltund_e  exc_e_fltund
#define exc_illaddr_e  exc_e_illaddr
#define exc_insfmem_e  exc_e_insfmem
#define exc_intdiv_e  exc_e_intdiv
#define exc_intovf_e  exc_e_intovf
#define exc_nopriv_e  exc_e_nopriv
#define exc_privinst_e  exc_e_privinst
#define exc_resaddr_e  exc_e_resaddr
#define exc_resoper_e  exc_e_resoper
#define exc_subrng_e  exc_e_subrng
#define exc_uninitexc_e  exc_e_uninitexc
#define exc_SIGBUS_e  exc_e_SIGBUS
#define exc_SIGEMT_e  exc_e_SIGEMT
#define exc_SIGFPE_e  exc_e_SIGFPE
#define exc_SIGILL_e  exc_e_SIGILL
#define exc_SIGIOT_e  exc_e_SIGIOT
#define exc_SIGPIPE_e  exc_e_SIGPIPE
#define exc_SIGSEGV_e  exc_e_SIGSEGV
#define exc_SIGSYS_e  exc_e_SIGSYS
#define exc_SIGTRAP_e  exc_e_SIGTRAP
#define exc_SIGXCPU_e  exc_e_SIGXCPU
#define exc_SIGXFSZ_e  exc_e_SIGXFSZ

#endif
