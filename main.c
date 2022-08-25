#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "indpdtbtn.h"
#include "delay.h"
#include "T0.h"

float T;
char TH, TL;
unsigned char btnCode;

void T0Routine() interrupt 1 {
	static unsigned int T0Count = 0;
	T0Count++;
 	TL0 = 0x66;
 	TH0 = 0xFC;
	if (T0Count >= 20) {
 		T0Count = 0;
		btnEventLoop();
	}
}

void reviseThreshold() {
	if (TH > 125) TH = 125;
	else if (TH <= TL) TH++;
	else if (TL >= TH) TL--;
	else if (TL < -55) TL = -55;
	if (TH <= TL) {
		TH = 20;
		TL = 15;
	}
}

void main() {
	convertT_DS18B20();
	T0Set();
	LCD_Init();
	LCD_ShowString(1,1,"T:");
	LCD_ShowString(2,1,"TH:");
	LCD_ShowString(2,9,"TL:");
	TH = read_AT24C02(0);
	TL = read_AT24C02(1);
	reviseThreshold();
	delay(500);
	while (1) {
		convertT_DS18B20();
		T = readT_DS18B20();
		LCD_ShowSignedNum(1,3,T,3);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(unsigned long)(T*10000)%10000,4);
		LCD_ShowSignedNum(2,4,TH,3);
		LCD_ShowSignedNum(2,12,TL,3);
		btnCode = independentBtnCheckByEventLoop();
		if (btnCode) {
			if (btnCode == 1) TH++;
			else if (btnCode == 2) TH--;
			else if (btnCode == 3) TL++;
			else if (btnCode == 4) TL--;
			reviseThreshold();
			write_AT24C02(0,TH);
			delay(5);
			write_AT24C02(1,TL);
			delay(5);
		}
		if (T > TH) {
			LCD_ShowString(1,13,"OV:H");
		} else if (T < TL) {
			LCD_ShowString(1,13,"OV:L");
		} else {
			LCD_ShowString(1,13,"    ");
		}
	}
}
