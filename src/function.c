//*************************************
//* File: 			
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <cstdint>

/* Standard includes. */
#include "inc/tm4c123ae6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "../inc/global.h"
#include "../inc/function.h"
#include "../inc/uart4.h"
#include "../inc/adc.h"
#include "../inc/display.h"

#define SAMPLE			4

unsigned long ulSample[SAMPLE];
unsigned char ucSampleIndex;
unsigned char datatime[7];
unsigned char count;

unsigned char count1;

unsigned long ulTimerInport;
volatile unsigned long timercheckAQ;
volatile unsigned long timersend;

unsigned long ulInport;
unsigned long ulPreInp;
unsigned long ulOutport;
unsigned long ulDataIn;
unsigned long timer_1s;

volatile unsigned long ulTimerAdc;

bool bCheckQuality;
bool bsend;

extern uint8_t _second,_minute,_hour,_day,_date,_month,_year;
extern 	double _gVoltageADC;
extern 	double _gVoltageCheckAQ;
bool _gReadADC;
bool _gReportStatusAQ;
uint8_t gStatusAQ;
bool _gWriteRelativeError=0;
bool _gReadRelativeError=1;
double _gRelativeError;
double _gRelativeErrorSaveEpprom;

extern unsigned char ucUART4BufferRx[UART4_FRAME_SIZE];
extern unsigned char ucUART4FrameTx[UART4_FRAME_SIZE];


void vInput(void)
{
	unsigned long result, all_or=0, all_and=0;
	unsigned char i;
	
	//INPUT
	ulSample[ucSampleIndex] = (unsigned long)GPIOPinRead(GPIO_PORTF_BASE, 0x0F) << 8;
	//BUTTON
	ulSample[ucSampleIndex] |= 0x00000000;
	ulSample[ucSampleIndex] &= 0x00000F00;
	
	all_or = ulSample[0];
	all_and = all_or;
	for(i=1;i<SAMPLE;i++)
	{
		all_or |= ulSample[i];
		all_and &= ulSample[i];
	}

	result = ~ulInport;
	result &= 0x00000F00;
	
	result &= all_or;
	result |= all_and;
	
	ulInport = ~result;
	ulInport &= 0x000000F00;
	
	if(++ucSampleIndex>=SAMPLE) ucSampleIndex=0;
}
//OUTPUT
void vOutput(void)
{	
	if((bCheckQuality==true)&&(timercheckAQ>0)){
		_gReadADC = ENABLE;
		_Q3_on();												//Dong relay cho phep test
		_clear(ucUART4FrameTx[5],0);
		if (_gReportStatusAQ==1){
			if(_gVoltageCheckAQ<60){_BAT_FAULT_ON();_BAT_LOW_OFF();_set(ucUART4FrameTx[5],1);}
			else if ((_gVoltageCheckAQ>=60)&(_gVoltageCheckAQ<62)){_BAT_LOW_ON();_BAT_FAULT_OFF();_set(ucUART4FrameTx[5],2);}
			else {_BAT_LOW_OFF();_BAT_FAULT_OFF();_clear(ucUART4FrameTx[5],1);_clear(ucUART4FrameTx[5],2);}
			_gReportStatusAQ=0;
		}
	}
	else {
		_gReadADC = DISABLE;
		_gReportStatusAQ=0;
		_gVoltageCheckAQ=0;
		_Q3_off();
	}
}

void vPoll(void)
{
	uint8_t i;
	unsigned char temp;
//HIEN THI 
	display();
//DOC GIA TRI ADC
	if((_gReadADC==ENABLE)&&(ulTimerAdc==0))
	{
		vReadADC();
	}
	
//DOC NGO VAO - NGO RA
	if(ulTimerInport == 0)
	{ 
		vInput();	
		vOutput();		
		if(ulInport != ulPreInp){ ulPreInp = ulInport;}
		ulTimerInport = 25;
	}
//GIAO TIEP SOC210
	if((ucUART4BufferRx[0]==0x96)&&(ucUART4BufferRx[1]==0x01)&&(ucUART4BufferRx[2]==0x04)){
		temp=0;
		for(i=0;i<7;i++){temp += ucUART4BufferRx[i];}
		if(ucUART4BufferRx[7] == temp){
			if((ucUART4BufferRx[5]==0x01)&&(_isset(ulInport,8))){
				bCheckQuality=true;
				bsend = 1;
				if(count==0){timercheckAQ = 125000;count = 1;}
			}
			else{
				bCheckQuality=false;
			}
		}
			//set frame send to SOC210
			ucUART4FrameTx[0]= 0x88;
			ucUART4FrameTx[1]= 0x04;
			ucUART4FrameTx[2]= 0x01;
			ucUART4FrameTx[3]= 0;
			ucUART4FrameTx[4]= 0;
			ucUART4FrameTx[6]= 0;
			temp=0;
			if((timercheckAQ<5000)&&(timercheckAQ>1000)){_set(ucUART4FrameTx[5],0);}
			for(i=0;i<7;i++){temp += ucUART4FrameTx[i];}
			ucUART4FrameTx[7]= temp;
			if(timercheckAQ==1){count = 0;ucUART4BufferRx[5]=0;}
			if((timersend==0)&&(bsend == 1)){vUART4Send(8);bsend = 0;timersend=500;}
		}
}
