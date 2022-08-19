#include <REGX52.H>
#include <INTRINS.H>
#include <stdlib.h>
#include "matrixled.h"
#include "matrixbtn.h"
#include "segdisplay.h"
#include "T0.h"

char p[8][8];
char e[63][2];
char x = 0;
char y = 0;
char l = 1;
char mode = 7;
char cnt = 0;

void T0Routine() interrupt 1
{
	static unsigned int T0Count = 0;
	char i, j, randint;
	T0Count++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count >= 500) {
		T0Count = 0;
		cnt = 0;
		switch (mode) {
			case 2: y--;y+=(y<0)*8; break;
			case 10: y++;y-=(y>=8)*8; break;
			case 5: x--;x+=(x<0)*8; break;
			case 7: x++;x-=(x>=8)*8; break;
		}
		if (p[y][x] > 0) mode = 0;
		else if (p[y][x] < 0) {
			l++;
			for (i=0; i<8; i++) {
				for (j=0; j<8; j++) {
					if (p[i][j]) p[i][j]++;
					else if (i != y && j != x) {
						e[cnt][0] = j;
						e[cnt][1] = i;
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
}

void main()
{
	unsigned char i, j, dat, btnCode;
	T0Set();
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
		btnCode = matrixBtnScanByFlip(0);
		if ((btnCode == 2 || btnCode == 10 || btnCode == 5 || btnCode == 7)
			&& abs(mode-btnCode) != 2
			&& abs(mode-btnCode) != 8)
			mode = btnCode;
	}
	nixieDynamicShow(l);
}
