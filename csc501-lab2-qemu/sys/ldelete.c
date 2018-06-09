/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL ldelete(int l)
{
        STATWORD ps;
        int     pid;
        struct  lentry  *lptr;
	struct  pentry  *pptr;
	

        disable(ps);
        if (isbadl(l) || lcks[l].lstate==LFREE) {
                restore(ps);
                return(SYSERR);
        }

	getdelete(l);
	lptr->lstate = DELETED;
//	printf("lock id: %d",l);	
	if (nonempty(lptr->lqhead)) {
                while( (pid=getfirst(lptr->lqhead)) != EMPTY)
                  {
			pptr->loc[l][0]=PDELETE;
                    proctab[pid].plockret = DELETED;
                    ready(pid,RESCHNO);
                  }
                resched();
        }


     //   lptr = &lcks[l];
      //  lptr->acqby=NOONE;
      // 	lptr->rcnt=0;
	
//	int x;
//	for(x=0;x<NPROC;x++){
//		lptr->waittime[x]=0;
//	}
//	lptr->lprio=0;
//        lptr->lqtail = 1 + (lptr->lqhead = newqueue());

	
        
//	int i;	
//	for(i=0; i<NPROC;i++)
//	{
//	    pptr=&proctab[i];
//	    if	(pptr->loc[l][0]==PLOCK || pptr->loc[l][0]== PQUEUE)
//		{
//		  pptr->loc[l][0]=PDELETE;
//		} 
//	}

	
	
        restore(ps);
        return(OK);
}



