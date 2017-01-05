// os_hw.c - ssOS Target HW specific code - Interrupt Service Routines, Target configurations
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

/*------Export interface---Self header Includes------*/
#include "os_hw.h"
//#include "../inc/tm4c123gh6pm.h"   //AleGaa not needed at the moment

#ifdef TARGET_TM4C
//TM4C specific code

PortSema_t SemPortA;
PortSema_t SemPortB;
PortSema_t SemPortC;
PortSema_t SemPortD;
PortSema_t SemPortE;
PortSema_t SemPortF;

void GPIOPortD_Handler(void){	 //PortD GPIO interrupt handler
	// step 1 acknowledge by clearing flag
  // step 2 signal semaphore (no need to run scheduler)
  // step 3 disarm interrupt to prevent bouncing to create multiple signals
	uint8_t status;	
	status = GPIOIntStatus(GPIO_PORTD_BASE,/*false*/true);
	if(status & GPIO_INT_PIN_0) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_0); //acknowledge by clearing flag
		OS_Signal(&SemPortD.pin0);  //signal semaphore
		GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_0);  //disarm interrupt to prevent bouncing to create multiple signals
	}
	if(status & GPIO_INT_PIN_1) { GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_1); OS_Signal(&SemPortD.pin1); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_1);}
	if(status & GPIO_INT_PIN_2) { GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_2); OS_Signal(&SemPortD.pin2); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_2);}
	if(status & GPIO_INT_PIN_3) {	GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_3); OS_Signal(&SemPortD.pin3); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_3);}
	if(status & GPIO_INT_PIN_4) { GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_4); OS_Signal(&SemPortD.pin4); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_4);}
	if(status & GPIO_INT_PIN_5) {	GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_5); OS_Signal(&SemPortD.pin5); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_5);}
	if(status & GPIO_INT_PIN_6) { GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_6); OS_Signal(&SemPortD.pin6); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);}
	if(status & GPIO_INT_PIN_7) {	GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_7); OS_Signal(&SemPortD.pin7); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_7);}
  OS_Suspend();
}

void GPIOPortF_Handler(void){	 //PortF GPIO interrupt handler
	uint8_t status;	
	status = GPIOIntStatus(GPIO_PORTF_BASE,/*false*/true);
	if(status & GPIO_INT_PIN_0) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0); //acknowledge by clearing flag
		OS_Signal(&SemPortF.pin0);  //signal semaphore	
		GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);  //disarm interrupt to prevent bouncing to create multiple signals	
	}
	if(status & GPIO_INT_PIN_1) {	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_1); OS_Signal(&SemPortF.pin1); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_1);}
	if(status & GPIO_INT_PIN_2) {	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_2); OS_Signal(&SemPortF.pin2); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_2);}
	if(status & GPIO_INT_PIN_3) { GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_3); OS_Signal(&SemPortF.pin3); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_3);}
	if(status & GPIO_INT_PIN_4) { GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4); OS_Signal(&SemPortF.pin4); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);}
  OS_Suspend();
}

// ******** OS_EdgeTrigger_Init ************
// Initialize a button, to signal on edge interrupt
// Inputs:  port , pin, priority, type, resistor
// Outputs: 1 if config is successfull, 0 if not
uint8_t OS_EdgeTrigger_Init(ports_t port, uint8_t pin, uint8_t priority, uint8_t type, uint8_t resistor){
	uint32_t clock;
	uint32_t bit_prio;
	switch (port) {
		case PortA:  //PortA 
			//Need to unlock
			GPIO_PORTA_LOCK_R = 0x4C4F434B;  //Unlock GPIO Port D
			GPIO_PORTA_CR_R |= 0xFF;  //Allow changes to PD7-0
			break;
		case PortB:  //PortB
			//Need to unlock
			break;
		case PortC:  //PortC
			//Need to unlock
			break;
		case PortD:  //PortD	
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
			if(pin && GPIO_PIN_7) { GPIO_PORTD_LOCK_R = 0x4C4F434B; } //Unlock GPIO PD7 if necessary
			GPIO_PORTD_CR_R |= 0xFF;  //Allow changes to PD7-0
			IntDisable(INT_GPIOD);
			GPIOIntDisable(GPIO_PORTD_BASE,pin);
			GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, pin);
			GPIOIntTypeSet(GPIO_PORTD_BASE, pin, type);
			GPIODirModeSet(GPIO_PORTD_BASE, pin, GPIO_DIR_MODE_IN);
			GPIOPadConfigSet(GPIO_PORTD_BASE, pin, GPIO_STRENGTH_2MA,resistor);
			GPIOIntEnable(GPIO_PORTD_BASE, pin);
			IntPrioritySet(INT_GPIOD,(priority<<5));
			IntEnable(INT_GPIOD);
			break;
		case PortE:  //PortE
			break;
		case PortF:  //PortF*/
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	//Enable clock on port F
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
			if(pin && GPIO_PIN_0) { GPIO_PORTF_LOCK_R = 0x4C4F434B; } //Unlock GPIO PF0 if necessary
			GPIO_PORTF_CR_R |= 0x1F;  //Allow changes to PF4-0
			
			IntDisable(INT_GPIOF);  //GPIO Port F disable of interrupts
			GPIOIntDisable(GPIO_PORTF_BASE,pin);  //Disable GPIO pin interrupt
			GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, pin);  //Set GPIO Input
			GPIOIntTypeSet(GPIO_PORTF_BASE, pin, type);  //Set Low level interrupt type
			GPIODirModeSet(GPIO_PORTF_BASE, pin, GPIO_DIR_MODE_IN);  //Set direction input
			GPIOPadConfigSet(GPIO_PORTF_BASE, pin, GPIO_STRENGTH_2MA,resistor); //Configure PUR
			GPIOIntEnable(GPIO_PORTF_BASE, pin);  //Enable GPIO pin interrupt
			IntPrioritySet(INT_GPIOF,(priority<<5)); //Priority 2 = "010"0.0000
			IntEnable(INT_GPIOF); //GPIO Port F enable of interrupts
			break;		
		default:
			return 0;  //error
	}
	return 1;  //OK
}

