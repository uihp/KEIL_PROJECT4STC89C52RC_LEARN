#include <REGX52.H>
#include "AT24C02.h"
#include "segdisplay.h"
#include "indpdtbtn.h"
#include "delay.h"

unsigned char btnCode, min, sec, cse;
bit flag;

void stopwatchLoop() {
	if (flag) {
		cse++;
		if (cse >= 100) {
			cse = 0;
			sec++;
			if (sec >= 60) {
				sec = 0;
				min++;
				min %= 60;
			}
		}
	}
}

void T0Routine() interrupt 1 {
	static unsigned int T0Count4IndpdtBtn = 0, T0Count4Stopwatch = 0;
	T0Count4IndpdtBtn++;
	T0Count4Stopwatch++;
 	TL0 = 0x66;
 	TH0 = 0xFC;
	if (T0Count4IndpdtBtn >= 20) {
 		T0Count4IndpdtBtn = 0;
		btnEventLoop();
	}
	if (T0Count4Stopwatch >= 10) {
		T0Count4Stopwatch = 0;
		stopwatchLoop();
	}
	nixieDisplayLoop();
}

void main() {
	T0Set();
	while (1) {
		btnCode = independentBtnCheckByEventLoop();
		if (btnCode == 1) flag = !flag;
		else if (btnCode == 2) { min = 0; sec = 0; cse = 0; }
		else if (btnCode == 3) {
			write_AT24C02(0, min);
			delay(5);
			write_AT24C02(1, sec);
			delay(5);
			write_AT24C02(2, cse);
			delay(5);
		} else if (btnCode == 4) {
			min = read_AT24C02(0);
			sec = read_AT24C02(1);
			cse = read_AT24C02(2);
		}
		nixieDisplayBuf[0] = min/10;
		nixieDisplayBuf[1] = min%10;
		nixieDisplayBuf[2] = 11;
		nixieDisplayBuf[3] = sec/10;
		nixieDisplayBuf[4] = sec%10;
		nixieDisplayBuf[5] = 11;
		nixieDisplayBuf[6] = cse/10;
		nixieDisplayBuf[7] = cse%10;
	}
}
