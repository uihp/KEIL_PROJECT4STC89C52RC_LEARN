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

void T0InitCounter() {
	TMOD &= 0xF0; // 1111 0000
	TMOD |= 0x01; // 0000 0001
	TF0 = 0;
	TR0 = 0;
}

void T0SetCounter(unsigned int val) {
	TL0 = val%256;
	TH0 = val/256;
}

unsigned int T0GetCounter() {
	return (TH0<<8)|TL0;
}
