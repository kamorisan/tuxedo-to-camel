/*
	Copyright (c) 2005 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/

/* #ident	"@(#) gp/libgp/Uunix.h	$Revision: 1.26 $" */

#ifndef UUNIX_H
#define UUNIX_H

#ifndef TMENV_H
#include <tmenv.h>
#endif

#ifndef NOWHAT
static	char	h_Uunix[] = "@(#) gp/libgp/Uunix.h	$Revision: 1.26 $";
#endif

/*
 *  DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.
 *
 *  Warning: This header file should not be changed in any way.
 *  Doing so will destroy the compatibility with TUXEDO programs
 *  and libraries.
 */

#define UUNIXMIN    0
#define UCLOSE      1
#define UCREAT      2
#define UEXEC       3
#define UFCTNL      4
#define UFORK       5
#define ULSEEK      6
#define UMSGCTL     7
#define UMSGGET     8
#define UMSGSND     9
#define UMSGRCV     10
#define UOPEN       11
#define UPLOCK      12
#define UREAD       13
#define USEMCTL     14
#define USEMGET     15
#define USEMOP      16
#define USHMCTL     17
#define USHMGET     18
#define USHMAT      19
#define USHMDT      20
#define USTAT       21
#define UWRITE      22
#define USBRK       23
#define USYSMUL     24
#define UWAIT       25
#define UKILL       26
#define UTIME       27
#define UMKDIR      28
#define ULINK       29
#define UUNLINK     30
#define UUNAME      31
#define UNLIST      32
#define UUNIXMAX    33


#if defined(__cplusplus)
extern "C" {
#endif
extern int _TM_FAR * _TMDLLENTRY _e_ctx_get_Uunixerr_addr _((void));
#ifndef Uunixerr
#define Uunixerr    (*_e_ctx_get_Uunixerr_addr())
#endif
extern  void    Uunix_err _((char *));
_TMIGP extern   char _TMCONST *_TMCONST Uunixmsg[];

#if defined(__cplusplus)
}
#endif


#if defined(_TMPROTOTYPES)
#include <stddef.h>
#include <stdlib.h>
#ifndef _MAC
#include <sys/types.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#if defined(_TMOVMS)
#include <unixlib.h>    /* for decc$translate_vms() */
#endif

#if !defined(_TMDOWN) && !defined(THINK_C) && !defined(applec) && !defined(NeXT)
#if !defined(_TMOVMS)
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#undef IN
#include <sys/msg.h>
#undef msg
#endif  /* not _TMOVMS */

#if !defined(_TM_NETWARE) && !defined(WIN32)
#include <unistd.h>
#endif

#if !defined(_TM_NETWARE) && !defined(_as400_)
#include <sys/times.h>
#endif

#endif  /* _TMPROTOTYPES ??? */

#ifdef __cplusplus
#if !defined(_TMOVMS) && !defined(_as400_)
#define entry ____entry__
#include <search.h>
#undef entry
#if defined(_TMEDG) || defined(sun)
#include <wait.h>
#else
#if !defined(_TMDOWN) && !defined(__alpha) && !defined(__hpux) && !defined(_SEQUENT_) && !defined(__open_xl__)
#include <osfcn.h>
#endif
#endif
#endif

extern "C" {
#ifdef _as400_
extern int isatty(int);
#endif
#ifndef WIN32
extern char *strdup(const char *);
#endif
#if defined(__cpp_stdc)
extern  int   setpgid(pid_t, pid_t);
#endif
#if !defined(_TMDOWN) && !(defined(__linux__) )
#if defined(_TMOVMS)
extern __char_ptr32 tempnam(const char *dir, const char *pfx, ...);
#else
extern char *tempnam(const char *, const char *);
#endif
#elif defined(__linux__)
extern char *tempnam(const char *, const char *) __THROW __attribute_malloc__;
#endif
}
#if defined(__cpp_stdc) && !defined(_TMNOCRYPTHDR)
#include <crypt.h>
#else
#if defined(__cplusplus)
extern "C" {
#endif
extern char *crypt _((const char* pw, const char* salt));
#if defined(__cplusplus)
}
#endif
#endif

