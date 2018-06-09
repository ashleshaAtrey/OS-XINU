#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int newl();

/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
SYSCALL lcreate()
{
        STATWORD ps;
        int     l,i;
	int ret=SYSERR;
	struct lentry *lptr;
//	kprintf("Inside lcreate");
        disable(ps);
	for (i=0 ; i<NLOCKS ; i++) {
                l=nextl;
                nextl--;
                lptr=&lcks[l];
                if (nextl < 0)
                        nextl = NLOCKS-1;
                if (lptr->lstate !=LUSED) {
                        lptr->lstate = LUSED;
               //         kprintf(".lck id=%d..state=%d ...",l,lptr->lstate);
			lptr->rcnt=0;

        		int x;
        		for(x=0;x<NPROC;x++){
                		lptr->waittime[x]=0;
        		}
        		lptr->lprio=0;
       			lptr->lqtail = 1 + (lptr->lqhead = newqueue());

                        ret=l;
			break;
                }
        }
        
	 if (l==SYSERR ) {
                restore(ps);
                return(SYSERR);
        }
	else{
//truct lentry *lptr;
//	lptr=&lcks[l];
//	lptr->lstate=LUSED;
       //lck[sem].semcnt = count;
        /* sqhead and sqtail were initialized at system startup */
        	restore(ps);
        	return(l);
	}
}
