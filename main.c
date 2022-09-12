#include <REGX52.H>
#include <INTRINS.H>
#include <stdlib.h>
#include "matrixled.h"
#include "matrixbtn.h"
#include "segdisplay.h"
#include "T1.h"
#include "IR.h"

char p[8][8];
char e[63][2];
char x = 0;
char y = 0;
char l = 1;
char mode = 7;
char cnt = 0;
char speed = 0;

void T1Routine() interrupt 3
{
	static unsigned int Count4Update = 0, Count4Btn = 0;
	char i, j, randint;
	Count4Update++;
	Count4Btn++;
	TL1 = 0x66;
	TH1 = 0xFC;
	if (Count4Update >= 500 - speed * 100) {
		Count4Update = 0;
		cnt = 0;
		switch (mode) {
			case 2: y--;y+=(y<0)*8; break;
			case 10: y++;y-=(y>=8)*8; break;
			case 5: x--;x+=(x<0)*8; break;
			case 7: x++;x-=(x>=8)*8; break;
		}
		if (p[y][x] > 1) mode = 0;
		else if (p[y][x] < 0) {
			l++;
			for (i=0; i<8; i++) {
				for (j=0; j<8; j++) {
					if (p[i][j] > 0) p[i][j]++;
					else if (!(i == y && j == x)) {
						e[cnt][0] = i;
						e[cnt][1] = j;
						cnt++;
					}
				}
			}
			randint = rand()%cnt;
			p[e[randint][0]][e[randint][1]] = -1;
		}
		for (i=0; i<8; i++) {
			for (j=0; j<8; j++) {
				if (p[i][j] > 0) p[i][j]--;
			}
		}
		p[y][x] = l;
	}
	if (Count4Btn >= 20) matrixBtnEventLoop(0);
}

void main()
{
	unsigned char i, j, dat, btnCode;
	init_IR();
	T1Set();
	for (i=0;i<8;i++) { for (j=0;j<8;j++) { p[i][j] = 0; } }
	p[y][x] = l;
	p[5][5] = -1;
	while (mode) {
		for (i=0;i<8;i++) {
			dat = 0x00;
			for (j=0; j<8; j++) {
				if (p[j][i]) dat |= (0x80>>j);
			}
			matrixLED_ShowCol(i, dat);
		}
		btnCode = matrixBtnScanByEventLoop();
		if (getDF_IR())	{
			switch (IR_Cmd) {
				case IR_2: btnCode = 2;break;
				case IR_8: btnCode = 10;break;
				case IR_4: btnCode = 5;break;
				case IR_6: btnCode = 7;break;
				case IR_TOG: TR1 = !TR1;break;
				case IR_MIN: speed--;break;
				case IR_ADD: speed++;break;
			}
		}
		if ((btnCode == 2 || btnCode == 10 || btnCode == 5 || btnCode == 7)
			&& abs(mode-btnCode) != 2
			&& abs(mode-btnCode) != 8)
			mode = btnCode;
		else if (TR1) {
			switch (btnCode) {
				case 4: speed++;break;
				case 8: speed--;break;
				case 12: while(!P1_5);TR1 = 0;break;
			}
		} else if (matrixBtnScanByFlip(1) == 12) TR1 = 1;
		P2 = 0xFE<<(l-1)%8;
	}
	P2 = P0 = 0;
	nixieDynamicShow(l);
}
