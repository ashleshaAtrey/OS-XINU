/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>
#include <lock.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr,*xptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	
//	kprintf("Inside kill");

	struct lentry *lptr;
	int prevx,x;
	
	

	if (pptr->lockid != -1){
			dequeue(pid);
		//	pptr->loc[x][1]=0;
	//		pptr->loc[x][2]=PINIT;
		//	pptr->loc[x][1]=0;
			lptr=&lcks[pptr->lockid];
			lptr->lprio=0;
			prevx=q[lptr->lqtail].qprev;
        		while(prevx != lptr->lqhead){
		//		printf("process in queue are %d....",prevx);
               	 		xptr=&proctab[prevx];
                		if(lptr->lprio < xptr->pprio){
                        		lptr->lprio=xptr->pprio;
                		 }
                		prevx=q[prevx].qprev;
       			 }
		//	kprintf("For lock %d ",lptr->lprio);
			getkill(pptr->lockid);
		//	flag=1;
			
	}
		
	for(x=0;x<NLOCKS;x++){
		if(pptr->loc[x][0]==PLOCK)
			releaseall(1,x);
	}	

	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
