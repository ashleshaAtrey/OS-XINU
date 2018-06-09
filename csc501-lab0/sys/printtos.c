#include <stdio.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

static unsigned long *esp;
static unsigned long *ebp;
void printtos()
{
	asm("movl %esp,esp");
	asm("movl %ebp,ebp");
	int count=1;
	kprintf("\nBefore[0x%x]:0x%d\n",(ebp+2),*(ebp+2));
	kprintf("After[0x%x]:0x%x\n",(ebp),*(ebp));
        kprintf("Address of top of stack: 0x%x\n", esp);
	while(count <5 && (ebp>(esp+count)))
	{
	printf("      Element[0x%x]: 0x%x\n",(esp+count),*(esp+count));                                        
	count++;
		
	}
}
