#include "lcd.h"
#include "delay.h"
#include "eint.h"
#include "can.h"
#include "ds18b20.h"
#include <lpc21xx.h>
extern volatile int flag;
extern volatile int flag1;
unsigned char left_arrow[]={0x02, 0x06, 0x0e, 0x1e, 0x0e, 0x06, 0x02, 0x00};
unsigned char right_arrow[]={0x08, 0x0c, 0x0e, 0x0f, 0x0e, 0x0c, 0x08, 0x00};
unsigned char degree[] ={0x06,0x09,0x09,0x06,0x00,0x00,0x00,0x00};
char *p="CAN ENABLED DRIVER INFO & MONITOR PANEL";
char *start="*-*-*-*-*-*-*-*-*-*-";
char *end="*-*-*-*-*-*-*-*-*-*-";
int o,j;
void Title(void)
{
	CmdLCD(0x80);
	while(*start)
	{
	 	CharLCD(*start);
		delay_ms(80);
		start++;
	}
	CmdLCD(0xd4);
	while(*p)	
	{
		o++;
		if(o<20)
		{
			CmdLCD(0xc0+o);
		}
		else
		{
			CmdLCD(0x94+j);
			j++;
		}
		CharLCD(*p);
		delay_ms(80);
		p++;
	}
	CmdLCD(0xd4);
	while(*end)
	{
	 	CharLCD(*end);
		delay_ms(80);
		end++;
	}

}
void DisplayFuelBar(unsigned int fuel)
{
    int i;
    int bars;

    /* Limit fuel to 100 */
    if(fuel > 100)
        fuel = 100;

    /*
       Convert percentage into 8 blocks.

       100% -> 8 blocks
        75% -> 6 blocks
        50% -> 4 blocks
        25% -> 2 blocks
         0% -> 0 blocks
    */
    bars = (fuel * 8) / 100;

    CmdLCD(0x94);

    StrLCD("Fuel:");

    U32LCD(fuel);

    CharLCD('%');
	/*CmdLCD(0x94+9);
	for(i=0;i<10;i++)
	{
	    CharLCD(' ');
	} */

	CmdLCD(0x94+9);
    CharLCD('[');

    for(i=0; i<8; i++)
    {
        if(i < bars)
        {
            /*
               0xFF = full block character
               on a standard HD44780-compatible LCD
            */
            CharLCD(0xFF);
        }
        else
        {
            CharLCD(' ');
        } 
    }

    CharLCD(']');
	
}
int main()
{
	struct CAN_Frame CANRF;
	struct CAN_Frame CANF;
	int temp;
	unsigned char tp,tpd;
	Init_CAN1();
	Init_LCD();
	enable_eint0();
	enable_eint1();
	Init_LCD();	
	Title();
	delay_s(2);
	CmdLCD(0x80);//selecting starting line and staring pos
    StrLCD("                    ");
	CmdLCD(0xc0);//selecting second line and starting pos
	StrLCD("                    ");
	CmdLCD(0x94);//selecting third line and staring pos				
	StrLCD("                    ");
	CmdLCD(0xd4);//selecting fourth line and staring pos
	StrLCD("                    ");
	CANF.vbf.RTR=0; //data frame
	CANF.vbf.DLC=8; //1byte
	CmdLCD(0x80);//selecting starting line and staring pos
    StrLCD("VEHICLE MONITOR:");
/*	CmdLCD(0xC0);
	StrLCD("Temp =  ");
    CharLCD(tpd);
    StrLCD(" C  ");*/

  CmdLCD(0x94);
	StrLCD("Fuel:");
	CmdLCD(0xD4);
	StrLCD("indicator:");
	BuildCGRAM(left_arrow,8,0x40);
	CmdLCD(0xDe);
	CharLCD(0);
	BuildCGRAM(right_arrow,8,0x48);
	CmdLCD(0xDf);
	CharLCD(1);	
	BuildCGRAM(degree,8,0x50);
	while(1)
	{
	//	if(ResetDS18b20())
	//	{
			temp=ReadTemp();  //READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCOL
			tp  = temp>>4;	  //GETTING INTEGER PART
			tpd=temp&0x08?0x35:0x30;//GETTING FRACTIONAL PART
		
			CmdLCD(0xC0);
	 		StrLCD("Temp =");
			U32LCD(tp);
	
			CharLCD('.');
			CharLCD(tpd);
		  CharLCD(2);
			StrLCD(" C  ");
	//	 }
	//	 else
	//	 {
	//		 CmdLCD(0xC5);
	//		 StrLCD("N/A");
	//	 }

		 if(rx_check())
		 {
			CAN1_Rx(&CANRF);
			if(CANRF.ID==3)
			{
		 		CmdLCD(0x94);
				StrLCD("Fuel:    ");
				CmdLCD(0x94+5);
				U32LCD(CANRF.Data1);
                StrLCD("%");	
                DisplayFuelBar(CANRF.Data1);				
			}
		 }
	/*	else 
		 {
			CmdLCD(0x94+5);
			StrLCD("    ");		
			CmdLCD(0x94+5);
			StrLCD("N/A");		
		 } 	   */
 		 if(flag==1)
		 {
			CANF.ID=1;
			CANF.Data1=flag;
			CANF.Data2=flag1;
		   	CAN1_Tx(CANF);
			CmdLCD(0xDE);
			CharLCD(0);
			delay_ms(200);
			CmdLCD(0xDE);
			CharLCD(' ');
			CmdLCD(0xDf);	    
			CharLCD(1);	
			delay_ms(200);
		 }
		 if(flag1==1)
		 {
			CANF.ID=1;
			CANF.Data1=flag;
			CANF.Data2=flag1;
		   	CAN1_Tx(CANF);
			CmdLCD(0xdE);
			CharLCD(0);
			CmdLCD(0xdF);	    
			CharLCD(1);
			delay_ms(200);
			CmdLCD(0xdF);	    
			CharLCD(' ' );
			delay_ms(200);	
	  	}	
	  	else if((flag==0 && flag1==0)||(flag==1 && flag1==1))
	  	{
		   BuildCGRAM(left_arrow,8,0x40);
		   CmdLCD(0xdE);
		   CharLCD(0);
		   BuildCGRAM(right_arrow,8,0x48);
		   CmdLCD(0xdF);
		   CharLCD(1);	
		   CANF.ID=1;
	       CANF.Data1=flag;
    	   CANF.Data2=flag1;
	   	   CAN1_Tx(CANF);
	  	} 
	}	
}
