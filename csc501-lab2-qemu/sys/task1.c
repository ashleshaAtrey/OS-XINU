#include <stdio.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <sem.h>
#define DEFAULT_LOCK_PRIO 20

void reader (char *msg, int lck, int lprio)
{
    int     ret;
    kprintf ("  %s: to acquire lock\n", msg);
    ret = lock (lck, READ, lprio);
    if (ret == SYSERR) {
        kprintf ("  %s: lock returned SYSERR\n", msg);
        return;
    }

    if (ret == DELETED) {
        kprintf ("  %s: lock was DELETED\n", msg);
        return;
    }

    kprintf ("  %s: acquired lock, sleep 5s\n", msg);
    sleep (5);
    kprintf ("  %s: to release lock\n", msg);
    releaseall (1, lck);
}



void writer (char *msg, int lck, int lprio)
{
    int ret;
    kprintf ("  %s: to acquire lock\n", msg);
    ret = lock (lck, WRITE, lprio);
    if (ret == SYSERR) {
        kprintf ("  %s: lock returned SYSERR\n", msg);
        return;
    }

    if (ret == DELETED) {
        kprintf ("  %s: lock was DELETED\n", msg);
        return;
    }
    kprintf ("  %s: acquired lock, sleep 5\n", msg);
    sleep (5);
    kprintf ("  %s: to release lock\n", msg);
    releaseall (1, lck);
}



void func_sem(char *msg, int sem) 
{
	int ret;
    kprintf ("  %s: to acquire sem\n", msg);
    ret = wait(sem);
    if (ret == SYSERR) {
        kprintf ("  %s: sem returned SYSERR\n", msg);
        return;
    }
    kprintf ("  %s: acquired sem \n", msg);
    sleep(10);
    kprintf ("  %s: to release sem\n",msg);
    signal(sem);

}


int main()
{
	int lck;
    int     rd1;
    int     wr1,sem,s1,s2,s3,wr2;

    kprintf(" PRIORITY INVERSION WITH LOCK\n");
    lck  = lcreate();
 
    wr1  = create(writer, 2000, 10, "writer1", 3, "writer 1", lck, 20);
    wr2  = create(writer, 2000, 20, "writer2", 3, "writer 2", lck, 20);
    rd1  = create(reader, 2000, 30, "reader1", 3, "reader 1", lck, 20);

    kprintf("Start Writer 1, then sleep 1s\n");
    resume(wr1);
    sleep(1);
    kprintf("Priority of writer 1 is %d ", getprio(wr1));

    kprintf("Start Writer 2, then sleep 1s\n");
    resume(wr2);
    sleep(1);
    kprintf("Priority of writer 1 is %d ", getprio(wr1));


    kprintf("Start reader1, then sleep 1s\n");
    resume(rd1);
    sleep(1);
    kprintf("Priority of writer 1 is %d ", getprio(wr1));
	

    sleep(20);
    kprintf ("LOCK TEST DONE !\n");
    
	kprintf(" PRIORITY INVERSION WITH SEMAPHORE \n");
    sem  = screate(1);   
    s1 = create(func_sem, 2000, 10, "writer1", 3, "writer 1", sem, 20);
    s2 = create(func_sem, 2000, 20, "writer1", 3, "writer 2", sem, 20);
    s3 = create(func_sem, 2000, 30, "reader2", 3, "reader 1", sem, 20);

    kprintf("Start Writer 1, then sleep 1s\n");
    resume(s1);
    sleep(1);
	kprintf("Priority of writer 1 is %d ", getprio(s1));

    kprintf("Start Writer 2, then sleep 1s\n");
    resume(s2);
	kprintf("Priority of writer 1 is %d ", getprio(s1));

  //  sleep(1);
    kprintf("Start Reader 1, then sleep 1s\n");
    resume(s3);
	kprintf("Priority of writer 1 is %d ", getprio(s1));

    sleep(10);
	
    kprintf ("SEMAPHORE DONE ! \n");
    return 0;
}
