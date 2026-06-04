/*
	Copyright (c) 2014 Oracle, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF ORACLE, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/


/*
	Copyright (c) 2005 BEA Systems, Inc.
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
#ifndef TMTYPES_H
#define TMTYPES_H
/* #ident	"@(#) tuxedo/include/tmtypes.h	$Revision: 1.16 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tmtypes[] = "@(#) tuxedo/include/tmtypes.h	$Revision: 1.16 $";
#endif

#ifndef E_ENGINE_H
#include <e_engine.h>
#endif

/*
 *	DEFINITIONS NEEDED BY INTERNAL TUXEDO PROGRAMS.
 *
 *	Warning: This is an internal TUXEDO header file and
 *	should not be included in any part of any user program.
 *	The definitions contained in this file MAY CHANGE from
 *	one release to the next.
 */

#define TMTYPELEN	ED_TYPELEN
#define TMSTYPELEN	ED_STYPELEN

#define TM_MAX_ITYPES	16		/* maximum # of internal types */
#define TMENCODE	0x00000001	/* message is encoded */
#define TMDECODE	0x00000002	/* message is not encoded */
#define TMCMPRS		0x00000004	/* compress message */
#define TMEXPND		0x00000008	/* expand message */


/*
 *  FROM 11GR1 WE START SUPPORT LONG SERVICE NAME,
 *  SO WE DEFINES TWO MACRO PASSED TO ENC/DEC FUNCTION
 *  FOR HANDLE OLD/NEW OPERATION IN ONE CALLBACK
 *  INTERFACE.
 *  PLEASE NOTE THE VALUE 0x00000010 AND 0x00000020
 *  ARE ALREADY USED IN tuxedo/include/tmbase.h.
 *  BUT IN HERE WE ONLY USE THEM TO DIFF WITH
 *  TMENCODE/TMDECODE WHEN CALL THESE ENCDEC FUNCTIONS.
 *  ADDING THESE TWO OF DEFINITION IS USED TO KEEP
 *  CONSISTENT OF THE CALLBACK INTERFACE.
 *  THESE TWO OF DEFINITIONS CAN'T BE USED FOR OTHER
 *  PURPOSE.
 */
#define TMINTOPENCODE	0x00000010
#define TMINTOPDECODE	0x00000020



/* Flag values for the presend2 functions */

#define TMPARENT	0x00000001
#define TMUSEOPTR	0x00000002
#define TMUSEIPTR	0x00000004

#if defined(__cplusplus)
extern "C" {
#endif

struct tmtype_sw_t {
	char type[TMTYPELEN];	   /* type of buffer */
	char subtype[TMSTYPELEN];  /* sub-type of buffer */
	long dfltsize;		   /* default size of buffer */
	/* buffer initialization function pointer */
	int (_TMDLLENTRY *initbuf) _((char _TM_FAR *, long));
	/* buffer re-initialization function pointer */
	int (_TMDLLENTRY *reinitbuf) _((char _TM_FAR *, long));
	/* buffer un-initialization function pointer */
	int (_TMDLLENTRY *uninitbuf) _((char _TM_FAR *, long));
	/* pre-send buffer manipulation func pointer */
	long (_TMDLLENTRY *presend) _((char _TM_FAR *, long, long));
	/* post-send buffer manipulation func pointer */
	void (_TMDLLENTRY *postsend) _((char _TM_FAR *, long, long));
	/* post-receive buffer manipulation func pointer*/
	long (_TMDLLENTRY *postrecv) _((char _TM_FAR *, long, long));
	/* encode/decode function pointer */
	long (_TMDLLENTRY *encdec) _((int, char _TM_FAR *, long, char _TM_FAR *, long));
	/* routing function pointer */
	int (_TMDLLENTRY *route) _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
	/* buffer filtering function pointer */
	int (_TMDLLENTRY *filter) _((char _TM_FAR *, long, char _TM_FAR *, long));
	/* buffer formatting function pointer */
	int (_TMDLLENTRY *format) _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
	/* process buffer before sending, possibly generating copy */
	long (_TMDLLENTRY *presend2) _((char _TM_FAR *, long, long, char _TM_FAR *, long, long _TM_FAR *));
	/* multi-byte codeset data conversion function pointer */
	long (_TMDLLENTRY *mbconv) _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long, TM32U *));
	/* this space reserved for future expansion */
	void (_TMDLLENTRY *reserved[8]) _((void));
};

