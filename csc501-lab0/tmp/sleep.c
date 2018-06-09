#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  * sleep  --  delay the calling process n seconds
 *   *------------------------------------------------------------------------
 *    */
extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];

SYSCALL sleep(int n)
{
        long start = ctr1000;
 	pidarray[currpid]=1;
        STATWORD ps;
        if (n<0 || clkruns==0)
                return(SYSERR);
        if (n == 0) {
                disable(ps);
                resched();
                restore(ps);
                return(OK);
        }
        while (n >= 1000) {
                sleep10(10000);
                n -= 1000;
        }
        if (n > 0)
                sleep10(10*n);

        proctab[currpid].countsys[SLEEP]++;
	long end = ctr1000;
        proctab[currpid].sumtime[SLEEP] += (end - start);
        return(OK);
}
