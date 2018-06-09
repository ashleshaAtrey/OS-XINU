/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];

SYSCALL getprio(int pid)
{	
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	restore(ps);
       

	proctab[currpid].countsys[GETPID]++;
        long end=ctr1000;
        proctab[currpid].sumtime[GETPID] += (end - start);
	return(pptr->pprio);
}
