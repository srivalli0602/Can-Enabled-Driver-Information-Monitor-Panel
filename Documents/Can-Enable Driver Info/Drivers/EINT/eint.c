#include <lpc21xx.h>
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#include "delay.h"
#include "lcd.h"

#define EINT0_CH 14
#define EINT1_CH 15
volatile int flag=0;
volatile int flag1=0;

						  
void eint0_isr(void)__irq
{
	flag=!flag;
	//end of interrupt
	VICVectAddr=0;
	//clear eint0 flag
	EXTINT=1<<0;
	
}
void eint1_isr(void)__irq
{
	
	flag1=!flag1;
	//end of interrupt
	VICVectAddr=0;
	//clear eint1 flag
	EXTINT=1<<1;
}

void enable_eint0(void)
{
	//cfg p1.16 as output pin
	//cfg p0.16 as eint0 pin
	PINSEL1|=0x00000001;
	//select the type irq or fiq
	//VICIntSelect=0<<EINT0_CH;
	//enable Interrupt source
	VICIntEnable=1<<EINT0_CH;
	//load ISR Address
	VICVectAddr0=(unsigned int)eint0_isr;
	//select slot0
	VICVectCntl0=(1<<5)|EINT0_CH;
	//edge triggering
	EXTMODE=1<<0;
	//falling edge
	EXTPOLAR=0<<0;
}
void enable_eint1(void)
{
	//cfg p1.17 as output pin
	//cfg p0.14 as eint1 pin
	PINSEL0|=0x20000000;
	//enable Interrupt source
	VICIntEnable=1<<EINT1_CH;
	//load ISR Address
	VICVectAddr1=(unsigned int)eint1_isr;
	//select slot1
	VICVectCntl1=(1<<5)|EINT1_CH;
	//edge triggering
	EXTMODE |= 1<<1;
	//falling edge
	EXTPOLAR=0<<1;
	
}

