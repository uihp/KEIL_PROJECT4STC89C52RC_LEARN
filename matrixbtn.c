#include <REGX52.H>
#include "delay.h"

unsigned char globalMatrixBtnCode;

unsigned char matrixBtnScanByRowColCheck()
{
	static unsigned char btnCode;
	P1 = 0xFF;
	P1_0 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=16;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=12;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=8;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=4;}
	P1 = 0xFF;
	P1_1 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=15;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=11;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=7;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=3;}
	P1 = 0xFF;
	P1_2 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=14;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=10;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=6;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=2;}
	P1 = 0xFF;
	P1_3 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=13;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=9;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=5;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=1;}
	return btnCode;
}

unsigned char matrixBtnScanByFlip(bit wait) {
	static unsigned char btnCode;
	P1 = 0x0F;
	if (P1 != 0x0F) {
		delay(20);
		if (P1 != 0x0F) {
			P1 = 0x0F;
			switch (P1) {
				case 0x07: btnCode = 1; break;
				case 0x0B: btnCode = 2; break;
				case 0x0D: btnCode = 3; break;
				case 0x0E: btnCode = 4; break;
			}  
			P1 = 0xF0;
			switch(P1) {
				case 0x70: btnCode = btnCode; break;
				case 0xB0: btnCode += 4; break;
				case 0xD0: btnCode += 8; break;
				case 0xE0: btnCode += 12; break;
			}
			while (wait && P1 != 0xF0);
		}
	} else btnCode = 0;
  return btnCode;
}

unsigned char matrixBtnState() {
	static unsigned char btnCode;
	P1 = 0x0F;
	if (P1 != 0x0F) {
		if (P1 != 0x0F) {
			P1 = 0x0F;
			switch (P1) {
				case 0x07: btnCode = 1; break;
				case 0x0B: btnCode = 2; break;
				case 0x0D: btnCode = 3; break;
				case 0x0E: btnCode = 4; break;
			}  
			P1 = 0xF0;
			switch(P1) {
				case 0x70: btnCode = btnCode; break;
				case 0xB0: btnCode += 4; break;
				case 0xD0: btnCode += 8; break;
				case 0xE0: btnCode += 12; break;
			}
		}
	} else btnCode = 0;
  return btnCode;
}

void matrixBtnEventLoop(bit press) {
	static unsigned char state, last;
	last = state;
	state = matrixBtnState();
	if (press) {
		if (!state && last) globalMatrixBtnCode = last;
	} else if (!last && state) globalMatrixBtnCode = state;
}

unsigned char matrixBtnScanByEventLoop() {
	unsigned char tmp = globalMatrixBtnCode;
	globalMatrixBtnCode = 0;
	return tmp;
}
