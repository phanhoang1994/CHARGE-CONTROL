#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define _ledrun_off()		  	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_PIN_4)
#define _ledrun_on()				GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0)

#define _dir_on()						GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6)
#define _dir_off()					GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0)

//Outport
#define _Q3_on()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7)
#define _Q3_off()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0)

#define _Q2_on()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6)
#define _Q2_off()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0)

#define _Q1_on()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_PIN_5)
#define _Q1_off()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0)

#define _Q0_on()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4)
#define _Q0_off()						GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0)

#define _BAT_LOW_OFF()						GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_PIN_2)
#define _BAT_LOW_ON()							GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2,0)

#define _BAT_FAULT_OFF()					GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_PIN_1)
#define _BAT_FAULT_ON()						GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1,0)

#define _BAT_TEST_OFF()						GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_PIN_3)
#define _BAT_TEST_ON()						GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,0)


#endif
