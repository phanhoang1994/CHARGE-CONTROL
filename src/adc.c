//*************************************
//* File: 			
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_can.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/can.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"

#include "../inc/global.h"
#include "../inc/function.h"

void vADC0Handler(void);
extern volatile unsigned long ulTimerAdc;
extern bool _gReportStatusAQ;
extern double _gRelativeErrorSaveEpprom;

double _gVoltageADC[100];
double _gVoltageAQ; 
double _gVoltageAverageAQ[5];
double _gVoltageCheckAQ;

uint8_t cout1=0;
uint8_t cout2=0;
//------------------------
uint16_t test;

void vADCConfig(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlDelay(10);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlDelay(10);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
	
	ADCReferenceSet(ADC0_BASE, ADC_REF_EXT_3V);
	
	ADCSequenceDisable(ADC0_BASE, 3);
	ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_IE|ADC_CTL_END|ADC_CTL_CH3);
	
	ADCIntRegister(ADC0_BASE,3,&vADC0Handler);
	ADCIntEnable(ADC0_BASE,3);
	
	ADCSequenceEnable(ADC0_BASE,3);
}

void vADC0Handler(void){
	uint32_t Data[1];
	uint8_t i;
	double temp1=0;
	double temp2=0;
	if (ulTimerAdc==0)
	{
		ADCIntClear(ADC0_BASE,3);
		ADCProcessorTrigger(ADC0_BASE,3);
		while(!ADCIntStatus(ADC0_BASE,3, false));
		ADCSequenceDataGet(ADC0_BASE,3,(uint32_t *)&Data);
		_gVoltageADC[cout1] = (double)((Data[0]*(3.31682/4095)));
		ADCIntDisable(ADC0_BASE,3);
		cout1++;
		//lay mau 100 gia tri ==> lay gia tri trung binh (1s --> xuat ket qua 1 lan)
		if(cout1==100){
			for (i=0;i<100;i++)
			{
				temp1 += _gVoltageADC[i];
			}
			temp2=temp1/100;
			_gVoltageAQ = (temp2/4.7)*(47+47+4.7);
			_gVoltageAverageAQ[cout2]=_gVoltageAQ;
			cout2++;
			//lay gia tri trung binh cong sau 5 lan do
			cout1=0;
		}
		if (cout2==5){
			_gVoltageCheckAQ=((_gVoltageAverageAQ[0]+_gVoltageAverageAQ[1]+_gVoltageAverageAQ[2]+_gVoltageAverageAQ[3]+_gVoltageAverageAQ[4])/5);
			_gReportStatusAQ=1;
			cout2=0;
		}
		ulTimerAdc=10;
	}
}

void (vReadADC)(void)
{
	ADCIntEnable(ADC0_BASE,3);
	SysCtlDelay(5);
	ADCProcessorTrigger(ADC0_BASE,3);
}
