#include "can.h"
#include "delay.h"
#include "adc_defines.h"
#include "adc.h"
u32 val;
u32 AdcDval;
f32 eAR;
#define min 0.45
#define max 2.1

int main()
{
	struct CAN_Frame CANF;
	Init_CAN1();
	Init_ADC();
	CANF.ID=3; //ID=1 --> 0x01
	CANF.vbf.RTR=0; //data frame
	CANF.vbf.DLC=4; //1byte
	while(1)
	{
		Read_ADC(CH1,&AdcDval,&eAR);
		val=((eAR-min)/(max-min))*100;
		if(val>100)
		{
		    val=100;
		}

		CANF.Data1=val;
		CAN1_Tx(CANF);
	//	delay_ms(500); //1 delay 0 delay 1 delay 0
	}
}

