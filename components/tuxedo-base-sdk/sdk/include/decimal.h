/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

#ifndef DECIMAL_H
#define DECIMAL_H
/* #ident	"@(#) gp/libgp/decimal.h	$Revision: 1.4 $" */
#ifndef TMENV_H
#include <tmenv.h>
#endif

#ifndef NOWHAT
static char	h_decimal[] = "@(#) gp/libgp/decimal.h	$Revision: 1.4 $";
#endif

/*
 *	DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.
 *
 *	Warning:  This header file should not be changed in any way.
 *	Doing so will destroy the compatibility with TUXEDO programs
 *	and libraries.
 */


/*
 *    Separate sign of mantissa "dec_pos"
 
 *    Base 100 digits (range 0 - 99) with decimal point
 
 *    immediately to the left of first digit.
 */

#define DECSIZE 16
#define DECUNKNOWN -2

#ifdef _as400_
struct decimal_t
#else
struct decimal
#endif
    {
    short dec_exp;		/* exponent base 100		*/
    short dec_pos;		/* sign: 1=pos, 0=neg, -1=null	*/
    short dec_ndgts;		/* number of significant digits	*/
    char  dec_dgts[DECSIZE];	/* actual digits base 100	*/
    };
#ifdef _as400_
typedef struct decimal_t dec_t;
#else
typedef struct decimal dec_t;
#endif

/*
 *  A decimal null will be represented internally by setting dec_pos
 *  equal to DECPOSNULL
 */

#define DECPOSNULL	(-1)

/*
 * DECLEN calculates minumum number of bytes
 * necessary to hold a decimal(m,n)
 * where m = total # significant digits and
 *	 n = significant digits to right of decimal
 */

#define DECLEN(m,n)	(((m)+((n)&1)+3)/2)
#define DECLENGTH(len)	DECLEN(PRECTOT(len),PRECDEC(len))

/*
 * DECPREC calculates a default precision given
 * number of bytes used to store number
 */

#define DECPREC(size)	(((size-1)<<9)+2)

/* macros to look at and make encoded decimal precision
 *
 *  PRECTOT(x)		return total precision (digits total)
 *  PRECDEC(x) 		return decimal precision (digits to right)
 *  PRECMAKE(x,y)	make precision from total and decimal
 */

#define PRECTOT(x)	(((x)>>8) & 0xff)
#define PRECDEC(x)	((x) & 0xff)
#define PRECMAKE(x,y)	(((x)<<8) + (y))

/*
 * Packed Format  (format in records in files)
 *
 *    First byte =
 *	  top 1 bit = sign 0=neg, 1=pos
 *	  low 7 bits = Exponent in excess 64 format
 *    Rest of bytes = base 100 digits in 100 complement format
 *    Notes --	This format sorts numerically with just a
 *		simple byte by byte unsigned comparison.
 *		Zero is represented as 80,00,00,... (hex).
 *		Negative numbers have the exponent complemented
 *		and the base 100 digits in 100's complement
 */

/*
 * Define decimal functions to internal names to prevent conflict with
 * other libraries that provide the same functions
 */
#define decadd(A,B,C)		_e_dec_add(A,B,C)
#define deccmp(A,B)		_e_dec_cmp(A,B)
#define deccopy(A,B)		_e_dec_copy(A,B)
#define deccvasc(A,B,C)		_e_dec_cvasc(A,B,C)
#define deccvdbl(A,B)		_e_dec_cvdbl(A,B)
#define deccvflt(A,B)		_e_dec_cvflt(A,B)
#define deccvint(A,B)		_e_dec_cvint(A,B)
#define deccvlong(A,B)		_e_dec_cvlong(A,B)
#define decdiv(A,B,C)		_e_dec_div(A,B,C)
#define dececvt(A,B,C,D)	_e_dec_ecvt(A,B,C,D)
#define decfcvt(A,B,C,D)	_e_dec_fcvt(A,B,C,D)
#define decload(A,B,C,D,E)	_e_dec_load(A,B,C,D,E)
#define decmul(A,B,C)		_e_dec_mul(A,B,C)
#define decprec(A)		_e_dec_prec(A)
#define decround(A,B)		_e_dec_round(A,B)
#define decsub(A,B,C)		_e_dec_sub(A,B,C)
#define dectoasc(A,B,C,D)	_e_dec_toasc(A,B,C,D)
#define dectodbl(A,B)		_e_dec_todbl(A,B)
#define dectoflt(A,B)		_e_dec_toflt(A,B)
#define dectoint(A,B)		_e_dec_toint(A,B)
#define dectolong(A,B)		_e_dec_tolong(A,B)
#define dectrunc(A,B)		_e_dec_trunc(A,B)
#define lddecimal(A,B,C)	_e_dec_lddecimal(A,B,C)
#define stdecimal(A,B,C)	_e_dec_stdecimal(A,B,C)

/* External functions */

#if defined(__cplusplus)
extern "C" {
#endif

extern int	_TMDLLENTRY _e_dec_add _((dec_t _TM_FAR *, dec_t _TM_FAR *, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cmp _((dec_t _TM_FAR *, dec_t _TM_FAR *));
extern void	_TMDLLENTRY _e_dec_copy _((dec_t _TM_FAR *, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cvasc _((char _TM_FAR *, int, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cvdbl _((double, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cvflt _((double, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cvint _((int, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_cvlong _((long, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_div _((dec_t _TM_FAR *, dec_t _TM_FAR *, dec_t _TM_FAR *));
extern char	_TM_FAR * _TMDLLENTRY _e_dec_ecvt _((dec_t _TM_FAR *, int, int _TM_FAR *, int _TM_FAR *));
extern char	_TM_FAR * _TMDLLENTRY _e_dec_fcvt _((dec_t _TM_FAR *, int, int _TM_FAR *, int _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_load _((dec_t _TM_FAR *, int, int, char _TM_FAR *, int));
extern int	_TMDLLENTRY _e_dec_mul _((dec_t _TM_FAR *, dec_t _TM_FAR *, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_prec _((dec_t _TM_FAR *));
extern void	_TMDLLENTRY _e_dec_round _((dec_t _TM_FAR *, int));
extern int	_TMDLLENTRY _e_dec_sub _((dec_t _TM_FAR *, dec_t _TM_FAR *, dec_t _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_toasc _((dec_t _TM_FAR *, char _TM_FAR *, int, int));
extern int	_TMDLLENTRY _e_dec_todbl _((dec_t _TM_FAR *, double _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_toflt _((dec_t _TM_FAR *, float _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_toint _((dec_t _TM_FAR *, int _TM_FAR *));
extern int	_TMDLLENTRY _e_dec_tolong _((dec_t _TM_FAR *, long _TM_FAR *));
extern void	_TMDLLENTRY _e_dec_trunc _((dec_t _TM_FAR *, int));
extern int	_TMDLLENTRY _e_dec_lddecimal _((char _TM_FAR *, int, dec_t _TM_FAR *));
extern void	_TMDLLENTRY _e_dec_stdecimal _((dec_t _TM_FAR *, char _TM_FAR *, int));

#if defined(__cplusplus)
}
#endif


#endif
