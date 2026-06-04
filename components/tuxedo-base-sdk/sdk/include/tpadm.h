/*
 * Copyright (c) 2016 Oracle and/or its affiliates.
 * All rights reserved
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
/* Copyright (c) 1994 Novell
   All rights reserved

   THIS IS UNPUBLISHED PROPRIETARY
   SOURCE CODE OF NOVELL
   The copyright notice above does not
   evidence any actual or intended
   publication of such source code.

#ident "@(#) tuxedo/include/tpadm $Revision: 1.140.2.6 $"
*/

/*
 * Warning: This file should not be changed in any
 * way, doing so will destroy the compatibility with TUXEDO programs
 * and libraries.
 */
#ifndef _TPADM_H
#define _TPADM_H
#ifndef NOWHAT
static char h_tpadm[] = "@(#) tuxedo/include/tpadm	$Revision: 1.140.2.6 $";
#endif
#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef ATMI_H
#include <atmi.h>
#endif

#include <fml32.h>

/* User level interface function prototypes */

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * PLEASE USE SPACE INSTEAD OF TAB AS THE INDENT IN
 * ORDER  TO  LET THE TEXT FILE TO BE MORE READABLE
 * AND  KEEP  CONSISTENT  STYLE WITH EXISTING FIELD
 * DEFINITIONS.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#if defined(__cplusplus)
extern "C" {
#endif

extern int	tpadmcall _((FBFR32 *, FBFR32 **, long));

#if defined(__cplusplus)
}
#endif

/*	ADMIN FIELD ENTRIES */

/*
 * Flags values stored in TA_FLAGS, bit-wise flag map.
 *
 * Flag names beginning with MIB_ are generic to all component MIBs and
 * should not be reused or overlapped.  These values should also not be
 * changed as changes will affect release interoperability.
 *
 * Flag names beginning with other values (e.g., TMIB_ or QMIB_ are
 * specific to particular component MIBs and may be overlapped to conserve
 * the "name" space of the TA_FLAGS values.  Be sure when overlapping and
 * defining component MIB flag values to avoid overlapping with MIB_ values.
 */
#define MIB_PREIMAGE       0x00000001
#define MIB_LOCAL          0x00010000
#define MIB_SELF           0x00100000
#define MIB_METRICS        0x00001000

/*#define TMIB_RSRVD1      0x00010000*/
#define TMIB_NOTIFY        0x00020000
#define TMIB_ADMONLY       0x00040000
#define TMIB_CONFIG        0x00080000
/*#define TMIB_RSRVD2      0x00100000*/
#define TMIB_APPONLY       0x00200000
#define TMIB_GLOBAL        0x00400000

/*#define QMIB_RSRVD1      0x00010000*/
#define QMIB_FORCECLOSE    0x00020000
#define QMIB_FORCEPURGE    0x00040000
#define QMIB_FORCEDELETE   0x00080000
/*#define QMIB_RSRVD2      0x00100000*/

#define MIB_ALLFLAGS       (MIB_PREIMAGE|MIB_LOCAL|MIB_SELF|MIB_METRICS|\
                            TMIB_NOTIFY|TMIB_ADMONLY|TMIB_CONFIG|TMIB_APPONLY|TMIB_GLOBAL|\
                            QMIB_FORCECLOSE|QMIB_FORCEPURGE|QMIB_FORCEDELETE)

/* TA_ATTFLAGS values, bit-wise flag map */
#define MIBATT_KEYFIELD    0x00000001
#define MIBATT_LOCAL       0x00000002
#define MIBATT_REGEXKEY    0x00000004
#define MIBATT_REQUIRED    0x00000008
#define MIBATT_SETKEY      0x00000010
#define MIBATT_NEWONLY     0x00000020
#define MIBATT_RUNTIME     0x00000040


/* Error Return codes set in TA_ERROR */

#define TAEAPP             -1  /* App component error during MIB processing */
#define TAECONFIG          -2  /* Operating system error */
#define TAEINVAL           -3  /* invalid argument */
#define TAEOS              -4  /* Operating system error */
#define TAEPERM            -5  /* permission error */
#define TAEPREIMAGE        -6  /* preimage does not match current image */
#define TAEPROTO           -7  /* MIB specific protocol error */
#define TAEREQUIRED        -8  /* field value required but not present */
#define TAESUPPORT         -9  /* Documented but unsupported feature */
#define TAESYSTEM          -10 /* Internal System/T error */
#define TAEUNIQ            -11 /* SET did not specify unique class instance */

/* Non-Error Return codes set in TA_ERROR */

#define TAOK               0   /* suceeded */
#define TAUPDATED          1   /* suceeded and updated a record */
#define TAPARTIAL          2   /* suceeded at master; failed elsewhere */

/*	fname	fldid            */
/*	-----	-----            */
#define	TA_ATTRIBUTE	((FLDID32)33560432)	/* number: 6000	 type: long */
#define	TA_BADFLD	((FLDID32)33560433)	/* number: 6001	 type: long */
#define	TA_CLASS	((FLDID32)167778162)	/* number: 6002	 type: string */
#define	TA_CLASSNAME	((FLDID32)167778163)	/* number: 6003	 type: string */
#define	TA_CURSOR	((FLDID32)167778164)	/* number: 6004	 type: string */
#define	TA_CURSORHOLD	((FLDID32)33560437)	/* number: 6005	 type: long */
#define	TA_ERROR	((FLDID32)33560438)	/* number: 6006	 type: long */
#define	TA_FILTER	((FLDID32)33560439)	/* number: 6007	 type: long */
#define	TA_FLAGS	((FLDID32)33560440)	/* number: 6008	 type: long */
#define	TA_MIBTIMEOUT	((FLDID32)33560441)	/* number: 6009	 type: long */
#define	TA_MORE	((FLDID32)33560442)	/* number: 6010	 type: long */
#define	TA_OCCURS	((FLDID32)33560443)	/* number: 6011	 type: long */
#define	TA_OPERATION	((FLDID32)167778172)	/* number: 6012	 type: string */
#define	TA_PERM	((FLDID32)33560445)	/* number: 6013	 type: long */
#define	TA_STATE	((FLDID32)167778174)	/* number: 6014	 type: string */
#define	TA_STATUS	((FLDID32)167778175)	/* number: 6015	 type: string */
/* ------------------------------------------------------------
 * The following fields are TM_MIB(5) specific fields.  Consult TM_MIB(5)
 * for details on each field and its use.
 */

#ifndef REDUCE_CPP
/*
 * The field numbers below should always begin at 0 and increase.
 * Field numbers cannot be reused or changed from release to release or
 * interoperability will be broken.  Fields 0-244 (base 6200) defined in
 * release 5.0, fields defined in release 6.0 begin at field 245 and
 * increase from there.
 *
 * Note that fields 7200-7299 are reserved for use by TUXEDO OEMs.
 */
#define	TA_ACCWORD	((FLDID32)33560632)	/* number: 6200	 type: long */
#define	TA_APPDIR	((FLDID32)167778361)	/* number: 6201	 type: string */
#define	TA_AUTHSVC	((FLDID32)167778362)	/* number: 6202	 type: string */
#define	TA_AUTOTRAN	((FLDID32)167778363)	/* number: 6203	 type: string */
#define	TA_BASESRVID	((FLDID32)33560636)	/* number: 6204	 type: long */
#define	TA_BBLQUERY	((FLDID32)33560637)	/* number: 6205	 type: long */
#define	TA_BLOCKTIME	((FLDID32)33560638)	/* number: 6206	 type: long */
#define	TA_BRIDGE	((FLDID32)167778367)	/* number: 6207	 type: string */
#define	TA_BUFTYPE	((FLDID32)167778368)	/* number: 6208	 type: string */
#define	TA_CFGDEVICE	((FLDID32)167778369)	/* number: 6209	 type: string */
#define	TA_CFGOFFSET	((FLDID32)33560642)	/* number: 6210	 type: long */
#define	TA_CLIENTID	((FLDID32)167778371)	/* number: 6211	 type: string */
#define	TA_CLOPT	((FLDID32)167778372)	/* number: 6212	 type: string */
#define	TA_CLOSEINFO	((FLDID32)167778373)	/* number: 6213	 type: string */
#define	TA_CLTLMID	((FLDID32)167778374)	/* number: 6214	 type: string */
#define	TA_CLTNAME	((FLDID32)167778375)	/* number: 6215	 type: string */
#define	TA_CLTPID	((FLDID32)33560648)	/* number: 6216	 type: long */
#define	TA_CLTREPLY	((FLDID32)167778377)	/* number: 6217	 type: string */
#define	TA_CMPLIMIT	((FLDID32)167778378)	/* number: 6218	 type: string */
#define	TA_CMTRET	((FLDID32)167778379)	/* number: 6219	 type: string */
#define	TA_CONNOGRPNO	((FLDID32)33560652)	/* number: 6220	 type: long */
#define	TA_CONNOLMID	((FLDID32)167778381)	/* number: 6221	 type: string */
#define	TA_CONNOPID	((FLDID32)33560654)	/* number: 6222	 type: long */
#define	TA_CONNOREGIDX	((FLDID32)33560655)	/* number: 6223	 type: long */
#define	TA_CONNOSNDCNT	((FLDID32)33560656)	/* number: 6224	 type: long */
#define	TA_CONNOSRVID	((FLDID32)33560657)	/* number: 6225	 type: long */
#define	TA_CONNSGRPNO	((FLDID32)33560658)	/* number: 6226	 type: long */
#define	TA_CONNSLMID	((FLDID32)167778387)	/* number: 6227	 type: string */
#define	TA_CONNSPID	((FLDID32)33560660)	/* number: 6228	 type: long */
#define	TA_CONNSSNDCNT	((FLDID32)33560661)	/* number: 6229	 type: long */
#define	TA_CONNSSRVID	((FLDID32)33560662)	/* number: 6230	 type: long */
#define	TA_CONTIME	((FLDID32)33560663)	/* number: 6231	 type: long */
#define	TA_CONV	((FLDID32)167778392)	/* number: 6232	 type: string */
#define	TA_COORDLMID	((FLDID32)167778393)	/* number: 6233	 type: string */
#define	TA_CURACCESSERS	((FLDID32)33560666)	/* number: 6234	 type: long */
#define	TA_CURCLIENTS	((FLDID32)33560667)	/* number: 6235	 type: long */
#define	TA_CURCONV	((FLDID32)33560668)	/* number: 6236	 type: long */
#define	TA_CURDRT	((FLDID32)33560669)	/* number: 6237	 type: long */
#define	TA_CURGROUPS	((FLDID32)33560670)	/* number: 6238	 type: long */
#define	TA_CURGTT	((FLDID32)33560671)	/* number: 6239	 type: long */
#define	TA_CURLMID	((FLDID32)167778400)	/* number: 6240	 type: string */
#define	TA_CURMACHINES	((FLDID32)33560673)	/* number: 6241	 type: long */
#define	TA_CURQUEUES	((FLDID32)33560674)	/* number: 6242	 type: long */
#define	TA_CURREQ	((FLDID32)33560675)	/* number: 6243	 type: long */
#define	TA_CURRFT	((FLDID32)33560676)	/* number: 6244	 type: long */
#define	TA_CURRLOAD	((FLDID32)33560677)	/* number: 6245	 type: long */
#define	TA_CURRSERVICE	((FLDID32)167778406)	/* number: 6246	 type: string */
#define	TA_CURRTDATA	((FLDID32)33560679)	/* number: 6247	 type: long */
#define	TA_CURSERVERS	((FLDID32)33560680)	/* number: 6248	 type: long */
#define	TA_CURSERVICES	((FLDID32)33560681)	/* number: 6249	 type: long */
#define	TA_CURSTYPE	((FLDID32)33560682)	/* number: 6250	 type: long */
#define	TA_CURTIME	((FLDID32)33560683)	/* number: 6251	 type: long */
#define	TA_CURTYPE	((FLDID32)33560684)	/* number: 6252	 type: long */
#define	TA_CURWSCLIENTS	((FLDID32)33560685)	/* number: 6253	 type: long */
#define	TA_DBBLWAIT	((FLDID32)33560686)	/* number: 6254	 type: long */
#define	TA_DEBUG	((FLDID32)167778415)	/* number: 6255	 type: string */
#define	TA_DEVICE	((FLDID32)167778416)	/* number: 6256	 type: string */
#define	TA_DEVINDEX	((FLDID32)33560689)	/* number: 6257	 type: long */
#define	TA_DEVOFFSET	((FLDID32)33560690)	/* number: 6258	 type: long */
#define	TA_DEVSIZE	((FLDID32)33560691)	/* number: 6259	 type: long */
#define	TA_DOMAINID	((FLDID32)167778420)	/* number: 6260	 type: string */
#define	TA_ENDTIME	((FLDID32)33560693)	/* number: 6261	 type: long */
#define	TA_ENVFILE	((FLDID32)167778422)	/* number: 6262	 type: string */
#define	TA_FIELD	((FLDID32)167778423)	/* number: 6263	 type: string */
#define	TA_FLOWCNT	((FLDID32)33560696)	/* number: 6264	 type: long */
#define	TA_FREEMAPAVAIL	((FLDID32)33560697)	/* number: 6265	 type: long */
#define	TA_FREEMAPCOUNT	((FLDID32)33560698)	/* number: 6266	 type: long */
#define	TA_FREEMAPINDEX	((FLDID32)33560699)	/* number: 6267	 type: long */
#define	TA_FREEMAPOFFSET	((FLDID32)33560700)	/* number: 6268	 type: long */
#define	TA_FREEMAPSIZE	((FLDID32)33560701)	/* number: 6269	 type: long */
#define	TA_GENERATION	((FLDID32)33560702)	/* number: 6270	 type: long */
#define	TA_GID	((FLDID32)33560703)	/* number: 6271	 type: long */
#define	TA_GRACE	((FLDID32)33560704)	/* number: 6272	 type: long */
#define	TA_GRPCOUNT	((FLDID32)33560705)	/* number: 6273	 type: long */
#define	TA_GRPINDEX	((FLDID32)33560706)	/* number: 6274	 type: long */
#define	TA_GRPNO	((FLDID32)33560707)	/* number: 6275	 type: long */
#define	TA_GSTATE	((FLDID32)167778436)	/* number: 6276	 type: string */
#define	TA_GTRID	((FLDID32)167778437)	/* number: 6277	 type: string */
#define	TA_HITICKET	((FLDID32)33560710)	/* number: 6278	 type: long */
#define	TA_HWACCESSERS	((FLDID32)33560711)	/* number: 6279	 type: long */
#define	TA_HWCLIENTS	((FLDID32)33560712)	/* number: 6280	 type: long */
#define	TA_HWCONV	((FLDID32)33560713)	/* number: 6281	 type: long */
#define	TA_HWDRT	((FLDID32)33560714)	/* number: 6282	 type: long */
#define	TA_HWGROUPS	((FLDID32)33560715)	/* number: 6283	 type: long */
#define	TA_HWGTT	((FLDID32)33560716)	/* number: 6284	 type: long */
#define	TA_HWMACHINES	((FLDID32)33560717)	/* number: 6285	 type: long */
#define	TA_HWQUEUES	((FLDID32)33560718)	/* number: 6286	 type: long */
#define	TA_HWRFT	((FLDID32)33560719)	/* number: 6287	 type: long */
#define	TA_HWRTDATA	((FLDID32)33560720)	/* number: 6288	 type: long */
#define	TA_HWSERVERS	((FLDID32)33560721)	/* number: 6289	 type: long */
#define	TA_HWSERVICES	((FLDID32)33560722)	/* number: 6290	 type: long */
#define	TA_HWWSCLIENTS	((FLDID32)33560723)	/* number: 6291	 type: long */
#define	TA_IPCKEY	((FLDID32)33560724)	/* number: 6292	 type: long */
#define	TA_ITERATION	((FLDID32)33560725)	/* number: 6293	 type: long */
#define	TA_LASTGRP	((FLDID32)33560726)	/* number: 6294	 type: long */
#define	TA_LDBAL	((FLDID32)167778455)	/* number: 6295	 type: string */
#define	TA_LICEXPIRE	((FLDID32)167778456)	/* number: 6296	 type: string */
#define	TA_LICMAXUSERS	((FLDID32)33560729)	/* number: 6297	 type: long */
#define	TA_LICSERIAL	((FLDID32)167778458)	/* number: 6298	 type: string */
#define	TA_LOAD	((FLDID32)33560731)	/* number: 6299	 type: long */
#define	TA_MASTER	((FLDID32)167778460)	/* number: 6300	 type: string */
#define	TA_MAX	((FLDID32)33560733)	/* number: 6301	 type: long */
#define	TA_MAXACCESSERS	((FLDID32)33560734)	/* number: 6302	 type: long */
#define	TA_MAXBUFSTYPE	((FLDID32)33560735)	/* number: 6303	 type: long */
#define	TA_MAXBUFTYPE	((FLDID32)33560736)	/* number: 6304	 type: long */
#define	TA_MAXCONV	((FLDID32)33560737)	/* number: 6305	 type: long */
#define	TA_MAXDRT	((FLDID32)33560738)	/* number: 6306	 type: long */
#define	TA_MAXGEN	((FLDID32)33560739)	/* number: 6307	 type: long */
#define	TA_MAXGROUPS	((FLDID32)33560740)	/* number: 6308	 type: long */
#define	TA_MAXGTT	((FLDID32)33560741)	/* number: 6309	 type: long */
#define	TA_MAXMACHINES	((FLDID32)33560742)	/* number: 6310	 type: long */
#define	TA_MAXMTYPE	((FLDID32)33560743)	/* number: 6311	 type: long */
#define	TA_MAXQUEUES	((FLDID32)33560744)	/* number: 6312	 type: long */
#define	TA_MAXRFT	((FLDID32)33560745)	/* number: 6313	 type: long */
#define	TA_MAXRTDATA	((FLDID32)33560746)	/* number: 6314	 type: long */
#define	TA_MAXSERVERS	((FLDID32)33560747)	/* number: 6315	 type: long */
#define	TA_MAXSERVICES	((FLDID32)33560748)	/* number: 6316	 type: long */
#define	TA_MAXWSCLIENTS	((FLDID32)33560749)	/* number: 6317	 type: long */
#define	TA_MIBMASK	((FLDID32)33560750)	/* number: 6318	 type: long */
#define	TA_MIN	((FLDID32)33560751)	/* number: 6319	 type: long */
#define	TA_MINOR	((FLDID32)33560752)	/* number: 6320	 type: long */
#define	TA_MMDDYY	((FLDID32)33560753)	/* number: 6321	 type: long */
#define	TA_MODEL	((FLDID32)167778482)	/* number: 6322	 type: string */
#ifndef REDUCE_CPP_NOIPC
#define	TA_MSGID	((FLDID32)33560755)	/* number: 6323	 type: long */
#define	TA_MSG_CBYTES	((FLDID32)33560756)	/* number: 6324	 type: long */
#define	TA_MSG_CTIME	((FLDID32)33560757)	/* number: 6325	 type: long */
#define	TA_MSG_LRPID	((FLDID32)33560758)	/* number: 6326	 type: long */
#define	TA_MSG_LSPID	((FLDID32)33560759)	/* number: 6327	 type: long */
#define	TA_MSG_QBYTES	((FLDID32)33560760)	/* number: 6328	 type: long */
#define	TA_MSG_QNUM	((FLDID32)33560761)	/* number: 6329	 type: long */
#define	TA_MSG_RTIME	((FLDID32)33560762)	/* number: 6330	 type: long */
#define	TA_MSG_STIME	((FLDID32)33560763)	/* number: 6331	 type: long */
#endif
#define	TA_NADDR	((FLDID32)167778492)	/* number: 6332	 type: string */
#define	TA_NCOMPLETED	((FLDID32)33560765)	/* number: 6333	 type: long */
#define	TA_NLSADDR	((FLDID32)167778494)	/* number: 6334	 type: string */
#define	TA_NOTIFY	((FLDID32)167778495)	/* number: 6335	 type: string */
#define	TA_NQUEUED	((FLDID32)33560768)	/* number: 6336	 type: long */
#define	TA_NUMCONV	((FLDID32)33560769)	/* number: 6337	 type: long */
#define	TA_NUMDEQUEUE	((FLDID32)33560770)	/* number: 6338	 type: long */
#define	TA_NUMENQUEUE	((FLDID32)33560771)	/* number: 6339	 type: long */
#define	TA_NUMPOST	((FLDID32)33560772)	/* number: 6340	 type: long */
#define	TA_NUMREQ	((FLDID32)33560773)	/* number: 6341	 type: long */
/* TA_NUMSEMWKUP was used previously by locking code - do no use offset 142 */
#define	TA_NUMSERVED	((FLDID32)33560775)	/* number: 6343	 type: long */
#define	TA_NUMSUBSCRIBE	((FLDID32)33560776)	/* number: 6344	 type: long */
#define	TA_NUMTRAN	((FLDID32)33560777)	/* number: 6345	 type: long */
#define	TA_NUMTRANABT	((FLDID32)33560778)	/* number: 6346	 type: long */
#define	TA_NUMTRANCMT	((FLDID32)33560779)	/* number: 6347	 type: long */
#define	TA_NUMUNSOL	((FLDID32)33560780)	/* number: 6348	 type: long */
#define	TA_OPENINFO	((FLDID32)167778509)	/* number: 6349	 type: string */
#define	TA_OPTIONS	((FLDID32)167778510)	/* number: 6350	 type: string */
#define	TA_PAGESIZE	((FLDID32)33560783)	/* number: 6351	 type: long */
#define	TA_PID	((FLDID32)33560784)	/* number: 6352	 type: long */
#define	TA_PMID	((FLDID32)167778513)	/* number: 6353	 type: string */
#define	TA_PRIO	((FLDID32)33560786)	/* number: 6354	 type: long */
#define	TA_RANGES	((FLDID32)201332947)	/* number: 6355	 type: carray */
#define	TA_RCMD	((FLDID32)167778516)	/* number: 6356	 type: string */
#define	TA_RCVDBYT	((FLDID32)33560789)	/* number: 6357	 type: long */
#define	TA_RCVDNUM	((FLDID32)33560790)	/* number: 6358	 type: long */
#define	TA_RELEASE	((FLDID32)33560791)	/* number: 6359	 type: long */
#define	TA_REPLYQ	((FLDID32)167778520)	/* number: 6360	 type: string */
#define	TA_RESTART	((FLDID32)167778521)	/* number: 6361	 type: string */
#define	TA_ROLE	((FLDID32)167778522)	/* number: 6362	 type: string */
#define	TA_ROUTINGNAME	((FLDID32)167778523)	/* number: 6363	 type: string */
#define	TA_RPID	((FLDID32)33560796)	/* number: 6364	 type: long */
#define	TA_RPPERM	((FLDID32)33560797)	/* number: 6365	 type: long */
#define	TA_RQADDR	((FLDID32)167778526)	/* number: 6366	 type: string */
#define	TA_RQID	((FLDID32)33560799)	/* number: 6367	 type: long */
#define	TA_RQPERM	((FLDID32)33560800)	/* number: 6368	 type: long */
#define	TA_SANITYSCAN	((FLDID32)33560801)	/* number: 6369	 type: long */
#define	TA_SCANUNIT	((FLDID32)33560802)	/* number: 6370	 type: long */
#define	TA_SECURITY	((FLDID32)167778531)	/* number: 6371	 type: string */
#ifndef REDUCE_CPP_NOIPC
#define	TA_SEMID	((FLDID32)33560804)	/* number: 6372	 type: long */
#define	TA_SEMTICKET	((FLDID32)33560805)	/* number: 6373	 type: long */
#define	TA_SEM_OTIME	((FLDID32)33560806)	/* number: 6374	 type: long */
#endif
#define	TA_SENTBYT	((FLDID32)33560807)	/* number: 6375	 type: long */
#define	TA_SENTNUM	((FLDID32)33560808)	/* number: 6376	 type: long */
#define	TA_SEQUENCE	((FLDID32)33560809)	/* number: 6377	 type: long */
#define	TA_SERVERCNT	((FLDID32)33560810)	/* number: 6378	 type: long */
#define	TA_SERVERNAME	((FLDID32)167778539)	/* number: 6379	 type: string */
#define	TA_SERVICENAME	((FLDID32)167778540)	/* number: 6380	 type: string */
#define	TA_SEVERITY	((FLDID32)167778541)	/* number: 6381	 type: string */
#ifndef REDUCE_CPP_NOIPC
#define	TA_SHMID	((FLDID32)33560814)	/* number: 6382	 type: long */
#define	TA_SHMKEY	((FLDID32)33560815)	/* number: 6383	 type: long */
#define	TA_SHMSZ	((FLDID32)33560816)	/* number: 6384	 type: long */
#define	TA_SHM_ATIME	((FLDID32)33560817)	/* number: 6385	 type: long */
#define	TA_SHM_CTIME	((FLDID32)33560818)	/* number: 6386	 type: long */
#define	TA_SHM_DTIME	((FLDID32)33560819)	/* number: 6387	 type: long */
#define	TA_SHM_NATTCH	((FLDID32)33560820)	/* number: 6388	 type: long */
#endif
#define	TA_SOURCE	((FLDID32)167778549)	/* number: 6389	 type: string */
#define	TA_SPINCOUNT	((FLDID32)33560822)	/* number: 6390	 type: long */
#define	TA_SUSPTIME	((FLDID32)33560823)	/* number: 6391	 type: long */
#define	TA_SVCRNAM	((FLDID32)167778552)	/* number: 6392	 type: string */
#define	TA_SVCTIMEOUT	((FLDID32)33560825)	/* number: 6393	 type: long */
#define	TA_SWRELEASE	((FLDID32)167778554)	/* number: 6394	 type: string */
#define	TA_SYSTEM_ACCESS	((FLDID32)167778555)	/* number: 6395	 type: string */
#define	TA_TIMELEFT	((FLDID32)33560828)	/* number: 6396	 type: long */
#define	TA_TIMEOUT	((FLDID32)33560829)	/* number: 6397	 type: long */
#define	TA_TIMERESTART	((FLDID32)33560830)	/* number: 6398	 type: long */
#define	TA_TIMESTART	((FLDID32)33560831)	/* number: 6399	 type: long */
#define	TA_TLOGCOUNT	((FLDID32)33560832)	/* number: 6400	 type: long */
#define	TA_TLOGDATA	((FLDID32)167778561)	/* number: 6401	 type: string */
#define	TA_TLOGDEVICE	((FLDID32)167778562)	/* number: 6402	 type: string */
#define	TA_TLOGINDEX	((FLDID32)33560835)	/* number: 6403	 type: long */
#define	TA_TLOGNAME	((FLDID32)167778564)	/* number: 6404	 type: string */
#define	TA_TLOGOFFSET	((FLDID32)33560837)	/* number: 6405	 type: long */
#define	TA_TLOGSIZE	((FLDID32)33560838)	/* number: 6406	 type: long */
#define	TA_TMDEBUG	((FLDID32)167778567)	/* number: 6407	 type: string */
#define	TA_TMNETLOAD	((FLDID32)33560840)	/* number: 6408	 type: long */
#define	TA_TMSCOUNT	((FLDID32)33560841)	/* number: 6409	 type: long */
#define	TA_TMSNAME	((FLDID32)167778570)	/* number: 6410	 type: string */
#define	TA_TOTNP	((FLDID32)33560843)	/* number: 6411	 type: long */
#define	TA_TOTNQUEUED	((FLDID32)33560844)	/* number: 6412	 type: long */
#define	TA_TOTNV	((FLDID32)33560845)	/* number: 6413	 type: long */
#define	TA_TOTREQC	((FLDID32)33560846)	/* number: 6414	 type: long */
#define	TA_TOTWANTERS	((FLDID32)33560847)	/* number: 6415	 type: long */
#define	TA_TOTWKQUEUED	((FLDID32)33560848)	/* number: 6416	 type: long */
#define	TA_TOTWKUPRCV	((FLDID32)33560849)	/* number: 6417	 type: long */
#define	TA_TOTWKUPSENT	((FLDID32)33560850)	/* number: 6418	 type: long */
#define	TA_TOTWORKL	((FLDID32)33560851)	/* number: 6419	 type: long */
#define	TA_TPTRANID	((FLDID32)167778580)	/* number: 6420	 type: string */
#define	TA_TRANLEV	((FLDID32)33560853)	/* number: 6421	 type: long */
#define	TA_TRANTIME	((FLDID32)33560854)	/* number: 6422	 type: long */
#define	TA_TUXCONFIG	((FLDID32)167778583)	/* number: 6423	 type: string */
#define	TA_TUXDIR	((FLDID32)167778584)	/* number: 6424	 type: string */
#define	TA_TUXOFFSET	((FLDID32)33560857)	/* number: 6425	 type: long */
#define	TA_TYPE	((FLDID32)167778586)	/* number: 6426	 type: string */
#define	TA_UID	((FLDID32)33560859)	/* number: 6427	 type: long */
#define	TA_ULOGCAT	((FLDID32)167778588)	/* number: 6428	 type: string */
#define	TA_ULOGLINE	((FLDID32)33560861)	/* number: 6429	 type: long */
#define	TA_ULOGMSG	((FLDID32)167778590)	/* number: 6430	 type: string */
#define	TA_ULOGMSGNUM	((FLDID32)33560863)	/* number: 6431	 type: long */
#define	TA_ULOGPFX	((FLDID32)167778592)	/* number: 6432	 type: string */
#define	TA_ULOGPROCNM	((FLDID32)167778593)	/* number: 6433	 type: string */
#define	TA_ULOGTIME	((FLDID32)33560866)	/* number: 6434	 type: long */
#define	TA_USIGNAL	((FLDID32)167778595)	/* number: 6435	 type: string */
#define	TA_USRNAME	((FLDID32)167778596)	/* number: 6436	 type: string */
#define	TA_VALIDATION	((FLDID32)167778597)	/* number: 6437	 type: string */
#define	TA_WAITS	((FLDID32)167778598)	/* number: 6438	 type: string */
#define	TA_WKCOMPLETED	((FLDID32)33560871)	/* number: 6439	 type: long */
#define	TA_WKINITIATED	((FLDID32)33560872)	/* number: 6440	 type: long */
#define	TA_WKQUEUED	((FLDID32)33560873)	/* number: 6441	 type: long */
#define	TA_WSC	((FLDID32)167778602)	/* number: 6442	 type: string */
#define	TA_WSH	((FLDID32)167778603)	/* number: 6443	 type: string */
#define	TA_XID	((FLDID32)167778604)	/* number: 6444	 type: string */
/* ------------------------------------------------------------
 * The field numbers below have been added to the system with release 6.0.
 * They begin at 245 (base 6200) and increase from there.
 * Field numbers cannot be reused or changed from release to release or
 * interoperability will be broken.  Fields 0-244 (base 6200) defined in
 * release 5.0, fields defined in release 6.0 begin at field 245 and
 * increase from there.
 *
 * Note that fields 7200-7299 are reserved for use by TUXEDO OEMs.
 */
