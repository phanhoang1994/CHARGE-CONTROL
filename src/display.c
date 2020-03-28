//*************************************
//* File: 			display.c
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "../inc/global.h"
#include "../inc/display.h"
uint8_t colx;
uint8_t Led7_Disp[4];
uint8_t led_RelativeError[4];
uint8_t led_gVoltageCheckAQ[4];
extern double _gVoltageCheckAQ;


unsigned char Code7Seg[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x00}; //katot chung
unsigned long timer_point;
extern unsigned long ulInport;


//HIEN THI LED 7 DOAN
void display(void)
{
//write data 
	decode_gVoltageCheckAQ();
	write_led(led_gVoltageCheckAQ[colx]);

//tat 4 led               
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);//led1 - hang don vi
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,0);//led2 - hang chuc
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_1,0);//led3 - hang tram
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,0);//led4 - hang ngan
	
	LATCH_HIGH;
	delay_us(10);
	LATCH_LOW;
//bat 1 trong 4 led
	switch (colx)
	{
		case 0:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
			colx+=1;
			break;
		case 1:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_PIN_2);
			colx+=1;
			break;
		case 2:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_1,GPIO_PIN_1);
			colx+=1;
			break;
		case 3:
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_PIN_0);
			colx=0;
			break;
	}	
}
//XUAT DATA --> 74HC595
void write_led(uint8_t data)
{
	uint8_t i;
	for (i = 0;i<8;i++)
	{
		if((data<<i)&0x80) 
			{SPI_HIGH;}
		else 
			{SPI_LOW;}
		
		delay_us(10);
		CLK_HIGH;
		delay_us(10);
		CLK_LOW;
		delay_us(10);
	}
}
//giai ma dien ap 
void decode_gVoltageCheckAQ(void)
{
	uint16_t temp;
	uint16_t u16temp;
	uint8_t  u8temp;
	
	temp = _gVoltageCheckAQ*100;
	if(temp <1000){
		led_gVoltageCheckAQ[3]=0x40;
		led_gVoltageCheckAQ[2]=0x40;
		led_gVoltageCheckAQ[1]=0x40;
		led_gVoltageCheckAQ[0]=0x40;
	}
	else {
		u8temp =temp/1000;
			if(u8temp==0)
				led_gVoltageCheckAQ[3]=0;
			else
				led_gVoltageCheckAQ[3]=Code7Seg[u8temp];
		u16temp=temp%1000;
		u8temp=u16temp/100;
			if((u8temp==0)&&(led_gVoltageCheckAQ[3]==0))
				led_gVoltageCheckAQ[2]=0;
			else
				if (_gVoltageCheckAQ*100<10000)
				{
					led_gVoltageCheckAQ[2]=Code7Seg[u8temp]|0x80;
				}
				else
				{
					led_gVoltageCheckAQ[2]=Code7Seg[u8temp];
				}
		u16temp%=100;
		u8temp=u16temp/10;
			if ((u8temp==0)&&(led_gVoltageCheckAQ[3]==0)&&(led_gVoltageCheckAQ[2]==0))
				led_gVoltageCheckAQ[1]=0;
			else
				led_gVoltageCheckAQ[1]=Code7Seg[u8temp];
		u8temp=u16temp%10;
				led_gVoltageCheckAQ[0]=Code7Seg[u8temp];
	}
}

//_delay_us
void delay_us(unsigned long time)
{
	unsigned int i;
	for (i=0;i<time;i++)
	{
		;
	}
}
//_delay_ms
void delay_ms(unsigned long time)
{
	unsigned int i,j;
	for (i=0;i<time;i++)
	{
		for (j=0; j<1250;j++)
		{
			;
		}
	}
}

