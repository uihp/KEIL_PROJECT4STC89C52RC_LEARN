#include <REGX52.H>
#include "delay.h"
#include "indpdtbtn.h"
#include "segdisplay.h"
#include "T0.h"

sbit motor = P1^0;

unsigned char btnCode, target = 0;

void T0Routine() interrupt 1 {
	static unsigned char T0Count = 0;
	T0Count++;
	T0Count %= 100;
	TL0 = 0xA4;
	TH0 = 0xFF;
	motor = T0Count < target;
}

void main() {
	T0Set();
	nixieLongShow(1, 0);
	while (1) { 
		btnCode = independentBtnCheck();
		if (btnCode) {
			switch (btnCode) {
				case 1: target = 0; break;
				case 2: target = 50; break;
				case 3: target = 75; break;
				case 4: target = 100; break;
			}
			nixieLongShow(1, btnCode-1);
		}
	}
}