#define	TA_ACLCACHEACCESS	((FLDID32)33560877)	/* number: 6445	 type: long */
#define	TA_ACLCACHEHITS	((FLDID32)33560878)	/* number: 6446	 type: long */
#define	TA_ACLFAIL	((FLDID32)33560879)	/* number: 6447	 type: long */
#define	TA_ACLGROUPIDS	((FLDID32)167778608)	/* number: 6448	 type: string */
#define	TA_ACLNAME	((FLDID32)167778609)	/* number: 6449	 type: string */
#define	TA_ACLTYPE	((FLDID32)167778610)	/* number: 6450	 type: string */
#define	TA_ACTIVE	((FLDID32)167778611)	/* number: 6451	 type: string */
#define	TA_ATTFLAGS	((FLDID32)33560884)	/* number: 6452	 type: long */
#define	TA_CURHANDLERS	((FLDID32)33560885)	/* number: 6453	 type: long */
#define	TA_CURWORK	((FLDID32)33560886)	/* number: 6454	 type: long */
#define	TA_DEFAULT	((FLDID32)167778615)	/* number: 6455	 type: string */
#define	TA_FACTPERM	((FLDID32)33560888)	/* number: 6456	 type: long */
#define	TA_GETSTATES	((FLDID32)167778617)	/* number: 6457	 type: string */
#define	TA_GROUPID	((FLDID32)33560890)	/* number: 6458	 type: long */
#define	TA_GROUPNAME	((FLDID32)167778619)	/* number: 6459	 type: string */
#define	TA_HWACLCACHE	((FLDID32)33560892)	/* number: 6460	 type: long */
#define	TA_HWHANDLERS	((FLDID32)33560893)	/* number: 6461	 type: long */
#define	TA_IDLETIME	((FLDID32)33560894)	/* number: 6462	 type: long */
#define	TA_INASTATES	((FLDID32)167778623)	/* number: 6463	 type: string */
#define	TA_MAXACLCACHE	((FLDID32)33560896)	/* number: 6464	 type: long */
#define	TA_MAXACLGROUPS	((FLDID32)33560897)	/* number: 6465	 type: long */
#define	TA_MAXHANDLERS	((FLDID32)33560898)	/* number: 6466	 type: long */
#define	TA_MAXIDLETIME	((FLDID32)33560899)	/* number: 6467	 type: long */
#define	TA_MAXINITTIME	((FLDID32)33560900)	/* number: 6468	 type: long */
#define	TA_MAXPERM	((FLDID32)33560901)	/* number: 6469	 type: long */
#define	TA_MINHANDLERS	((FLDID32)33560902)	/* number: 6470	 type: long */
#define	TA_MULTIPLEX	((FLDID32)33560903)	/* number: 6471	 type: long */
#define	TA_NUMBLOCKQ	((FLDID32)33560904)	/* number: 6472	 type: long */
#define	TA_PREFERENCES	((FLDID32)167778633)	/* number: 6473	 type: string */
#define	TA_PRINCLTNAME	((FLDID32)167778634)	/* number: 6474	 type: string */
#define	TA_PRINGRP	((FLDID32)33560907)	/* number: 6475	 type: long */
#define	TA_PRINID	((FLDID32)33560908)	/* number: 6476	 type: long */
#define	TA_PRINNAME	((FLDID32)167778637)	/* number: 6477	 type: string */
#define	TA_PRINPASSWD	((FLDID32)167778638)	/* number: 6478	 type: string */
#define	TA_SETSTATES	((FLDID32)167778639)	/* number: 6479	 type: string */
#define	TA_SUSPENDED	((FLDID32)167778640)	/* number: 6480	 type: string */
#define	TA_SVCTYPE	((FLDID32)167778641)	/* number: 6481	 type: string */
#define	TA_TOTACTTIME	((FLDID32)33560914)	/* number: 6482	 type: long */
#define	TA_TOTIDLTIME	((FLDID32)33560915)	/* number: 6483	 type: long */
#define	TA_VIEWREFRESH	((FLDID32)167778644)	/* number: 6484	 type: string */
#define	TA_WSHCLIENTID	((FLDID32)167778645)	/* number: 6485	 type: string */
#define	TA_WSHNAME	((FLDID32)167778646)	/* number: 6486	 type: string */
#define	TA_WSPROTO	((FLDID32)33560919)	/* number: 6487	 type: long */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 6.3
 */
#define	TA_COORDGRPNO	((FLDID32)33560920)	/* number: 6488	 type: long */
#define	TA_COORDSRVGRP	((FLDID32)167778649)	/* number: 6489	 type: string */
#define	TA_MINWSHPORT	((FLDID32)33560922)	/* number: 6490	 type: long */
#define	TA_MAXWSHPORT	((FLDID32)33560923)	/* number: 6491	 type: long */
#define	TA_MINENCRYPTBITS	((FLDID32)167778652)	/* number: 6492	 type: string */
#define	TA_MAXENCRYPTBITS	((FLDID32)167778653)	/* number: 6493	 type: string */
#define	TA_CURENCRYPTBITS	((FLDID32)167778654)	/* number: 6494	 type: string */
#define	TA_EXT_NADDR	((FLDID32)167778655)	/* number: 6495	 type: string */
#define	TA_COMPONENTS	((FLDID32)167778656)	/* number: 6496	 type: string */
#define	TA_OLDENCRYPT	((FLDID32)167778657)	/* number: 6497	 type: string */
#define	TA_OLDCMPLIMIT	((FLDID32)167778658)	/* number: 6498	 type: string */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 6.4
 */
#define	TA_MAXPENDINGBYTES	((FLDID32)33560931)	/* number: 6499	 type: long */
#define	TA_NETGROUP	((FLDID32)167778660)	/* number: 6500	 type: string */
#define	TA_NETGRPNO	((FLDID32)33560933)	/* number: 6501	 type: long */
#define	TA_NETPRIO	((FLDID32)33560934)	/* number: 6502	 type: long */
#define	TA_MAXNETGROUPS	((FLDID32)33560935)	/* number: 6503	 type: long */
#define	TA_KEEPALIVE	((FLDID32)167778664)	/* number: 6504	 type: string */
#define	TA_NETTIMEOUT	((FLDID32)33560937)	/* number: 6505	 type: long */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 6.5
 */
#define	TA_CURINTERFACE	((FLDID32)167778666)	/* number: 6506	 type: string */
#define	TA_CUROBJECTS	((FLDID32)33560939)	/* number: 6507	 type: long */
#define	TA_CURTRANSACTIONS	((FLDID32)33560940)	/* number: 6508	 type: long */
#define	TA_INTERFACENAME	((FLDID32)167778669)	/* number: 6509	 type: string */
#define	TA_MAXINTERFACES	((FLDID32)33560942)	/* number: 6510	 type: long */
#define	TA_MAXOBJECTS	((FLDID32)33560943)	/* number: 6511	 type: long */
#define	TA_NUMSERVERS	((FLDID32)33560944)	/* number: 6512	 type: long */
#define	TA_TPPOLICY	((FLDID32)167778673)	/* number: 6513	 type: string */
#define	TA_TXPOLICY	((FLDID32)167778674)	/* number: 6514	 type: string */
#define	TA_HWOBJECTS	((FLDID32)33560947)	/* number: 6515	 type: long */
#define	TA_FBROUTINGNAME	((FLDID32)167778676)	/* number: 6516	 type: string */
#define	TA_CURINTERFACES	((FLDID32)33560949)	/* number: 6517	 type: long */
#define	TA_HWINTERFACES	((FLDID32)33560950)	/* number: 6518	 type: long */
#define	TA_FACTORYID	((FLDID32)167778679)	/* number: 6519	 type: string */
#define	TA_FIELDTYPE	((FLDID32)167778680)	/* number: 6520	 type: string */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 7.1
 */
