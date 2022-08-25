#include <REGX52.H>
#include "LCD1602.h"
#include "delay.h"
#include "XPT2046.h"

unsigned int adv;

void main() {
	LCD_Init();
	LCD_ShowString(1,1,"ADJ  NTC  GR");
	while (1) {
		adv = read_XPT2046(XPT2046_XP);
		LCD_ShowNum(2,1,adv,3);
		adv = read_XPT2046(XPT2046_YP);
		LCD_ShowNum(2,6,adv,3);
		adv = read_XPT2046(XPT2046_VBAT);
		LCD_ShowNum(2,11,adv,3);
		delay(100);
	}
}
