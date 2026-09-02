#include <lpc21xx.h>
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "ADC_defines.h"
#include "delay.h"
void Init_ADC(void)
{
	//make p0.27 as GPIO
	PINSEL1&=~(3<<((28-16)*2));
	//cfg p0.27 as AIN0 pin
	//PINSEL1=(PINSEL1&~(3<<((27-16)*2)))|(1<<((27-16)*2));
	cfgportpin(PORT0,PIN28,FUN2);
	//activate the ADC peripheral, set adc clk freq
	ADCR=PDN_BIT|CLKDIV_VALUE;
}
void Read_ADC(u32 CHNO, u32* AdcDval,f32* eAR)
{
	//clear the channel bits in ADCR
	ADCR&=~(255<<0);
	//select channel no and start conv
	ADCR|=CHNO|START_CONV;
	//wait for conv upto 3usec
	delay_us(3);
	//wait for done_bit status
	while(((ADDR>>DONE_BIT)&1)==0);
	//stop the conv
	ADCR&=~START_CONV;
	//extract the result from ADDR
	*AdcDval=((ADDR>>RESULT)&0x3FF);
	//get the equivalent analog reading
	*eAR= (3.3/1023)*(*AdcDval);
}