#define	TA_SIGNATURE_AHEAD	((FLDID32)33560953)	/* number: 6521	 type: long */
#define	TA_SIGNATURE_BEHIND	((FLDID32)33560954)	/* number: 6522	 type: long */
#define	TA_SIGNATURE_REQUIRED	((FLDID32)167778683)	/* number: 6523	 type: string */
#define	TA_ENCRYPTION_REQUIRED	((FLDID32)167778684)	/* number: 6524	 type: string */
#define	TA_THREADID	((FLDID32)33560957)	/* number: 6525	 type: long */
#define	TA_CONTEXTID	((FLDID32)33560958)	/* number: 6526	 type: long */
#define	TA_MAXDISPATCHTHREADS	((FLDID32)33560959)	/* number: 6527	 type: long */
#define	TA_MINDISPATCHTHREADS	((FLDID32)33560960)	/* number: 6528	 type: long */
#define	TA_THREADSTACKSIZE	((FLDID32)33560961)	/* number: 6529	 type: long */
#define	TA_CURDISPATCHTHREADS	((FLDID32)33560962)	/* number: 6530	 type: long */
#define	TA_HWDISPATCHTHREADS	((FLDID32)33560963)	/* number: 6531	 type: long */
#define	TA_NUMDISPATCHTHREADS	((FLDID32)33560964)	/* number: 6532	 type: long */
#define	TA_SEC_PRINCIPAL_NAME	((FLDID32)167778693)	/* number: 6533	 type: string */
#define	TA_SEC_PRINCIPAL_LOCATION	((FLDID32)167778694)	/* number: 6534	 type: string */
#define	TA_SEC_PRINCIPAL_PASSVAR	((FLDID32)167778695)	/* number: 6535	 type: string */
#define	TA_SEC_PRINCIPAL_PASSWORD	((FLDID32)167778696)	/* number: 6536	 type: string */
#define	TA_FADDR	((FLDID32)167778697)	/* number: 6537	 type: string */
#define	TA_FRANGE	((FLDID32)33560970)	/* number: 6538	 type: long */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 7.2
 */
#define	TA_SICACHEENTRIESMAX	((FLDID32)167778699)	/* number: 6539	 type: string */
#define	TA_MAXQUEUELEN	((FLDID32)33560972)	/* number: 6540	 type: long */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 9.0
 */
#define	TA_TPBLK_ALL	((FLDID32)33560973)	/* number: 6541	 type: long */
#define	TA_RTPOLICY	((FLDID32)167778702)	/* number: 6542	 type: string */
#define	TA_BUFTYPECONV	((FLDID32)167778703)	/* number: 6543	 type: string */
/* ------------------------------------------------------------
 * The fields below have been added to TMIB beginning release 10.0
 */
#define	TA_SSL_RENEGOTIATION	((FLDID32)33560976)	/* number: 6544	 type: long */
#define	TA_SECUREPORT	((FLDID32)33560977)	/* number: 6545	 type: long */
#define	TA_CERTIFICATE_AUTHENTICATION	((FLDID32)167778706)	/* number: 6546	 type: string */
/*
       The fields below have been added for security enhancement
*/
#define	TA_ACLACTION	((FLDID32)167778707)	/* number: 6547	 type: string */
#define	TA_USER_CREDENTIAL	((FLDID32)167778708)	/* number: 6548	 type: string */
/*
       The fields below have been added for T_ULOG enhancement
*/
#define	TA_BACKWARDS	((FLDID32)33560981)	/* number: 6549	 type: long */
/*
	The field numbers below have been added to the system with release 12cR2.
	They have been introduced by bypass domain feature.
*/
#define	TA_BYPASSDOM_ID	((FLDID32)167778710)	/* number: 6550	 type: string */
#define	TA_BYPASSDOM_SEQ_NUM	((FLDID32)33560983)	/* number: 6551	 type: long */
#define	TA_BYPASSDOM_SHARED_DIR	((FLDID32)167778712)	/* number: 6552	 type: string */
#define	TA_MAXDOMAINS	((FLDID32)33560985)	/* number: 6553	 type: long */
#define	TA_RM_ERR_THRESHOLD	((FLDID32)33560986)	/* number: 6554	 type: long */
#define	TA_RM_ERR_INTERVAL	((FLDID32)33560987)	/* number: 6555	 type: long */
/*
 * The field numbers below have been added to the system with release 12.2.2
 * They have been introduced by opss audit feature.
 */
#define	TA_AUD_EVENTTYPE	((FLDID32)167778714)	/* number: 6554	 type: string */
#define	TA_AUD_VISITORID	((FLDID32)167778715)	/* number: 6555	 type: string */
#define	TA_AUD_MSG	((FLDID32)167778716)	/* number: 6556	 type: string */
#define	TA_AUD_STATUS	((FLDID32)167778717)	/* number: 6557	 type: string */
#define	TA_AUD_STARTTIME	((FLDID32)167778718)	/* number: 6558	 type: string */
#define	TA_AUD_ENDTIME	((FLDID32)167778719)	/* number: 6559	 type: string */
/* end of this 
*/
/*
 * The last entry in this section must be 154 ( 6445 + 155 = 6600 )
 */
#ifndef REDUCE_CPP_NOQMIB
/* ------------------------------------------------------------
 * The fields below belong to the /Q MIB.
 */
#define	TA_APPQMSGID	((FLDID32)167778760)	/* number: 6600	 type: string */
#define	TA_APPQNAME	((FLDID32)167778761)	/* number: 6601	 type: string */
#define	TA_APPQORDER	((FLDID32)167778762)	/* number: 6602	 type: string */
#define	TA_APPQSPACENAME	((FLDID32)167778763)	/* number: 6603	 type: string */
#define	TA_APPQSPACERM	((FLDID32)167778764)	/* number: 6604	 type: string */
#define	TA_BLOCKING	((FLDID32)33561037)	/* number: 6605	 type: long */
#define	TA_CMD	((FLDID32)167778766)	/* number: 6606	 type: string */
#define	TA_CMDHW	((FLDID32)167778767)	/* number: 6607	 type: string */
#define	TA_CMDLW	((FLDID32)167778768)	/* number: 6608	 type: string */
#define	TA_CORRID	((FLDID32)167778769)	/* number: 6609	 type: string */
#define	TA_CURBLOCKS	((FLDID32)33561042)	/* number: 6610	 type: long */
#define	TA_CUREXTENT	((FLDID32)33561043)	/* number: 6611	 type: long */
#define	TA_CURMSG	((FLDID32)33561044)	/* number: 6612	 type: long */
#define	TA_CURPROC	((FLDID32)33561045)	/* number: 6613	 type: long */
#define	TA_CURRETRIES	((FLDID32)33561046)	/* number: 6614	 type: long */
#define	TA_CURTRANS	((FLDID32)33561047)	/* number: 6615	 type: long */
#define	TA_ERRORQNAME	((FLDID32)167778776)	/* number: 6616	 type: string */
#define	TA_FORCEINIT	((FLDID32)167778777)	/* number: 6617	 type: string */
#define	TA_HIGHPRIORITY	((FLDID32)33561050)	/* number: 6618	 type: long */
#define	TA_HWMSG	((FLDID32)33561051)	/* number: 6619	 type: long */
#define	TA_HWPROC	((FLDID32)33561052)	/* number: 6620	 type: long */
#define	TA_HWTRANS	((FLDID32)33561053)	/* number: 6621	 type: long */
#define	TA_LOWPRIORITY	((FLDID32)33561054)	/* number: 6622	 type: long */
#define	TA_LSTATE	((FLDID32)167778783)	/* number: 6623	 type: string */
#define	TA_MAXMSG	((FLDID32)33561056)	/* number: 6624	 type: long */
#define	TA_MAXPAGES	((FLDID32)33561057)	/* number: 6625	 type: long */
#define	TA_MAXPROC	((FLDID32)33561058)	/* number: 6626	 type: long */
#define	TA_MAXRETRIES	((FLDID32)33561059)	/* number: 6627	 type: long */
#define	TA_MAXTRANS	((FLDID32)33561060)	/* number: 6628	 type: long */
#define	TA_MSGENDTIME	((FLDID32)167778789)	/* number: 6629	 type: string */
#define	TA_MSGSIZE	((FLDID32)33561062)	/* number: 6630	 type: long */
#define	TA_MSGSTARTTIME	((FLDID32)167778791)	/* number: 6631	 type: string */
#define	TA_NEWAPPQNAME	((FLDID32)167778792)	/* number: 6632	 type: string */
#define	TA_OUTOFORDER	((FLDID32)167778793)	/* number: 6633	 type: string */
#define	TA_PERCENTINIT	((FLDID32)33561066)	/* number: 6634	 type: long */
#define	TA_PRIORITY	((FLDID32)33561067)	/* number: 6635	 type: long */
#define	TA_QMCONFIG	((FLDID32)167778796)	/* number: 6636	 type: string */
#define	TA_RETRYDELAY	((FLDID32)33561069)	/* number: 6637	 type: long */
#define	TA_STARTTIME	((FLDID32)167778798)	/* number: 6638	 type: string */
#define	TA_TIME	((FLDID32)167778799)	/* number: 6639	 type: string */
/*
 * The fields below have been added to APPMIB beginning release 7.1
 */
#define	TA_CONTEXT_ID	((FLDID32)33561072)	/* number: 6640	 type: long */
/*
 * New APPQ fileds
 */
#define	TA_CMDNONPERSIST	((FLDID32)167778801)	/* number: 6641	 type: string */
#define	TA_CMDNONPERSISTHW	((FLDID32)167778802)	/* number: 6642	 type: string */
#define	TA_CMDNONPERSISTLW	((FLDID32)167778803)	/* number: 6643	 type: string */
#define	TA_CURNONPERSISTBYTES	((FLDID32)33561076)	/* number: 6644	 type: long */
#define	TA_CURNONPERSISTMSG	((FLDID32)33561077)	/* number: 6645	 type: long */
#define	TA_DEFDELIVERYPOLICY	((FLDID32)167778806)	/* number: 6646	 type: string */
#define	TA_DEFEXPIRATIONTIME	((FLDID32)167778807)	/* number: 6647	 type: string */
/*
	New APPQMSG fileds
*/
#define	TA_EXPIRETIME	((FLDID32)167778808)	/* number: 6648	 type: string */
#define	TA_MSGEXPIREENDTIME	((FLDID32)167778809)	/* number: 6649	 type: string */
#define	TA_MSGEXPIRESTARTTIME	((FLDID32)167778810)	/* number: 6650	 type: string */
#define	TA_PERSISTENCE	((FLDID32)167778811)	/* number: 6651	 type: string */
#define	TA_REPLYPERSISTENCE	((FLDID32)167778812)	/* number: 6652	 type: string */
/*
	New APPQSPACE fileds
*/
#define	TA_CURACTIONS	((FLDID32)33561085)	/* number: 6653	 type: long */
#define	TA_CURCURSORS	((FLDID32)33561086)	/* number: 6654	 type: long */
#define	TA_CURHANDLES	((FLDID32)33561087)	/* number: 6655	 type: long */
#define	TA_CURMEMFILTERS	((FLDID32)33561088)	/* number: 6656	 type: long */
#define	TA_CURMEMNONPERSIST	((FLDID32)33561089)	/* number: 6657	 type: long */
#define	TA_CURMEMOVERFLOW	((FLDID32)33561090)	/* number: 6658	 type: long */
#define	TA_CUROWNERS	((FLDID32)33561091)	/* number: 6659	 type: long */
#define	TA_CURTMPQUEUES	((FLDID32)33561092)	/* number: 6660	 type: long */
#define	TA_HWACTIONS	((FLDID32)33561093)	/* number: 6661	 type: long */
#define	TA_HWCURSORS	((FLDID32)33561094)	/* number: 6662	 type: long */
#define	TA_HWHANDLES	((FLDID32)33561095)	/* number: 6663	 type: long */
#define	TA_HWMEMFILTERS	((FLDID32)33561096)	/* number: 6664	 type: long */
#define	TA_HWMEMNONPERSIST	((FLDID32)33561097)	/* number: 6665	 type: long */
#define	TA_HWMEMOVERFLOW	((FLDID32)33561098)	/* number: 6666	 type: long */
#define	TA_HWOWNERS	((FLDID32)33561099)	/* number: 6667	 type: long */
#define	TA_HWTMPQUEUES	((FLDID32)33561100)	/* number: 6668	 type: long */
#define	TA_MAXACTIONS	((FLDID32)33561101)	/* number: 6669	 type: long */
#define	TA_MAXCURSORS	((FLDID32)33561102)	/* number: 6670	 type: long */
#define	TA_MAXHANDLES	((FLDID32)33561103)	/* number: 6671	 type: long */
#define	TA_MAXOWNERS	((FLDID32)33561104)	/* number: 6672	 type: long */
#define	TA_MAXTMPQUEUES	((FLDID32)33561105)	/* number: 6673	 type: long */
#define	TA_MEMFILTERS	((FLDID32)33561106)	/* number: 6674	 type: long */
#define	TA_MEMNONPERSIST	((FLDID32)167778835)	/* number: 6675	 type: string */
#define	TA_MEMOVERFLOW	((FLDID32)33561108)	/* number: 6676	 type: long */
#define	TA_MEMSYSTEMRESERVED	((FLDID32)33561109)	/* number: 6677	 type: long */
#define	TA_MEMTOTALALLOCATED	((FLDID32)33561110)	/* number: 6678	 type: long */
/* ------------------------------------------------------------
 * New OTMQ fileds
 */
#define	TA_FIRSTTMPQ	((FLDID32)33561111)	/* number: 6679	 type: long */
#define	TA_MSGSENT	((FLDID32)33561112)	/* number: 6680	 type: long */
#define	TA_MSGRCV	((FLDID32)33561113)	/* number: 6681	 type: long */
#define	TA_OTMQALIAS	((FLDID32)167778842)	/* number: 6682	 type: string */
#define	TA_OTMQSPACETARGET	((FLDID32)167778843)	/* number: 6683	 type: string */
#define	TA_OTMQTARGET	((FLDID32)167778844)	/* number: 6684	 type: string */
#define	TA_OTMQALIASSCOPE	((FLDID32)167778845)	/* number: 6685	 type: string */
#define	TA_CONFIRM	((FLDID32)167778846)	/* number: 6686	 type: string */
#define	TA_TATTACH	((FLDID32)167778847)	/* number: 6687	 type: string */
#define	TA_TDETACH	((FLDID32)167778848)	/* number: 6688	 type: string */
#define	TA_ATTACHSTATE	((FLDID32)167778849)	/* number: 6689	 type: string */
#define	TA_ACTIVESTATE	((FLDID32)167778850)	/* number: 6690	 type: string */
#define	TA_QUETYPE	((FLDID32)167778851)	/* number: 6691	 type: string */
#define	TA_PRIMQUE	((FLDID32)167778852)	/* number: 6692	 type: string */
#define	TA_MRSJRNSAF	((FLDID32)167778853)	/* number: 6693	 type: string */
#define	TA_MRSJRNDQF	((FLDID32)167778854)	/* number: 6694	 type: string */
#define	TA_MRSJRNDLJ	((FLDID32)167778855)	/* number: 6695	 type: string */
#define	TA_MRSJRNPCJ	((FLDID32)167778856)	/* number: 6696	 type: string */
#endif
#endif
/* ------------------------------------------------------------
 * The field numbers below should always begin at 500 and increase.
 * Field numbers cannot be reused or changed from release to release or
 * interoperability will be broken.
 *
 * Note that fields 7200-7299 are reserved for use by TUXEDO OEMs.
 */
#define	TA_LMID	((FLDID32)167778860)	/* number: 6700	 type: string */
#define	TA_PASSWORD	((FLDID32)167778861)	/* number: 6701	 type: string */
#define	TA_SRVGRP	((FLDID32)167778862)	/* number: 6702	 type: string */
#define	TA_SRVID	((FLDID32)33561135)	/* number: 6703	 type: long */
#define	TA_TMTRACE	((FLDID32)167778864)	/* number: 6704	 type: string */
#define	TA_TMMONITOR	((FLDID32)167778865)	/* number: 6705	 type: string */
/* ------------------------------------------------------------
 * The field add for T_RMS
 */
#define	TA_RMID	((FLDID32)33561138)	/* number: 6706	 type: long */
#define	TA_RMSNAME	((FLDID32)167778867)	/* number: 6707	 type: string */
#define	TA_RMAUTO	((FLDID32)167778868)	/* number: 6708	 type: string */
#define	TA_MRM	((FLDID32)167778869)	/* number: 6709	 type: string */
/* ------------------------------------------------------------
 * The following fields have been defined for use by the Domain MIB.
 * The field numbers reserved for Domain MIB are 6750 - 6950.
 */

#ifndef REDUCE_CPP

 /* DM_SNADOM    */
#define	TA_DMSNADOM	((FLDID32)167778920)	/* number: 6760	 type: string */
#define	TA_DMLUNAME	((FLDID32)167778921)	/* number: 6761	 type: string */
#define	TA_DMMODENAME	((FLDID32)167778922)	/* number: 6762	 type: string */
#define	TA_DMNETID	((FLDID32)167778923)	/* number: 6763	 type: string */
#define	TA_DMSECTYPE	((FLDID32)167778924)	/* number: 6764	 type: string */
#define	TA_DMSYMDESTNAME	((FLDID32)167778925)	/* number: 6765	 type: string */
#define	TA_DMMAXSNASESS	((FLDID32)6766)	/* number: 6766	 type: short */
#define	TA_DMLCONV	((FLDID32)167778927)	/* number: 6767	 type: string */

 /* DM_TDOMAIN */
#define	TA_DMTDOM	((FLDID32)167778935)	/* number: 6775	 type: string */
#define	TA_DMNWADDR	((FLDID32)167778936)	/* number: 6776	 type: string */
#define	TA_NWADDRLEN	((FLDID32)6777)	/* number: 6777	 type: short */
#define	TA_DMNWDEVICE	((FLDID32)167778938)	/* number: 6778	 type: string */
#define	TA_DMNWIDLETIME	((FLDID32)33561211)	/* number: 6779	 type: long */

 /* DM_OSITP */
#define	TA_DMOSITP	((FLDID32)167778940)	/* number: 6780	 type: string */
#define	TA_DMAPT	((FLDID32)167778941)	/* number: 6781	 type: string */
#define	TA_APTLEN	((FLDID32)6782)	/* number: 6782	 type: short */
#define	TA_DMAEQ	((FLDID32)167778943)	/* number: 6783	 type: string */
#define	TA_AEQLEN	((FLDID32)6784)	/* number: 6784	 type: short */
#define	TA_DMAET	((FLDID32)167778945)	/* number: 6785	 type: string */
#define	TA_DMACN	((FLDID32)167778946)	/* number: 6786	 type: string */
#define	TA_ACN2	((FLDID32)6787)	/* number: 6787	 type: short */
#define	TA_DMAPID	((FLDID32)6788)	/* number: 6788	 type: short */
#define	TA_DMAEID	((FLDID32)6789)	/* number: 6789	 type: short */
#define	TA_DMPROFILE	((FLDID32)167778950)	/* number: 6790	 type: string */
#define	TA_PROFILE2	((FLDID32)6791)	/* number: 6791	 type: short */
#define	TA_DMURCH	((FLDID32)167778952)	/* number: 6792	 type: string */

 /* DM_LOCAL_DOMAIN */
