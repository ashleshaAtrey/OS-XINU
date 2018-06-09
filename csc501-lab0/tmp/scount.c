/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <proc.h>
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];

SYSCALL scount(int sem)
{

	long start = ctr1000;
	pidarray[currpid]=1;
	extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
		return(SYSERR);

	proctab[currpid].countsys[SCOUNT]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SCOUNT] += (end - start);

	return(semaph[sem].semcnt);
}
