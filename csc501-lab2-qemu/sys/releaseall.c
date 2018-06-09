#include <conf.h>
#include<lock.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
int releaseall(int numlocks,int descriptors){

STATWORD ps;
register struct lentry  *lptr;
struct  pentry  *pptr,*optr,*nptr,*xptr;
disable(ps);
int   *a;             /* points to list of args       */
pptr = &proctab[currpid];
int flag=0;
int pi=0,x=0,prev=0,set=0;
int pstat;
int prio_writer=0;
long waittime_writer=0;
int prevx=0;
int prevd;
a = (int *)(&descriptors) + (numlocks-1); /* last argument      */
        for ( ; numlocks > 0 ; numlocks--){   /* machine dependent; copy args */
		lptr = &lcks[*a];
		pi=0;x=0;prev=0;set=0;
		pstat=0;
		prio_writer=0;
		waittime_writer=0;
		prevx=0;
		prevd=0;
//	kprintf("Hi this is %d.... %d.....%d.................... ", lptr->lstate,pptr->loc[*a][0], *a);
        	if (isbadl(*a) || lptr->lstate!=LUSED || pptr->loc[*a][0] != PLOCK) {
           		flag=1; 
		}
	        else if(lptr->acqby==READ){
			lptr->rcnt-=1;
			
			pptr->loc[*a][0]= PINIT;
                        pptr->loc[*a][1]= 0;
                        pptr->loc[*a][2]=  NOONE;

	//		printf("reader count: %d",lptr->rcnt);
			if(lptr->rcnt==0){
				prev = q[lptr->lqtail].qprev;
		//		printf("All readerd done, giving lock to writer");
				if(prev!=lptr->lqhead){
				//	printf("Scheduling writer");
					optr = &proctab[ (pi = getlast(lptr->lqtail)) ];
        				optr->pstate = PRREADY;
					optr->lockid= -1;	
					insert(pi,rdyhead,optr->pprio);
					lptr->acqby=WRITE;
					optr->loc[*a][0] = PLOCK;							
					}
				else{
					lptr->acqby=NOONE;
					lptr->lprio=0;
				}
			}
			
		}
		else{
	//		printf("Writer releasing lock");
			pptr->loc[*a][0]= PINIT;
                        pptr->loc[*a][1]= 0;
                        pptr->loc[*a][2]=  NOONE;
			long f;
			prev=q[lptr->lqtail].qprev;
			if(prev!=lptr->lqhead){
				optr=&proctab[prev];
				pstat=optr->loc[*a][2];
				if(pstat==WRITE){
				//	printf("end of queue is writer");
					prio_writer=optr->loc[*a][1];
                                        waittime_writer=lptr->waittime[prev];
					prev=q[prev].qprev;
				//	printf("Reader time is %ld...",lptr->waittime[prev]);
				//	printf("Writer waiting time %ld...",waittime_writer);
				//	printf("heade is %d ..", lptr->lqhead);
					f=lptr->waittime[prev]-500;	
					while(prev != lptr->lqhead){
                                                optr=&proctab[prev];
						prevd=prev;
                                                if(optr->loc[*a][1]==prio_writer && optr->loc[*a][2]==READ && f <= waittime_writer){
						//	kprintf("Inside");
							prevd=q[prev].qnext;
                                                	nptr = &proctab[ (pi = getlast(q[prev].qnext))];
                                                        nptr->pstate = PRREADY;
							nptr->lockid= -1;
							insert(pi,rdyhead,nptr->pprio);
                                                      //  prev=q[prev].qprev;
							set=1;
							lptr->acqby=READ;
							lptr->rcnt+=1;
							nptr->loc[*a][0] = PLOCK;	
						}
						prev=q[prevd].qprev;
						printf("prev is %d",prev);
					}
					if(set==0){
						
						optr = &proctab[(pi = getlast(lptr->lqtail))];
                                        	optr->pstate = PRREADY;
						optr->lockid = -1;
						insert(pi,rdyhead,optr->pprio);
						lptr->acqby=WRITE;
						optr->loc[*a][0] = PLOCK;
					}
				}
				else{
					
				//	printf("end of queue is Reader");
					nptr = &proctab[ (pi = getlast(lptr->lqtail)) ];
                                        nptr->pstate = PRREADY;
					nptr->lockid = -1;
					nptr->loc[*a][0] = PLOCK;
					insert(pi,rdyhead,nptr->pprio);
					prev=q[lptr->lqtail].qprev;
				//	printf("schduling reader");
					lptr->acqby=READ;
					lptr->rcnt+=1;
					while(prev!=lptr->lqhead){
						optr=&proctab[prev];
						pstat=optr->loc[*a][2];
						if(pstat==READ){
							prevd=q[prev].qnext;
							nptr = &proctab[ (pi = getlast(lptr->lqtail))];
                                        		nptr->pstate = PRREADY;
							nptr->lockid=-1;
							insert(pi,rdyhead,nptr->pprio);
							prev=q[prevd].qprev;
							nptr->loc[*a][0] = PLOCK;
							lptr->rcnt+=1;
                   				}
						else{
							prio_writer=optr->loc[*a][1];
							waittime_writer=lptr->waittime[prev];
							prev=q[prev].qprev;
							while(prev!=lptr->lqhead){
								optr=&proctab[prev];
								prevd=prev;
								if(optr->loc[*a][1]==prio_writer && optr->loc[*a][2]==READ && (lptr->waittime[prev]-500)<=waittime_writer){
									prevd=q[prev].qnext;
									nptr = &proctab[ (pi = getlast(q[prev].qnext))];
		                                                        nptr->pstate = PRREADY;
									nptr->lockid = -1;
									nptr->loc[*a][0] = PLOCK;
									insert(pi,rdyhead,nptr->pprio);
								}
								prev=q[prevd].qprev;
							}
						}	
	
					}
					
				}
			}
			else{
	//			kprintf("Releasing writer");
				lptr->acqby=NOONE;
				lptr->lprio=0;
			}
		}
//rintf("Reader scheduling");	
	
	prevx=q[lptr->lqtail].qprev;
        while(prevx != lptr->lqhead){
        	xptr=&proctab[prevx];
                if(lptr->lprio < xptr->pprio){
               	lptr->lprio=xptr->pprio;
                 }
                prevx=q[prevx].qprev;
        	}
	getback(*a);
	
	
	*a--;
	
	}
	if(flag==1){
		restore(ps);
                return(SYSERR);
	}
	else{	//	kprintf("Relaseing lock");
		restore(ps);
		return(OK);
	}
}