#define	TA_LDOMAIN	((FLDID32)167778953)	/* number: 6793	 type: string */
#define	TA_DMSRVGROUP	((FLDID32)167778954)	/* number: 6794	 type: string */
#define	TA_DMTYPE	((FLDID32)167778955)	/* number: 6795	 type: string */
#define	TA_DOMAINAME	((FLDID32)167778956)	/* number: 6796	 type: string */
#define	TA_DMTLOGDEV	((FLDID32)167778957)	/* number: 6797	 type: string */
#define	TA_DMAUDITLOG	((FLDID32)167778958)	/* number: 6798	 type: string */
#define	TA_DMBLOCKTIME	((FLDID32)6799)	/* number: 6799	 type: short */
#define	TA_DMTLOGNAME	((FLDID32)167778960)	/* number: 6800	 type: string */
#define	TA_DMTLOGSIZE	((FLDID32)33561233)	/* number: 6801	 type: long */
#define	TA_MAXDATALEN	((FLDID32)33561234)	/* number: 6802	 type: long */
#define	TA_DMMAXRDOM	((FLDID32)33561235)	/* number: 6803	 type: long */
#define	TA_DMMAXRDTRAN	((FLDID32)6804)	/* number: 6804	 type: short */
#define	TA_DMMAXTRAN	((FLDID32)6805)	/* number: 6805	 type: short */
#define	TA_MAXSENDLEN	((FLDID32)33561238)	/* number: 6806	 type: long */
#define	TA_DMSECURITY	((FLDID32)167778967)	/* number: 6807	 type: string */
#define	TA_SECURITY2	((FLDID32)6808)	/* number: 6808	 type: short */
#define	TA_DMTUXCONFIG	((FLDID32)167778969)	/* number: 6809	 type: string */
#define	TA_DMTUXOFFSET	((FLDID32)33561242)	/* number: 6810	 type: long */
#define	TA_BLOB_SHM_SIZE	((FLDID32)33561243)	/* number: 6811	 type: long */

 /* DM_REMOTE_DOMAIN */
#define	TA_RDOMAIN	((FLDID32)167778972)	/* number: 6812	 type: string */
#define	TA_LOOPBACK	((FLDID32)6813)	/* number: 6813	 type: short */

 /* DM_ACCESS_CONTROL */
#define	TA_DMACLNAME	((FLDID32)167778974)	/* number: 6814	 type: string */
#define	TA_NRDOM	((FLDID32)6815)	/* number: 6815	 type: short */
#define	TA_DMRDOMLIST	((FLDID32)167778976)	/* number: 6816	 type: string */

 /* DM_LOCAL_SERVICES */
#define	TA_DMBUFTYPE	((FLDID32)167778977)	/* number: 6817	 type: string */
#define	TA_DMBUFSTYPE	((FLDID32)167778978)	/* number: 6818	 type: string */
#define	TA_DMINBUFTYPE	((FLDID32)167778979)	/* number: 6819	 type: string */
#define	TA_DMOBUFTYPE	((FLDID32)167778980)	/* number: 6820	 type: string */
#define	TA_DMOBUFSTYPE	((FLDID32)167778981)	/* number: 6821	 type: string */
#define	TA_DMOUTBUFTYPE	((FLDID32)167778982)	/* number: 6822	 type: string */
#define	TA_DMREMOTENAME	((FLDID32)167778983)	/* number: 6823	 type: string */
#define	TA_DMSERVICENAME	((FLDID32)167778984)	/* number: 6824	 type: string */

 /* DM_ROUTING */
#define	TA_DMROUTINGNAME	((FLDID32)167778985)	/* number: 6825	 type: string */
#define	TA_DMFIELD	((FLDID32)167778986)	/* number: 6826	 type: string */
#define	TA_DMRANGES	((FLDID32)167778987)	/* number: 6827	 type: string */

 /* DM_REMOTE_SERVICES */
#define	TA_DMAUTOTRAN	((FLDID32)167778988)	/* number: 6828	 type: string */
#define	TA_AUTOTRAN2	((FLDID32)6829)	/* number: 6829	 type: short */
#define	TA_DMCONV	((FLDID32)167778990)	/* number: 6830	 type: string */
#define	TA_CONV2	((FLDID32)6831)	/* number: 6831	 type: short */
#define	TA_DMLOAD	((FLDID32)6832)	/* number: 6832	 type: short */
#define	TA_DMPRIO	((FLDID32)6833)	/* number: 6833	 type: short */
#define	TA_DMTRANTIME	((FLDID32)33561266)	/* number: 6834	 type: long */
#define	TA_DUMMYSRVGRP	((FLDID32)167778995)	/* number: 6835	 type: string */
#define	TA_DUMMYSRVID	((FLDID32)6836)	/* number: 6836	 type: short */
#define	TA_DUMMYSTATE	((FLDID32)6837)	/* number: 6837	 type: short */
#define	TA_DUMMYACTIVITY	((FLDID32)6838)	/* number: 6838	 type: short */
#define	TA_DUMMYALLSTATS	((FLDID32)6839)	/* number: 6839	 type: short */
#define	TA_DUMMYDOMSTATS	((FLDID32)6840)	/* number: 6840	 type: short */
#define	TA_DUMMYOPTION	((FLDID32)6841)	/* number: 6841	 type: short */
#define	TA_ON	((FLDID32)6842)	/* number: 6842	 type: short */
#define	TA_OFF	((FLDID32)6843)	/* number: 6843	 type: short */
#define	TA_RESET	((FLDID32)6844)	/* number: 6844	 type: short */
#define	TA_DUMMYTOGGLE	((FLDID32)6845)	/* number: 6845	 type: short */
#define	TA_DMSTATISTICS	((FLDID32)6846)	/* number: 6846	 type: short */
#define	TA_DMAUDIT	((FLDID32)6847)	/* number: 6847	 type: short */
#define	TA_TRAN_STATE	((FLDID32)33561280)	/* number: 6848	 type: long */
/*TA_DUMMYMTYPE            100     string  -   Machine Type in LDOM/RDOM */
#define	TA_DMNUMREQLSVC	((FLDID32)33561282)	/* number: 6850	 type: long */
#define	TA_DMNUMREQRSVC	((FLDID32)33561283)	/* number: 6851	 type: long */
#define	TA_DMNUMREPLSVC	((FLDID32)33561284)	/* number: 6852	 type: long */
#define	TA_DMNUMREPRSVC	((FLDID32)33561285)	/* number: 6853	 type: long */
#define	TA_CURACTRQ	((FLDID32)33561286)	/* number: 6854	 type: long */
#define	TA_DMNUMREQCOMP	((FLDID32)33561287)	/* number: 6855	 type: long */
#define	TA_DMNUMREQFAIL	((FLDID32)33561288)	/* number: 6856	 type: long */
#define	TA_DMNUMCONVACT	((FLDID32)33561289)	/* number: 6857	 type: long */
#define	TA_DMNUMCONVLOC	((FLDID32)33561290)	/* number: 6858	 type: long */
#define	TA_DMNUMCONVLSND	((FLDID32)33561291)	/* number: 6859	 type: long */
#define	TA_DMNUMCONVREM	((FLDID32)33561292)	/* number: 6860	 type: long */
#define	TA_DMNUMCONVRSND	((FLDID32)33561293)	/* number: 6861	 type: long */
#define	TA_CURACTEV	((FLDID32)33561294)	/* number: 6862	 type: long */
#define	TA_CURSUSPTEV	((FLDID32)33561295)	/* number: 6863	 type: long */
#define	TA_CURSUSPNEV	((FLDID32)33561296)	/* number: 6864	 type: long */
#define	TA_DMNUMTXBEGUN	((FLDID32)33561297)	/* number: 6865	 type: long */
#define	TA_DMNUMTXCOMMIT	((FLDID32)33561298)	/* number: 6866	 type: long */
#define	TA_DMNUMTXHCOMMIT	((FLDID32)33561299)	/* number: 6867	 type: long */
#define	TA_DMNUMTXHRLBCK	((FLDID32)33561300)	/* number: 6868	 type: long */
#define	TA_DMNUMTXRLBCK	((FLDID32)33561301)	/* number: 6869	 type: long */
#define	TA_DMSTATRESETIME	((FLDID32)33561302)	/* number: 6870	 type: long */
#define	TA_SHM_CONTENTIONS	((FLDID32)33561303)	/* number: 6871	 type: long */
#define	TA_LOG_CONTENTIONS	((FLDID32)33561304)	/* number: 6872	 type: long */
#define	TA_PASSWD	((FLDID32)167779035)	/* number: 6875	 type: string */
#define	TA_ENCPASSWD	((FLDID32)167779036)	/* number: 6876	 type: string */
#define	TA_DMLPWD	((FLDID32)167779037)	/* number: 6877	 type: string */
#define	TA_DMRPWD	((FLDID32)167779038)	/* number: 6878	 type: string */
#define	TA_ENC2_LPWD	((FLDID32)167779039)	/* number: 6879	 type: string */
#define	TA_ENC2_RPWD	((FLDID32)167779040)	/* number: 6880	 type: string */
#define	TA_REENCRYPT_PWD	((FLDID32)6881)	/* number: 6881	 type: short */
#define	TA_DMGWNUM	((FLDID32)33561314)	/* number: 6882	 type: long */
#define	TA_DMRDOMNUM	((FLDID32)33561315)	/* number: 6883	 type: long */
#define	TA_DMTXPARENT	((FLDID32)167779044)	/* number: 6884	 type: string */
#define	TA_DMTXID	((FLDID32)167779045)	/* number: 6885	 type: string */
#define	TA_DMPRINNAME	((FLDID32)167779053)	/* number: 6893	 type: string */
#define	TA_DMRPRINNAME	((FLDID32)167779054)	/* number: 6894	 type: string */
#define	TA_DMRPRINPASSWD	((FLDID32)167779055)	/* number: 6895	 type: string */
#define	TA_DMDIRECTION	((FLDID32)167779056)	/* number: 6896	 type: string */
#define	TA_DMRDOMSEC	((FLDID32)167779057)	/* number: 6897	 type: string */
#define	TA_DMRDOMUSR	((FLDID32)167779058)	/* number: 6898	 type: string */
#define	TA_DMCMPLIMIT	((FLDID32)33561332)	/* number: 6900	 type: long */
#define	TA_DMMINENCRYPTBITS	((FLDID32)167779061)	/* number: 6901	 type: string */
#define	TA_DMMAXENCRYPTBITS	((FLDID32)167779062)	/* number: 6902	 type: string */
/* ------------------------------------------------------------
 * SNA2 Begin
 */
#define	TA_DMSNALINK	((FLDID32)167779063)	/* number: 6903	 type: string */
#define	TA_DMLSYSID	((FLDID32)167779064)	/* number: 6904	 type: string */
#define	TA_DMRSYSID	((FLDID32)167779065)	/* number: 6905	 type: string */
#define	TA_DMMINWIN	((FLDID32)6906)	/* number: 6906	 type: short */
#define	TA_DMMAXSYNCLVL	((FLDID32)6907)	/* number: 6907	 type: short */
#define	TA_DMSNASTACK	((FLDID32)167779068)	/* number: 6908	 type: string */
#define	TA_DMSNACRM	((FLDID32)167779069)	/* number: 6909	 type: string */
#define	TA_DMSTACKTYPE	((FLDID32)167779070)	/* number: 6910	 type: string */
#define	TA_DMTPNAME	((FLDID32)167779071)	/* number: 6911	 type: string */
#define	TA_DMSTACKPARMS	((FLDID32)167779072)	/* number: 6912	 type: string */
#define	TA_DMAPI	((FLDID32)167779073)	/* number: 6913	 type: string */
#define	TA_DMFUNCTION	((FLDID32)167779074)	/* number: 6914	 type: string */
#define	TA_DMCODEPAGE	((FLDID32)167779075)	/* number: 6915	 type: string */
#define	TA_DMSTARTTYPE	((FLDID32)167779076)	/* number: 6916	 type: string */
/* SNA2 End */
/* ------------------------------------------------------------
 */
#define	TA_DMCONNECTION_POLICY	((FLDID32)167779077)	/* number: 6917	 type: string */
#define	TA_DMRETRY_INTERVAL	((FLDID32)33561350)	/* number: 6918	 type: long */
#define	TA_DMMAXRETRY	((FLDID32)33561351)	/* number: 6919	 type: long */
/* ------------------------------------------------------------
 * adds for OSITP V4.0 and TUX V7.2
 */
/* TA_DMACCESSPOINT string */
/* TA_STATE string */
/* TA_DMAET string */
/* TA_DMNWADDR string */
#define	TA_DMTSEL	((FLDID32)167779080)	/* number: 6920	 type: string */
#define	TA_DMTAILORPATH	((FLDID32)167779081)	/* number: 6921	 type: string */
#define	TA_DMPSEL	((FLDID32)167779082)	/* number: 6922	 type: string */
#define	TA_DMSSEL	((FLDID32)167779083)	/* number: 6923	 type: string */
#define	TA_DMEXTENSIONS	((FLDID32)167779084)	/* number: 6924	 type: string */
/* TA_DMMINENCRIPTBITS string */
/* TA_DMMAXENCRIPTBITS string */
#define	TA_DMMULTIPLEXING	((FLDID32)6925)	/* number: 6925	 type: short */
#define	TA_DMDNSRESOLUTION	((FLDID32)167779086)	/* number: 6926	 type: string */
#define	TA_DMOPTIONS	((FLDID32)167779087)	/* number: 6927	 type: string */
/* TA_DMXATMIENCODING string */
/* Local Services adds for OSITP V4.0 */
#define	TA_DMCOUPLING	((FLDID32)167779088)	/* number: 6928	 type: string */
#define	TA_DMINRECTYPE	((FLDID32)167779089)	/* number: 6929	 type: string */
#define	TA_DMINRECSTYPE	((FLDID32)167779090)	/* number: 6930	 type: string */
#define	TA_DMOUTRECTYPE	((FLDID32)167779091)	/* number: 6931	 type: string */
#define	TA_DMOUTRECSTYPE	((FLDID32)167779092)	/* number: 6932	 type: string */
/* Remote Services adds for OSITP V4.0 */
#define	TA_DMAUTOPREPARE	((FLDID32)167779093)	/* number: 6933	 type: string */
#define	TA_DMTPSUTTYPE	((FLDID32)167779094)	/* number: 6934	 type: string */
#define	TA_DMREMTPSUT	((FLDID32)167779095)	/* number: 6935	 type: string */
/* OSITP V4.0 End */
/*
 *  The following fields have been added in Tuxedo 12.2.2
 * DM STAT REMOTE SERVICE CALL
 */
#define	TA_DMSTATISTICSLOG_PFX	((FLDID32)167779099)	/* number: 6939	 type: string */
#define	TA_DMSTATISTICS_INTERVAL	((FLDID32)6940)	/* number: 6940	 type: short */
/*  end of Tuxedo 12.2.2 DM STAT REMOTE SERVICE CALL*/
/*
 *  The following fields have been added in Tuxedo 12.2.2
 * DM STAT REMOTE SERVICE CALL INBOUND
 */
#define	TA_DMRSTATISTICSLOG_PFX	((FLDID32)167779101)	/* number: 6941	 type: string */
#define	TA_DMRSTATISTICS_INTERVAL	((FLDID32)6942)	/* number: 6942	 type: short */
/*  end of Tuxedo 12.2.2 DM STAT REMOTE SERVICE CALL*/
/* ------------------------------------------------------------
 * 30000700 - 30001200 is reserved for ART.
 * 0 - 82 is already used in fmb.def
 */
#define	TA_MAXSESSIONS	((FLDID32)63555233)	/* number: 30000801	 type: long */
#define	TA_CURSESSIONS	((FLDID32)63555234)	/* number: 30000802	 type: long */
#define	TA_NUMTRANFAIL	((FLDID32)63555235)	/* number: 30000803	 type: long */
#define	TA_SESSIONID	((FLDID32)197772964)	/* number: 30000804	 type: string */
#define	TA_CTIME	((FLDID32)63555237)	/* number: 30000805	 type: long */
#define	TA_ATIME	((FLDID32)63555238)	/* number: 30000806	 type: long */
/* ------------------------------------------------------------
 * New Domains MIB class attributes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * The range 30001501 - 30001700 is reserved for DMIB fields.
 */