#if defined(__cplusplus)
}
#endif

typedef struct tmtype_sw_t TMTYPESW;

_TMITUX2WSC extern TMTYPESW _TM_NEAR tm_itypesw[];  /* internal buffer type switch */

/********************************************************************/
/* EVERYTHING BELOW THIS LINE SHOULD BE MOVED TO AN INTERNAL HEADER */
/********************************************************************/
#define _tmsw ((TUXT->_TUXT__tm_swindex >= TM_MAX_ITYPES) ? \
	&TUXP->_TUXP_tm_typeswp[TUXT->_TUXT__tm_swindex-TM_MAX_ITYPES] : \
	&tm_itypesw[TUXT->_TUXT__tm_swindex])

#define _tmtype			(_tmsw->type)
#define _tmsubtype		(_tmsw->subtype)
#define _tmsize			(_tmsw->dfltsize)
#ifndef lint
#define _tminitbuf(p, l)	\
	((*(_tmsw->initbuf == NULL ? _dfltinitbuf : _tmsw->initbuf)) \
	 ((p), (l)))
#define _tmreinitbuf(p, l)	\
	((*(_tmsw->reinitbuf == NULL ? _dfltinitbuf : _tmsw->reinitbuf)) \
	 ((p), (l)))
#define _tmuninitbuf(p, l)	\
	((*(_tmsw->uninitbuf == NULL ? _dfltinitbuf : _tmsw->uninitbuf)) \
	 ((p), (l)))
#define _tmpresend(p, l, ml)	\
	((*(_tmsw->presend == NULL ? _dfltblen : _tmsw->presend)) \
	 ((p), (l), (ml)))
#define _tmpostsend(p, l, ml)	\
	((*(_tmsw->postsend == NULL ? _dfltpostsend : _tmsw->postsend)) \
	 ((p), (l), (ml)))
#define _tmpostrecv(p, rl, ml)	\
	((*(_tmsw->postrecv == NULL ? _dfltblen : _tmsw->postrecv)) \
	 ((p), (rl), (ml)))
#define _tmencdec(op, e, el, o, ol)	\
	((*(_tmsw->encdec == NULL ? _dfltencdec : _tmsw->encdec)) \
	 ((op), (e), (el), (o), (ol)))
#define _tmroute(n, s, d, l, g)	\
	((*(_tmsw->route == NULL ? _dfltroute : _tmsw->route)) \
	 ((n), (s), (d), (l), (g)))
#define _tmfilter(p, dl, e, el)		\
	((*(_tmsw->filter == NULL ? _dfltfilter : _tmsw->filter)) \
	 ((p), (dl), (e), (el)))
#define _tmformat(p, dl, f, r, rl)	\
	((*(_tmsw->format == NULL ? _dfltformat : _tmsw->format)) \
	 ((p), (dl), (f), (r), (rl)))
#define _tmpresend2(ip, il, ml, op, ol, f)	\
	((*(_tmsw->presend2 == NULL ? NULL : _tmsw->presend2)) \
	 ((ip), (il), (ml), (op), (ol), (f)))
#define _tmconvmb(ip, il, en, op, ol, f)	\
	((*(_tmsw->mbconv == NULL ? NULL : _tmsw->mbconv)) \
	 ((ip), (il), (en), (op), (ol), (f)))
#else
extern	int	_tminitbuf _((char *, long));
extern	int	_tmreinitbuf _((char *, long));
extern	int	_tmuninitbuf _((char *, long));
extern	long	_tmpresend _((char *, long, long));
extern	void	_tmpostsend _((char *, long, long));
extern	long	_tmpostrecv _((char *, long, long));
extern	long	_tmencdec _((int, char *, long, char *, long));
extern	int	_tmroute _((char *, char *, char *, long, char *));
extern	int	_tmfilter _((char *, long, char *, long));
extern	int	_tmformat _((char *, long, char *, char *, long));
extern	long	_tmpresend2 _((char *, long, long, char *, long, long *));
extern	long	_tmconvmb _((char *, long, char *, char *, long, TM32U *));
#endif


