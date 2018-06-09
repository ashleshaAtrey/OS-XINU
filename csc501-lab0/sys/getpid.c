/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */

extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];


SYSCALL getpid()
{       
	int start = ctr1000;
	pidarray[currpid]=1;
	proctab[currpid].countsys[GETPID]++;
        long end=ctr1000;
        proctab[currpid].sumtime[GETPID] += (end - start);

	return(currpid);
}