#define	TA_DMACCESSPOINT	((FLDID32)197773661)	/* number: 30001501	 type: string */
#define	TA_DMACCESSPOINTID	((FLDID32)197773662)	/* number: 30001502	 type: string */
#define	TA_DMLACCESSPOINT	((FLDID32)197773663)	/* number: 30001503	 type: string */
#define	TA_DMRACCESSPOINT	((FLDID32)197773664)	/* number: 30001504	 type: string */
#define	TA_DMRESOURCENAME	((FLDID32)197773665)	/* number: 30001505	 type: string */
#define	TA_DMRESOURCETYPE	((FLDID32)197773666)	/* number: 30001506	 type: string */
#define	TA_DMTE_PRODUCT	((FLDID32)197773667)	/* number: 30001507	 type: string */
#define	TA_DMTE_FUNCTION	((FLDID32)197773668)	/* number: 30001508	 type: string */
#define	TA_DMTE_TARGET	((FLDID32)197773669)	/* number: 30001509	 type: string */
#define	TA_DMTE_QUALIFIER	((FLDID32)63555942)	/* number: 30001510	 type: long */
#define	TA_DMTE_RTQGROUP	((FLDID32)197773671)	/* number: 30001511	 type: string */
#define	TA_DMTE_RTQNAME	((FLDID32)197773672)	/* number: 30001512	 type: string */
#define	TA_DMTE_TP_SYSTEM	((FLDID32)197773673)	/* number: 30001513	 type: string */
#define	TA_DMMAXRAPTRAN	((FLDID32)30001514)	/* number: 30001514	 type: short */
#define	TA_DMMACHINETYPE	((FLDID32)197773675)	/* number: 30001515	 type: string */
#define	TA_DMMAXLISTENINGEP	((FLDID32)30001516)	/* number: 30001516	 type: short */
#define	TA_DMVERSION	((FLDID32)197773677)	/* number: 30001517	 type: string */
#define	TA_DMEVNUMACT	((FLDID32)63555950)	/* number: 30001518	 type: long */
#define	TA_DMEVNUMSUSPN	((FLDID32)63555951)	/* number: 30001519	 type: long */
#define	TA_DMEVNUMSUSPT	((FLDID32)63555952)	/* number: 30001520	 type: long */
#define	TA_DMLCONVNUMACT	((FLDID32)63555953)	/* number: 30001521	 type: long */
#define	TA_DMLCONVNUMRCV	((FLDID32)63555954)	/* number: 30001522	 type: long */
#define	TA_DMLCONVNUMSND	((FLDID32)63555955)	/* number: 30001523	 type: long */
#define	TA_DMLCONVTOT	((FLDID32)63555956)	/* number: 30001524	 type: long */
#define	TA_DMLCONVTOTFAIL	((FLDID32)63555957)	/* number: 30001525	 type: long */
#define	TA_DMLCONVTOTRCV	((FLDID32)63555958)	/* number: 30001526	 type: long */
#define	TA_DMLCONVTOTSND	((FLDID32)63555959)	/* number: 30001527	 type: long */
#define	TA_DMLSVCNUMACT	((FLDID32)63555960)	/* number: 30001528	 type: long */
#define	TA_DMLSVCTOT	((FLDID32)63555961)	/* number: 30001529	 type: long */
#define	TA_DMLSVCTOTFAIL	((FLDID32)63555962)	/* number: 30001530	 type: long */
#define	TA_DMLSVCTOTRPLY	((FLDID32)63555963)	/* number: 30001531	 type: long */
#define	TA_DMLOGCONTENTIONS	((FLDID32)63555964)	/* number: 30001532	 type: long */
#define	TA_DMRCONVNUMACT	((FLDID32)63555965)	/* number: 30001533	 type: long */
#define	TA_DMRCONVNUMRCV	((FLDID32)63555966)	/* number: 30001534	 type: long */
#define	TA_DMRCONVNUMSND	((FLDID32)63555967)	/* number: 30001535	 type: long */
#define	TA_DMRCONVTOT	((FLDID32)63555968)	/* number: 30001536	 type: long */
#define	TA_DMRCONVTOTFAIL	((FLDID32)63555969)	/* number: 30001537	 type: long */
#define	TA_DMRCONVTOTRCV	((FLDID32)63555970)	/* number: 30001538	 type: long */
#define	TA_DMRCONVTOTSND	((FLDID32)63555971)	/* number: 30001539	 type: long */
#define	TA_DMRSVCNUMACT	((FLDID32)63555972)	/* number: 30001540	 type: long */
#define	TA_DMRSVCTOT	((FLDID32)63555973)	/* number: 30001541	 type: long */
#define	TA_DMRSVCTOTFAIL	((FLDID32)63555974)	/* number: 30001542	 type: long */
#define	TA_DMRSVCTOTRPLY	((FLDID32)63555975)	/* number: 30001543	 type: long */
#define	TA_DMSHMCONTENTIONS	((FLDID32)63555976)	/* number: 30001544	 type: long */
#define	TA_DMSTATRESETTIME	((FLDID32)63555977)	/* number: 30001545	 type: long */
#define	TA_DMTXNUMACT	((FLDID32)63555978)	/* number: 30001546	 type: long */
#define	TA_DMTXTOTCOMMIT	((FLDID32)63555979)	/* number: 30001547	 type: long */
#define	TA_DMTXTOTHCOMMIT	((FLDID32)63555980)	/* number: 30001548	 type: long */
#define	TA_DMTXTOTHRLBCK	((FLDID32)63555981)	/* number: 30001549	 type: long */
#define	TA_DMTXTOTRLBCK	((FLDID32)63555982)	/* number: 30001550	 type: long */
#define	TA_DMFAILOVERSEQ	((FLDID32)30001551)	/* number: 30001551	 type: short */
#define	TA_DMTE_PWD	((FLDID32)197773712)	/* number: 30001552	 type: string */
#define	TA_DMBLOB_SHM_SIZE	((FLDID32)63555985)	/* number: 30001553	 type: long */
#define	TA_DMTPTRANID	((FLDID32)197773714)	/* number: 30001554	 type: string */
#define	TA_DMTXACCESSPOINT	((FLDID32)197773715)	/* number: 30001555	 type: string */
#define	TA_DMTXNETTRANID	((FLDID32)197773716)	/* number: 30001556	 type: string */
#define	TA_DMBRANCHCOUNT	((FLDID32)63555989)	/* number: 30001557	 type: long */
#define	TA_DMBRANCHINDEX	((FLDID32)63555990)	/* number: 30001558	 type: long */
#define	TA_DMBRANCHNO	((FLDID32)63555991)	/* number: 30001559	 type: long */
#define	TA_DMBRANCHSTATE	((FLDID32)197773720)	/* number: 30001560	 type: string */
#define	TA_DMNETTRANID	((FLDID32)197773721)	/* number: 30001561	 type: string */
#define	TA_DMXATMIENCODING	((FLDID32)197773722)	/* number: 30001562	 type: string */
#define	TA_DMCURENCRYPTBITS	((FLDID32)197773723)	/* number: 30001563	 type: string */
#define	TA_DMFIELDTYPE	((FLDID32)197773724)	/* number: 30001564	 type: string */
#define	TA_DMACLPOLICY	((FLDID32)197773725)	/* number: 30001565	 type: string */
#define	TA_DMCONNPRINCIPALNAME	((FLDID32)197773726)	/* number: 30001566	 type: string */
#define	TA_DMLOCALPRINCIPALNAME	((FLDID32)197773727)	/* number: 30001567	 type: string */
#define	TA_DMRACCESSPOINTLIST	((FLDID32)197773728)	/* number: 30001568	 type: string */
#define	TA_DMPRIORITY_TYPE	((FLDID32)197773729)	/* number: 30001569	 type: string */
#define	TA_DMINPRIORITY	((FLDID32)30001570)	/* number: 30001570	 type: short */
#define	TA_DMCREDENTIALPOLICY	((FLDID32)197773731)	/* number: 30001571	 type: string */
#define	TA_DMTCPKEEPALIVE	((FLDID32)197773732)	/* number: 30001572	 type: string */
#define	TA_DMKEEPALIVE	((FLDID32)63556005)	/* number: 30001573	 type: long */
#define	TA_DMKEEPALIVEWAIT	((FLDID32)63556006)	/* number: 30001574	 type: long */
#define	TA_ABORT_THRESHOLD	((FLDID32)63556007)	/* number: 30001575	 type: long */
#define	TA_DMSTARTTIME	((FLDID32)197773736)	/* number: 30001576	 type: string */
#define	TA_DMENDTIME	((FLDID32)197773737)	/* number: 30001577	 type: string */
#define	TA_DMMAC	((FLDID32)197773738)	/* number: 30001578	 type: string */
#define	TA_DMMACLEVEL	((FLDID32)197773739)	/* number: 30001579	 type: string */
/*
 * The following fields have been added in Tuxedo 10.0
 */
#define	TA_DMNW_PROTOCOL	((FLDID32)197773740)	/* number: 30001580	 type: string */
#define	TA_DMSSL_RENEGOTIATION	((FLDID32)63556013)	/* number: 30001581	 type: long */
/* End of new domains MIB class attributes */

/*
 * The following fields have been added in Tuxedo 11gR1 PS1
 */
#define	TA_DMDYNAMIC_RAP	((FLDID32)197773742)	/* number: 30001582	 type: string */
/* end of Tuxedo 11gR1 PS1 */

/*
 * For DM_EVT_IN and DM_EVT_OUT
 */
#define	TA_DMEVTNAME	((FLDID32)197773743)	/* number: 30001583	 type: string */
#define	TA_DMEVTTRAN	((FLDID32)197773744)	/* number: 30001584	 type: string */
#define	TA_DMEVTEXPR	((FLDID32)197773745)	/* number: 30001585	 type: string */
#define	TA_DMEVTFILTER	((FLDID32)197773746)	/* number: 30001586	 type: string */
#define	TA_DMREVTNAME	((FLDID32)197773747)	/* number: 30001587	 type: string */
/*
 * The following fields have been added in Tuxedo 12cR2
 */
#define	TA_DMTHROUGHGATEWAY	((FLDID32)197773748)	/* number: 30001588	 type: string */
/* end of Tuxedo 12cR2 */
/* end REDUCE_CPP */
#endif
/* ------------------------------------------------------------
 * The following fields have been defined for use by the Event MIB.
 * The field numbers reserved for Event MIB are 6950 - 7000.
 */
#ifndef REDUCE_CPP
#define	TA_EVENT_EXPR	((FLDID32)167779111)	/* number: 6951	 type: string */
#define	TA_EVENT_FILTER	((FLDID32)167779112)	/* number: 6952	 type: string */
#define	TA_EVENT_FILTER_BINARY	((FLDID32)201333545)	/* number: 6953	 type: carray */
#define	TA_QSPACE	((FLDID32)167779114)	/* number: 6954	 type: string */
#define	TA_QNAME	((FLDID32)167779115)	/* number: 6955	 type: string */
#define	TA_QCTL_QTIME_ABS	((FLDID32)6956)	/* number: 6956	 type: short */
#define	TA_QCTL_QTIME_REL	((FLDID32)6957)	/* number: 6957	 type: short */
#define	TA_QCTL_QTOP	((FLDID32)6958)	/* number: 6958	 type: short */
#define	TA_QCTL_BEFOREMSGID	((FLDID32)6959)	/* number: 6959	 type: short */
#define	TA_QCTL_DEQ_TIME	((FLDID32)33561392)	/* number: 6960	 type: long */
#define	TA_QCTL_PRIORITY	((FLDID32)33561393)	/* number: 6961	 type: long */
#define	TA_QCTL_MSGID	((FLDID32)167779122)	/* number: 6962	 type: string */
#define	TA_QCTL_CORRID	((FLDID32)167779123)	/* number: 6963	 type: string */
#define	TA_QCTL_REPLYQUEUE	((FLDID32)167779124)	/* number: 6964	 type: string */
#define	TA_QCTL_FAILUREQUEUE	((FLDID32)167779125)	/* number: 6965	 type: string */
#define	TA_EVENT_PERSIST	((FLDID32)6966)	/* number: 6966	 type: short */
#define	TA_EVENT_TRAN	((FLDID32)6967)	/* number: 6967	 type: short */
#define	TA_USERLOG	((FLDID32)167779128)	/* number: 6968	 type: string */
#define	TA_COMMAND	((FLDID32)167779129)	/* number: 6969	 type: string */
#define	TA_EVENT_SERVER	((FLDID32)167779130)	/* number: 6970	 type: string */
#define	TA_SUBSCRIPTION_VERSION	((FLDID32)33561403)	/* number: 6971	 type: long */
#define	TA_BLOB_TYPE	((FLDID32)167779132)	/* number: 6972	 type: string */
#define	TA_BLOB_VALUE	((FLDID32)201333565)	/* number: 6973	 type: carray */
#define	TA_EVENT_SET_HANDLE	((FLDID32)6974)	/* number: 6974	 type: short */
#define	TA_EVENT_SET_BLOB	((FLDID32)6975)	/* number: 6975	 type: short */
#define	TA_EVENT_UNIQUE	((FLDID32)6976)	/* number: 6976	 type: short */
#define	TA_QCTL_USERTAG	((FLDID32)33561409)	/* number: 6977	 type: long */
#define	TA_EVENT_GWEVT	((FLDID32)6978)	/* number: 6978	 type: short */
#define	TA_SUBSCRIPTION_HANDLE	((FLDID32)33561439)	/* number: 7007	 type: long */
#endif
#ifndef REDUCE_CPP
/* ------------------------------------------------------------
 * OBSOLETE, BUT DON'T REUSE THEM.
 */
#define	TA_PROGMODELS	((FLDID32)197773162)	/* number: 30001002	 type: string */
#define	TA_SRVTYPE	((FLDID32)197773163)	/* number: 30001003	 type: string */
#define	TA_JAVAHEAPSIZE	((FLDID32)63555436)	/* number: 30001004	 type: long */
#define	TA_JAVAHEAPUSE	((FLDID32)63555438)	/* number: 30001006	 type: long */
#define	TA_CLASSPATH	((FLDID32)197773170)	/* number: 30001010	 type: string */
#define	TA_JAVAVERSION	((FLDID32)197773171)	/* number: 30001011	 type: string */
#define	TA_JAVAVENDOR	((FLDID32)197773172)	/* number: 30001012	 type: string */
#define	TA_INTERN_TACTSRVID	((FLDID32)63555445)	/* number: 30001013	 type: long */
#define	TA_INTERN_TACTSRVGRP	((FLDID32)197773174)	/* number: 30001014	 type: string */
#define	TA_INTERN_TACTGRPNO	((FLDID32)63555447)	/* number: 30001015	 type: long */
#define	TA_INTERN_CURSOR	((FLDID32)63555448)	/* number: 30001016	 type: long */
#define	TA_INTERN_FLDNAME	((FLDID32)197773177)	/* number: 30001017	 type: string */
#define	TA_INTERN_FLDVALUE	((FLDID32)197773178)	/* number: 30001018	 type: string */
#define	TA_INTERN_FLDTYPE	((FLDID32)63555451)	/* number: 30001019	 type: long */
#define	TA_INTERN_APPKEY	((FLDID32)63555452)	/* number: 30001020	 type: long */
#define	TA_MAXEJBCACHE	((FLDID32)63555453)	/* number: 30001021	 type: long */
#define	TA_EJBCACHE_FLUSH	((FLDID32)63555454)	/* number: 30001022	 type: long */
#define	TA_INTERN_ERROR	((FLDID32)63555455)	/* number: 30001023	 type: long */
#define	TA_INTERN_DBBL	((FLDID32)63555456)	/* number: 30001024	 type: long */
/*T_MODULE fields */
#define	TA_MODULE	((FLDID32)197773202)	/* number: 30001042	 type: string */
#define	TA_MODULEARGS	((FLDID32)197773203)	/* number: 30001043	 type: string */
#define	TA_MODULETYPE	((FLDID32)197773204)	/* number: 30001044	 type: string */
#define	TA_MODULEFILE	((FLDID32)197773205)	/* number: 30001045	 type: string */
#define	TA_MODULECLASSPATH	((FLDID32)197773206)	/* number: 30001046	 type: string */
/* T_CONNECTION_POOL fields */
#define	TA_DSNAME	((FLDID32)197773222)	/* number: 30001062	 type: string */
#define	TA_DRIVER	((FLDID32)197773223)	/* number: 30001063	 type: string */
#define	TA_URL	((FLDID32)197773224)	/* number: 30001064	 type: string */
#define	TA_DBNAME	((FLDID32)197773225)	/* number: 30001065	 type: string */
#define	TA_DBUSER	((FLDID32)197773226)	/* number: 30001066	 type: string */
#define	TA_DBPASSWORD	((FLDID32)197773227)	/* number: 30001067	 type: string */
#define	TA_DBHOST	((FLDID32)197773229)	/* number: 30001069	 type: string */
#define	TA_DBNETPROTOCOL	((FLDID32)197773230)	/* number: 30001070	 type: string */
#define	TA_DBPORT	((FLDID32)63555503)	/* number: 30001071	 type: long */
#define	TA_PROPS	((FLDID32)197773232)	/* number: 30001072	 type: string */
#define	TA_ENABLEXA	((FLDID32)197773233)	/* number: 30001073	 type: string */
#define	TA_CREATEONSTARTUP	((FLDID32)197773234)	/* number: 30001074	 type: string */
#define	TA_LOGINDELAY	((FLDID32)63555507)	/* number: 30001075	 type: long */
#define	TA_INITCAPACITY	((FLDID32)63555508)	/* number: 30001076	 type: long */
#define	TA_MAXCAPACITY	((FLDID32)63555509)	/* number: 30001077	 type: long */
#define	TA_CAPACITYINCR	((FLDID32)63555510)	/* number: 30001078	 type: long */
#define	TA_SHRINKPERIOD	((FLDID32)63555511)	/* number: 30001079	 type: long */
#define	TA_TESTTABLE	((FLDID32)197773240)	/* number: 30001080	 type: string */
#define	TA_REFRESH	((FLDID32)63555513)	/* number: 30001081	 type: long */
#define	TA_TESTONRESERVE	((FLDID32)197773242)	/* number: 30001082	 type: string */
#define	TA_TESTONRELEASE	((FLDID32)197773243)	/* number: 30001083	 type: string */
#define	TA_WAITFORCONN	((FLDID32)197773244)	/* number: 30001084	 type: string */
#define	TA_WAITTIMEOUT	((FLDID32)63555517)	/* number: 30001085	 type: long */
#define	TA_CONNUSED	((FLDID32)63555518)	/* number: 30001086	 type: long */
#define	TA_CONNAVAILABLE	((FLDID32)63555519)	/* number: 30001087	 type: long */
#define	TA_HWMCONNUSED	((FLDID32)63555520)	/* number: 30001088	 type: long */
#define	TA_HWMCONNCREATED	((FLDID32)63555521)	/* number: 30001089	 type: long */
#define	TA_AWAITINGCONN	((FLDID32)63555522)	/* number: 30001090	 type: long */
#define	TA_HWMFORWAIT	((FLDID32)63555523)	/* number: 30001091	 type: long */
#define	TA_ALLOWSHRINKING	((FLDID32)197773252)	/* number: 30001092	 type: string */
#define	TA_USERROLE	((FLDID32)197773253)	/* number: 30001093	 type: string */
/* ------------------------------------------------------------
 * ART reserved fields:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 30000700 - 30000800, 30001100 - 30001200
 * is used for ART Runtime.
 *
 */
/* New fields in Release 8.1 */
#define	TA_MAXTRANTIME	((FLDID32)63556134)	/* number: 30001702	 type: long */
#define	TA_MAXSPDATA	((FLDID32)63556135)	/* number: 30001703	 type: long */
#define	TA_BRTHREADS	((FLDID32)197773864)	/* number: 30001704	 type: string */
#define	TA_CONCURR_STRATEGY	((FLDID32)197773865)	/* number: 30001705	 type: string */
/* ------------------------------------------------------------
 * Add in 12gR1 for T_JSL
 */
#define	TA_MAXSNOOZETIME	((FLDID32)63556138)	/* number: 30001706	 type: long */
#define	TA_JCONNTYPE	((FLDID32)197773867)	/* number: 30001707	 type: string */
#define	TA_IDENTITY_PROPAGATION	((FLDID32)197773868)	/* number: 30001708	 type: string */
/* ------------------------------------------------------------
 * New fields for service metadata repository
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * the range 30001801 - 30002000 is reserved
 */
#define	TA_REPOSEMBED	((FLDID32)365546121)	/* number: 30001801	 type: fml32 */
/* ------------------------------------------------------------
 * service-level attributes
 */
#define	TA_REPOSSERVICE	((FLDID32)197773971)	/* number: 30001811	 type: string */
#define	TA_REPOSSERVICETYPE	((FLDID32)197773972)	/* number: 30001812	 type: string */
#define	TA_REPOSEXPORT	((FLDID32)197773973)	/* number: 30001813	 type: string */
#define	TA_REPOSINBUF	((FLDID32)197773974)	/* number: 30001814	 type: string */
#define	TA_REPOSOUTBUF	((FLDID32)197773975)	/* number: 30001815	 type: string */
#define	TA_REPOSINVIEW	((FLDID32)197773976)	/* number: 30001816	 type: string */
#define	TA_REPOSOUTVIEW	((FLDID32)197773977)	/* number: 30001817	 type: string */
#define	TA_REPOSSVCDESCRIPTION	((FLDID32)197773978)	/* number: 30001818	 type: string */
#define	TA_REPOSSENDQSPACE	((FLDID32)197773979)	/* number: 30001819	 type: string */
#define	TA_REPOSSENDQUEUE	((FLDID32)197773980)	/* number: 30001820	 type: string */
#define	TA_REPOSRPLYQUEUE	((FLDID32)197773981)	/* number: 30001821	 type: string */
#define	TA_REPOSERRQUEUE	((FLDID32)197773982)	/* number: 30001822	 type: string */
#define	TA_REPOSRCVQSPACE	((FLDID32)197773983)	/* number: 30001823	 type: string */
#define	TA_REPOSRCVQUEUE	((FLDID32)197773984)	/* number: 30001824	 type: string */
#define	TA_REPOSVERSION	((FLDID32)197773985)	/* number: 30001825	 type: string */
#define	TA_REPOSATTRIBUTES	((FLDID32)197773986)	/* number: 30001826	 type: string */
#define	TA_REPOSFIELDTBLS	((FLDID32)197773987)	/* number: 30001827	 type: string */
#define	TA_REPOSPARAMCOUNT	((FLDID32)63556260)	/* number: 30001828	 type: long */
#define	TA_REPOSTUXSERVICE	((FLDID32)197773989)	/* number: 30001829	 type: string */
#define	TA_REPOSERRBUF	((FLDID32)197773990)	/* number: 30001830	 type: string */
#define	TA_REPOSERRVIEW	((FLDID32)197773991)	/* number: 30001831	 type: string */
#define	TA_REPOSINBUFSCHEMA	((FLDID32)197773992)	/* number: 30001832	 type: string */
#define	TA_REPOSOUTBUFSCHEMA	((FLDID32)197773993)	/* number: 30001833	 type: string */
#define	TA_REPOSERRBUFSCHEMA	((FLDID32)197773994)	/* number: 30001834	 type: string */
#define	TA_REPOSSERVICEMODE	((FLDID32)197773995)	/* number: 30001835	 type: string */
#define	TA_REPOSAUTODISCOVERY	((FLDID32)197773996)	/* number: 30001836	 type: string */
#define	TA_REPOSINRECORD	((FLDID32)197773997)	/* number: 30001837	 type: string */
#define	TA_REPOSOUTRECORD	((FLDID32)197773998)	/* number: 30001838	 type: string */
#define	TA_REPOSERRRECORD	((FLDID32)197773999)	/* number: 30001839	 type: string */
/*
 * parameter-level attributes
 */
