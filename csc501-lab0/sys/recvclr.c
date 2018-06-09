/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */


extern  int currpid;
extern long ctr1000;
extern int pidarray[NPROC];
SYSCALL	recvclr()
{      
 	long start = ctr1000;
	pidarray[currpid]=1;
	STATWORD ps;    
	WORD	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);

	proctab[currpid].countsys[RECVCLR]++;
        long end=ctr1000;
        proctab[currpid].sumtime[RECVCLR] += (end - start);

	return(msg);
}
