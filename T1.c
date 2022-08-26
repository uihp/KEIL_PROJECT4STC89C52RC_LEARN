#include <REGX52.H>

void T1Set() {
	TMOD &= 0x0F; // 1111 0000
	TMOD |= 0x10; // 0001 0000
	TL1 = 0x66;
	TH1 = 0xFC;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1;
	EA = 1;
	PT1 = 0;
}
