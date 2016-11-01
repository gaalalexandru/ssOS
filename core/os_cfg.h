/*ssOS Configurations, types and global variables definition*/
#ifndef __OSCFG_H
#define __OSCFG_H  1

#include <stdint.h>

#define NUMTHREADS  8        // maximum number of threads
#define NUMPERIODIC 10       // maximum number of periodic threads
#define STACKSIZE   100      // number of 32-bit words in stack per thread
#define STARTUP_DELAY 10  //AleGaa
#define INT_PRIO_PERIODIC_EV (1)  //Timer interrupt priority for periodic events
#define INT_PRIO_SLEEP (3)  //Timer interrupt priority for sleep decrementing

#define FSIZE 10  // general FIFO size

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct tcb{  //main thread controll block
  int32_t *sp;  // pointer to stack (valid for threads not running
  struct tcb *next;  // linked-list pointer
	int32_t *blocked;  //pointer to blocked semaphore, nonzero if blocked on this semaphore
	int32_t sleep;  //time to sleep, nonzero if this thread is sleeping
  uint32_t priority;  //priority of the thread, 0 - highest priority, 254 - lowest
};
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ptcb{	//periodic thread controll block
	int32_t *semaphore;
	uint32_t period;
	uint32_t counter;
};
typedef struct ptcb ptcbType;
ptcbType PerTask[NUMPERIODIC];

uint8_t Periodic_Event_Nr = 0;	//AleGaa

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct fifo_st{
	uint32_t Fifo[FSIZE];
	uint32_t LostData;	
	int32_t CurrentSize;
	int32_t Mutex;
	uint8_t PutI;
	uint8_t GetI;
};
typedef struct fifo_st fifo_t;

#endif
//EOF