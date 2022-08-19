#include <REGX52.H>

sbit SER = P3^4;
sbit RCK = P3^5;
sbit SCK = P3^6;

void write_74HC595(unsigned char byte)
{
	unsigned char i;
	SCK = 0;RCK = 0;
	for (i=0;i<8;i++)
	{
		SER = byte&(0x80>>i);
		SCK = 1;SCK = 0;
	}
	RCK = 1;RCK = 0;
}
