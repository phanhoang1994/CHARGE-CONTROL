//*************************************
//* File: 			
//*	Date: 			1/1/2019
//*	Creator:		Phan Van Hoang
//* Last modify:	
//* Version: 		V1.0. 
//*************************************

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "../inc/uart4.h"
#include "../inc/global.h"
#include "../inc/function.h"

unsigned char ucUART4FrameTx[UART4_FRAME_SIZE];
unsigned char ucUART4BufferRx[UART4_FRAME_SIZE];

volatile unsigned char ucUART4DelayTx;
volatile unsigned char ucUART4DelayRx;
volatile unsigned char ucUART4DelaySendRequest;
volatile unsigned char ucUART4CounterRx;

volatile eUartStateHandler eUART4StateRx;
static volatile unsigned char ucUART4CounterTx;
static volatile unsigned char ucUART4RdIndexTx;
unsigned char ucUART4IntervalTx;

void vUART4Config(void)
{
	// Enable the peripherals used by the application.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
 
 	// Thiet lap thong so cho Port UART4
	// Toc do baud 115200, 8 bit, 1 stop, no parity
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);	
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX);

	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(),19200,
                       (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                        UART_CONFIG_PAR_EVEN));
	// Enable received interrupt and receive time out interrupt
  IntEnable(INT_UART4_TM4C123);
  UARTIntEnable(UART4_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);	// Interrupt receive and  timeout			   
	UARTFIFOLevelSet(UART4_BASE,UART_FIFO_TX2_8,UART_FIFO_RX2_8);
	ucUART4DelaySendRequest = UART4_INTERVAL_SEND_REQ;
	eUART4StateRx = STATE_RX_IDLE;
	_dir_off();
}
//*-----------------------------------------------------------*/
// Ham xu ly ngat UART4
void vUART4Handler(void)
{
  unsigned char ucData,i;
	unsigned long ulStatus;

	// Doc trang thai tao ngat va xoa de cho ngat moi.
    ulStatus = UARTIntStatus(UART4_BASE, true);
    UARTIntClear(UART4_BASE, ulStatus);
		
	if(ulStatus==UART_INT_TX)
	{
		i=0;
		while((i<8)&(ucUART4CounterTx>0)&(ucUART4RdIndexTx>=16))
		{
			UARTCharPutNonBlocking(UART4_BASE, ucUART4FrameTx[ucUART4RdIndexTx]);
		 	++i;
			--ucUART4CounterTx;
			++ucUART4RdIndexTx;
		}
		ucUART4DelayTx = UART4_INTERVAL_TX;
	}	
	else
	{
		while(UARTCharsAvail(UART4_BASE))
		{
			ucUART4DelayRx = UART4_INTERVAL_RX;
			ucData = UARTCharGet(UART4_BASE);
			switch(eUART4StateRx)
			{
				case STATE_RX_INIT:
				{

					break;
				}
				case STATE_RX_ERROR:
				{

					break;
				}		
				case STATE_RX_IDLE:
				{
					ucUART4CounterRx = 0;
					ucUART4BufferRx[ucUART4CounterRx++] = ucData;
					if(ucUART4BufferRx[0]!=0x96){ucUART4BufferRx[0]=0;break;}
					eUART4StateRx = STATE_RX_RECEIVE;
					
					break;
				}
				case STATE_RX_RECEIVE:
				{
					if(ucUART4CounterRx<UART4_FRAME_SIZE)
					{
						ucUART4BufferRx[ucUART4CounterRx++] = ucData;
					}
					else
					{
						eUART4StateRx = STATE_RX_ERROR;
					}
					break;
				}
			}
		}	
	}
}

/*-----------------------------------------------------------*/
// Ham phat chuoi ky tu
void vUART4PutString(unsigned char *pucStr)
{
	// Gui ky tu cho den het chuoi, khi do noi dung = 0.
	while(*pucStr){UARTCharPut(UART4_BASE, *pucStr++);};
}


// Send message in buffer.
void vUART4Send(unsigned char ucLen)
{
	// Put transmit frame to tx buffer.
	ucUART4CounterTx=ucLen;
	ucUART4RdIndexTx=0;
	_dir_on();
	while((ucUART4RdIndexTx<16)&(ucUART4CounterTx>0))
	{
		UARTCharPutNonBlocking(UART4_BASE, ucUART4FrameTx[ucUART4RdIndexTx]);
		--ucUART4CounterTx;
		++ucUART4RdIndexTx;
	}

	// Transmit again.	
	ucUART4DelayTx = UART4_INTERVAL_TX; // >=11 character ,baudrate=2500 => 36ms
}

