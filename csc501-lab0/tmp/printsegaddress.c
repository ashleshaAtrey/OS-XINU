
# include<stdio.h>
extern int etext, edata, end;
void printsegaddress()
{

kprintf("\nCurrent: etext[0x%x]=0x%x, edata[0x%x]=0x%x, ebss[0x%x]=0x%x",&etext,etext,&edata,edata,&end,end);
kprintf("\nPreceding: etext[0x%x]=0x%x, edata[0x%x]=0x%x, ebss[0x%x]=0x%x",&etext-1,*(&etext-1),&edata-1,*(&edata-1),&end-1,*(&end-1));
kprintf("\nAfter: etext[0x%x]=0x%x, edata[0x%x]=0x%x, ebss[0x%x]=0x%x\n",&etext+1,*(&etext+1),&edata+1,*(&edata+1),&end+1,*(&end+1));
}
