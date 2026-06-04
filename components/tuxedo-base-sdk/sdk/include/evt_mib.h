/*	Copyright (c) 1995 Novell
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF NOVELL
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.

#ident	"@(#) tuxedo/include/evt_mib	$Revision: 1.3 $"
*/
/*
	Warning: This file should not be changed in any
	way, doing so will destroy the compatibility with TUXEDO programs
	and libraries.
*/
#ifndef _EVT_MIB_H
#define _EVT_MIB_H
#ifndef NOWHAT
static	char h_evt_mib[] = "@(#) tuxedo/include/evt_mib	$Revision: 1.3 $";
#endif

#include <fml32.h>
#include <tpadm.h>

#ifndef REDUCE_CPP
/*
	The field numbers below should always begin at 0 and increase.
	Field numbers cannot be reused or changed from release to release or
	interoperability will be broken.
*/
/*	fname	fldid            */
/*	-----	-----            */
#define	TA_EVENT_NAME	((FLDID32)167779160)	/* number: 7000	 type: string */
#define	TA_EVENT_SEVERITY	((FLDID32)167779161)	/* number: 7001	 type: string */
#define	TA_EVENT_LMID	((FLDID32)167779162)	/* number: 7002	 type: string */
#define	TA_EVENT_TIME	((FLDID32)33561435)	/* number: 7003	 type: long */
#define	TA_EVENT_USEC	((FLDID32)33561436)	/* number: 7004	 type: long */
#define	TA_EVENT_DESCRIPTION	((FLDID32)167779165)	/* number: 7005	 type: string */
#define	TA_ERROR_STRING	((FLDID32)167779166)	/* number: 7006	 type: string */
#define	TA_EB_PID	((FLDID32)33561440)	/* number: 7008	 type: long */
#define	TA_EB_MID	((FLDID32)33561441)	/* number: 7009	 type: long */
#define	TA_EB_API_CALL	((FLDID32)167779170)	/* number: 7010	 type: string */
#define	TA_EB_SUBSCRIBER_TYPE	((FLDID32)33561443)	/* number: 7011	 type: long */

/*Fields used for synchronization of subscription database*/
#define	TA_EB_LAST_SEQNO	((FLDID32)33561444)	/* number: 7012	 type: long */
#define	TA_EB_LAST_POLLED	((FLDID32)33561445)	/* number: 7013	 type: long */
#define	TA_EB_POLL_INTERVAL	((FLDID32)33561446)	/* number: 7014	 type: long */
#define	TA_EB_COMP_SUB	((FLDID32)7015)	/* number: 7015	 type: short */
#define	TA_EB_DB_SEQNO	((FLDID32)33561448)	/* number: 7016	 type: long */
/*Fields used in events posted by the JavaServer that are not part of any mib class.*/
#define	TA_EJB_NAME	((FLDID32)167779177)	/* number: 7017	 type: string */
#define	TA_EJB_METHOD_NAME	((FLDID32)167779178)	/* number: 7018	 type: string */


#endif
#endif