#define	TA_REPOSPARAMINDEX	((FLDID32)63556333)	/* number: 30001901	 type: long */
#define	TA_REPOSPARAM	((FLDID32)197774062)	/* number: 30001902	 type: string */
#define	TA_REPOSTYPE	((FLDID32)197774063)	/* number: 30001903	 type: string */
#define	TA_REPOSSUBTYPE	((FLDID32)197774064)	/* number: 30001904	 type: string */
#define	TA_REPOSACCESS	((FLDID32)197774065)	/* number: 30001905	 type: string */
#define	TA_REPOSCOUNT	((FLDID32)63556338)	/* number: 30001906	 type: long */
#define	TA_REPOSPARAMDESCRIPTION	((FLDID32)197774067)	/* number: 30001907	 type: string */
#define	TA_REPOSSIZE	((FLDID32)63556340)	/* number: 30001908	 type: long */
#define	TA_REPOSREQUIREDCOUNT	((FLDID32)63556341)	/* number: 30001909	 type: long */
#define	TA_REPOSFLDNUM	((FLDID32)63556342)	/* number: 30001910	 type: long */
#define	TA_REPOSFLDID	((FLDID32)63556343)	/* number: 30001911	 type: long */
#define	TA_REPOSFLDINDEX	((FLDID32)63556344)	/* number: 30001912	 type: long */
#define	TA_REPOSVFBNAME	((FLDID32)197774073)	/* number: 30001913	 type: string */
#define	TA_REPOSVFLAG	((FLDID32)197774074)	/* number: 30001914	 type: string */
#define	TA_REPOSVNULL	((FLDID32)197774075)	/* number: 30001915	 type: string */
#define	TA_REPOSPARAMSCHEMA	((FLDID32)197774076)	/* number: 30001916	 type: string */
#define	TA_REPOSPRIMETYPE	((FLDID32)197774077)	/* number: 30001917	 type: string */
#define	TA_REPOSUNION	((FLDID32)197774078)	/* number: 30001918	 type: string */
#define	TA_REPOSHIDDEN	((FLDID32)197774079)	/* number: 30001919	 type: string */
#define	TA_REPOSPARAMALIAS	((FLDID32)197774080)	/* number: 30001920	 type: string */
#define	TA_REPOSFRACTIONDIGITS	((FLDID32)63556353)	/* number: 30001921	 type: long */
#define	TA_REPOSNILLABLE	((FLDID32)197774082)	/* number: 30001922	 type: string */
#define	TA_REPOSISARRAY	((FLDID32)197774083)	/* number: 30001923	 type: string */
#define	TA_REPOSWHITESPACE	((FLDID32)197774084)	/* number: 30001924	 type: string */
/* ------------------------------------------------------------
 * New fields for Phoenix Release
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 30002001 - 30002100, 30002401 - 3002600
 * is used for Phoenix release. 30002101 - 30002400
 * is used for SALT.
 *
 */
/* ------------------------------------------------------------
 * 30002001 - 30002100 is for general purpose
 * and not reflected in the performance metrics
 */
#define	TA_MONTYPE	((FLDID32)197774161)	/* number: 30002001	 type: string */
#define	TA_MONLOGTIMESEC	((FLDID32)63556434)	/* number: 30002002	 type: long */
#define	TA_MONLOGTIMEUSEC	((FLDID32)63556435)	/* number: 30002003	 type: long */
#define	TA_MONFIELDSMAP1	((FLDID32)63556436)	/* number: 30002004	 type: long */
#define	TA_MONFIELDSMAP2	((FLDID32)63556437)	/* number: 30002005	 type: long */
#define	TA_MONFIELDSMAP3	((FLDID32)63556438)	/* number: 30002006	 type: long */
#define	TA_MONFIELDSMAP4	((FLDID32)63556439)	/* number: 30002007	 type: long */
#define	TA_MONVERSION	((FLDID32)30002008)	/* number: 30002008	 type: short */
#define	TA_MONOWNER	((FLDID32)197774169)	/* number: 30002009	 type: string */
#define	TA_MONNOLOG	((FLDID32)30002010)	/* number: 30002010	 type: short */
#define	TA_MONEVENTNAME	((FLDID32)197774171)	/* number: 30002011	 type: string */
#define	TA_MONSEVERITY	((FLDID32)197774172)	/* number: 30002012	 type: string */
#define	TA_MONVERSIONSTR	((FLDID32)197774173)	/* number: 30002013	 type: string */
/* ------------------------------------------------------------
 * Notes 30002101 - 30002400 was used for SALT
 * New fields for SALT(Service Architecture Leverage Tuxedo)
 */
#define	TA_WS_GWWSID	((FLDID32)197774261)	/* number: 30002101	 type: string */
#define	TA_WS_CONFIDXNUM	((FLDID32)30002102)	/* number: 30002102	 type: short */
#define	TA_WS_CONFINDEX	((FLDID32)30002103)	/* number: 30002103	 type: short */
#define	TA_WS_CONFDURATION	((FLDID32)197774264)	/* number: 30002104	 type: string */
#define	TA_WS_CONFCLTNUM	((FLDID32)63556537)	/* number: 30002105	 type: long */
#define	TA_WS_CONFMAXATTM	((FLDID32)197774266)	/* number: 30002106	 type: string */
#define	TA_WS_IPADDRESS	((FLDID32)197774267)	/* number: 30002107	 type: string */
#define	TA_WS_TUXSERVICE	((FLDID32)197774268)	/* number: 30002108	 type: string */
#define	TA_WS_WSSERVICE	((FLDID32)197774269)	/* number: 30002109	 type: string */
#define	TA_WS_SOAPVERSION	((FLDID32)197774270)	/* number: 30002110	 type: string */
#define	TA_WS_DURATIONTIME	((FLDID32)63556543)	/* number: 30002111	 type: long */
#define	TA_WS_REQREPDONE	((FLDID32)63556544)	/* number: 30002112	 type: long */
#define	TA_WS_REQREPFAIL	((FLDID32)63556545)	/* number: 30002113	 type: long */
#define	TA_WS_ONEWAYDONE	((FLDID32)63556546)	/* number: 30002114	 type: long */
#define	TA_WS_ONEWAYFAIL	((FLDID32)63556547)	/* number: 30002115	 type: long */
#define	TA_WS_SEQUENCEID	((FLDID32)197774276)	/* number: 30002116	 type: string */
#define	TA_WS_SEQUENCESTATUS	((FLDID32)197774277)	/* number: 30002117	 type: string */
#define	TA_WS_SEQUENCEDIRECTION	((FLDID32)197774278)	/* number: 30002118	 type: string */
#define	TA_WS_SEQUENCEACKTO	((FLDID32)197774279)	/* number: 30002119	 type: string */
#define	TA_WS_MSGID	((FLDID32)197774280)	/* number: 30002120	 type: string */
#define	TA_WS_MSGNUMBER	((FLDID32)63556553)	/* number: 30002121	 type: long */
#define	TA_WS_LASTMSG	((FLDID32)63556554)	/* number: 30002122	 type: long */
#define	TA_WS_SEQUENCEDURATION	((FLDID32)63556555)	/* number: 30002123	 type: long */
#define	TA_WS_TIMETOEXPIRE	((FLDID32)63556556)	/* number: 30002124	 type: long */
#define	TA_WS_TIMEOUT	((FLDID32)63556557)	/* number: 30002125	 type: long */
#define	TA_WS_PAIRSEQUENCE	((FLDID32)197774286)	/* number: 30002126	 type: string */
#define	TA_WS_OUTBOUNDDONE	((FLDID32)63556559)	/* number: 30002127	 type: long */
#define	TA_WS_OUTBOUNDFAIL	((FLDID32)63556560)	/* number: 30002128	 type: long */
#define	TA_WS_OUTBOUNDPENDING	((FLDID32)63556561)	/* number: 30002129	 type: long */
#define	TA_WS_SVCDONE	((FLDID32)63556562)	/* number: 30002130	 type: long */
#define	TA_WS_SVCFAIL	((FLDID32)63556563)	/* number: 30002131	 type: long */
#define	TA_WS_SVCTIME	((FLDID32)63556564)	/* number: 30002132	 type: long */
#define	TA_WS_OUTBOUND_ONEWAYDONE	((FLDID32)63556565)	/* number: 30002133	 type: long */
#define	TA_WS_OUTBOUND_ONEWAYFAIL	((FLDID32)63556566)	/* number: 30002134	 type: long */
#define	TA_WS_INBOUNDTIME	((FLDID32)63556567)	/* number: 30002135	 type: long */
#define	TA_WS_OUTBOUNDTIME	((FLDID32)63556568)	/* number: 30002136	 type: long */
#define	TA_WS_THREADS	((FLDID32)63556569)	/* number: 30002137	 type: long */
#define	TA_WS_TOTALPENDING	((FLDID32)63556570)	/* number: 30002138	 type: long */

#define	TA_WS_TRANPROCESSID	((FLDID32)63556571)	/* number: 30002139	 type: long */
#define	TA_WS_TRANSVCNAME	((FLDID32)197774300)	/* number: 30002140	 type: string */
#define	TA_WS_TRANGTRID	((FLDID32)197774301)	/* number: 30002141	 type: string */
#define	TA_WS_TRANCOORCONTEXT	((FLDID32)197774302)	/* number: 30002142	 type: string */
#define	TA_WS_TRANRECTYPE	((FLDID32)197774303)	/* number: 30002143	 type: string */
#define	TA_WS_TRANTIMESTAMP	((FLDID32)63556576)	/* number: 30002144	 type: long */
#define	TA_WS_TRANBRANCHES	((FLDID32)365546465)	/* number: 30002145	 type: fml32 */
#define	TA_WS_TRANBRANCH	((FLDID32)197774306)	/* number: 30002146	 type: string */

#define	TA_WS_TRANTRANID	((FLDID32)197774307)	/* number: 30002147	 type: string */
#define	TA_WS_TRANPROCESSED	((FLDID32)63556580)	/* number: 30002148	 type: long */
#define	TA_WS_INBOUNDTOTALREQNUM	((FLDID32)63556581)	/* number: 30002149	 type: long */
#define	TA_WS_INBOUNDTOTALTIME	((FLDID32)63556582)	/* number: 30002150	 type: long */
#define	TA_WS_OUTBOUNDTOTALREQNUM	((FLDID32)63556583)	/* number: 30002151	 type: long */
#define	TA_WS_OUTBOUNDTOTALTIME	((FLDID32)63556584)	/* number: 30002152	 type: long */
#define	TA_HTTP_HEADER_NAME	((FLDID32)197774313)	/* number: 30002153	 type: string */
#define	TA_HTTP_HEADER_VALUE	((FLDID32)197774314)	/* number: 30002154	 type: string */
#define	TA_SOAP_HEADER	((FLDID32)365546475)	/* number: 30002155	 type: fml32 */
#define	TA_HTTP_UNKNOWN_NAME	((FLDID32)197774316)	/* number: 30002156	 type: string */
#define	TA_HTTP_UNKNOWN_STRING_VALUE	((FLDID32)197774317)	/* number: 30002157	 type: string */
#define	TA_HTTP_UNKNOWN_CHAR_VALUE	((FLDID32)197774318)	/* number: 30002158	 type: string */
#define	TA_HTTP_UNKNOWN_INTEGER_VALUE	((FLDID32)63556591)	/* number: 30002159	 type: long */
#define	TA_HTTP_UNKNOWN_FLOAT_VALUE	((FLDID32)164219888)	/* number: 30002160	 type: double */
#define	TA_HTTP_UNKNOWN_OBJECT	((FLDID32)365546481)	/* number: 30002161	 type: fml32 */
#define	TA_WS_CONSOLE_COMMAND	((FLDID32)197774322)	/* number: 30002162	 type: string */
#define	TA_WS_CONSOLE_SUBCOMMAND	((FLDID32)197774323)	/* number: 30002163	 type: string */
#define	TA_WS_CONSOLE_JSON	((FLDID32)197774324)	/* number: 30002164	 type: string */
#define	TA_HTTP_CONNECT_URI	((FLDID32)197774325)	/* number: 30002165	 type: string */
 /* SALT MIB interface */
 /* T_WSGW class */
#define	TA_INSTANCEID	((FLDID32)197774360)	/* number: 30002200	 type: string */
#define	TA_WSATENDPOINT	((FLDID32)197774363)	/* number: 30002203	 type: string */
#define	TA_MAXTRAN	((FLDID32)63556636)	/* number: 30002204	 type: long */
#define	TA_SOCKSADDRLIST	((FLDID32)197774365)	/* number: 30002205	 type: string */
#define	TA_MAXCONTENTLEN	((FLDID32)63556638)	/* number: 30002206	 type: long */
#define	TA_THREADPOOLSIZE	((FLDID32)63556639)	/* number: 30002207	 type: long */
#define	TA_NWTIMEOUT	((FLDID32)63556640)	/* number: 30002208	 type: long */
#define	TA_MAXBACKLOG	((FLDID32)63556641)	/* number: 30002209	 type: long */
#define	TA_ENABLEMULTIENC	((FLDID32)197774370)	/* number: 30002210	 type: string */
#define	TA_ENABLESOAPVAL	((FLDID32)197774371)	/* number: 30002211	 type: string */
#define	TA_RESTHTTPADDRESS	((FLDID32)197774372)	/* number: 30002212	 type: string */
#define	TA_RESTHTTPSADDRESS	((FLDID32)197774373)	/* number: 30002213	 type: string */
#define	TA_PRIVATEKEY	((FLDID32)197774374)	/* number: 30002214	 type: string */
#define	TA_VERIFYCLIENT	((FLDID32)197774375)	/* number: 30002215	 type: string */
#define	TA_TRUSTEDCERT	((FLDID32)197774376)	/* number: 30002216	 type: string */
#define	TA_CERTPATH	((FLDID32)197774377)	/* number: 30002217	 type: string */
#define	TA_PLUGINLIBRARIES	((FLDID32)197774378)	/* number: 30002218	 type: string */
#define	TA_PLUGINPARAMS	((FLDID32)197774379)	/* number: 30002219	 type: string */
 /* T_WSWEBSERVICE class */
#define	TA_WSDFNAME	((FLDID32)197774380)	/* number: 30002220	 type: string */
#define	TA_BINDINGID	((FLDID32)197774381)	/* number: 30002221	 type: string */
#define	TA_WSDFDIRECTION	((FLDID32)197774382)	/* number: 30002222	 type: string */
#define	TA_ENDPOINTIDLIST	((FLDID32)197774383)	/* number: 30002223	 type: string */
#define	TA_WSAENDPOINT	((FLDID32)197774384)	/* number: 30002224	 type: string */
#define	TA_REALM	((FLDID32)197774385)	/* number: 30002225	 type: string */
 /* T_WSBINDING class */
#define	TA_SOAPVERSION	((FLDID32)197774386)	/* number: 30002226	 type: string */
#define	TA_SOAPSTYLE	((FLDID32)197774387)	/* number: 30002227	 type: string */
#define	TA_SOAPENCODING	((FLDID32)197774388)	/* number: 30002228	 type: string */
#define	TA_POLICIES	((FLDID32)197774389)	/* number: 30002229	 type: string */
#define	TA_ENDPOINTS	((FLDID32)197774390)	/* number: 30002230	 type: string */
 /* T_WSOPERATION class */
#define	TA_TUXEDOREF	((FLDID32)197774392)	/* number: 30002232	 type: string */
#define	TA_NAMESPACE	((FLDID32)197774393)	/* number: 30002233	 type: string */
#define	TA_SOAPACTION	((FLDID32)197774394)	/* number: 30002234	 type: string */
#define	TA_ASYNCTIMEOUT	((FLDID32)63556667)	/* number: 30002235	 type: long */
#define	TA_DISABLEWSA	((FLDID32)197774396)	/* number: 30002236	 type: string */
#define	TA_INPUTMSGNAME	((FLDID32)197774397)	/* number: 30002237	 type: string */
#define	TA_INPUTWSAACTION	((FLDID32)197774398)	/* number: 30002238	 type: string */
#define	TA_INPUTMSGHANDLER	((FLDID32)197774399)	/* number: 30002239	 type: string */
#define	TA_OUTPUTMSGNAME	((FLDID32)197774400)	/* number: 30002240	 type: string */
#define	TA_OUTPUTWSAACTION	((FLDID32)197774401)	/* number: 30002241	 type: string */
#define	TA_OUTPUTMSGHANDLER	((FLDID32)197774402)	/* number: 30002242	 type: string */
#define	TA_ERRMSGNAME	((FLDID32)197774403)	/* number: 30002243	 type: string */
#define	TA_ERRWSAACTION	((FLDID32)197774404)	/* number: 30002244	 type: string */
#define	TA_ERRMSGHANDLER	((FLDID32)197774405)	/* number: 30002245	 type: string */
 /* T_WSREST class */
#define	TA_HTTPSVCNAME	((FLDID32)197774406)	/* number: 30002246	 type: string */
#define	TA_HTTPDIRECTION	((FLDID32)197774407)	/* number: 30002247	 type: string */
#define	TA_HTTPMETHOD	((FLDID32)197774408)	/* number: 30002248	 type: string */
#define	TA_HTTPOUTBUF	((FLDID32)197774409)	/* number: 30002249	 type: string */
#define	TA_HTTPOUTADDRESS	((FLDID32)197774410)	/* number: 30002250	 type: string */
#define	TA_HTTPCONTENTTYPE	((FLDID32)197774411)	/* number: 30002251	 type: string */
#define	TA_HTTPPOSTSERVICE	((FLDID32)197774412)	/* number: 30002252	 type: string */
#define	TA_HTTPPOSTINBUF	((FLDID32)197774413)	/* number: 30002253	 type: string */
#define	TA_HTTPPOSTTUXREF	((FLDID32)197774414)	/* number: 30002254	 type: string */
#define	TA_HTTPGETSERVICE	((FLDID32)197774415)	/* number: 30002255	 type: string */
#define	TA_HTTPGETINBUF	((FLDID32)197774416)	/* number: 30002256	 type: string */
#define	TA_HTTPGETTUXREF	((FLDID32)197774417)	/* number: 30002257	 type: string */
#define	TA_HTTPPUTSERVICE	((FLDID32)197774418)	/* number: 30002258	 type: string */
#define	TA_HTTPPUTINBUF	((FLDID32)197774419)	/* number: 30002259	 type: string */
#define	TA_HTTPPUTTUXREF	((FLDID32)197774420)	/* number: 30002260	 type: string */
#define	TA_HTTPDELETESERVICE	((FLDID32)197774421)	/* number: 30002261	 type: string */
#define	TA_HTTPDELETEINBUF	((FLDID32)197774422)	/* number: 30002262	 type: string */
#define	TA_HTTPDELETETUXREF	((FLDID32)197774423)	/* number: 30002263	 type: string */
#define	TA_HTTP_DONE	((FLDID32)63556696)	/* number: 30002264	 type: long */
#define	TA_HTTP_FAIL	((FLDID32)63556697)	/* number: 30002265	 type: long */
#define	TA_HTTP_TIME	((FLDID32)63556698)	/* number: 30002266	 type: long */
#define	TA_HTTPINBOUNDDONE	((FLDID32)63556699)	/* number: 30002267	 type: long */
#define	TA_HTTPINBOUNDFAIL	((FLDID32)63556700)	/* number: 30002268	 type: long */
#define	TA_HTTPINBOUNDTIME	((FLDID32)63556701)	/* number: 30002269	 type: long */
#define	TA_HTTPOUTBOUNDDONE	((FLDID32)63556702)	/* number: 30002270	 type: long */
#define	TA_HTTPOUTBOUNDFAIL	((FLDID32)63556703)	/* number: 30002271	 type: long */
#define	TA_HTTPOUTBOUNDTIME	((FLDID32)63556704)	/* number: 30002272	 type: long */
/* ------------------------------------------------------------
 * 30002401 - 30002600 is reserved for performance metrics
 * When new fields added, update them to
 * tmmon_parse.c:fields_array and LMS.c:fldmaps
 */
