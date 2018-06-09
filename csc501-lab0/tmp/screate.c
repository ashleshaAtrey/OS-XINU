/* screate.c - screate, newsem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

LOCAL int newsem();

/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL screate(int count1)
{
	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	int	sem;

	disable(ps);
	if ( count1<0 || (sem=newsem())==SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	semaph[sem].semcnt = count1;
	/* sqhead and sqtail were initialized at system startup */
	restore(ps);

	proctab[currpid].countsys[SCREATE]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SCREATE] += (end-start);

	return(sem);
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newsem()
{
	int	sem;
	int	i;

	for (i=0 ; i<NSEM ; i++) {
		sem=nextsem--;
		if (nextsem < 0)
			nextsem = NSEM-1;
		if (semaph[sem].sstate==SFREE) {
			semaph[sem].sstate = SUSED;
			return(sem);
		}
	}


	return(SYSERR);
}
