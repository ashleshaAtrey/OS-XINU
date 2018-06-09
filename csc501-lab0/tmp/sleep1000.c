/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL sleep1000(int n)
{
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    

	if (n < 0  || clkruns==0)
	         return(SYSERR);
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	proctab[currpid].countsys[SLEEP1000]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SLEEP1000] += (end - start);

	return(OK);
}
