/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab1.h>
unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */

int class1;
int x ;
int get;
int y;
resched()
{

get=getschedclass();

if (get==0)
default_resched();
else if(get==1)
random_resched();
else if (x<2 && currpid==49 && get ==2)
{
preempt=10;
x+=1;
return OK;
}
else if(get==2)
linux_resched();
}

default_resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = QUANTUM;		/* reset preemption counter	*/
#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}

int random_resched()
{
	register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */
	int     next;                   /* runs through list            */
        int     prev;
	int     key_sum;
	key_sum=0;
	int prob;

	optr= &proctab[currpid];
	if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,optr->pprio);
        }
        prev = q[rdytail].qprev;
	while (prev != rdyhead)
	{
		key_sum= key_sum+ q[prev].qkey;
		prev = q[prev].qprev;
	}	
	if (key_sum!=0)
	prob=rand() % key_sum ;
	else
	{
		if (currpid != 0)
                        {
                        nptr = &proctab[ (currpid = getlast(q[0].qnext))];
                        nptr->pstate=PRCURR;
  //                      #ifdef RTCLOCK
//			preempt=QUANTUM;
//			#endif
                        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                       }
//			#ifdef RTCLOCK
  //                      preempt=QUANTUM;
//			#endif 
                        return OK;
	}
	
	prev = q[rdytail].qprev;
	while (prev != rdyhead)
	{
		if (q[prev].qkey > prob)
		{ 
		
		nptr = &proctab[ (currpid = getlast(q[prev].qnext)) ]; 
		
        	nptr->pstate = PRCURR; 
//		#ifdef  RTCLOCK
  //      	preempt = QUANTUM;              /* reset preemption counter     */
//		#endif

        	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
        	return OK;
 	
		}
		else 
		{
		 prob= prob - q[prev].qkey;
		 prev = q[prev].qprev;
		}	
	}
}

