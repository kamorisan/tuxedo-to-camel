/*	Copyright (c) 1997 BEA Systems, Inc.
  	All rights reserved

  	THIS IS UNPUBLISHED PROPRIETARY
  	SOURCE CODE OF BEA Systems, Inc.
  	The copyright notice above does not
  	evidence any actual or intended
  	publication of such source code.
*/

/*	Copyright (c) 1991 UNIX System Laboratories, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF USL
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
/*
 * Start of tx.h header
 *
 * Define a symbol to prevent multiple inclusions of this header file
 */
#ifndef TX_H
#define TX_H

/* #ident	"@(#) tuxedo/include/tx.h	$Revision: 1.7 $" */

#ifndef TMENV_H
#include <tmenv.h>
#endif
#ifndef NOWHAT
static	char	h_tx[] = "@(#) tuxedo/include/tx.h	$Revision: 1.7 $";
#endif


#define TX_H_VERSION	0	/* current version of this header file */

/*
 * Transaction identifier
 */

#ifndef XIDDATASIZE
#define XIDDATASIZE	128	/* size in bytes */
struct xid_t {
	long formatID;		/* format identifier */
	long gtrid_length;	/* value not to exceed 64 */
	long bqual_length;	/* value not to exceed 64 */
	char data[XIDDATASIZE];
};
typedef struct xid_t XID;
#endif

/*
 * A value of -1 in formatID means that the XID is null.
 */

/*
 * Definitions for tx_ routines
 */
/* commit return values */
typedef long COMMIT_RETURN;
#define TX_COMMIT_COMPLETED 0
#define TX_COMMIT_DECISION_LOGGED 1

/* transaction control values */
typedef long TRANSACTION_CONTROL;
#define TX_UNCHAINED 0
#define TX_CHAINED 1

/* type of transaction timeouts */
typedef long TRANSACTION_TIMEOUT;

/* transaction state values */
#if ( defined(WIN32) && ( ( _MSC_VER >= 1500 ) || defined(__BORLANDC__) ) )
typedef long TX_TRANSACTION_STATE;
#else
typedef long TRANSACTION_STATE;
#endif
#define TX_ACTIVE 0
#define TX_TIMEOUT_ROLLBACK_ONLY 1
#define TX_ROLLBACK_ONLY 2

/* structure populated by tx_info() */
struct tx_info_t {
	XID	xid;
	COMMIT_RETURN when_return;
	TRANSACTION_CONTROL transaction_control;
	TRANSACTION_TIMEOUT transaction_timeout;
#if ( defined(WIN32) && ( ( _MSC_VER >= 1500 ) || defined(__BORLANDC__) ) )
	TX_TRANSACTION_STATE   transaction_state;
#else
	TRANSACTION_STATE   transaction_state;
#endif
};
typedef struct tx_info_t TXINFO;

/*
 * Declarations of routines by which Applications call TMs:
 */
#ifndef _TM_FAR
#define _TM_FAR
#endif

#if defined(__cplusplus)
extern "C" {
#endif

extern int _TMDLLENTRY tx_begin _((void));
extern int _TMDLLENTRY tx_close _((void));
extern int _TMDLLENTRY tx_commit _((void));
extern int _TMDLLENTRY tx_info _((TXINFO _TM_FAR *));
extern int _TMDLLENTRY tx_open _((void));
extern int _TMDLLENTRY tx_rollback _((void));
extern int _TMDLLENTRY tx_set_commit_return _((COMMIT_RETURN));
extern int _TMDLLENTRY tx_set_transaction_control _((TRANSACTION_CONTROL));
extern int _TMDLLENTRY tx_set_transaction_timeout _((TRANSACTION_TIMEOUT));

#if defined(__cplusplus)
}
#endif


/*
 * tx_ () return codes (transaction manager reports to application)
 */
#define TX_NOT_SUPPORTED	1	/* option not supported */
#define TX_OK			0	/* normal execution */
#define TX_OUTSIDE		-1	/* application is in an RM local
					   transaction */
#define TX_ROLLBACK		-2	/* transaction was rolled back */
#define TX_MIXED		-3	/* transaction was partially committed
					   and partially rolled back */
#define TX_HAZARD		-4	/* transaction may have been partially
					   committed and partially rolled back*/
#define TX_PROTOCOL_ERROR	-5	/* routine invoked in an improper
					   context */
#define TX_ERROR		-6	/* transient error */
#define TX_FAIL			-7	/* fatal error */
#define TX_EINVAL		-8	/* invalid arguments were given */
#define TX_COMMITTED		-9	/* transaction has heuristically
					   committed */
#define TX_NO_BEGIN		-100	/* transaction committed plus new
					   transaction could not be started */
#define TX_ROLLBACK_NO_BEGIN	(TX_ROLLBACK+TX_NO_BEGIN)
					/* transaction rollback plus new
					   transaction could not be started */
#define TX_MIXED_NO_BEGIN		(TX_MIXED+TX_NO_BEGIN)
					/* mixed plus new transaction could
					   not be started */
#define TX_HAZARD_NO_BEGIN		(TX_HAZARD+TX_NO_BEGIN)
					/* hazard plus new transaction could
					   not be started */
#define TX_COMMITTED_NO_BEGIN		(TX_COMMITTED+TX_NO_BEGIN)
					/* heuristically committed plus new
					   transaction could not be started */
#endif /* ifndef TX_H */

/*
 * End of tx.h header
 */
