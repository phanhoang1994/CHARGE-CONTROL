#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define SPI_HIGH 		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,GPIO_PIN_3)
#define SPI_LOW			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0)

#define CLK_HIGH		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_PIN_0)
#define CLK_LOW			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,0)

#define LATCH_HIGH	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_PIN_1)
#define LATCH_LOW		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1,0)


void display(void);
void write_led(uint8_t data);
void decode_gVoltageCheckAQ(void);
void decode_RelativeError(void);


void delay_us(unsigned long time);
void delay_ms(unsigned long time);
#endif
