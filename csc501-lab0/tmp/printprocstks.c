#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
static unsigned long *esp;


void printprocstks(int priority){
int i=0;
long x;
for (i=0; i< NPROC; i++)
{
//kprintf("Checking");
if (proctab[i].pstate != PRFREE && proctab[i].pprio > priority)
{
//proc=&proctab[i];

if(proctab[i].pstate == PRCURR)
         {
        asm("movl %esp, esp");
	x=esp;
        }
 else{
	x=proctab[i].pesp;	
        }
kprintf("\nProcess [proc %s]\npid:%d\npriority:%d\nbase:0x%x\nlen:%d\nlimit:0x%x\npointer:0x%x\n",proctab[i].pname,i,proctab[i].pprio,proctab[i].pbase,proctab[i].pstklen,proctab[i].plimit,x);

}
}
}

