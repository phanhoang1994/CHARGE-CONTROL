//*************************************
//* File: 			display.c
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************
#include <stdint.h>
#include <stdbool.h>

/* Standard includes. */
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "../inc/global.h"
#include "../inc/function.h"
#include "../inc/uart4.h"

volatile uint16_t count_1s;
volatile uint8_t toggle;

extern unsigned long ulInport;
extern volatile uint16_t ulTimerInport;
extern volatile uint16_t timer_1s;
extern volatile unsigned long ulTimerAdc;
extern volatile unsigned long timercheckAQ;
extern volatile unsigned char ucUART4DelayTx;
extern volatile unsigned char ucUART4DelayRx;
extern volatile eUartStateHandler eUART4StateRx;
extern volatile unsigned long timercheckAQ;
extern volatile unsigned long timersend;

void vSysTickHandler(void)//1ms
{
	if(++count_1s >= 1000){ count_1s = 0; if(toggle == 1){_ledrun_on(); toggle = 0;} else {_ledrun_off(); toggle = 1;}}
	if(ulTimerInport>0){--ulTimerInport;}
	if(ulTimerAdc>0){--ulTimerAdc;}
	if(timercheckAQ>0){--timercheckAQ;}
	if(timersend>0){--timersend;}
	if(timer_1s>0){--timer_1s;}
	if(ucUART4DelayRx>0){--ucUART4DelayRx;}
	if(ucUART4DelayTx>0){--ucUART4DelayTx;}
	if(ucUART4DelayRx == 1){eUART4StateRx=STATE_RX_IDLE; ucUART4DelayRx  = 0;}
	if(ucUART4DelayTx == 1){_dir_off();ucUART4DelayTx = 0;}
	
}

void vSysTickConfig(void)
{
	SysTickPeriodSet((CPU_CLOCK_HZ / TICK_RATE_HZ)-1UL);
	SysTickIntEnable();
	SysTickEnable();
}
