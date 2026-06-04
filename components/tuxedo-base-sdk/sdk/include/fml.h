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
	Copyright (c) 2012 Oracle, Inc.
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

/*	Copyright (c) 1984 AT&T; 1991 USL
	All rights reserved

*/
#ifndef FML_H
#define FML_H

/* #ident	"@(#) fml/libfml/fml.h	$Revision: 1.18 $" */
#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_fml[] = "@(#) fml/libfml/fml.h	$Revision: 1.18 $";
#endif

/*
 *	DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.
 *
 *	Warning: This header file should not be changed in any
 *	way; doing so will destroy the compatibility with TUXEDO programs
 *	and libraries.
 */

/*
 *	---------- Definitions and Macros ----------
 */

#define MAXFBLEN	0xfffc		/* Maximum FBFR length */

#ifndef FML32_H
#define FSTDXINT	16		/* Default indexing interval */
#define FMAXNULLSIZE	2660
#define FVIEWCACHESIZE	128
#define FVIEWNAMESIZE	33
#define FRECORDCACHESIZE	128
#define FRECORDNAMESIZE		33

/* operations presented to _Fmodidx function */
#define FADD	1
#define FMLMOD	2
#define FDEL	3

/* Flag options used in Fvstof() */
#define F_OFFSET	1
#define F_SIZE		2
#define F_PROP		4			/* P */
#define F_FTOS		8			/* S */
#define F_STOF		16			/* F */
#define F_BOTH		(F_STOF | F_FTOS)	/* S,F */
#define F_OFF		0			/* Z */
#define F_LENGTH        32                      /* L */
#define F_COUNT         64                      /* C */
#define F_NONE          128                     /* NONE flag for null value */

/* These are used in Fstof() */
#define FUPDATE		1
#define FCONCAT		2
#define FJOIN		3
#define FOJOIN		4

/* field types */
#define FLD_SHORT	0	/* short int */
#define FLD_LONG	1	/* long int */
#define FLD_CHAR	2	/* character */
#define FLD_FLOAT	3	/* single-precision float */
#define FLD_DOUBLE	4	/* double-precision float */
#define FLD_STRING	5	/* string - null terminated */
#define FLD_CARRAY	6	/* character array */
				/* 7 reserved for FLD_INT (see viewmap.h) */
				/* 8 reserved for FLD_DECIMAL (see viewmap.h) */
#define FLD_PTR		9	/* pointer - for WLE Java */
#define FLD_XPATH		14 /* for xpath */
#define FLD_MAX		14	/* for field type validation */

/* invalid field id - returned from functions where field id not found */
#define BADFLDID (FLDID)0
/* define an invalid field id used for first call to Fnext */
#define FIRSTFLDID (FLDID)0

#endif	/* FML32_H */

/* Field Error Codes - these correspond to the error messages in
 *			F_error.c - make sure to update the error
 *			message list if a new error is added
 */
#ifndef FML32_H
#define FMINVAL 0		/* bottom of error message codes */
#define FALIGNERR 1		/* fielded buffer not aligned */
#define FNOTFLD 2		/* buffer not fielded */
#define FNOSPACE 3		/* no space in fielded buffer */
#define FNOTPRES 4		/* field not present */
#define FBADFLD 5		/* unknown field number or type */
#define FTYPERR 6               /* illegal field type */
#define FEUNIX 7		/* unix system call error */
#define FBADNAME 8		/* unknown field name */
#define FMALLOC 9		/* malloc failed */
#define FSYNTAX 10		/* bad syntax in boolean expression */
#define FFTOPEN 11		/* cannot find or open field table */
#define FFTSYNTAX 12		/* syntax error in field table */
#define FEINVAL 13		/* invalid argument to function */
#define FBADTBL 14      	/* destructive concurrent access to field
				   table */
#define FBADVIEW 15		/* cannot find or get view */
#define FVFSYNTAX 16		/* bad viewfile */
#define FVFOPEN 17		/* cannot find or open viewfile */
#define FBADACM 18              /* ACM contains negative value */
#define FNOCNAME 19             /* cname not found */
#define FEBADOP 20		/* operation invalid for field type */
#define FMAXVAL  21 		/* top of error message codes */

