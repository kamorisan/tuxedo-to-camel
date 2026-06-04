/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1997 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#ifndef TSETJMP_H
#define TSETJMP_H
/* #ident	"@(#) trpc/rpc/tsetjmp.h	$Revision: 1.3 $" */

#ifdef WIN32
/* the following defn. should be identical to NTjmp_buf_t.  Redefined to
 * avoid dragging in tuxnt.h
 */
struct trpc_jmpbuf_t {
        jmp_buf base_buf;
        unsigned int  hTarget; /* should be same type as datatype HANDLE */
        unsigned long  hTargetId; /* should be same type as datatype DWORD */
};
typedef struct trpc_jmpbuf_t trpc_jmpbuf;

#ifndef TUXNT_H
struct NTjmp_buf_t {
        jmp_buf base_buf;
        unsigned int  hTarget; /* should be same type as datatype HANDLE */
        unsigned long  hTargetId; /* should be same type as datatype DWORD */
};
typedef struct NTjmp_buf_t NTjmp_buf;
#endif

extern void NTsetjmp(NTjmp_buf* buf);
extern void NTlongjmp(NTjmp_buf* buf, int ret);

#define trpc_setjmp(buf) (NTsetjmp((NTjmp_buf *)&buf), _setjmp(buf.base_buf))
#define trpc_longjmp(buf,lvl)	NTlongjmp((NTjmp_buf *)&buf, lvl)

#else 

/* the following defn.s should not be changed. trpc functioning depends on the
 * trpc_jmpbuf structure being same as the jmpbuf structure used by libtux.
 */
#define trpc_jmpbuf	jmp_buf
#define trpc_setjmp(buf)	setjmp(buf)
#define trpc_longjmp(buf,lvl)	longjmp(buf,lvl)

#endif

#endif
