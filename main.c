#include <REGX52.H>
#include "LCD1602.h"
#include "delay.h"
#include "IR.h"

unsigned char num;

void main() {
	LCD_Init();
	LCD_ShowString(1,1,"ADDR  CMD  NUM");
	LCD_ShowString(2,1,"00    00   000");
	init_IR();
	while (1) {
		if (getDF_IR() || getRF_IR())	{
			LCD_ShowHexNum(2,1,IR_Addr,2);
			LCD_ShowHexNum(2,7,IR_Cmd,2);
			if (IR_Cmd == IR_MIN) num--;
			else if (IR_Cmd == IR_ADD) num++;
			LCD_ShowNum(2,12,num,3);
		}
	}
}
