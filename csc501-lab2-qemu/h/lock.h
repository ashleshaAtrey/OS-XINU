#ifndef NLOCKS
#define NLOCKS            50      /* number of semaphores, if not defined */
#endif
#define LFREE    0           /* this semaphore is free               */
#define LUSED    2           /* this semaphore is used               */

#define isbadl(l)     (l<0 || l>=NLOCKS)

#define READ 1
#define WRITE 2
#define NOONE -1

#define PLOCK 1
#define PQUEUE 2
#define PDELETE 3
#define PINIT -1

struct  lentry  {               /* semaphore table entry                */
        int     lstate;         /* the state SFREE or SUSED  meaning that lock is available or not  */
        int     rcnt;         /* count for this semaphore             */
        int     lqhead;         /* q index of head of list              */
        int     lqtail;         /* q index of tail of list              */
	int 	acqby;             /* check if lock is acquired by someone or not*/
	long 	waittime[50];
	int 	lprio;
//	int 	acqpro[50];
};

extern struct  lentry  lcks[];
extern  int     nextl;
void linit();
void getdepends(int locid);
void getkill(int locid);
void getdelete(int locid);
void getback(int locid);
//int lcreate (void);
//int ldelete (int lockdescriptor);
//int lock (int ldes1, int type, int priority);
//int releaseall();

