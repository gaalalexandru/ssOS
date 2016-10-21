// main.c - ssOS usage example
// Runs on LM4F120/TM4C123 will be ported to MSP432 in next version
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#include <stdint.h>
#include "os.h"
#include "profile.h"
#include "BSP.h"

#define THREADFREQ 1000   // frequency in Hz

// runs each thread 2 ms
uint32_t Count0;   // number of times Task0 loops
uint32_t Count1;   // number of times Task1 loops
uint32_t Count2;   // number of times Task2 loops
uint32_t Count3;   // number of times Task2 loops
uint32_t Count4;   // number of times Task2 loops
uint32_t CountIdle;   // number of times Task2 loops
uint32_t CountEv0 = 0;   // number of times Task2 loops
uint32_t CountEv1 = 0;   // number of times Task2 loops

fifo_t FifoA;
fifo_t FifoB;
fifo_t FifoC;

int32_t Task34Sync;
int32_t Task43Sync;

void Event0(void){
	CountEv0++;
  Profile_Toggle5();    // toggle bit
	OS_FIFO_Put(&FifoA,CountEv0);
}

void Event1(void){
	CountEv1++;
  Profile_Toggle6();    // toggle bit
	OS_FIFO_Put(&FifoB,CountEv1);
}

void Task0(void){
  Count0 = 0;
  while(1){
    Count0 = OS_FIFO_Get(&FifoA);
    Profile_Toggle0();    // toggle bit
		OS_Sleep(10);
  }
}
void Task1(void){
  Count1 = 0;
  while(1){
    Count1 = OS_FIFO_Get(&FifoA);
    Profile_Toggle1();    // toggle bit
		OS_Sleep(50);
  }
}
void Task2(void){
  Count2 = 0;
  while(1){
    Count2 = OS_FIFO_Get(&FifoB);
    Profile_Toggle2();    // toggle bit
		OS_Sleep(100);
  }
}
void Task3(void){
  Count3 = 0;
  while(1){
		OS_Signal(&Task34Sync);
		OS_Wait(&Task43Sync);
    Count3++;
    Profile_Toggle3();    // toggle bit
  }
}
void Task4(void){
  Count4 = 0;
  while(1){
		OS_Signal(&Task43Sync);
		OS_Wait(&Task34Sync);
    Count4++;
    Profile_Toggle4();    // toggle bit
		OS_Sleep(500);
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
	OS_AddPeriodicEventThread(&Event0, 10);
	OS_AddPeriodicEventThread(&Event1, 100);
  OS_AddThreads(&Task0, &Task1, &Task2, &Task3, &Task4, &Idle_Task);
	OS_InitSemaphore(&Task34Sync,0);
	OS_InitSemaphore(&Task43Sync,0);
	OS_FIFO_Init(&FifoA);
	OS_FIFO_Init(&FifoB);
  OS_Launch(BSP_Clock_GetFreq()/THREADFREQ); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
//EOF
