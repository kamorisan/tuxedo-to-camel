/*
	Copyright (c) 2016 Oracle, Inc.
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

/*	Copyright (c) 1993 USL
	All rights reserved

*/
#ifndef FML1632_H
#define FML1632_H

/* #ident	"@(#) fml/libfml/fml1632.h	$Revision: 1.8 $" */
#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_fml1632[] = "@(#) fml/libfml/fml1632.h	$Revision: 1.8 $";
#endif

#undef MAXFBLEN
#define MAXFBLEN	MAXFBLEN32
#define FLDID FLDID32
#define FLDLEN FLDLEN32
#define FLDOCC FLDOCC32
#define Fbfr Fbfr32
#define FBFR FBFR32

#define CFadd(a,b,c,d,e)	CFadd32(a,b,c,d,e)
#define CFchg(a,b,c,d,e,f)	CFchg32(a,b,c,d,e,f)
#define CFfind(a,b,c,d,e)	CFfind32(a,b,c,d,e)
#define CFfindocc(a,b,c,d,e)	CFfindocc32(a,b,c,d,e)
#define CFget(a,b,c,d,e,f)	CFget32(a,b,c,d,e,f)
#define CFgetalloc(a,b,c,d,e)	CFgetalloc32(a,b,c,d,e)
#define F_error(a)		F_error32(a)
#define Fadd(a,b,c,d)		Fadd32(a,b,c,d)
#define Fadds(a,b,c)		Fadds32(a,b,c)
#define Falloc(a,b)		Falloc32(a,b)
#define Fappend(a,b,c,d)	Fappend32(a,b,c,d)
#define Fboolco(a)		Fboolco32(a)
#define Fvboolco(a,b)		Fvboolco32(a,b)
#define Fboolev(a,b)		Fboolev32(a,b)
#define Fvboolev(a,b,c)		Fvboolev32(a,b,c)
#define Fboolpr(a,b)		Fboolpr32(a,b)
#define Fvboolpr(a,b,c)		Fvboolpr32(a,b,c)
#define Fchg(a,b,c,d,e)		Fchg32(a,b,c,d,e)
#define Fchgs(a,b,c,d)		Fchgs32(a,b,c,d)
#define Fchksum(a)		Fchksum32(a)
#define Fcmp(a,b)		Fcmp32(a,b)
#define Fcmpl(a,b,c,d)		Fcmpl32(a,b,c,d)
#define Fconcat(a,b)		Fconcat32(a,b)
#define Fcodeset(a)		Fcodeset32(a)
#define Fcpy(a,b)		Fcpy32(a,b)
#define Fdel(a,b,c)		Fdel32(a,b,c)
#define Fdelall(a,b)		Fdelall32(a,b)
#define Fdelete(a,b)		Fdelete32(a,b)
#define Fextread(a,b)		Fextread32(a,b)
#define Ffind(a,b,c,d)		Ffind32(a,b,c,d)
#define Ffindlast(a,b,c,d)	Ffindlast32(a,b,c,d)
#define Ffindocc(a,b,c,d)	Ffindocc32(a,b,c,d)
#define Ffinds(a,b,c)		Ffinds32(a,b,c)
#define Ffloatev(a,b)		Ffloatev32(a,b)
#define Fvfloatev(a,b,c)	Fvfloatev32(a,b,c)
#define Ffprint(a,b)		Ffprint32(a,b)
#define Fsprint(a,b)        Fsprint32(a,b)
#define Ffree(a)		Ffree32(a)
#define Fget(a,b,c,d,e)		Fget32(a,b,c,d,e)
#define Fgetalloc(a,b,c,d)	Fgetalloc32(a,b,c,d)
#define Fgetlast(a,b,c,d,e)	Fgetlast32(a,b,c,d,e)
#define Fgets(a,b,c,d)		Fgets32(a,b,c,d)
#define Fgetsa(a,b,c,d)		Fgetsa32(a,b,c,d)
#define Fidnm_unload()		Fidnm_unload32()
#define Fidxused(a)		Fidxused32(a)
#define Fielded(a)		Fielded32(a)
#define Findex(a,b)		Findex32(a,b)
#define Finit(a,b)		Finit32(a,b)
#define Fjoin(a,b)		Fjoin32(a,b)
#define Fldid(a)		Fldid32(a)
#define Fldno(a)		Fldno32(a)
#define Fldtype(a)		Fldtype32(a)
#define Flen(a,b,c)		Flen32(a,b,c)
#define Flevels(a)		Flevels32(a)
#define Fmkfldid(a,b)		Fmkfldid32(a,b)
#define Fmove(a,b)		Fmove32(a,b)
#define Fname(a)		Fname32(a)
#define Fneeded(a,b)		Fneeded32(a,b)
#define Fnext(a,b,c,d,e)	Fnext32(a,b,c,d,e)
#define Fnmid_unload()		Fnmid_unload32()
#define Fnum(a)			Fnum32(a)
#define Foccur(a,b)		Foccur32(a,b)
#define Fojoin(a,b)		Fojoin32(a,b)
#define Fpres(a,b,c)		Fpres32(a,b,c)
#define Fprint(a)		Fprint32(a)
#define Fproj(a,b)		Fproj32(a,b)
#define Fprojcpy(a,b,c)		Fprojcpy32(a,b,c)
#define Frealloc(a,b,c)		Frealloc32(a,b,c)
#define Frstrindex(a,b)		Frstrindex32(a,b)
#define Fsettarg(a)		Fsettarg32(a)
#define Fsizeof(a)		Fsizeof32(a)
#define Fstrerror(a)		Fstrerror32(a)
#define Ftypcvt(a,b,c,d,e)	Ftypcvt32(a,b,c,d,e)
#define Ftype(a)		Ftype32(a)
#define Funindex(a)		Funindex32(a)
#define Funused(a)		Funused32(a)
#define Fupdate(a,b)		Fupdate32(a,b)
#define Fused(a)		Fused32(a)
#define Fvall(a,b,c)		Fvall32(a,b,c)
#define Fvals(a,b,c)		Fvals32(a,b,c)
#define Fvftos(a,b,c)		Fvftos32(a,b,c)
#define Fvneeded(a)		Fvneeded32(a)
#define Fvnull(a,b,c,d)		Fvnull32(a,b,c,d)
#define Fvopt(a,b,c)		Fvopt32(a,b,c)
#define Fvrefresh		Fvrefresh32
#define Fvselinit(a,b,c)	Fvselinit32(a,b,c)
#define Fvsinit(a,b)		Fvsinit32(a,b)
#define Fvstof(a,b,c,d)		Fvstof32(a,b,c,d)
#define Fvttos(a,b,c)		Fvttos32(a,b,c)
#define Fvstot(a,b,c,d)		Fvstot32(a,b,c,d)
#define Femsgs			Femsgs32
#define setFerror		setFerror32
#define setUerror		setUerror32
#define tpxmltofml(a,b,c,d,e)	tpxmltofml32(a,b,c,d,e)
#define tpfmltoxml(a,b,c,d,e)	tpfml32toxml(a,b,c,d,e)

