/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL sreset(int sem, int count)
{
	long start = ctr1000;
	pidarray[currpid]=1;

	STATWORD ps;    
	struct	sentry	*sptr;
	int	pid;
	int	slist;

	disable(ps);
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore(ps);

        proctab[currpid].countsys[SRESET]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SRESET] += (end - start);

	return(OK);
}