#define	TA_MONCORRID	((FLDID32)197774561)	/* number: 30002401	 type: string */
#define	TA_MONMSGTYPE	((FLDID32)197774562)	/* number: 30002402	 type: string */
#define	TA_MONMSGSIZE	((FLDID32)63556835)	/* number: 30002403	 type: long */
#define	TA_MONSTAGE	((FLDID32)197774564)	/* number: 30002404	 type: string */
#define	TA_MONLOCATION	((FLDID32)197774565)	/* number: 30002405	 type: string */
#define	TA_MONMSGQUEUED	((FLDID32)63556838)	/* number: 30002406	 type: long */
#define	TA_MONNUMPEND	((FLDID32)63556839)	/* number: 30002407	 type: long */
#define	TA_MONBYTESPEND	((FLDID32)63556840)	/* number: 30002408	 type: long */
#define	TA_MONLASTTIMESEC	((FLDID32)63556841)	/* number: 30002409	 type: long */
#define	TA_MONLASTTIMEUSEC	((FLDID32)63556842)	/* number: 30002410	 type: long */
#define	TA_MONSTARTTIMESEC	((FLDID32)63556843)	/* number: 30002411	 type: long */
#define	TA_MONSTARTTIMEUSEC	((FLDID32)63556844)	/* number: 30002412	 type: long */
#define	TA_MONMSGWAITTIME	((FLDID32)63556845)	/* number: 30002413	 type: long */
#define	TA_MONTOTALTIME	((FLDID32)63556846)	/* number: 30002414	 type: long */
#define	TA_MONMSGID	((FLDID32)197774575)	/* number: 30002415	 type: string */
#define	TA_MONOPERATION	((FLDID32)197774576)	/* number: 30002416	 type: string */
#define	TA_MONLGTRID	((FLDID32)197774577)	/* number: 30002417	 type: string */
#define	TA_MONRGTRID	((FLDID32)197774578)	/* number: 30002418	 type: string */
#define	TA_MONCLTADDR	((FLDID32)197774579)	/* number: 30002419	 type: string */
#define	TA_MONDEPTH	((FLDID32)30002420)	/* number: 30002420	 type: short */
#define	TA_MONERRNO	((FLDID32)63556853)	/* number: 30002421	 type: long */
#define	TA_MONNUMCLTS	((FLDID32)30002422)	/* number: 30002422	 type: short */
#define	TA_MONACCNUMCLTS	((FLDID32)30002423)	/* number: 30002423	 type: short */
#define	TA_MONNUMNETCLTS	((FLDID32)30002424)	/* number: 30002424	 type: short */
#define	TA_MONLINKADDR	((FLDID32)197774585)	/* number: 30002425	 type: string */
#define	TA_MONLINKSTATUS	((FLDID32)30002426)	/* number: 30002426	 type: short */
#define	TA_MONSVCNAME	((FLDID32)197774587)	/* number: 30002427	 type: string */
#define	TA_MONHOSTSVC	((FLDID32)197774588)	/* number: 30002428	 type: string */
#define	TA_MONSTATUS	((FLDID32)30002429)	/* number: 30002429	 type: short */
#define	TA_MONRMID	((FLDID32)63556862)	/* number: 30002430	 type: long */
#define	TA_MONXANAME	((FLDID32)197774591)	/* number: 30002431	 type: string */
#define	TA_MONEXECTIME	((FLDID32)63556864)	/* number: 30002432	 type: long */
#define	TA_MONXACODE	((FLDID32)63556865)	/* number: 30002433	 type: long */
#define	TA_MONACCNUM	((FLDID32)63556866)	/* number: 30002434	 type: long */
#define	TA_MONACCBYTES	((FLDID32)63556867)	/* number: 30002435	 type: long */
#define	TA_MONHOSTID	((FLDID32)63556868)	/* number: 30002436	 type: long */
#define	TA_MONLDOM	((FLDID32)197774597)	/* number: 30002437	 type: string */
#define	TA_MONLINKNUM	((FLDID32)30002438)	/* number: 30002438	 type: short */
#define	TA_MONSVCSEQ	((FLDID32)197774599)	/* number: 30002439	 type: string */
#define	TA_MONPSVCSEQ	((FLDID32)197774600)	/* number: 30002440	 type: string */
#define	TA_MONNUMWAITRPLY	((FLDID32)63556873)	/* number: 30002441	 type: long */
#define	TA_MONCALLFLAG	((FLDID32)63556874)	/* number: 30002442	 type: long */
#define	TA_MONCALLMODE	((FLDID32)30002443)	/* number: 30002443	 type: short */
#define	TA_MONQID	((FLDID32)197774604)	/* number: 30002444	 type: string */
#define	TA_MONPROCTYPE	((FLDID32)30002445)	/* number: 30002445	 type: short */
#define	TA_MONURCODE	((FLDID32)63556878)	/* number: 30002446	 type: long */
#define	TA_MONELAPSETIME	((FLDID32)63556879)	/* number: 30002447	 type: long */
#define	TA_MONWSENDPOINT	((FLDID32)197774608)	/* number: 30002448	 type: string */
#define	TA_MONRDOM	((FLDID32)197774609)	/* number: 30002449	 type: string */
/* ------------------------------------------------------------
 * Tuxedo 12g new metrics fields for TMIB.
 */
#define	TA_MONINBOUNDNUM	((FLDID32)63556882)	/* number: 30002450	 type: long */
#define	TA_MONINBOUNDBYT	((FLDID32)63556883)	/* number: 30002451	 type: long */
#define	TA_MONOUTBOUNDNUM	((FLDID32)63556884)	/* number: 30002452	 type: long */
#define	TA_MONOUTBOUNDBYT	((FLDID32)63556885)	/* number: 30002453	 type: long */
#define	TA_MSGTAG	((FLDID32)197774614)	/* number: 30002454	 type: string */
/* ------------------------------------------------------------
 * Following fields are already defined
 * in monflds file.
 */
/* T_WS_GWWS */
#define	TA_MONINRPCSUCC	((FLDID32)63556918)	/* number: 30002486	 type: long */
#define	TA_MONINRPCFAIL	((FLDID32)63556919)	/* number: 30002487	 type: long */
#define	TA_MONINOWSUCC	((FLDID32)63556920)	/* number: 30002488	 type: long */
#define	TA_MONINOWFAIL	((FLDID32)63556921)	/* number: 30002489	 type: long */
#define	TA_MONOUTRPCSUCC	((FLDID32)63556922)	/* number: 30002490	 type: long */
#define	TA_MONOUTRPCFAIL	((FLDID32)63556923)	/* number: 30002491	 type: long */
#define	TA_MONOUTOWSUCC	((FLDID32)63556924)	/* number: 30002492	 type: long */
#define	TA_MONOUTOWFAIL	((FLDID32)63556925)	/* number: 30002493	 type: long */
#define	TA_MONINTIME	((FLDID32)63556926)	/* number: 30002494	 type: long */
#define	TA_MONOUTTIME	((FLDID32)63556927)	/* number: 30002495	 type: long */
#define	TA_MONTHRNUM	((FLDID32)30002496)	/* number: 30002496	 type: short */
#define	TA_MONGWWSID	((FLDID32)197774657)	/* number: 30002497	 type: string */
#define	TA_MONINBOUNDPEND	((FLDID32)63556930)	/* number: 30002498	 type: long */
#define	TA_MONOUTBOUNDPEND	((FLDID32)63556931)	/* number: 30002499	 type: long */
#define	TA_MONINREQNUM	((FLDID32)63556944)	/* number: 30002512	 type: long */
#define	TA_MONINTALTIME	((FLDID32)63556945)	/* number: 30002513	 type: long */
#define	TA_MONOUTREQNUM	((FLDID32)63556946)	/* number: 30002514	 type: long */
#define	TA_MONOUTALTIME	((FLDID32)63556947)	/* number: 30002515	 type: long */
/* ------------------------------------------------------------
 * TSAM defines from 60 to 102, so new metrics fields
 * start from 120 to keep some room for maintain purpose.
 */
/* T_EVENT_MON */
#define	TA_MONEVTCLASS	((FLDID32)197774681)	/* number: 30002521	 type: string */
#define	TA_MONEVTNONTXNUM	((FLDID32)63556954)	/* number: 30002522	 type: long */
#define	TA_MONEVTTXNUM	((FLDID32)63556955)	/* number: 30002523	 type: long */
#define	TA_MONEVTSVCNUM	((FLDID32)63556956)	/* number: 30002524	 type: long */
#define	TA_MONEVTUNSOLNUM	((FLDID32)63556957)	/* number: 30002525	 type: long */
#define	TA_MONEVTQUENUM	((FLDID32)63556958)	/* number: 30002526	 type: long */
#define	TA_MONEVTCMDNUM	((FLDID32)63556959)	/* number: 30002527	 type: long */
#define	TA_MONEVTULOGNUM	((FLDID32)63556960)	/* number: 30002528	 type: long */
/* T_MACHINE */
#define	TA_HWJOLTCLIENTS	((FLDID32)63556961)	/* number: 30002529	 type: long */
#define	TA_HWSALTCLIENTS	((FLDID32)63556962)	/* number: 30002530	 type: long */
#define	TA_HWICECLIENTS	((FLDID32)63556963)	/* number: 30002531	 type: long */
#define	TA_HWARTCLIENTS	((FLDID32)63556964)	/* number: 30002532	 type: long */
#define	TA_CURJOLTCLIENTS	((FLDID32)63556965)	/* number: 30002533	 type: long */
#define	TA_CURSALTCLIENTS	((FLDID32)63556966)	/* number: 30002534	 type: long */
#define	TA_CURARTCLIENTS	((FLDID32)63556967)	/* number: 30002535	 type: long */
#define	TA_CURICECLIENTS	((FLDID32)63556968)	/* number: 30002536	 type: long */

#define	TA_MONFLAGS	((FLDID32)63556969)	/* number: 30002537	 type: long */
/* T_SVCGRP */
#define	TA_TOTEXECTIMESEC	((FLDID32)63556970)	/* number: 30002538	 type: long */
#define	TA_TOTEXECTIMEUSEC	((FLDID32)63556971)	/* number: 30002539	 type: long */
#define	TA_TOTCPUTIMESEC	((FLDID32)63556972)	/* number: 30002540	 type: long */
#define	TA_TOTCPUTIMEUSEC	((FLDID32)63556973)	/* number: 30002541	 type: long */
#define	TA_TOTUSRCPUTIMESEC	((FLDID32)63556974)	/* number: 30002542	 type: long */
#define	TA_TOTUSRCPUTIMEUSEC	((FLDID32)63556975)	/* number: 30002543	 type: long */
#define	TA_TOTSYSCPUTIMESEC	((FLDID32)63556976)	/* number: 30002544	 type: long */
#define	TA_TOTSYSCPUTIMEUSEC	((FLDID32)63556977)	/* number: 30002545	 type: long */
#define	TA_TOTMSGSIZEMEGA	((FLDID32)63556978)	/* number: 30002546	 type: long */
#define	TA_TOTMSGSIZEBYTE	((FLDID32)63556979)	/* number: 30002547	 type: long */
#define	TA_MAXEXECTIMESEC	((FLDID32)63556980)	/* number: 30002548	 type: long */
#define	TA_MAXEXECTIMEUSEC	((FLDID32)63556981)	/* number: 30002549	 type: long */
#define	TA_MINEXECTIMESEC	((FLDID32)63556982)	/* number: 30002550	 type: long */
#define	TA_MINEXECTIMEUSEC	((FLDID32)63556983)	/* number: 30002551	 type: long */
#define	TA_MAXCPUTIMESEC	((FLDID32)63556984)	/* number: 30002552	 type: long */
#define	TA_MAXCPUTIMEUSEC	((FLDID32)63556985)	/* number: 30002553	 type: long */
#define	TA_MAXUSRCPUTIMESEC	((FLDID32)63556986)	/* number: 30002554	 type: long */
#define	TA_MAXUSRCPUTIMEUSEC	((FLDID32)63556987)	/* number: 30002555	 type: long */
#define	TA_MAXSYSCPUTIMESEC	((FLDID32)63556988)	/* number: 30002556	 type: long */
#define	TA_MAXSYSCPUTIMEUSEC	((FLDID32)63556989)	/* number: 30002557	 type: long */
#define	TA_MINCPUTIMESEC	((FLDID32)63556990)	/* number: 30002558	 type: long */
#define	TA_MINCPUTIMEUSEC	((FLDID32)63556991)	/* number: 30002559	 type: long */
#define	TA_MINUSRCPUTIMESEC	((FLDID32)63556992)	/* number: 30002560	 type: long */
#define	TA_MINUSRCPUTIMEUSEC	((FLDID32)63556993)	/* number: 30002561	 type: long */
#define	TA_MINSYSCPUTIMESEC	((FLDID32)63556994)	/* number: 30002562	 type: long */
#define	TA_MINSYSCPUTIMEUSEC	((FLDID32)63556995)	/* number: 30002563	 type: long */
#define	TA_MAXMSGBSIZE	((FLDID32)63556996)	/* number: 30002564	 type: long */
#define	TA_MINMSGBSIZE	((FLDID32)63556997)	/* number: 30002565	 type: long */
#define	TA_TOTSUCCNUM	((FLDID32)63556998)	/* number: 30002566	 type: long */
#define	TA_TOTSFAILNUM	((FLDID32)63556999)	/* number: 30002567	 type: long */
#define	TA_TOTUFAILNUM	((FLDID32)63557000)	/* number: 30002568	 type: long */
/* T_QUEUE */
#define	TA_TOTMSGWAITTIMESEC	((FLDID32)63557001)	/* number: 30002569	 type: long */
#define	TA_TOTMSGWAITTIMEUSEC	((FLDID32)63557002)	/* number: 30002570	 type: long */
#define	TA_MAXMSGWAITTIMESEC	((FLDID32)63557003)	/* number: 30002571	 type: long */
#define	TA_MAXMSGWAITTIMEUSEC	((FLDID32)63557004)	/* number: 30002572	 type: long */
#define	TA_MINMSGWAITTIMESEC	((FLDID32)63557005)	/* number: 30002573	 type: long */
#define	TA_MINMSGWAITTIMEUSEC	((FLDID32)63557006)	/* number: 30002574	 type: long */

#define	TA_TOTNUMCONV	((FLDID32)63557007)	/* number: 30002575	 type: long */
#define	TA_TOTNUMREQ	((FLDID32)63557008)	/* number: 30002576	 type: long */
#define	TA_TOTNUMENQUEUE	((FLDID32)63557009)	/* number: 30002577	 type: long */
#define	TA_TOTNUMDEQUEUE	((FLDID32)63557010)	/* number: 30002578	 type: long */
#define	TA_TOTNUMPOST	((FLDID32)63557011)	/* number: 30002579	 type: long */
#define	TA_TOTNUMSUBSCRIBE	((FLDID32)63557012)	/* number: 30002580	 type: long */
 /* SERVICE */
#define	TA_LASTEXECTIMESEC	((FLDID32)63557013)	/* number: 30002581	 type: long */
#define	TA_LASTEXECTIMEUSEC	((FLDID32)63557014)	/* number: 30002582	 type: long */
#define	TA_LASTCPUTIMESEC	((FLDID32)63557015)	/* number: 30002583	 type: long */
#define	TA_LASTCPUTIMEUSEC	((FLDID32)63557016)	/* number: 30002584	 type: long */
#define	TA_LASTUSRCPUTIMESEC	((FLDID32)63557017)	/* number: 30002585	 type: long */
#define	TA_LASTUSRCPUTIMEUSEC	((FLDID32)63557018)	/* number: 30002586	 type: long */
#define	TA_LASTSYSCPUTIMESEC	((FLDID32)63557019)	/* number: 30002587	 type: long */
#define	TA_LASTSYSCPUTIMEUSEC	((FLDID32)63557020)	/* number: 30002588	 type: long */
#define	TA_LASTMSGBSIZE	((FLDID32)63557021)	/* number: 30002589	 type: long */
 /* T_BRIDGE etc. NETLINK */
#define	TA_SENDNUM	((FLDID32)63557022)	/* number: 30002590	 type: long */
#define	TA_SENDBYT	((FLDID32)63557023)	/* number: 30002591	 type: long */
 /* for payload monitor, it already defined on monflds */
#define	TA_PAYLOAD_STORAGE	((FLDID32)197774752)	/* number: 30002592	 type: string */
#define	TA_HADOOP_URL	((FLDID32)197774753)	/* number: 30002593	 type: string */
#define	TA_MON_PAYLOAD_TYPE	((FLDID32)30002594)	/* number: 30002594	 type: short */
#define	TA_REQUEST_PROPAGATION	((FLDID32)197774755)	/* number: 30002595	 type: string */
#define	TA_LOAD_REQUEST_FILTER	((FLDID32)197774756)	/* number: 30002596	 type: string */
#define	TA_REPLY_PROPAGATION	((FLDID32)197774757)	/* number: 30002597	 type: string */
#define	TA_LOAD_REPLY_FILTER	((FLDID32)197774758)	/* number: 30002598	 type: string */
#define	TA_THREADID_STR	((FLDID32)197774759)	/* number: 30002599	 type: string */
#define	TA_BLOB_SIZE	((FLDID32)63557032)	/* number: 30002600	 type: long */
#define	TA_LOAD_REPLY_EVALUATER	((FLDID32)197774761)	/* number: 30002601	 type: string */
#define	TA_LOAD_REQUEST_EVALUATER	((FLDID32)197774762)	/* number: 30002602	 type: string */
#define	TA_INTERNAL_STRING	((FLDID32)197774763)	/* number: 30002603	 type: string */
#define	TA_HADOOP_PRINCIPAL	((FLDID32)197774764)	/* number: 30002604	 type: string */
#define	TA_HADOOP_CREDENTIAL	((FLDID32)197774765)	/* number: 30002605	 type: string */
#define	TA_MONSCDCONF	((FLDID32)197774766)	/* number: 30002606	 type: string */
#define	TA_MONPAYLOADSTASTIC	((FLDID32)197774767)	/* number: 30002607	 type: string */
#define	TA_PAYLOADSCD	((FLDID32)63557040)	/* number: 30002608	 type: long */
#define	TA_SIZEPOST	((FLDID32)63557041)	/* number: 30002609	 type: long */
/* ------------------------------------------------------------
 * Tuxedo 10.1 reserved fields, from 30002601 to 30002700.
 */
#define	TA_SVCCONTRACT	((FLDID32)365546921)	/* number: 30002601	 type: fml32 */
/* ------------------------------------------------------------
 * Tuxedo 11g reserved fields, from 30002701 to 30002800.
 */