#define _Fcchg(a,b,c,d,e,f)	_Fcchg32(a,b,c,d,e,f)
#define _Fcurview		_Fcurview32
#define _Floadidtbls(a,b,c,d)	_Floadidtbls32(a,b,c,d)
#define _Floadnmtbls(a,b,c,d)	_Floadnmtbls32(a,b,c,d)
#define _Fn_views		_Fn_views32
#define _Fr_vtbl		_Fr_vtbl32
#define _Fstr_or_carray		_Fstr_or_carray32
#define _Fstrpool		_Fstrpool32
#define _Fvisnull(a,b,c,d,e)	_Fvisnull32(a,b,c,d,e)
#define _Fvtbl			_Fvtbl32
#define __fxlens		__fxlens32

#undef Fread
#undef Fwrite
#if defined(_TMDOWN) && !(defined(WIN32) && !defined(_TMOLDNTWS))
#define Fread(A,B) FMLread32(A,B)
#define Fwrite(A,B) FMLwrite32(A,B)
#else
#define Fread(A,B) Fread32(A,B)
#define Fwrite(A,B) Fwrite32(A,B)
#endif

#undef Ferror
#define Ferror	(*_Fget_Ferror_addr32())
#if (defined(_TM_WIN) || defined(_TM_OS2) || (defined(WIN32) && defined(_TM_OLDNTWS)))
#define _Fget_Ferror_addr() _Fget_Ferror_addr32()
#define getFerror() getFerror32()
#define _Fget_Fn_views_addr() _Fget_Fn_views_addr32()
#define _Fget_Fvtbl_addr() _Fget_Fvtbl_addr32()

#if !defined(_TMDLL)
#undef _Fn_views
#define _Fn_views	(*_Fget_Fn_views_addr32())
#undef _Fvtbl
#define _Fvtbl		(_Fget_Fvtbl_addr32())
#endif
#endif

#undef V_MAGIC
#define V_MAGIC	V_MAGIC32

/* typed-buffer types */
#undef FMLTYPE
#undef VIEWTYPE
#define FMLTYPE "FML32"
#define VIEWTYPE "VIEW32"

#endif
