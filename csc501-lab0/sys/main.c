#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
//#include <lab0.h>

int prX;
void halt();

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 *
*/

prch(c)
char c;
{	
int i;
//syscallsummary_start();
	sleep(5);	
}
int main()
{
	kprintf("Task 5 (printsyscallsummary)\n");
	syscallsummary_start();
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	//syscallsummary_start();
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();
	int a=0xaabbccdd;
	int ret;
	ret=zfunction(a);
	kprintf("%x",ret);
	kprintf("...............................................");
	printtos();
	kprintf("...............................................");
	printprocstks(-1);
	kprintf("...............................................");
	printsegaddress();
	kprintf("...............................................");
	return 0;
}