// ******** OS_EdgeTrigger_Restart ************
// restart button1 to signal on a falling edge interrupt
// rearm interrupt
// Inputs:  port, pin
// Outputs: 1 if config is successfull, 0 if not
uint8_t OS_EdgeTrigger_Restart(ports_t port, uint8_t pin){
	//arm interrupt
	//enable in NVIC
	//clear flags	???
	switch (port) {
		case PortA:  //PortA 
			break;
		case PortB:  //PortB
			break;
		case PortC:  //PortC
			break;
		case PortD:  //PortD
			GPIOIntEnable(GPIO_PORTD_BASE, pin);  //Enable GPIO pin interrupt
			IntEnable(INT_GPIOD); //GPIO Port D enable of interrupts
			break;
		case PortE:  //PortE
			break;
		case PortF:  //PortF
			GPIOIntEnable(GPIO_PORTF_BASE, pin);  //Enable GPIO pin interrupt
			IntEnable(INT_GPIOF); //GPIO Port F enable of interrupts
			break;		
		default:
			return 0;  //error
	}
	return 1;  //OK
}

// ******** OS_Clock_Init ************
// init the system clock to the desired speed in Mhz
// Inputs:  Desired frequency, MHz
// Outputs: 1 if config is successfull, 0 if not
uint8_t OS_Clock_Init(uint8_t clock_Mhz){
	switch (clock_Mhz) {
		case 16:  //16 MHz 
			break;
		case 80:  //80 MHz
			break;
		default:
			return 0;  //error
	}
	return 1;  //OK
}

// ******** OS_Timer_Init ************
// Init sesired system timer at desired frequency in Hz
// Inputs:  Selected timer, Desired frequency, MHz
// Outputs: 1 if config is successfull, 0 if not
uint8_t OS_Timer_Init(timers_t timer, uint16_t freqency, uint8_t priority){
	switch (timer) {
		case Timer0A:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	//Timer 0 enable
			IntDisable(INT_TIMER0A);	//Timer 0A disable of interrupts
			TimerIntDisable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	
			TimerDisable(TIMER0_BASE, TIMER_A);	
			TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
			TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	
			TimerLoadSet(TIMER0_BASE, TIMER_A, TIMER_reload_calculator(50));	//Set cycle nr for 50 ms	
			TimerEnable(TIMER0_BASE, TIMER_A);
	
			TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
			IntPrioritySet(INT_TIMER0A,(priority)<<5);	//Priority 0 = "000"0.0000
			IntEnable(INT_TIMER0A);	//Timer 0A enable of interrupts	
			break;
		case Timer0B:
		case Timer1A:
		case Timer1B:
		case Timer2A:
		case Timer2B:
		case Timer3A:
		case Timer3B:
		case Timer4A:
		case Timer4B:
		case Timer5A:
		case Timer5B:
		case WTimer0A:  // 32/64 bit
			SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);  //Wide Timer 0 enable 
			IntDisable(INT_WTIMER0A);  //Wide Timer 0A disable of interrupts
			TimerIntDisable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
	
			TimerDisable(WTIMER0_BASE, TIMER_A);
			TimerClockSourceSet(WTIMER0_BASE, TIMER_CLOCK_SYSTEM);
			TimerConfigure(WTIMER0_BASE, TIMER_CFG_A_PERIODIC);

			TimerLoadSet(WTIMER0_BASE, TIMER_A, TIMER_reload_calculatorx(1000));	//Set cycle nr for 1000 ms	
			TimerEnable(WTIMER0_BASE, TIMER_A);
	
			TimerIntEnable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
			IntPrioritySet(INT_WTIMER0A,(priority)<<5);  //Priority 1 = "001"0.0000
			IntEnable(INT_WTIMER0A);	//Wide Timer 0A enable of interrupts
			break;
		case WTimer0B:
		case WTimer1A:
		case WTimer1B:
		case WTimer2A:
		case WTimer2B:
		case WTimer3A:
		case WTimer3B:
		case WTimer4A:
		case WTimer4B:
		case WTimer5A:
		case WTimer5B:
		default:
			return 0;  //error
	}
	return 1;  //OK
}






#endif //TARGET_TM4C

#ifdef TARGET_MSP432
//MSP432 specific code
#endif //TARGET_TM4C

//EOF
