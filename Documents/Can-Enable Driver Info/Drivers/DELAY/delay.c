
void DelayMS(unsigned int dly)
{
	unsigned int i;
	for(;dly>0;dly--)
	{
		for(i=0;i<1200;i++);
	}
}

void DelayUs(int us)
{
	unsigned int i,j;
	for(j=0;j<us;j++)
	for(i=0;i<10;i++);
}

void delay_us(unsigned int dlyus)
{
	for(dlyus*=12; dlyus>0; dlyus--);
}

void delay_ms(unsigned int dlyms)
{
	dlyms*=12000;
	while(dlyms--);
}

void delay_s(unsigned int dlys)
{
	for(dlys*=12000000; dlys>0; dlys--);
}