/* Defines for parser flags */
#define TPXPARSNEVER			0x0001
#define TPXPARSALWAYS			0x0002
#define TPXPARSSCHFULL			0x0004
#define TPXPARSCONFATAL			0x0008
#define TPXPARSNSPACE			0x0010
#define TPXPARSDOSCH			0x0020
#define TPXPARSEREFN			0x0040
#define TPXPARSNOEXIT			0x0080
#define TPXPARSNOINCWS			0x0100
#define TPXPARSCACHESET			0x0200
#define TPXPARSCACHERESET		0x0400
#define TPXPARSSTDURI			0x0800		/* TPXPARSFILE use only */
#define TPXPARSUSECACHEDGRAM		0x1000		/* TPXPARSFILE use only */
#define TPXMLALLOWEMPTY			0x2000		/* None parser flag, ATMI use only */
#define MINBUFSIZE			256
#define BUFSIZE				512
#define BLOCKBUFSIZE			4096
struct parser_option{
	long	flags;				/* parser flags the same with it in ATMI functions */
	short	scanner;			/* UseScanner */
	short	refresh;			/* need refresh? 0:not need; 1: need */
	char 	extsche[MINBUFSIZE];	/* ExternalSchemaLocation */
	char	nonssche[MINBUFSIZE];	/* NoNamespaceSchemaLocation */
	long 	datalen;			/* store the xml buf len in auto conversion */
};
#endif	/* FML32_H */



/*
 *	---------- Type Definitions ----------
 */

/* another typedef of FLDID/FLDLEN exists in tuxedo/include/tmbase.h */
#ifndef _FLDID
#define _FLDID
typedef unsigned short FLDID;
typedef unsigned short FLDLEN;
typedef int FLDOCC;
#endif
typedef struct Fbfr FBFR;

/*
 *	---------- External declarations ----------
 */
#if defined(__cplusplus)
extern "C" {
#endif

_TMIFML extern	char	*Femsgs[];

#if defined(_TMDOWN) && !(defined(WIN32) && !defined(_TM_OLDNTWS))
/* handle case-insensitive link conflict for globals and functions */
#define Fread(A,B) FMLread(A,B)
#define Fwrite(A,B) FMLwrite(A,B)
#endif

#define Ferror	(*_Fget_Ferror_addr())
extern int _TM_FAR * _TMDLLENTRY _Fget_Ferror_addr _((void));

#if (defined(_TM_WIN) || defined(_TM_OS2) || (defined(WIN32) && defined(_TM_OLDNTWS))) && !defined(_TMDLL)

extern int _TMDLLENTRY getFerror(void);

#endif

#if defined(__cplusplus)
}
#endif

#ifdef _TMPROTOTYPES
#include <stdio.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_TMDOWN) && !(defined(WIN32) && !defined(_TM_OLDNTWS))
/* handle case-insensitive link conflict for and functions */
extern int _TMDLLENTRY FMLread _((FBFR _TM_FAR *, FILE _TM_FAR *));
extern int _TMDLLENTRY FMLwrite _((FBFR _TM_FAR *, FILE _TM_FAR *));
#else
extern int _TMDLLENTRY Fread _((FBFR *, FILE *));
extern int _TMDLLENTRY Fwrite _((FBFR *, FILE *));
#endif