#else
/* not c++ */
#if !defined(_TMDOWN) && !defined(THINK_C) && !defined(applec) && !defined(_TM_NETWARE)
#include <sys/wait.h>
#endif
#if !defined(_TMNOCRYPTHDR)
#include <crypt.h>
#else
extern char *crypt _((const char* pw, const char* salt));
#endif
#ifdef _TMNOSTRDUP
extern char *strdup _((const char *));
#endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif
extern char *Ustrerror(int);
#if !(defined (WIN32) || defined(_TMOVMS))
extern size_t strftime(char *, size_t, const char *, const struct tm *);
#undef strerror
extern char *strerror(int);
#endif
#if defined(__cplusplus)
}
#endif

#else
/* classic c */
#include <string.h>
#if !defined(_TMDOWN) && !defined(NeXT)
#include <memory.h>
#endif
#if defined(sun) || defined(r3000) || defined(r4000)
#include <sys/wait.h>
#endif
#if defined(_IBMR2) || defined(__osf__) || defined(NeXT) || defined(hpux) || defined(sun)
extern  void            abort();
#else
extern  int             abort();
extern  int             abs();
#endif
extern  int             access();
extern  unsigned int    alarm();
extern  int             atoi();
extern  long            atol();
extern  double          atof();
extern  double          strtod();
#if defined(_IBMR2) || defined(__osf__) || defined(NeXT) || defined(hpux) || defined(sun)
extern  void           *bsearch();
#else
extern  char           *bsearch();
#endif

#if defined(sun) || defined(_SEQUENT_)
#include <malloc.h>
#else
#if defined(ultrix) || defined(hpux) || defined(_IBMR2) || defined(__osf__) || defined(NeXT)
extern  void    *malloc();
extern  void    *calloc();
extern  void    *realloc();
extern  void    free();
#else
extern  char    *malloc();
extern  char    *calloc();
extern  char    *realloc();
extern  void    free();
#endif
#endif

extern  int     chdir();
extern  int     close();
extern  int     creat();
extern  char    *crypt();
extern  int     dup();
#if !defined(_as400_)
#if !defined(_IBMR2)
extern  int     execl();
extern  int     execle();
extern  int     execlp();
#endif
extern  int     execv();
extern  int     execve();
extern  int     execvp();
#endif
extern  void    exit();
#ifdef sun
extern  char    *getpass();
#endif
extern  void    _exit();
#if !defined(_as400_)
extern  pid_t   fork();
#endif
extern  char    *gcvt();
extern  char    *getcwd();
extern  char    *getenv();
extern  gid_t   getegid();
extern  uid_t   geteuid();
extern  uid_t   getuid();
extern  gid_t   getgid();
extern  int     getopt();
extern  pid_t   getpid();
extern  int     ioctl();
#if !defined(_as400_)
extern  int     isatty();
#endif
extern  int     kill();
extern  off_t   lseek();
extern  int     msgget();
extern  int     msgsnd();
extern  int     msgrcv();
#if !defined(_as400_)
extern  int     nice();
#endif
#if !defined(_IBMR2)
extern  int     open();
#endif
extern  void    perror();
extern  int     pipe();
extern  int     putenv();
extern  void    qsort();
extern  int     rand();
extern  int     read();
extern  int     semget();
extern  int     setgid();
extern  pid_t   setpgrp();
extern  void    srand();
extern  int     setuid();
extern  char    *shmat();
extern  int     shmget();
extern  unsigned int    sleep();
extern  char    *strdup();
extern  long    strtol();
extern  char    *tmpnam();
extern  char    *mktemp();
extern  time_t  time();
extern  clock_t times();
#if !defined(_IBMR2)
extern  long    ulimit();
#endif
extern  mode_t  umask();
extern  int     unlink();
extern  pid_t   wait();
extern  int     write();
extern  char    *strdup();

#define const

