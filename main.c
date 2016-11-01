// main.c - ssOS usage example
// Runs on LM4F120/TM4C123 will be ported to MSP432 in next version
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#include <stdint.h>
#include "os.h"
#include "os_cfg.h"
#include "profile.h"
#include "BSP.h"

#define THREADFREQ 1000   // frequency in Hz

// runs each thread 2 ms
uint32_t Count0;   // number of times Task0 loops
uint32_t Count1;   // number of times Task1 loops
uint32_t Count2;   // number of times Task2 loops
uint32_t Count3;   // number of times Task3 loops
uint32_t Count4;   // number of times Task4 loops
uint32_t Count5;   // number of times Task5 loops
uint32_t Count6;   // number of times Task6 loops
uint32_t Count7;   // number of times Task7 loops
uint32_t Count8;   // number of times Task8 loops

uint32_t CountIdle;   // number of times Idle_Task loops

fifo_t FifoA;
fifo_t FifoB;

int32_t Task34Sync;
int32_t Task43Sync;

extern ptcbType PerTask[NUMPERIODIC];

void Task0(void){
  Count0 = 0;
  while(1){
    Count0 = OS_FIFO_Get(&FifoA);
    Profile_Toggle0();    // toggle bit
		OS_Sleep(10);
  }
}
void Task1(void){		//AleGaa Will be edge trigered event task
  Count1 = 0;
  while(1){
    Count1 = OS_FIFO_Get(&FifoA);
    Profile_Toggle1();    // toggle bit
		OS_Sleep(50);
  }
}
void Task2(void){		//AleGaa Will be edge trigered event task
  Count2 = 0;
  while(1){
    Count2 = OS_FIFO_Get(&FifoB);
    Profile_Toggle2();    // toggle bit
		OS_Sleep(100);
  }
}
void Task3(void){	//Syncronize task 3 with 4
  Count3 = 0;
  while(1){
		OS_Signal(&Task34Sync);
		OS_Wait(&Task43Sync);
    Count3++;
    Profile_Toggle3();    // toggle bit
  }
}
void Task4(void){	//Syncronize task 3 with 4
  Count4 = 0;
  while(1){
		OS_Signal(&Task43Sync);
		OS_Wait(&Task34Sync);
    Count4++;
    Profile_Toggle4();    // toggle bit
		OS_Sleep(150);
  }
}

void Task5(void){	//peridic event task 10 ms
	Count5++;
  Profile_Toggle5();    // toggle bit
	while(1){
		OS_Wait(PerTask[0].semaphore);
		OS_FIFO_Put(&FifoA,Count5);
	}
	
}

void Task6(void){	//periodic event 100 ms
	Count6++;
  Profile_Toggle6();    // toggle bit
	while(1){
		OS_FIFO_Put(&FifoB,Count6);
	}
	
}

void Idle_Task(void){
  CountIdle = 0;
  while(1){
    CountIdle++;
  }
}

int main(void){
  OS_Init();            // initialize, disable interrupts
  Profile_Init();       // enable digital I/O on profile pins
	//OS_AddPeriodicEventThread(int32_t *semaPt, uint32_t period)
	
	OS_AddPeriodicEventThread(PerTask[0].semaphore, 10);
	OS_AddPeriodicEventThread(PerTask[1].semaphore, 100);

  OS_AddThreads(&Task0,100, 
	              &Task1, 50,
	              &Task2, 50,
	              &Task3, 50,
	              &Task4, 50,
	              &Task5, 10,
	              &Task6, 10,
	              &Idle_Task,254);	//Idle task is lowest priority

	OS_InitSemaphore(&Task34Sync,0);
	OS_InitSemaphore(&Task43Sync,0);
	OS_FIFO_Init(&FifoA);
	OS_FIFO_Init(&FifoB);
  OS_Launch(BSP_Clock_GetFreq()/THREADFREQ); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
//EOF
