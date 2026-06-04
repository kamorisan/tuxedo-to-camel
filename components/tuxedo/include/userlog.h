/*
 * Minimal userlog.h stub for Tuxedo logging
 */

#ifndef _USERLOG_H
#define _USERLOG_H

#include <stdarg.h>

/* userlog function - log to Tuxedo ULOG */
extern int userlog(const char *format, ...);

#endif /* _USERLOG_H */