extern char *Ustrerror();
#undef strerror
extern char *strerror();
/* don't define strftime because of problems with typedef for size_t */

#endif

#ifdef __cplusplus
extern void __cplusinit _((char *));
#else
#define __cplusinit(a)
#endif

#if defined(__cplusplus)
extern "C" {
#endif
#if defined (_TM_NETWARE)
char           ** _e_gp_get_optarg_addr(void);
int            (*_e_gp_get_optind_addr(void));
int            (*_e_gp_get_opterr_addr(void));

#define optarg  (*_e_gp_get_optarg_addr())
#define opterr  (*_e_gp_get_opterr_addr())
#define optind  (*_e_gp_get_optind_addr())
#else
/* global variables */
#if !(defined(__alpha) && defined(__osf__)) /* these conflict with Tru64 decls in getopt.h */
#if !(defined(_TMOVMS) && (__INITIAL_POINTER_SIZE == 64))
_TMIGP extern  char    *optarg;
#endif
_TMIGP extern  int     opterr;
_TMIGP extern  int     optind;
#endif
#endif
#if defined(__cplusplus)
}
#endif

#if defined(_TMPROTOTYPES) || defined(__cpp_stdc)
#include <limits.h>
#include <float.h>
/* define the values that used to appear in <values.h> */
#ifndef BITSPERBYTE
#define BITSPERBYTE CHAR_BIT
#endif
#ifndef MAXLONG
#define MAXLONG     LONG_MAX
#endif
#ifndef MINLONG
#define MINLONG     LONG_MIN
#endif
#ifndef MAXSHORT
#define MAXSHORT    SHRT_MAX
#endif
#ifndef MINSHORT
#define MINSHORT    SHRT_MIN
#endif
#ifndef MAXINT
#define MAXINT      INT_MAX
#endif
#ifndef MININT
#define MININT      INT_MIN
#endif
#undef MAXFLOAT
#if defined(HPUX_GAUTHSVR_64)
#undef MAXFLOAT
#else
#define MAXFLOAT    FLT_MAX
#endif
#ifndef MAXDOUBLE
#define MAXDOUBLE   DBL_MAX
#endif
#ifndef MINFLOAT
#define MINFLOAT    FLT_MIN
#endif


#else

/* classic C   or  C++ */

#ifndef WIN32
#include <values.h>
#endif
#ifndef MINSHORT
#define MINSHORT    (-MAXSHORT-1)
#endif
#ifndef MININT
#define MININT      (-MAXINT-1)
#endif
#ifndef MINLONG
#define MINLONG     (-MAXLONG-1)
#endif

#ifndef _POSIX_NAME_MAX
#define _POSIX_NAME_MAX 14
#endif
#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 255
#endif

/* following kludge needed for a current bug in the 3b20 compiler */
#if u3b||u3b20d
#undef MAXSHORT
#define MAXSHORT 32767
#endif

/* don't need these - don't waste defining */
#undef DMAXEXP
#undef DMAXPOWTWO
#undef DMINEXP
#undef DSIGNIF
#undef FMAXEXP
#undef FMAXPOWTWO
#undef FMINEXP
#undef FSIGNIF
#undef M_LN2
#undef M_PI
#undef M_SQRT2
#undef X_EPS
#undef X_PLOSS
#undef X_TLOSS
#undef _DEXPLEN
#undef _EXPBASE
#undef _FEXPLEN
#undef _HIDDENBIT
#undef _IEEE
#undef _LENBASE
#endif

#define BITSPERLONG     (sizeof(long) * BITSPERBYTE)
#define BITSPERLONG32   (sizeof(TM32I) * BITSPERBYTE)
#ifndef MAXLONG32
#ifdef _TMLONG64
#define MAXLONG32 MAXINT
#define MINLONG32 MININT
#define xdr_TM32I xdr_int
#define xdr_TM32U xdr_u_int
#else
#define MAXLONG32 MAXLONG
#define MINLONG32 MINLONG
#define xdr_TM32I xdr_long
#define xdr_TM32U xdr_u_long
#endif
#else
#ifdef _TMLONG64
#define xdr_TM32I xdr_int
#define xdr_TM32U xdr_u_int
#else
#define xdr_TM32I xdr_long
#define xdr_TM32U xdr_u_long
#endif
#endif

