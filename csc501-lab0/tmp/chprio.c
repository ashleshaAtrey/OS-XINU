/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
 
SYSCALL chprio(int pid, int newprio)
{
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
        
	proctab[currpid].countsys[CHPRIO]++;
        long end=ctr1000;
        proctab[currpid].sumtime[CHPRIO] += (end - start);

	return(newprio);
}
