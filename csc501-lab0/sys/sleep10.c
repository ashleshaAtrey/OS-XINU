/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];

SYSCALL	sleep10(int n)
{ 
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	if (n < 0  || clkruns==0)
	         return(SYSERR);
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	proctab[currpid].countsys[SLEEP10]++;
 	long end=ctr1000;
        proctab[currpid].sumtime[SLEEP10] += (end - start);

	return(OK);
}