#if defined(__cplusplus)
extern "C" {
#endif

extern  int     _TMDLLENTRY _dfltinitbuf _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _dfltblen _((char _TM_FAR *, long, long));
extern  void    _TMDLLENTRY _dfltpostsend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _dfltencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _dfltroute _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern	int	_TMDLLENTRY _dfltfilter _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _dfltformat _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern  long    _TMDLLENTRY _strpresend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _strencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _sfilter _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _sformat _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _finit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _freinit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _funinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _fpresend _((char _TM_FAR *, long, long));
extern  void    _TMDLLENTRY _fpostsend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _fpostrecv _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _fencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _froute _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern	int	_TMDLLENTRY _ffilter _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _fformat _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _finit32 _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _freinit32 _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _funinit32 _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _fpresend32 _((char _TM_FAR *, long, long));
extern  void    _TMDLLENTRY _fpostsend32 _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _fpostrecv32 _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _fencdec32 _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _froute32 _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern	int	_TMDLLENTRY _ffilter32 _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _fformat32 _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern  long    _TMDLLENTRY _fpresend232 _((char _TM_FAR *, long, long, char _TM_FAR *, long, long *));
extern  int     _TMDLLENTRY _xroute _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern  long    _TMDLLENTRY _xpostrecv _((char _TM_FAR *, long, long));
extern  int     _TMDLLENTRY _vinit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _vreinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _vpresend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _vencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _vroute _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern	int	_TMDLLENTRY _vfilter _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _vformat _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _vinit32 _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _vreinit32 _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _vpresend32 _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _vencdec32 _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _vroute32 _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *, long, char _TM_FAR *));
extern	int	_TMDLLENTRY _vfilter32 _((char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _vformat32 _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _TPINITinit _((char _TM_FAR *, long));
extern	int	_TMDLLENTRY _TPINITreinit _((char _TM_FAR *, long));
extern	long	_TMDLLENTRY _TPINITpresend _((char _TM_FAR *, long , long));
extern	void	_TMDLLENTRY _TPINITpostsend _((char _TM_FAR *, long , long));
extern	long	_TMDLLENTRY _TPINITpostrecv _((char _TM_FAR *, long , long));
extern	long	_TMDLLENTRY _TPINITencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern	int	_TMDLLENTRY _TGIOPinit _((char _TM_FAR *, long));
extern  long   _TMDLLENTRY _itcmpropencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  long    _TMDLLENTRY _itcmrouteencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  long    _TMDLLENTRY _itcmroutepresend _((char _TM_FAR *, long, long));
extern  void    _TMDLLENTRY _itcmroutepostsend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _itcmroutepostrecv _((char _TM_FAR *, long, long));
extern  int    _TMDLLENTRY _itcmrouteinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _itcmcalloutencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  long    _TMDLLENTRY _itcmcalloutpresend _((char _TM_FAR *, long, long));
extern  void    _TMDLLENTRY _itcmcalloutpostsend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _itcmcalloutpostrecv _((char _TM_FAR *, long, long));
extern  int     _TMDLLENTRY _itcmcalloutinit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _itcmtdomvalsinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _itcmtdomvalspostrecv _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _itcmtdomvalsencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));
extern  int     _TMDLLENTRY _rinit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _rreinit _((char _TM_FAR *, long));
extern  int     _TMDLLENTRY _runinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _rpresend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _rencdec _((int, char _TM_FAR *, long, char _TM_FAR *, long));

extern	int	_TMDLLENTRY AEWaddtypesw _((TMTYPESW _TM_FAR *newtype));
extern TMTYPESW _TM_FAR * _TMDLLENTRY _tmtypeswaddr _((void));

extern	int	_tuxdftcmpexp _((int, char *, long *, char *, long));
extern  int     _TMDLLENTRY _mbsinit _((char _TM_FAR *, long));
extern  long    _TMDLLENTRY _mbsconv _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long, TM32U *));
extern  long    _TMDLLENTRY _mbspresend _((char _TM_FAR *, long, long));
extern  long    _TMDLLENTRY _fmbconv32 _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long, TM32U *));
extern  long    _TMDLLENTRY _vmbconv32 _((char _TM_FAR *, long, char _TM_FAR *, char _TM_FAR *, long, TM32U *));

#if defined(__cplusplus)
}
#endif

#endif