#define	TA_AFFSCOPE	((FLDID32)30002701)	/* number: 30002701	 type: short */
#define	TA_AFFROLE	((FLDID32)30002702)	/* number: 30002702	 type: short */
#define	TA_AFFSTRICT	((FLDID32)30002703)	/* number: 30002703	 type: short */
#define	TA_AFFDEPTH	((FLDID32)30002704)	/* number: 30002704	 type: short */
#define	TA_AFFMACHINEID	((FLDID32)63557137)	/* number: 30002705	 type: long */
#define	TA_AFFGROUPID	((FLDID32)30002706)	/* number: 30002706	 type: short */
#define	TA_AFFSERVERID	((FLDID32)30002707)	/* number: 30002707	 type: short */
#define	TA_AFFQUEUEID	((FLDID32)63557140)	/* number: 30002708	 type: long */
#define	TA_AFFRDOMNAME	((FLDID32)197774869)	/* number: 30002709	 type: string */
#define	TA_AFFINITYSCOPE	((FLDID32)197774870)	/* number: 30002710	 type: string */
#define	TA_AFFINITYROLE	((FLDID32)197774871)	/* number: 30002711	 type: string */
#define	TA_AFFINITYSTRICT	((FLDID32)197774872)	/* number: 30002712	 type: string */
#define	TA_LINK_GROUP_NUMBER	((FLDID32)197774873)	/* number: 30002713	 type: string */
#define	TA_LINK_FILLER1	((FLDID32)30002714)	/* number: 30002714	 type: short */
#define	TA_LINK_OS_TYPE	((FLDID32)97111579)	/* number: 30002715	 type: char */
#define	TA_LINK_FILLER2	((FLDID32)97111580)	/* number: 30002716	 type: char */
#define	TA_EVENT_LINK_NOTIFICATION	((FLDID32)365547037)	/* number: 30002717	 type: fml32 */
#define	TA_TMMQI_SUSTHRESH	((FLDID32)63557150)	/* number: 30002718	 type: long */
#define	TA_TMMQI_ERRORCOUNT	((FLDID32)63557151)	/* number: 30002719	 type: long */
#define	TA_METAMSG_FLAG	((FLDID32)63557152)	/* number: 30002720	 type: long */
/* ------------------------------------------------------------
 * ExLogic reserved fields, from 30002801 to 30002900.
 */
#define	TA_RDMASENDERRPQKEY	((FLDID32)63557233)	/* number: 30002801	 type: long */
#define	TA_RDMASENDERRPQNAME	((FLDID32)231329394)	/* number: 30002802	 type: carray */
#define	TA_RDMARCVRRPQKEY	((FLDID32)63557235)	/* number: 30002803	 type: long */
#define	TA_RDMARCVRRPQNAME	((FLDID32)231329396)	/* number: 30002804	 type: carray */
#define	TA_RDMARPLYTORPQKEY	((FLDID32)63557237)	/* number: 30002805	 type: long */
#define	TA_RDMARPLYTORPQNAME	((FLDID32)231329398)	/* number: 30002806	 type: carray */
#define	TA_SPINTUNING_MINIDLECPU	((FLDID32)30002807)	/* number: 30002807	 type: short */
#define	TA_SPINTUNING_FACTOR	((FLDID32)30002808)	/* number: 30002808	 type: short */
#define	TA_RDMADAEMONIP	((FLDID32)197774969)	/* number: 30002809	 type: string */
#define	TA_RDMADAEMONPORT	((FLDID32)30002810)	/* number: 30002810	 type: short */
#define	TA_RDMAQSIZE	((FLDID32)63557243)	/* number: 30002811	 type: long */
#define	TA_RDMAQENTRIES	((FLDID32)63557244)	/* number: 30002812	 type: long */
#define	TA_IPCRPLYTORPQID	((FLDID32)63557245)	/* number: 30002813	 type: long */
#define	TA_EXALOGIC_MSGQ_CACHE_SIZE	((FLDID32)30002814)	/* number: 30002814	 type: short */
#define	TA_EXALOGIC_SHARED_PATH	((FLDID32)197774975)	/* number: 30002815	 type: string */
#define	TA_SHMMSGLISTS	((FLDID32)197774976)	/* number: 30002816	 type: string */
#define	TA_RDMA_DAEMON_PID	((FLDID32)63557249)	/* number: 30002817	 type: long */
#define	TA_SHMQMAXMEM	((FLDID32)63557250)	/* number: 30002818	 type: long */
#define	TA_SHMQSTAT	((FLDID32)197774979)	/* number: 30002819	 type: string */
#define	TA_MSG_SHMQNUM	((FLDID32)63557252)	/* number: 30002820	 type: long */
/* ------------------------------------------------------------
 * Tuxedo 12c reserved fields, from 30002901 to 30003000.
 */
#define	TA_ECID	((FLDID32)197775061)	/* number: 30002901	 type: string */
#define	TA_DYNWKQUEUED_USEC	((FLDID32)63557334)	/* number: 30002902	 type: long */
#define	TA_QINDEX	((FLDID32)63557335)	/* number: 30002903	 type: long */
#define	TA_Q_NUM	((FLDID32)63557336)	/* number: 30002904	 type: long */
#define	TA_DYNLOAD_USEC	((FLDID32)63557338)	/* number: 30002906	 type: long */
#define	TA_SVCINDEX	((FLDID32)63557339)	/* number: 30002907	 type: long */
#define	TA_SVC_NUM	((FLDID32)63557340)	/* number: 30002908	 type: long */
#define	TA_LMS_TYPE	((FLDID32)63557341)	/* number: 30002909	 type: long */
#define	TA_SYN_SEQ	((FLDID32)63557342)	/* number: 30002910	 type: long */
#define	TA_DRB	((FLDID32)197775071)	/* number: 30002911	 type: string */
#define	TA_DRB_SYNC_INTERVAL	((FLDID32)30002912)	/* number: 30002912	 type: short */
#define	TA_DBBLFAILOVER	((FLDID32)30002913)	/* number: 30002913	 type: short */
#define	TA_SGRPFAILOVER	((FLDID32)30002914)	/* number: 30002914	 type: short */
#define	TA_OUTSREQNUM	((FLDID32)63557347)	/* number: 30002915	 type: long */
#define	TA_IMPORTEDFORMATID	((FLDID32)63557348)	/* number: 30002916	 type: long */
#define	TA_IMPORTEDGTRID	((FLDID32)231329509)	/* number: 30002917	 type: carray */
#define	TA_IMPORTEDBQUAL	((FLDID32)231329510)	/* number: 30002918	 type: carray */
#define	TA_SCANUNIT_MSEC	((FLDID32)63557351)	/* number: 30002919	 type: long */
#define	TA_TIMELEFT_MSEC	((FLDID32)63557352)	/* number: 30002920	 type: long */
#define	TA_VERSION_RANGE	((FLDID32)197775081)	/* number: 30002921	 type: string */
#define	TA_REQUEST_VERSION	((FLDID32)197775082)	/* number: 30002922	 type: string */
#define	TA_VERSION_POLICY	((FLDID32)197775083)	/* number: 30002923	 type: string */
#define	TA_SCANUNIT_EXT	((FLDID32)197775084)	/* number: 30002924	 type: string */
#define	TA_TRANAFFINITYCONTEXT	((FLDID32)63557357)	/* number: 30002925	 type: long */
#define	TA_TRANAFFINITYCONTEXT_STR	((FLDID32)197775086)	/* number: 30002926	 type: string */
#define	TA_RMOPTIONS	((FLDID32)197775087)	/* number: 30002927	 type: string */
#define	TA_TRAN_COORDINSTID	((FLDID32)63557360)	/* number: 30002928	 type: long */
#define	TA_TRAN_COORDINSTSTR	((FLDID32)197775089)	/* number: 30002929	 type: string */
#define	TA_TRAN_CGROUP	((FLDID32)63557362)	/* number: 30002930	 type: long */
#define	TA_TRAN_GWTCOMMXID	((FLDID32)97111795)	/* number: 30002931	 type: char */
#define	TA_TRANAFFINITYGRPID	((FLDID32)63557364)	/* number: 30002932	 type: long */
#define	TA_BYASSDOMSEQNUM	((FLDID32)231329525)	/* number: 30002933	 type: carray */
#define	TA_LOGCOORDMSEQNUM	((FLDID32)63557366)	/* number: 30002934	 type: long */
#define	TA_INSTSTR	((FLDID32)197775095)	/* number: 30002935	 type: string */
#define	TA_TRAN_GBRANCHID	((FLDID32)30002936)	/* number: 30002936	 type: short */
#define	TA_EXTSVC_PRMID	((FLDID32)63557369)	/* number: 30002937	 type: long */
#define	TA_EXTSVC_PRGRPID	((FLDID32)63557370)	/* number: 30002938	 type: long */
#define	TA_EXTSVC_PRSRVID	((FLDID32)63557371)	/* number: 30002939	 type: long */
#define	TA_EXTSVC_QNAME	((FLDID32)231329532)	/* number: 30002940	 type: carray */
#define	TA_EXTSVC_RDOM	((FLDID32)197775101)	/* number: 30002941	 type: string */
#define	TA_EXTSVC_MAGIC	((FLDID32)63557374)	/* number: 30002942	 type: long */
#define	TA_EXTSVC_RTNAME	((FLDID32)197775103)	/* number: 30002943	 type: string */
#define	TA_TMFAN_TRACE	((FLDID32)197775104)	/* number: 30002944	 type: string */
#define	TA_TMFAN_THRESHOLD	((FLDID32)30002945)	/* number: 30002945	 type: short */
#define	TA_TMFAN_CLRDBNAME	((FLDID32)197775106)	/* number: 30002946	 type: string */
#define	TA_TMFAN_CLRSVCNAME	((FLDID32)197775107)	/* number: 30002947	 type: string */
#define	TA_KEY_ID	((FLDID32)197775108)	/* number: 30002948	 type: string */
#define	TA_ENCRYPTSTRING	((FLDID32)197775109)	/* number: 30002949	 type: string */
#define	TA_LLR_SRVID	((FLDID32)30002950)	/* number: 30002950	 type: short */
#define	TA_LLR_GROUPID	((FLDID32)30002951)	/* number: 30002951	 type: short */
#define	TA_LLR_MCHID	((FLDID32)63557384)	/* number: 30002952	 type: long */
#define	TA_LLR_ADDR	((FLDID32)63557385)	/* number: 30002953	 type: long */
#define	TA_LLR_REPLYTYPE	((FLDID32)63557386)	/* number: 30002954	 type: long */
#define	TA_LLR_REPLYITER	((FLDID32)63557387)	/* number: 30002955	 type: long */
#define	TA_LLR_DELSWAPSIZE	((FLDID32)63557388)	/* number: 30002956	 type: long */
#define	TA_LLR_DELDELAY	((FLDID32)63557389)	/* number: 30002957	 type: long */
#define	TA_LLR_TLOGDATA	((FLDID32)231329550)	/* number: 30002958	 type: carray */
#define	TA_LLR_TLOGOWNER	((FLDID32)197775119)	/* number: 30002959	 type: string */
/* ------------------------------------------------------------
 * ART 12c reserved fields, from 30003001 to 30004000.
 */
/* ------------------------------------------------------------
 * Tuxedo 12cR3 (12.2.2.0) reserved fields.
 * From 30004001 to 30004100.
 */
#define	TA_TDC_CMD	((FLDID32)197776161)	/* number: 30004001	 type: string */
#define	TA_TDC_CACHENAME	((FLDID32)197776162)	/* number: 30004002	 type: string */
#define	TA_TDC_KEYTYPE	((FLDID32)97112867)	/* number: 30004003	 type: char */
#define	TA_TDC_KEY_STR	((FLDID32)197776164)	/* number: 30004004	 type: string */
#define	TA_TDC_KEY_BIN	((FLDID32)231330597)	/* number: 30004005	 type: carray */
#define	TA_TDC_VALTYPE	((FLDID32)97112870)	/* number: 30004006	 type: char */
#define	TA_TDC_VAL_STR	((FLDID32)197776167)	/* number: 30004007	 type: string */
#define	TA_TDC_VAL_BIN	((FLDID32)231330600)	/* number: 30004008	 type: carray */
#define	TA_DEPENDSON	((FLDID32)197776169)	/* number: 30004009	 type: string */
#define	TA_ORAREQBOUNDARY	((FLDID32)197776170)	/* number: 30004010	 type: string */
#define	TA_CACHINGNAME	((FLDID32)197776171)	/* number: 30004011	 type: string */
#define	TA_CACHING_NAME	((FLDID32)197776172)	/* number: 30004012	 type: string */
#define	TA_CACHING_CACHENAME	((FLDID32)197776173)	/* number: 30004013	 type: string */
#define	TA_CACHING_KEY	((FLDID32)197776174)	/* number: 30004014	 type: string */
#define	TA_CACHING_KEY_BUFTYPE	((FLDID32)197776175)	/* number: 30004015	 type: string */
#define	TA_CACHING_KEY_FIELD	((FLDID32)197776176)	/* number: 30004016	 type: string */
#define	TA_SECONDARYRQ	((FLDID32)197776177)	/* number: 30004017	 type: string */
#define	TA_TDC_KEYFORMAT	((FLDID32)197776178)	/* number: 30004018	 type: string */
#define	TA_TDC_PROPERTY	((FLDID32)197776179)	/* number: 30004019	 type: string */
#define	TA_TDC_PRO_UPDATEMODE	((FLDID32)197776180)	/* number: 30004020	 type: string */
#define	TA_TDC_ERR_DESCRIPTION	((FLDID32)197776181)	/* number: 30004021	 type: string */
#define	TA_DMSSL_VERSION	((FLDID32)197776182)	/* number: 30004022	 type: string */
#define	TA_PARALLEL_NUM	((FLDID32)63558455)	/* number: 30004023	 type: long */
/* ------------------------------------------------------------
 * TJS Reserved Fields. From 30004101 to 30004399.
 * TuxJS/include/tmjflds
 * Remember to keep the consistence for ever between the two
 * files tmjflds and tpadm when any of it is changed!!!
 */
#ifndef TA_TJS_CLASS
#define	TA_TJS_CLASS	((FLDID32)197776261)	/* number: 30004101	 type: string */
#define	TA_TJS_COMMAND	((FLDID32)197776262)	/* number: 30004102	 type: string */
#define	TA_TJS_SUBCOMMAND	((FLDID32)197776263)	/* number: 30004103	 type: string */
#define	TA_TJS_MODULE	((FLDID32)197776264)	/* number: 30004104	 type: string */
#define	TA_TJS_MODULENO	((FLDID32)63558537)	/* number: 30004105	 type: long */
#define	TA_TJS_MODULEDEFVER	((FLDID32)197776266)	/* number: 30004106	 type: string */
#define	TA_TJS_MODULECONFIG	((FLDID32)197776267)	/* number: 30004107	 type: string */
#define	TA_TJS_MODULEDEFINE	((FLDID32)197776268)	/* number: 30004108	 type: string */
#define	TA_TJS_MODULEDEFFILE	((FLDID32)197776269)	/* number: 30004109	 type: string */
#define	TA_TJS_CLASSPATH	((FLDID32)197776270)	/* number: 30004110	 type: string */
#define	TA_TJS_SERVERCLOPT	((FLDID32)197776271)	/* number: 30004111	 type: string */
#define	TA_TJS_CLASSNAME	((FLDID32)197776272)	/* number: 30004112	 type: string */
#define	TA_TJS_SERVCLSSAUTOADV	((FLDID32)197776273)	/* number: 30004113	 type: string */
#define	TA_TJS_METHODNAME	((FLDID32)197776274)	/* number: 30004114	 type: string */
#define	TA_TJS_MAXMODULENUM	((FLDID32)63558547)	/* number: 30004115	 type: long */
#define	TA_TJS_TOTMODULENUM	((FLDID32)63558548)	/* number: 30004116	 type: long */
#define	TA_TJS_MAXADMCMDNUM	((FLDID32)63558549)	/* number: 30004117	 type: long */
#define	TA_TJS_TOTADMCMDNUM	((FLDID32)63558550)	/* number: 30004118	 type: long */
#define	TA_TJS_MAXSVRCLSNUM	((FLDID32)63558551)	/* number: 30004119	 type: long */
#define	TA_TJS_TOTSVRCLSNUM	((FLDID32)63558552)	/* number: 30004120	 type: long */
#define	TA_TJS_MAXADMCLSNUM	((FLDID32)63558553)	/* number: 30004121	 type: long */
#define	TA_TJS_TOTADMCLSNUM	((FLDID32)63558554)	/* number: 30004122	 type: long */
#define	TA_TJS_MAXSVCNUM	((FLDID32)63558555)	/* number: 30004123	 type: long */
#define	TA_TJS_TOTSVCNUM	((FLDID32)63558556)	/* number: 30004124	 type: long */
#define	TA_TJS_SERVERNO	((FLDID32)63558557)	/* number: 30004125	 type: long */
#define	TA_TJS_TIMESTAMP	((FLDID32)63558558)	/* number: 30004126	 type: long */
#define	TA_TJS_SERVSVCMETHODNUM	((FLDID32)63558559)	/* number: 30004127	 type: long */
#define	TA_TJS_SERVTOTADVCNT	((FLDID32)63558560)	/* number: 30004128	 type: long */
#define	TA_TJS_SERVSVCADVCNT	((FLDID32)63558561)	/* number: 30004129	 type: long */
#define	TA_TJS_SERVJMEADVCNT	((FLDID32)63558562)	/* number: 30004130	 type: long */
#define	TA_TJS_SERVAPIADVCNT	((FLDID32)63558563)	/* number: 30004131	 type: long */
#define	TA_TJS_METHODADVSVCNUM	((FLDID32)63558564)	/* number: 30004132	 type: long */
#define	TA_TJS_METHODIVKSUCC	((FLDID32)63558565)	/* number: 30004133	 type: long */
#define	TA_TJS_METHODIVKFAIL	((FLDID32)63558566)	/* number: 30004134	 type: long */
#define	TA_TJS_CFGFILENAME	((FLDID32)197776295)	/* number: 30004135	 type: string */
#define	TA_TJS_CFGFILEVERSION	((FLDID32)197776296)	/* number: 30004136	 type: string */
#define	TA_TJS_CFGXSDFILENAME	((FLDID32)197776297)	/* number: 30004137	 type: string */
#define	TA_TJS_OPERATION	((FLDID32)197776298)	/* number: 30004138	 type: string */
#define	TA_TJS_OPERDESCRIPTION	((FLDID32)197776299)	/* number: 30004139	 type: string */
#define	TA_TJS_ACTEDTHREADNUMS	((FLDID32)63558572)	/* number: 30004140	 type: long */
#define	TA_TJS_RESULTDESCRIPTION	((FLDID32)197776301)	/* number: 30004141	 type: string */
#define	TA_TJS_PARENTSVCNAME	((FLDID32)197776302)	/* number: 30004142	 type: string */
#define	TA_TJS_SERVERPROPOPT	((FLDID32)197776303)	/* number: 30004143	 type: string */
#define	TA_TJS_TPRETURNVAL	((FLDID32)63558576)	/* number: 30004144	 type: long */
#define	TA_TJS_TPRETURNURCODE	((FLDID32)63558577)	/* number: 30004145	 type: long */
#define	TA_TJS_DUPCFGFILENAME	((FLDID32)197776306)	/* number: 30004146	 type: string */
#define	TA_TJS_METHODSVCIVKSUCC	((FLDID32)63558579)	/* number: 30004147	 type: long */
#define	TA_TJS_METHODSVCIVKFAIL	((FLDID32)63558580)	/* number: 30004148	 type: long */
#define	TA_TJS_METHODSVCIVKFWDS	((FLDID32)63558581)	/* number: 30004149	 type: long */
#define	TA_TJS_CURMODULECOUNT	((FLDID32)63558582)	/* number: 30004150	 type: long */
#endif /* #ifndef TA_TJS_CLASS */

/* ------------------------------------------------------------
 * New fields start from 30004400.
 */
#endif
#endif