extern int _TMDLLENTRY CFadd _((FBFR _TM_FAR *, FLDID, char _TM_FAR*, FLDLEN , int));
extern int _TMDLLENTRY CFchg _((FBFR _TM_FAR *, FLDID, FLDOCC , char _TM_FAR *, FLDLEN , int ));
extern char _TM_FAR * _TMDLLENTRY CFfind _((FBFR _TM_FAR *, FLDID, FLDOCC , FLDLEN _TM_FAR *, int));
extern FLDOCC _TMDLLENTRY CFfindocc _((FBFR _TM_FAR *, FLDID, char _TM_FAR *, FLDLEN , int ));
extern int _TMDLLENTRY CFget _((FBFR _TM_FAR *, FLDID, FLDOCC , char _TM_FAR *, FLDLEN _TM_FAR *, int ));
extern char _TM_FAR * _TMDLLENTRY CFgetalloc _((FBFR _TM_FAR *, FLDID, FLDOCC , int , FLDLEN _TM_FAR *));
extern void _TMDLLENTRY F_error _((char *));
extern int _TMDLLENTRY Fappend _((FBFR _TM_FAR *, FLDID, char _TM_FAR *, FLDLEN));
extern int _TMDLLENTRY Fadd _((FBFR _TM_FAR *, FLDID, char _TM_FAR *, FLDLEN));
extern FBFR _TM_FAR * _TMDLLENTRY Falloc _((FLDOCC, FLDLEN));
extern FBFR _TM_FAR * _TMDLLENTRY Frealloc _((FBFR _TM_FAR *, FLDOCC, FLDLEN));
extern int _TMDLLENTRY Ffree _((FBFR _TM_FAR *));
extern int _TMDLLENTRY Fboolev _((FBFR _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fvboolev _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *));
extern double _TMDLLENTRY Ffloatev _((FBFR _TM_FAR *, char _TM_FAR *));
extern double _TMDLLENTRY Fvfloatev _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *));
extern void _TMDLLENTRY Fboolpr _((char _TM_FAR *, FILE _TM_FAR *));
extern int _TMDLLENTRY Fvboolpr _((char _TM_FAR *, FILE _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fchg _((FBFR _TM_FAR *, FLDID, FLDOCC, char _TM_FAR *, FLDLEN));
extern long _TMDLLENTRY Fchksum _((FBFR _TM_FAR *));
extern int _TMDLLENTRY Fcmp _((FBFR _TM_FAR *, FBFR _TM_FAR *));
extern int _TMDLLENTRY Fconcat _((FBFR _TM_FAR *, FBFR _TM_FAR *));
extern int _TMDLLENTRY Fcpy _((FBFR _TM_FAR *,FBFR _TM_FAR *));
extern int _TMDLLENTRY Fdelall _((FBFR _TM_FAR *, FLDID));
extern int _TMDLLENTRY Fdelete _((FBFR _TM_FAR *, FLDID _TM_FAR *));
extern int _TMDLLENTRY Fextread _((FBFR _TM_FAR *, FILE _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Ffind _((FBFR _TM_FAR *, FLDID, FLDOCC , FLDLEN _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fvals _((FBFR _TM_FAR *, FLDID , FLDOCC ));
extern long _TMDLLENTRY Fvall _((FBFR _TM_FAR *, FLDID , FLDOCC ));
extern FLDOCC _TMDLLENTRY Ffindocc _((FBFR _TM_FAR *, FLDID , char _TM_FAR *, FLDLEN ));
extern int _TMDLLENTRY Fget _((FBFR _TM_FAR *, FLDID , FLDOCC , char _TM_FAR *, FLDLEN _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fgetalloc _((FBFR _TM_FAR *, FLDID , FLDOCC , FLDLEN _TM_FAR *));
extern int _TMDLLENTRY Fldtype _((FLDID));
extern FLDOCC _TMDLLENTRY Fldno _((FLDID));
extern int _TMDLLENTRY Fielded _((FBFR _TM_FAR *));
extern long _TMDLLENTRY Fneeded _((FLDOCC, FLDLEN));
extern long _TMDLLENTRY Fused _((FBFR _TM_FAR *));
extern long _TMDLLENTRY Fidxused _((FBFR _TM_FAR *));
extern long _TMDLLENTRY Funused _((FBFR _TM_FAR *));
extern long _TMDLLENTRY Fsizeof _((FBFR _TM_FAR *));
extern FLDID _TMDLLENTRY Fmkfldid _((int , FLDID ));
extern FLDLEN _TMDLLENTRY Fieldlen _((char _TM_FAR *, FLDLEN _TM_FAR *, FLDLEN _TM_FAR *));
extern FLDOCC _TMDLLENTRY Funindex _((FBFR _TM_FAR *));
extern int _TMDLLENTRY Frstrindex _((FBFR _TM_FAR *, FLDOCC ));
extern int _TMDLLENTRY Findex _((FBFR _TM_FAR *, FLDOCC ));
extern int _TMDLLENTRY Finit _((FBFR _TM_FAR *, FLDLEN));
extern int _TMDLLENTRY Fjoin _((FBFR _TM_FAR *, FBFR _TM_FAR *));
extern int _TMDLLENTRY Fojoin _((FBFR _TM_FAR *, FBFR _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Ffindlast _((FBFR _TM_FAR *, FLDID, FLDOCC _TM_FAR *, FLDLEN _TM_FAR *));
extern int _TMDLLENTRY Fgetlast _((FBFR _TM_FAR *, FLDID , FLDOCC _TM_FAR *, char _TM_FAR *, FLDLEN _TM_FAR *));
extern int _TMDLLENTRY Flen _((FBFR _TM_FAR *, FLDID , FLDOCC ));
extern int _TMDLLENTRY Fmove _((char _TM_FAR *, FBFR _TM_FAR *));
extern int _TMDLLENTRY Fnext _((FBFR _TM_FAR *, FLDID _TM_FAR *, FLDOCC _TM_FAR *, char _TM_FAR *, FLDLEN _TM_FAR *));
extern FLDID _TMDLLENTRY Fldid _((char _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fname _((FLDID));
extern char _TM_FAR * _TMDLLENTRY Ftype _((FLDID));
extern void _TMDLLENTRY Fnmid_unload _((void));
extern void _TMDLLENTRY Fidnm_unload _((void));
extern FLDOCC _TMDLLENTRY Fnum _((FBFR _TM_FAR *));
extern FLDOCC _TMDLLENTRY Foccur _((FBFR _TM_FAR *, FLDID ));
extern int _TMDLLENTRY Fprint _((FBFR *));
extern int _TMDLLENTRY Ffprint _((FBFR _TM_FAR *, FILE _TM_FAR *));
extern int _TMDLLENTRY Fsprint _((FBFR _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fproj _((FBFR _TM_FAR *, FLDID _TM_FAR *));
extern int _TMDLLENTRY Fprojcpy _((FBFR _TM_FAR *, FBFR _TM_FAR *, FLDID _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Ftypcvt _((FLDLEN _TM_FAR *, int , char _TM_FAR *, int , FLDLEN ));
extern int _TMDLLENTRY Fupdate _((FBFR _TM_FAR *, FBFR _TM_FAR *));
extern int _TMDLLENTRY Fvneeded _((char _TM_FAR *));
extern int _TMDLLENTRY Fvopt _((char _TM_FAR *, int , char _TM_FAR *));
extern int _TMDLLENTRY Fvsinit _((char _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fvnull _((char _TM_FAR *, char _TM_FAR *, FLDOCC, char _TM_FAR *));
extern int _TMDLLENTRY Fvselinit _((char _TM_FAR *, char _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fvftos _((FBFR _TM_FAR *, char _TM_FAR *, char _TM_FAR *));
extern int _TMDLLENTRY Fvstof _((FBFR _TM_FAR *, char _TM_FAR *, int , char _TM_FAR *));
extern void _TMDLLENTRY Fvrefresh _((void));
extern char _TM_FAR * _TMDLLENTRY Fboolco _((char _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fvboolco _((char _TM_FAR *, char _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fstrerror _((int));
extern long _TMDLLENTRY Fvttos _((char _TM_FAR *cstruct, char _TM_FAR *trecord, char _TM_FAR *viewname));
extern long _TMDLLENTRY Fvstot _((char _TM_FAR *cstruct, char _TM_FAR *trecord, long treclen, char _TM_FAR *viewname));
extern int _TMDLLENTRY Fcodeset _((unsigned char _TM_FAR *codeset));
extern int _TMDLLENTRY Flevels _((int));

#ifdef FML32_H
extern int _TMDLLENTRY F16to32 _((FBFR32 _TM_FAR *dbfr, FBFR _TM_FAR *sbfr));
extern int _TMDLLENTRY F32to16 _((FBFR _TM_FAR *dbfr, FBFR32 _TM_FAR *sbfr));
#endif

/* former macro definitions */
extern int _TMDLLENTRY Fdel _((FBFR _TM_FAR *, FLDID, FLDOCC));
extern int _TMDLLENTRY Fpres _((FBFR _TM_FAR *, FLDID, FLDOCC));
extern int _TMDLLENTRY Fadds _((FBFR _TM_FAR *, FLDID, char _TM_FAR *));
extern int _TMDLLENTRY Fchgs _((FBFR _TM_FAR *, FLDID, FLDOCC, char _TM_FAR *));
extern int _TMDLLENTRY Fgets _((FBFR _TM_FAR *, FLDID, FLDOCC, char _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Fgetsa _((FBFR _TM_FAR *, FLDID, FLDOCC, FLDLEN _TM_FAR *));
extern char _TM_FAR * _TMDLLENTRY Ffinds _((FBFR _TM_FAR *, FLDID, FLDOCC));
extern int _TMDLLENTRY _Finitx _((FBFR *, FLDLEN, FLDOCC));

extern int _TMDLLENTRY tpxmltofml _((char *, char *, FBFR **, char **, long));
extern int _TMDLLENTRY tpfmltoxml _((FBFR *, char *, char *, char **, long));

#if defined(__cplusplus)
}
#endif

/* typed-buffer types */
#define FMLTYPE "FML"
#define VIEWTYPE "VIEW"

#endif
