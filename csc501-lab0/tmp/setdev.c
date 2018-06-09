/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL	setdev(int pid, int dev1, int dev2)
{	
	long start = ctr1000;
	pidarray[currpid]=1;
	short	*nxtdev;

	if (isbadpid(pid))
		return(SYSERR);
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;

	proctab[currpid].countsys[SETDEV]++;
        long end=ctr1000;
        proctab[currpid].sumtime[SETDEV] += (end - start);


	return(OK);
}
