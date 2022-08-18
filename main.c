#include <REGX52.H>
#include "T0.h"
#include "delay.h"

int mode = 0;
int temp = 0x80;

void T0Routine() interrupt 1
{
	static unsigned int T0Count = 0;
	T0Count++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count >= 200) {
		T0Count = 0;
		if (mode) {
			if (temp == 0x01) temp = 0x80;
			else temp >>= 1;
		} else {
			if (temp == 0x80) temp = 0x01;
			else temp <<= 1;
		}
		P2 = ~temp;
	}
}

void main()
{
	T0Set();
	while (1) {
		if (P3_1 == 0) {
			delay(20);
			while (P3_1 == 0);
			delay(20);
			mode = !mode;
		}
	}
}
