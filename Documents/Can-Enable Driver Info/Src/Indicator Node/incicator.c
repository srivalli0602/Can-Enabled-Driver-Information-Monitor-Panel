#include "can.h"
#include "lcd.h"
#include "delay.h"
#include <lpc21xx.h>
#define LED 0
int main()
{
	struct CAN_Frame CANRF;
	int i,ls,rs;
	Init_CAN1();
	IODIR0|=255<<LED;
   	IOPIN0=255<<LED;
	while(1)
	{
	if(rx_check())
	{
		CAN1_Rx(&CANRF);
		ls=CANRF.Data1;
		rs=CANRF.Data2;
		if(CANRF.ID==1)
		{
		if((ls==1)&&(rs==0))
	  	{
			for(i=0;i<=7;i++)
			{
		  	if(ls==0)
				 break;
			 IOPIN0=((~(1<<i))&255)<<LED;
			 delay_ms(50);
			 }
		  }
		  else if((rs==1)&&(ls==0))
		  {
		     for(i=7;i>=0;i--)
			 {
				 if(rs==0)
					 break;
				 IOPIN0=((~(1<<i))&255)<<LED;
				 delay_ms(50);
			 }
	      }	
	  	  else if((ls==0 && rs==0)||(ls==1 && rs==1))
	      {
	      	IOPIN0=255<<LED;
	      }
		  }
		}	
	//]U32LCD(CANRF.Data1);
	}
}


