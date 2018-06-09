/* uummaryer.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

extern  int currpid;

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */




int main()
{       long ret1;
        kprintf("\n\nHello World, Xinu lives\n\n");

        kprintf("Calling printsegaddress\n");
	printsegaddress();
	kprintf("printsegaddress finished.............................");
        kprintf("\n calling printprocstks\n");
	printprocstks(-1);
	kprintf("printprocstks finished..............");
	syscallsummary_start(); 
        sleep(5);
        sleep(5);
	syscallsummary_stop();
        
	printsyscallsummary();
	long a;
	long b;
	long c;
	//a=0xaabbccdd;
	a=500;
	//b=890;
	//c=900;
	kprintf("\nCalling zfunction........\n");
	ret1= zfunction(a);
	kprintf("%x %d", ret1);
	kprintf("\n Zfunction finished........................\n");
	//printsegaddress();
        printtos();
	//printprocstks(-1);

	return 0;
}
