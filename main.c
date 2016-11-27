// main.c - ssOS usage example
// Runs on LM4F120/TM4C123 will be ported to MSP432 in next version
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

#include <stdint.h>
#include "os_core.h"
#include "os_hw.h"
#include "profile.h"
//#include "../inc/tm4c123gh6pm.h"  //AleGaa not needed at the moment
#include "driverlib/gpio.h"

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

extern ptcbType PerTask[NUMPERIODIC];

extern PortSema_t SemPortA;
extern PortSema_t SemPortB;
extern PortSema_t SemPortC;
extern PortSema_t SemPortD;
extern PortSema_t SemPortE;
extern PortSema_t SemPortF;
uint8_t last1,last2;
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
  }
}
void Task2(void){		//Edge triggered task PF0
  Count2 = 0;
  while(1){
		OS_Wait(&SemPortF.pin0); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_INT_PIN_0)) {   
			Profile_Toggle2();
			Count2++;
		}
		OS_EdgeTrigger_Restart(PortF,GPIO_PIN_0);
	}
}
void Task3(void){	 //response to task PF4
  Count3 = 0;
  while(1){
		OS_Wait(&SemPortF.pin4); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_INT_PIN_4)) {   
			Profile_Toggle3();
			Count3++;
		}
		OS_EdgeTrigger_Restart(PortF,GPIO_PIN_4);
	}
}

void Task4(void){	 //Edge triggered task PD6
	Count4 = 0;
  while(1){
		OS_Wait(&SemPortD.pin6); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTD_BASE,GPIO_INT_PIN_6)) {   
			Profile_Toggle4();
			Count4++;
		}
		OS_EdgeTrigger_Restart(PortD,GPIO_PIN_6);
	}
}

void Task5(void){	 //response to task PD7
	Count5 = 0;
  while(1){
		OS_Wait(&SemPortD.pin7); // signaled in OS on button touch
		OS_Sleep(50); //sleep to debounce switch		
		if(!GPIOPinRead(GPIO_PORTD_BASE,GPIO_INT_PIN_7)) {   
			Profile_Toggle5();
			Count5++;
		}
		OS_EdgeTrigger_Restart(PortD,GPIO_INT_PIN_7);
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
	
	OS_InitSemaphore(&SemPortD.pin6,0);
	OS_InitSemaphore(&SemPortD.pin7,0);	
	OS_InitSemaphore(&SemPortF.pin0,0);
	OS_InitSemaphore(&SemPortF.pin4,0);	
	
	OS_FIFO_Init(&FifoA);
	
	OS_EdgeTrigger_Init(PortD,GPIO_PIN_6|GPIO_PIN_7,0,GPIO_FALLING_EDGE,GPIO_PIN_TYPE_STD_WPU);
	OS_EdgeTrigger_Init(PortF,GPIO_PIN_0|GPIO_PIN_4,0,GPIO_FALLING_EDGE,GPIO_PIN_TYPE_STD_WPU);
// OS_EdgeTrigger_Init( port,  pin,  priority,  type, resistor)
	
	OS_AddPeriodicEventThread(&PerTask[0].semaphore, 10);
	OS_AddPeriodicEventThread(&PerTask[1].semaphore, 20);

  OS_AddThreads(&Task0,10, 
	              &Task1, 20,
	              &Task2, 5,
	              &Task3, 5,
	              &Task4, 5,
	              &Task5, 5,
	              &Task6, 250,
	              &Idle_Task,254);	//Idle task is lowest priority
	
  OS_Launch(BSP_Clock_GetFreq()/THREADFREQ); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}
//EOF