int linux_resched()
{
		
        register struct pentry  *optr;  // pointer to old process entry */
        register struct pentry  *nptr;  // pointer to new process entry */
	optr= &proctab[currpid];
	
	if(y==0)
	{
	preempt=0;
	y=y+1;
	}
	if (currpid==0)
	{	optr->counter=0;
		optr->goodness=0;
		optr->count=0;
	}
	else optr->counter= preempt;
	    	
	optr->goodness = optr->oldprio+preempt;

if (proctab[currpid].pstate==PRFREE)
        {
        optr->goodness = 0;
        optr->quantum=0;
        optr->counter=0;
	optr->count=0;
        }

	 
//kprintf("Current process is %d, counter is %d and quantum is %d and goodness is %d\n", currpid, optr->counter,optr->quantum, optr->goodness);

int num=0;
int prev=0;
int maxgoodness=0;
int i=0;
int var=0;
int flag=0;
	prev = q[rdytail].qprev;
	while(prev != rdyhead)
	{
//	kprintf("Processes in ready queue are pid: %d , Quantum=%d, counter=%d",prev,proctab[prev].quantum,proctab[prev].counter);
	proctab[prev].goodness= proctab[prev].counter+ proctab[prev].oldprio;
	if (proctab[prev].count!=0)
	{
//printf("Processes in runnable ready queue are pid: %d , Quantum=%d, count=%d",prev,proctab[prev].quantum,proctab[prev].count);
	flag=1;
	if (proctab[prev].goodness > maxgoodness)
		{
		maxgoodness= proctab[prev].goodness;
		var=prev;
		}
	}
	prev=q[prev].qprev;
	}
	
if(flag==0 && optr->counter==0)
	{
//printf("\nNew epoch");	
	maxgoodness=0;
	var=0;
	i=0;
	int flag2=0;
	struct pentry *n;
	for (i = 0; i < NPROC; i++) {
		n= &proctab[i];
		if (n->pstate == PRFREE)
			continue;
		else if (i!=0 && (n->pstate==PRCURR || n->pstate== PRREADY)) {
		n->count=1;
			if( n->counter==0){
		n->quantum=n->pprio;	
		n->goodness= 0;
		n->oldprio=n->pprio;
			}
			else {
		n->quantum=(n->counter/2)+n->pprio;		
		n->goodness= n->pprio + n->counter;
		n->oldprio=n->pprio;
			}
		}
		else  {
		n->goodness= n->pprio + n->counter;
                n->oldprio=n->pprio;
		n->quantum=(n->counter/2)+n->pprio;
		n->count=0;
		}
		}
		prev = q[rdytail].qprev;
		
		if(prev==rdyhead)
			flag2=1;
		else {
		while(prev != rdyhead)
		{
		if (n->goodness>maxgoodness && n->count!=0)
                {
                maxgoodness= n->goodness;
		var= i;
		}
		prev=q[prev].qprev;
		}
		}
	
	if (flag2==1 && n->pstate != PRCURR)    {                                        //ready queue is empty and 
		 	if (currpid != 0)
			{
			nptr = &proctab[ (currpid = getlast(q[0].qnext))];
			nptr->pstate=PRCURR;
			preempt=QUANTUM;
                        nptr->count=1;
                        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;
			}
			else{
			preempt=QUANTUM;
			optr->count=1;
			return OK;
	}	}
	else if (optr->goodness==maxgoodness && optr->pstate== PRCURR)         // current process is schded
			{	
				#ifdef RTCLOCK				
				preempt=optr->quantum;
				#endif 
				optr->count=1;
//		kprintf("\nCurrent process is the highest process in new epoch %d \n ", optr->pprio);
				return OK;
			}
	else  {          
				if (optr->pstate==PRCURR)
				{	optr->pstate = PRREADY;
					optr->count=1;
                			insert(currpid,rdyhead,optr->pprio);
				}
        			nptr = &proctab[ (currpid = getlast(q[var].qnext))];
				nptr->pstate = PRCURR;
		                #ifdef  RTCLOCK
                		preempt= nptr->quantum;             /* reset preemption counter     */
               			#endif
				nptr->count=1;
//				kprintf("\nContext switch the to new process with higher priority %d \n",nptr->pprio);
                		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                		return OK;
			}
	
	}
else  
{
	if (maxgoodness>optr->goodness)
		{
		if (optr->pstate== PRCURR)
		{	optr->pstate = PRREADY;
			optr->count=1;
			insert(currpid,rdyhead,optr->oldprio);
		}
//		kprintf("\nNew proces have high prioory%d\n",proctab[var].oldprio);
		nptr = &proctab[ (currpid =getlast(q[var].qnext)) ];
                nptr->pstate = PRCURR;
                #ifdef  RTCLOCK
                preempt = nptr->quantum;              /* reset preemption counter     */
                #endif
		nptr->count=1;
                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                return OK;
		}
	else if (optr->goodness >= maxgoodness && optr->counter > 0 && optr->pstate==PRCURR)
		{	
//		kprintf("\n curennt proces is high prioory process %d \n",optr->oldprio);
		preempt= optr->counter;
		optr->count=1;
		return OK;
		}
	else if (optr->goodness >= maxgoodness && optr->counter==0)
		{
		if (optr->pstate== PRCURR)
                {	optr->pstate = PRREADY;
			optr->goodness=0;
			optr->count=0;
                	insert(currpid,rdyhead,optr->oldprio);
		}
//			kprintf("\ncurrent process exhausted. choosing new process in same epoch%d",q[var].qkey);
                                nptr = &proctab[ (currpid =getlast(q[var].qnext)) ];
                                nptr->pstate = PRCURR;
                                #ifdef  RTCLOCK
                                preempt = nptr->quantum;              /* reset preemption counter     */
                                 #endif
				nptr->count=1;
                                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
				
                                return OK;
		}
	
	else 
	{
		if (currpid != 0)
                        {
                        nptr = &proctab[ (currpid = getlast(q[0].qnext))];
                        nptr->pstate=PRCURR;
                        preempt=QUANTUM;
                        nptr->count=1;
                        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
			return OK;
                        }
			else{
                        preempt=QUANTUM;
                        optr->count=1;
                        return OK;
			}
               
	}

}

}

void setschedclass(int sched_class)
{
if (sched_class==RANDOMSCHED)
        class1=1;
else if (sched_class==LINUXSCHED)
        class1=2;
else class1=0;

}

int getschedclass()
{
 return class1;
}
