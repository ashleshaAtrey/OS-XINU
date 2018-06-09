#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

extern  int currpid; 	
int pidarray[NPROC];
void printsyscallsummary() 
{

int i=0; 
int k=0;
long ave[27]={};
char *sysname[]={"FREEMEM","CHPRIO","GETPID","GETPRIO","GETTIME","KILL","RECEIVE","RECVCLR","RECVTIM","RESUME","SCOUNT","SDELETE","SEND","SETDEV","SETNOK","SCREATE","SIGNAL","SIGNALN","SLEEP","SLEEP10","SLEEP100","SLEEP1000","SRESET","STACKTRACE","SUSPEND","UNSLEEP","WAIT"};
for (i=0; i< NPROC; i++)
{
if (pidarray[i] == 1)
{
kprintf("\nPROCESS [pid:%d]\n",i);
for(k=0;k<27;k++)
{
if (proctab[i].countsys[k]!=0)
{
        ave[k]= proctab[i].sumtime[k]/proctab[i].countsys[k];
 

kprintf("       SYSCALL: %s, count: %d, average execution time: %d (ms)\n",sysname[k],proctab[i].countsys[k],ave[k]);
}
}
}
}
}


void syscallsummary_start()
{
int k =0;
int i=0;
for(i=0;i<NPROC;i++)
{
        for(k=0;k<27;k++){
                proctab[i].sumtime[k] = 0;
                proctab[i].countsys[k] = 0;
        }
}
}


void syscallsummary_stop()
{

return;

}


