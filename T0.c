#include <REGX52.H>

void T0Set()
{
	TMOD &= 0xF0; // 1111 0000
	TMOD |= 0x01; // 0000 0001
	TL0 = 0x66;
	TH0 = 0xFC;
	TF0 = 0;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}
