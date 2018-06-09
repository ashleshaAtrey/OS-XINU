#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>
#include <proc.h>
#include <sem.h>
#include <sleep.h>
#include <mem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <stdio.h>

#include <lock.h>
#include <q.h>

void linit(){

int i;
struct lentry  *lptr;
kprintf("Inside linit");
nextl = NLOCKS-1;
for (i=0 ; i<NLOCKS ; i++) {      /* initialize semaphores */
                
		lptr = &lcks[i];
		lptr->lstate = LFREE;
		lptr->acqby=NOONE;
		lptr->rcnt=0;
	//	lptr->waittime[50]=0;
		lptr->lprio=0;
		int x;
		for(x=0;x<NPROC;x++){			
			//lptr->acqproc[i];
			lptr->waittime[x]=0;
		}
                lptr->lqtail = 1 + (lptr->lqhead = newqueue());
        }
return;
}