/* at this point, malloc is defined */
#ifndef _MALLOC_H
#define _MALLOC_H
#endif

/* CR193068, round up should not use long, because
 * ((long)ptr / sz) != ((long)ptr & ~(sz-1))  when
 * the most significant bit of ptr is 1.
 * To minimize the impact, this  fix  only  handle
 * linux platform in one CR it should be fixed  in
 * later releases for all platforms
 */
#ifdef _as400_
#define GP_PTR2INT(ptr, pos)        (((TM32U*)(&(ptr)))[(pos)])
#define GP_ROUNDUP_P_FLOOR(ptr, sz) ((ptr)-(GP_PTR2INT(ptr, 3)-(GP_PTR2INT(ptr, 3) &~ ((sz) - 1))))
#define GP_ROUNDUP_P_CEIL(ptr, sz)  ((ptr)+(((GP_PTR2INT(ptr, 3) + (sz) - 1)&~ ((sz) - 1))-GP_PTR2INT(ptr, 3)))
#define GP_ROUNDUP_P(ptr,sz)        ((sz) == 0 ? (ptr) : GP_ROUNDUP_P_CEIL(ptr, sz))
#define GP_ROUNDUP(amt,sz)          ((((size_t)(amt) + (sz) - 1) / (sz)) * (sz))
#define GP_ROUNDUP4(amt)            (((size_t)(amt) + 3L) & ~3L)
#define GP_ROUNDUP8(amt)            (((size_t)(amt) + 7L) & ~7L)
#else
#define GP_ROUNDUP_P(ptr,sz)        (((TMNUMPTR)(ptr) + (sz) - 1) & ~((sz) - 1))
#define GP_ROUNDUP(amt,sz)          ((((TMNUMPTR)(amt) + (sz) - 1) / (sz)) * (sz))
#define GP_ROUNDUP4(amt)            (((TMNUMPTR)(amt) + 3L) & ~3L)
#define GP_ROUNDUP8(amt)            (((TMNUMPTR)(amt) + 7L) & ~7L)
#endif

/* PR030213: Potential problem on Win64. Modify Alignment*/
#if defined(_TMLONG64) || defined(_WIN64)
#define ALIGN_LONG(x)   ((x)+(sizeof(long)-((x)%sizeof(long))))
#else
#define ALIGN_LONG(x)   (x)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

extern char _TM_FAR * _TMDLLENTRY   tuxgetenv  _((char _TM_FAR *));
extern int _TMDLLENTRY              tuxputenv  _((char _TM_FAR *));
extern int _TMDLLENTRY              tuxreadenv _((char _TM_FAR *, char _TM_FAR *));

#if defined(__cplusplus)
}
#endif

/**
 ** Added for sucurity, avoid to writing "long" "int" overflow.
 **/
/* For 64bit Unix SunOS HP AIX Linux "long" */
#if defined(__64BIT__) || defined(__LP64__) || defined(__sparcv9) || defined(__amd64)
#define STR_LONG_MAX    "9223372036854775807"
#define exceed_max_long(a)  \
        ( (strlen(a) > 19) || ( (strlen(a) == 19) && (strcmp((a), STR_LONG_MAX) > 0) ) )
#else
/* For 32bit and Win64 "long" */
#define STR_LONG_MAX    "2147483647"
#define exceed_max_long(a)  \
        ((strlen(a) > 10) || ( (strlen(a) == 10) && (strcmp((a), STR_LONG_MAX) > 0) ) )
#endif
/* For 32bit and 64bit "int" */
#define STR_INT_MAX     "2147483647"
#define exceed_max_int(a)   \
        ( (strlen(a) > 10) || ( (strlen(a) == 10) && (strcmp((a),STR_INT_MAX) > 0 ) ) )

#endif
