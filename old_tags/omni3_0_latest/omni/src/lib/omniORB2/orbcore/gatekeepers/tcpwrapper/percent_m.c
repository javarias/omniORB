 /*
  * Replace %m by system error message.
  * 
  * Author: Wietse Venema, Eindhoven University of Technology, The Netherlands.
  */

#ifndef lint
static char sccsid[] = "@(#) percent_m.c 1.1 94/12/28 17:42:37";
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno;
#if !(defined(__GLIBC__) && __GLIBC__ >=2)
#if !defined(SYS_ERRLIST_DEFINED) && !defined(__freebsd__) && !defined(__darwin__)
extern char *sys_errlist[];
extern int sys_nerr;
#endif
#endif

#include "mystdarg.h"

char   *percent_m(obuf, ibuf)
char   *obuf;
char   *ibuf;
{
    char   *bp = obuf;
    char   *cp = ibuf;

    while (*bp = *cp)
	if (*cp == '%' && cp[1] == 'm') {
	    if (errno < sys_nerr && errno > 0) {
		strcpy(bp, sys_errlist[errno]);
	    } else {
		sprintf(bp, "Unknown error %d", errno);
	    }
	    bp += strlen(bp);
	    cp += 2;
	} else {
	    bp++, cp++;
	}
    return (obuf);
}