/*
        Copyright (c) 2012 Oracle, Inc.
        All rights reserved

        THIS IS UNPUBLISHED PROPRIETARY
        SOURCE CODE OF ORACLE, Inc.
        The copyright notice above does not
        evidence any actual or intended
        publication of such source code.
*/

/*	Copyright (c) 1984 AT&T
	All rights reserved

       /units2/units/src/tuxedo/include/s.Usysflds
       Usysflds     40.1 
*/

#ifndef NOWHAT
static	char h_Usysflds[] = "@(#) tuxedo/include/Usysflds	$Revision: 1.12 $";
#endif

/* #ident	"@(#) tuxedo/include/Usysflds	$Revision: 1.12 $" */

/*	DEFINITIONS NEEDED BY USER APPLICATION PROGRAMS.

	Warning: This file should not be changed in any
	way, doing so will destroy the compatibility with TUXEDO programs
	and libraries.
*/
/*	SYSTEM RESERVED FIELD ENTRIES	*/
/*	fname	fldid            */
/*	-----	-----            */
#define	INITMSK	((FLDID)40961)	/* number: 1	 type: string */
#define	CURSID	((FLDID)8194)	/* number: 2	 type: long */
#define	CURSOC	((FLDID)8195)	/* number: 3	 type: long */
#define	LEVKEY	((FLDID)40964)	/* number: 4	 type: string */
#define	STATLIN	((FLDID)40965)	/* number: 5	 type: string */
#define	FORMNAM	((FLDID)40966)	/* number: 6	 type: string */
#define	UPDTMOD	((FLDID)7)	/* number: 7	 type: short */
#define	SRVCNM	((FLDID)40968)	/* number: 8	 type: string */
#define	NEWFORM	((FLDID)40969)	/* number: 9	 type: string */
#define	CHGATTS	((FLDID)49162)	/* number: 10	 type: carray */
#define	USYS1FLD	((FLDID)40971)	/* number: 11	 type: string */
#define	USYS2FLD	((FLDID)40972)	/* number: 12	 type: string */
#define	USYS3FLD	((FLDID)40973)	/* number: 13	 type: string */
#define	USYS4FLD	((FLDID)49166)	/* number: 14	 type: carray */
#define	USYS5FLD	((FLDID)49167)	/* number: 15	 type: carray */
#define	USYS6FLD	((FLDID)49168)	/* number: 16	 type: carray */
#define	DESTSRVC	((FLDID)40977)	/* number: 17	 type: string */
#define	MODS	((FLDID)49170)	/* number: 18	 type: carray */
#define	VALONENTRY	((FLDID)40979)	/* number: 19	 type: string */
#define	EVT_MONEVTNONTXNUM	((FLDID)8243)	/* number: 51	 type: long */
#define	EVT_MONEVTTXNUM	((FLDID)8244)	/* number: 52	 type: long */
#define	EVT_MONEVTSVCNUM	((FLDID)8245)	/* number: 53	 type: long */
#define	EVT_MONEVTUNSOLNUM	((FLDID)8246)	/* number: 54	 type: long */
#define	EVT_MONEVTQUENUM	((FLDID)8247)	/* number: 55	 type: long */
#define	EVT_MONEVTCMDNUM	((FLDID)8248)	/* number: 56	 type: long */
#define	EVT_MONEVTULOGNUM	((FLDID)8249)	/* number: 57	 type: long */
#define	EVT_CROSSDOMAINNAME	((FLDID)41018)	/* number: 58	 type: string */
#define	TUX_RTNCHAR	((FLDID)16455)	/* number: 71	 type: char */
#define	TUX_RTNSHORT	((FLDID)72)	/* number: 72	 type: short */
#define	TUX_RTNLONG	((FLDID)8265)	/* number: 73	 type: long */
#define	TUX_RTNFLOAT	((FLDID)24650)	/* number: 74	 type: float */
#define	TUX_RTNDOUBLE	((FLDID)32843)	/* number: 75	 type: double */
#define	BQCMD	((FLDID)41041)	/* number: 81	 type: string */
#define	TPMQ_ADAPTER_ERR	((FLDID)41051)	/* number: 91	 type: string */
#define	TPMQ_ADAPTER_ERR_CODE	((FLDID)41052)	/* number: 92	 type: string */
#define	TPMQ_APP_ERR	((FLDID)41053)	/* number: 93	 type: string */
