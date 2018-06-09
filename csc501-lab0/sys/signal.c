/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL signal(int sem)
{
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	proctab[currpid].countsys[SIGNAL]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SIGNAL] += (end - start);

	return(OK);
}
