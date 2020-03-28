//*************************************
//* File: 			
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************


/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>


#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/watchdog.h"

#include "../inc/sysclock.h"
#include "../inc/global.h"
#include "../inc/function.h"
#include "../inc/adc.h"
#include "../inc/uart4.h"

void vHardwareConfig(void);
extern void vPoll(void);
extern void vSysTickConfig(void);
extern void vCANConfig(void);
extern void vADCConfig(void);
volatile unsigned char ucWDT;


void vWDTHandler(void)
{ 
	if(ucWDT == 0){ WatchdogIntClear(WATCHDOG0_BASE);}
	ucWDT = 1;
}

int main(void)
{
 	vHardwareConfig();
	while(1)
	{ 	
		vPoll();
		ucWDT = 0;
	}
} 

void vWDTConfig(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
   	IntEnable(INT_WATCHDOG);
    WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet());
    WatchdogResetEnable(WATCHDOG0_BASE);
    WatchdogEnable(WATCHDOG0_BASE);
}

void vHardwareConfig(void)
{
	volatile unsigned long timeout = 50000;

	/* Set the clocking to run from the PLL at 50 MHz */
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) &= ~0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
		
// set up inport
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //IN1..IN2, SWD1..SWD2
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,	GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
// set up outport
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xFF); //L1..L4, OUTR1..OUTR4
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4); //LED RUN
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //TESTTING,FAULT,LOW
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3); //SPI: CLK,LATCH,DS
	
	
	while(--timeout);
	vADCConfig();
	vUART4Config();
	vSysTickConfig();
	vWDTConfig();
	IntMasterEnable();
} 
