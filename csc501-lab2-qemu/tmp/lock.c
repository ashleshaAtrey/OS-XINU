#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

extern long ctr1000;

SYSCALL lock(int ldes1, int type, int priority)
{
        STATWORD ps;
        struct  lentry  *lptr;
	struct  pentry  *pptr,*xptr;
	int prevx,prev,x,pix;

	pptr = &proctab[currpid];	
	lptr = &lcks[ldes1];
//	kprintf("Inside lock");
        disable(ps);
//	kprintf("---%d---------%d---------------------",isbadl(ldes1),lptr->lstate);
        if (isbadl(ldes1) || (lptr->lstate) != LUSED || pptr->loc[0]==PDELETE  ){// pptr->loc[ldes1][0] == PLOCK) {
                restore(ps);
	//	kprintf("=========");
                return(SYSERR);
        }
	pptr->loc[ldes1][1]=  priority;
	pptr->loc[ldes1][2]=  type;
//	kprintf("acq by is %d",lptr->acqby);	
	if(lptr->acqby==NOONE)
	{
		if(type==READ){
	  		lptr->acqby=READ;
			lptr->rcnt=1;
			pptr->loc[ldes1][0] = PLOCK;
		//	kprintf("\nLock acq by reader intital");
		}
		else{
			 lptr->acqby=WRITE;
			 pptr->loc[ldes1][0] = PLOCK;
		//	 kprintf("\nlock acq by writer initial");
		}
	restore(ps);
        return(OK);		
	}
	else if(lptr->acqby==READ){		
		if (type==READ){
			prev = q[lptr->lqtail].qprev;
		//	kprintf("\nwating time %ld...",ctr1000-lptr->waittime[prev]);
			if (lptr->lqhead != prev && ((q[prev].qkey==priority && ((ctr1000-lptr->waittime[prev])>500))|| q[prev].qkey>priority)){
				lptr->waittime[currpid]=ctr1000;
				insert(currpid,lptr->lqhead,priority);	//cal resched and
				pptr->pstate=PRSUSP;
				pptr->lockid=ldes1;
				pptr->loc[ldes1][0] = PQUEUE;
				
				prevx=q[lptr->lqtail].qprev;
				while(prevx != lptr->lqhead){
					xptr=&proctab[prevx];
					if(lptr->lprio < xptr->pprio){
						lptr->lprio=xptr->pprio;		
					}
					prevx=q[prevx].qprev;
				}
				
				getdepends(ldes1);
		//		kprintf("\nlock acq by reader, putting reder in queue");
				restore(ps);
				resched();
				return(OK);
			}
			else{	
			lptr->rcnt=lptr->rcnt+1;
			pptr->loc[ldes1][0] = PLOCK;
		//	kprintf("\nlock acq by reader,Reader acquirin lock");
			restore(ps);
			return(OK);
			}
		}
		else{	
			lptr->waittime[currpid]=ctr1000;
			insert(currpid,lptr->lqhead,priority);
			pptr->pstate=PRSUSP;
			pptr->lockid=ldes1;
			pptr->loc[ldes1][0] = PQUEUE;
			prevx=q[lptr->lqtail].qprev;
                        while(prevx != lptr->lqhead){
                        	xptr=&proctab[prevx];
                                if(lptr->lprio < xptr->pprio){
                                	lptr->lprio=xptr->pprio;           //maximum watitng time for this lock
                                 }
                                prevx=q[prevx].qprev;
                        }
	
			getdepends(ldes1);
		//	kprintf("\nlock aqcurired by reader ,Putting writer in queue");
			restore(ps);
			resched();
			return(OK);
		}	
		
	 }
	else {
			lptr->waittime[currpid]=ctr1000;
			insert(currpid,lptr->lqhead,priority);
			pptr->pstate=PRSUSP;
			pptr->lockid=ldes1;
			pptr->loc[ldes1][0] = PQUEUE;
			prevx=q[lptr->lqtail].qprev;
                        while(prevx != lptr->lqhead){
                        	xptr=&proctab[prevx];
                                if(lptr->lprio < xptr->pprio){
                                	lptr->lprio=xptr->pprio;
                                 }
                                prevx=q[prevx].qprev;
                       }

			getdepends(ldes1);
		//	kprintf("lock acy by writer, putting others in queue");
        		restore(ps);
			resched();
        		return(OK);
		
		
	}
}



