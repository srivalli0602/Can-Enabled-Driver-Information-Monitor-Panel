#include "defines.h"
#include "delay.h"
#include "LCD_defines.h"
#include <lpc21xx.h>
#include "types.h"

void Write_LCD(u8 data)
{
	//perform write operation r/w=0
	SCLRBIT(IOCLR0,RW_PIN);
	//write data into LCD
	//IOPIN0=(IOPIN0&~(255<<LCD_DATA))|(data<<LCD_DATA);
	WRITEBYTE(IOPIN0,LCD_DATA,data);
	//apply H to L pulse on EN_PIN
	SSETBIT(IOSET0,EN_PIN);//high pulse on EN_PIN
	delay_us(1);
	SCLRBIT(IOCLR0,EN_PIN);//low pulse on EN_PIN
	//delay for internal process
	delay_ms(2);
}

void CmdLCD(u8 cmd)
{
	//select command reg RS=0
	SCLRBIT(IOCLR0,RS_PIN);
	//write command into LCD
	Write_LCD(cmd);
}
void CharLCD(u8 ascii)
{
	//select data reg RS=1
	SSETBIT(IOSET0,RS_PIN);
	//write dta into LCD
	Write_LCD(ascii);
}

void Init_LCD(void)
{
	//cfg p0.0 t0 p0.7(lcd_data_pins) as output pins
	WRITEBYTE(IODIR0,LCD_DATA,0XFF);
	//cfg p0.8, p0.9,and p0.10(rs,rw,en pins) asoutput pins
	SETBIT(IODIR0,RS_PIN);
	SETBIT(IODIR0,RW_PIN);
	SETBIT(IODIR0,EN_PIN);
	//wait for 15 ms@ 5V
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(5);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30); //8-bit mode function set

	CmdLCD(0x38); //8-bit mode 2lines
	//CmdLCD(0x0E); //disp_on_cur_on
	CmdLCD(0x0c);//disp_on_cur_off
	CmdLCD(0x01); //clear LCD
	CmdLCD(0x06); //entry mode(shift_cur_right)
}
void StrLCD(char* str)
{
	while(*str)
	{
		CharLCD(*str);
		*str++;
	}
}
void U32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+'0';
			n/=10;
		}
		for(--i; i>=0; i--)
		{
			CharLCD(a[i]);
		}
	}
}
void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);
}

void BuildCGRAM(u8* p, u32 nbytes,int a)
{
	s32 i;
	//goto CGRAM
	CmdLCD(a);
	for(i=0; i<nbytes; i++)
	{
		CharLCD(p[i]);
	}
	//go back to DDRAM
//	CmdLCD(0xC0);
}	
void F32LCD(f32 f, u8 ndp)
{
	u32 n,i;
	if(f<0.0)
	{
		CharLCD('-');
		f=-f;
	}
	n=f;
	U32LCD(n);
	CharLCD('.');
	for(i=0; i<ndp; i++)
	{
		f=(f-n)*10;
		n=f; 
		CharLCD(n+48);
	}
}




