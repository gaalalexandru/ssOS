// main.c - ssOS usage example
// Runs on LM4F120/TM4C123 will be ported to MSP432 in next version
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#include <stdint.h>
#include "os_core.h"
#include "os_hw.h"
#include "profile.h"
//#include "../inc/tm4c123gh6pm.h"

#define THREADFREQ 1000   // frequency in Hz

// runs each thread 2 ms
uint32_t Count0;   // number of times Task0 loops
uint32_t Count1;   // number of times Task1 loops
uint32_t Count2;   // number of times Task2 loops
uint32_t Count3;   // number of times Task3 loops
uint32_t Count4;   // number of times Task4 loops
uint32_t Count5;   // number of times Task5 loops
uint32_t Count6;   // number of times Task6 loops
uint32_t CountIdle;   // number of times Idle_Task loops

fifo_t FifoA;

int32_t Task23Semaphore;
int32_t Task45Semaphore;

extern ptcbType PerTask[NUMPERIODIC];

extern PortSema_t SemPortA;
extern PortSema_t SemPortB;
extern PortSema_t SemPortC;
extern PortSema_t SemPortD;
extern PortSema_t SemPortE;
extern PortSema_t SemPortF;

void Task0(void){	//Periodic task0 - 10 ms
  Count0 = 0;
  while(1){
		OS_Wait(&PerTask[0].semaphore);
		Count0++;
		OS_FIFO_Put(&FifoA,Count0);
    Profile_Toggle0();
  }
}
void Task1(void){		//Periodic task1 - 20 ms
  Count1 = 0;
  while(1){
		OS_Wait(&PerTask[1].semaphore);
    Count1 = OS_FIFO_Get(&FifoA);
    Profile_Toggle1();
		Profile_Toggle3();
  }
}
void Task2(void){		//Edge triggered task1
  Count2 = 0;
  while(1){
		OS_Wait(&SemPortD.pin6); // signaled in OS on button touch
		//OS_Sleep(200);
    //Profile_Toggle2();
		Count2++;
		OS_Signal(&Task23Semaphore);
		//OS_EdgeTrigger_Restart(PortD,Pin6);
  }
}
void Task3(void){	 //response to task2
  Count3 = 0;
  while(1){
		OS_Wait(&Task23Semaphore);
		Count3++;
		OS_Sleep(30); //sleep to debounce switch
    Profile_Toggle3();
		OS_EdgeTrigger_Restart(PortD,Pin6);
  }
}
void Task4(void){	 //Edge triggered task2
  Count4 = 0;
  while(1){
		OS_Wait(&SemPortD.pin7); // signaled in OS on button touch
		Count4++;
    Profile_Toggle4();
		OS_Signal(&Task45Semaphore);
  }
}

void Task5(void){	 //response to task2
	Count5 = 0;
	while(1){
		OS_Wait(&Task45Semaphore);
		Count5++;
    Profile_Toggle3();
		OS_Sleep(20); //sleep to debounce switch
		OS_EdgeTrigger_Restart(PortD,Pin7);
	}
}

void Task6(void){	//periodic event 100 ms
	Count6 = 0;
	while(1){
		Count6++;
		Profile_Toggle6();
		OS_Sleep(100);
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
	
	OS_InitSemaphore(&Task23Semaphore,0);
	OS_InitSemaphore(&Task45Semaphore,0);
	OS_InitSemaphore(&SemPortD.pin6,0);
	OS_InitSemaphore(&SemPortD.pin7,0);	
	
	OS_FIFO_Init(&FifoA);
	
	OS_EdgeTrigger_Init(PortD,Pin7,0,0,0x0000000A);
	OS_EdgeTrigger_Init(PortD,Pin6,0,0,0x0000000A);
	
	OS_AddPeriodicEventThread(&PerTask[0].semaphore, 10);
	OS_AddPeriodicEventThread(&PerTask[1].semaphore, 20);

  OS_AddThreads(&Task0,10, 
	              &Task1, 20,
	              &Task2, 50,
	              &Task3, 50,
	              &Task4, 150,
	              &Task5, 150,
	              &Task6, 250,
	              &Idle_Task,254);	//Idle task is lowest priority
	
  OS_Launch(BSP_Clock_GetFreq()/THREADFREQ); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
//EOF