void getdepends(int locid){
//	printf("Inside getdepends");
        int x=0;
    	struct pentry *yptr;
       struct lentry *lptr;
        lptr=&lcks[locid];
        for(x=0;x<50;x++){                                                      //which processes  have lock ldes1
                yptr=&proctab[x];
                if(yptr->loc[locid][0] == PLOCK){
		//	kprintf("hi again");
                        if( yptr->pinh < lptr->lprio){  //change pinh value accordingly to all the processes which have that lock
                                 yptr->pinh=lptr->lprio;
                                 yptr->loc[locid][3]=1;
		//		kprintf("..locid3....%d...",yptr->loc[locid][3]);
                        //       yptr = &proctab[ (pix = getlast(q[x].qnext))];
                        //       insert(pix,rdyhead,yptr->pinh);
                                 if(yptr->lockid != -1){                                //change pinh value of all the dependents of those who have lock
                                        getdepends(yptr->lockid);
                                 }
                        }
                        else{
                                yptr->loc[locid][3]=0;
                        }
                }
        }
//printf("outside get depends");
return;
}

void getback(int locid){
//printf("Indie getback");
        int x=0;
       struct pentry  *wptr;

        wptr=&proctab[currpid];
        if(wptr->loc[locid][3]==1){
                wptr->pinh=0;
                wptr->loc[locid][3]=0;
                for(x=0;x<50;x++){                  //what all locks do current process have
                        if(wptr->loc[x][1]==PLOCK){
                                getdepends(x);          //
                        }
                }
        }
//printf("Outside getback");
return;
}






void getdelete(int locid){
     struct pentry *xptr;
        int x,y;
//	printf("inside get delete");
        for(x=0;x<NPROC;x++){               //which process have lock l
                xptr=&proctab[x];
		if(xptr->loc[locid][0]==PLOCK && xptr->loc[locid][3]==0){
			xptr->loc[locid][0]=PDELETE;
		}
                if(xptr->loc[locid][0]==PLOCK && xptr->loc[locid][3]==1){
                        xptr->loc[locid][0]=PDELETE;
                        xptr->loc[locid][3]=0;
			xptr->pinh=0;
                        for(y=0;y<NLOCKS;y++){              //which locks, process which has lock locid has excluding locid
                                if(xptr->loc[y][0]==PLOCK){
                                        getdepends(y);
                                }
                        }
                        if(xptr->lockid != -1){                                //change pinh value of all the dependents of those who have lock
                                        getdepends(xptr->lockid);
                                 }
                }
        }
//printf("outisde get delete");
return;
}


void getkill(int locid){
//	printf("Inside getkill");
    	struct pentry *tptr;
        int prevx,x,y;
        for(x=0;x<NPROC;x++){               //which process have lock l
                tptr=&proctab[x];	
	//	kprintf("Hi ");/////////////
	//	kprintf("%d... %d. ",tptr->loc[locid][0],tptr->loc[locid][3]);
                if(tptr->loc[locid][0]==PLOCK && tptr->loc[locid][3]==1){
		//	kprintf("Found it");
                    	tptr->loc[locid][3]=0;
			tptr->pinh=0;
                        for(y=0;y<NLOCKS;y++){              //which locks, process which has lock locid has including locid
                                if(tptr->loc[y][0]==PLOCK){
                                        getdepends(y);
                                }
                        }
                        if(tptr->lockid != -1){                                //change pinh value of all the dependents of those who have lock
                                        getdepends(tptr->lockid);
                                 }
                }
        }
//printf("outside getkill");
return;
}

