// os_hw.c - ssOS Target HW specific code - Interrupt Service Routines, Target configurations
// ssOS - stupid simple Operating System
// A very simple real time operating system with minimal features.
// For copyright, configuration and usage read readme.txt

/*------Export interface---Self header Includes------*/
#include "os_hw.h"
//#include "../inc/tm4c123gh6pm.h" //AleGaa

#ifdef TARGET_TM4C
//TM4C specific code

PortSema_t SemPortA;
PortSema_t SemPortB;
PortSema_t SemPortC;
PortSema_t SemPortD;
PortSema_t SemPortE;
PortSema_t SemPortF;

void GPIOPortD_Handler(void){	 //PortD Edge GPIO interrupt handler
	// step 1 acknowledge by clearing flag
  // step 2 signal semaphore (no need to run scheduler)
  // step 3 disarm interrupt to prevent bouncing to create multiple signals
	uint8_t status;	
	status = GPIOIntStatus(GPIO_PORTD_BASE,false);
	IntDisable(INT_GPIOD); //GPIO Port D disable of interrupts	
	if(status & GPIO_INT_PIN_0) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_0); //acknowledge by clearing flag
		GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_0);  //disarm interrupt to prevent bouncing to create multiple signals
		OS_Signal(&SemPortD.pin0);  //signal semaphore
	}
	if(status & GPIO_INT_PIN_1) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_1); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_1); OS_Signal(&SemPortD.pin1);}
	if(status & GPIO_INT_PIN_2) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_2); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_2); OS_Signal(&SemPortD.pin2);}
	if(status & GPIO_INT_PIN_3) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_3); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_3); OS_Signal(&SemPortD.pin3);}
	if(status & GPIO_INT_PIN_4) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_4); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_4); OS_Signal(&SemPortD.pin4);}
	if(status & GPIO_INT_PIN_5) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_5); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_5); OS_Signal(&SemPortD.pin5);}
	if(status & GPIO_INT_PIN_6) {
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_6); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_6); OS_Signal(&SemPortD.pin6);}
	if(status & GPIO_INT_PIN_7) {		
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_7); GPIOIntDisable(GPIO_PORTD_BASE, GPIO_INT_PIN_7); OS_Signal(&SemPortD.pin7);}
	//IntDisable(INT_GPIOD);  
  OS_Suspend();
}

void GPIOPortF_Handler(void){	 //PortF Edge GPIO interrupt handler
	uint8_t status;	
	status = GPIOIntStatus(GPIO_PORTF_BASE,false);
	if(status & GPIO_INT_PIN_0) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0); //acknowledge by clearing flag
		OS_Signal(&SemPortF.pin0);  //signal semaphore
		GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);  //disarm interrupt to prevent bouncing to create multiple signals
	}
	if(status & GPIO_INT_PIN_1) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_1); OS_Signal(&SemPortF.pin1); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_1);}
	if(status & GPIO_INT_PIN_2) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_2); OS_Signal(&SemPortF.pin2); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_2);}
	if(status & GPIO_INT_PIN_3) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_3); OS_Signal(&SemPortF.pin3); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_3);}
	if(status & GPIO_INT_PIN_4) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4); OS_Signal(&SemPortF.pin4); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);}
	if(status & GPIO_INT_PIN_5) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_5); OS_Signal(&SemPortF.pin5); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_5);}
	if(status & GPIO_INT_PIN_6) {
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_6); OS_Signal(&SemPortF.pin6); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_6);}
	if(status & GPIO_INT_PIN_7) {		
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_7); OS_Signal(&SemPortF.pin7); GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_7);}
	IntDisable(INT_GPIOF);  //GPIO Port F disable of interrupts
  OS_Suspend();	
}

// ******** OS_EdgeTrigger_Init ************
// Initialize a button, to signal on edge interrupt
// Inputs:  port , pin, priority, type, resistor
// Outputs: 1 if config is successfull, 0 if not
//#define ClockEnable(port)		(SysCtlPeripheralEnable(SYSCTL_PERIPH_##port))	//AleGaa
//#define InterruptDisable(port)	(IntDisable(INT_##port))	//AleGaa
uint8_t OS_EdgeTrigger_Init(ports_t port, uint8_t pin, uint8_t priority, uint8_t type, uint8_t resistor){
	uint32_t clock;
	uint32_t bit_prio;
	switch (port) {
		case PortA:  //PortA 
			break;
		case PortB:  //PortB
			break;
		case PortC:  //PortC
			break;
		case PortD:  //PortD	
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
			//if(pin == Pin0) { GPIO_PORTD_LOCK_R = 0x4C4F434B; } //Unlock PortF PF0 	//AleGaa
			//GPIO_PORTD_CR_R |= 0x1F;  //Allow changes 	//AleGaa
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
			if(pin == Pin0) { GPIO_PORTF_LOCK_R = 0x4C4F434B; } //Unlock PortF PF0
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
#endif //TARGET_TM4C

#ifdef TARGET_MSP432
//MSP432 specific code
#endif //TARGET_TM4C

//EOF
