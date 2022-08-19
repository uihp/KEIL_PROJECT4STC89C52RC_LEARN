#include <REGX52.H>
#include "T0.h"
#include "LCD1602.h"
#include "delay.h"

unsigned int second, minute, hour;

void T0Routine() interrupt 1
{
	static unsigned int T0Count = 0;
	T0Count++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count >= 1000) {
		T0Count = 0;
		second++;
		if (second >= 60) {
			second = 0;
			minute++;
			if (minute >=60) {
				minute = 0;
				hour++;
				if (hour >= 60) {
					hour = 0;
				}
			}
		}
	}
}

void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"Clock:");
	T0Set();
	while (1) {
		LCD_ShowNum(2,1,hour,2);
		LCD_ShowChar(2,3,':');
		LCD_ShowNum(2,4,minute,2);
		LCD_ShowChar(2,6,':');
		LCD_ShowNum(2,7,second,2);
	}
}
