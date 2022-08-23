#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "indpdtbtn.h"
#include "T0.h"

unsigned char btnCode, mode, selected, flashFlag;
char* days[] = {"MON","TUE","THR","FOU","FRI","SAT","SUN"};
char* modifiable[] = {"SEC","MIN","HOU","DAT","MON","DAY","YEA"};

void show_DS1302Time() {
	unsigned char i;
	get_DS1302();
	for (i=0;i<7;i++) {
		if (i == 5) {
			LCD_ShowString(1,10,days[DS1302Time[i++]-1]);
			LCD_ShowNum(1,1,DS1302Time[i],2);
		} else LCD_ShowNum((5-i)/3+1,7-i%3*3,DS1302Time[i],2);
	}
}

void revise_DS1302Time() {
	if (DS1302Time[0]>59) DS1302Time[0] = 0;
	else if (DS1302Time[0]<0) DS1302Time[0] = 59;
	if (DS1302Time[1]>59) DS1302Time[1] = 0;
	else if (DS1302Time[1]<0) DS1302Time[1] = 59;
	if (DS1302Time[2]>23) DS1302Time[2] = 0;
	else if (DS1302Time[2]<0) DS1302Time[2] = 23;
	if (DS1302Time[4]>12) DS1302Time[4] = 1;
	else if (DS1302Time[4]<1) DS1302Time[4] = 12;
	if (DS1302Time[6]>99) DS1302Time[6] = 0;
	else if (DS1302Time[6]<0) DS1302Time[6] = 99;
	if(DS1302Time[4]==1 || DS1302Time[4]==3 || DS1302Time[4]==5 || DS1302Time[4]==7
		|| DS1302Time[4]==8 || DS1302Time[4]==10 || DS1302Time[4]==12) {
		if (DS1302Time[3]>31) DS1302Time[3]=1;
		else if (DS1302Time[3]<1) DS1302Time[3]=31;
	} else if (DS1302Time[4]==4 || DS1302Time[4]==6 || DS1302Time[4]==9 || DS1302Time[4]==11) {
		if (DS1302Time[3]>30) DS1302Time[3]=1;
		else if (DS1302Time[3]<1) DS1302Time[3]=30;
	} else if (DS1302Time[4]==2 && !DS1302Time[6]%4) {
		if (DS1302Time[3]>29) DS1302Time[3]=1;
		else if (DS1302Time[3]<1) DS1302Time[3]=29;
	} else if (DS1302Time[3]>28) DS1302Time[3]=1;
	else if (DS1302Time[3]<1) DS1302Time[3]=28;
}

void modify_DS1302Time() {
	if (btnCode == 2) {
		selected++;
		selected %= 7;
	} else if (btnCode == 3) DS1302Time[selected]++;
	else if (btnCode == 4) DS1302Time[selected]--;
	revise_DS1302Time();
	set_DS1302();
	if (flashFlag) {
		if (selected == 6) LCD_ShowString(1,1,"  ");
		else if (selected == 5) LCD_ShowString(1,10,"   ");
		else LCD_ShowString((5-selected)/3+1,7-selected%3*3,"  ");
	} else show_DS1302Time();
	LCD_ShowString(2,10,modifiable[selected]);
}

void T0Routine() interrupt 1 {
	static unsigned int T0Count = 0;
	T0Count++;
 	TL0 = 0x66;
 	TH0 = 0xFC;
	if (T0Count >= 500) {
 		T0Count = 0;
		flashFlag = !flashFlag;
	}
}

void main() {
	LCD_Init();
	init_DS1302();
	T0Set();
	set_DS1302();
	LCD_ShowString(1,1,"  -  -  ");
	LCD_ShowString(2,1,"  :  :  ");
	LCD_ShowString(2,10,modifiable[selected]);
	while (1) {
		btnCode = independentBtnCheck();
		if (btnCode == 1) mode = !mode;
		if (mode) modify_DS1302Time();
		else show_DS1302Time();
	}
}
