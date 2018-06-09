/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <proc.h>
extern int getutim(unsigned long *);

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
	long start = ctr1000;
	pidarray[currpid]=1;
    /* long	now; */

	/* FIXME -- no getutim */
    proctab[currpid].countsys[GETTIME]++;
        long end=ctr1000;
        proctab[currpid].sumtime[GETTIME] += (end - start);
    return OK;
}
