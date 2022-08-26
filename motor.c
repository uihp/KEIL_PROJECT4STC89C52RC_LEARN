#include <REGX52.H>
#include "T1.h"

sbit motor = P1^0;

unsigned char motorSpeed = 0;

void init_Motor() {
	T1Set();
}

void T1Routine() interrupt 3 {
	static unsigned char T1Count = 0;
	T1Count++;
	TL1 = 0xA4;
	TH1 = 0xFF;
	motor = T1Count < motorSpeed;
}
