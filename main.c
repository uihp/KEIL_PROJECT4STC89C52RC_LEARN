// MODULARIZE CODE
/*
#include <REGX52.H>
#include <stdlib.h>
#include "delay.h"
#include "indpdtbtn.h"
#include "matrixbtn.h"
#include "matrixled.h"
#include "segdisplay.h"
#include "motor.h"
#include "LCD1602.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "XPT2046.h"
#include "T0.h"
#include "T1.h"
#include "IR.h"
*/

/*
PROGRAM CODE START
[NAME] Tetris
#include <REGX52.H>
included <stdlib.h>
included "matrixled.h"
included "matrixbtn.h"
included "segdisplay.h"
included "indpdtbtn.h"
included "DS18B20.h"
included "XPT2046.h"
#include "T0.h"
*/
#include <REGX52.H>
#include <stdlib.h>
#include "matrixled.h"
#include "matrixbtn.h"
#include "segdisplay.h"
#include "indpdtbtn.h"
#include "DS18B20.h"
#include "XPT2046.h"
#include "T0.h"

#define HEIGHT 8
#define BLOCK_HEIGHT 4
#define BLOCK_KINDNUM 5

int score;
char data screen[HEIGHT];
const unsigned char code blocks[BLOCK_KINDNUM][4][BLOCK_HEIGHT] = {
	0, 0x38, 0x20, 0,
	0x10, 0x10, 0x18, 0,
	0, 0x08, 0x38, 0,
	0x30, 0x10, 0x10, 0,

	0x20, 0x30, 0x10, 0,
	0x18, 0x30, 0, 0,
	0x20, 0x30, 0x10, 0,
	0x18, 0x30, 0, 0,

	0x10, 0x30, 0x20, 0,
	0x30, 0x18, 0, 0,
	0x10, 0x30, 0x20, 0,
	0x30, 0x18, 0, 0,

	0, 0x38, 0x08, 0,
	0x10, 0x10, 0x30, 0,
	0x20, 0x38, 0, 0,
	0x18, 0x10, 0x10, 0,

	0x10, 0x38, 0, 0,
	0x20, 0x30, 0x20, 0,
	0x38, 0x10, 0, 0,
	0x10, 0x30, 0x10, 0};
unsigned char block[BLOCK_HEIGHT];
char dropdis, movedis, blktype, blkdire;

bit newb()
{
	char i;
	blktype = rand() % BLOCK_KINDNUM; // Random
	dropdis = 0;
	movedis = 0;
	blkdire = 0;
	for (i = 0; i < BLOCK_HEIGHT; i++)
	{
		block[i] = blocks[blktype][0][i];
		if (screen[i] & block[i]) // Occupied
			return 0;
	}
	return 1;
}

bit drop()
{
	char i;
	for (i = 0; i < BLOCK_HEIGHT; i++)
		if (dropdis + i + 1 >= HEIGHT && block[i] || screen[dropdis + i + 1] & block[i]) // Bottomed || Touched
			return 0;
	dropdis++;
	return 1;
}

void fix()
{
	char i;
	for (i = 0; i < BLOCK_HEIGHT; i++)
		screen[dropdis + i] |= block[i];
}

void del()
{
	char i, j;
	for (i = 0; i < HEIGHT; i++)
		if (screen[i] == -1)
		{
			for (j = i; j >= 0; j--)
				screen[j] = j ? screen[j - 1] : 0;
			score++;
		}
}

void move(bit dire)
{
	char i;
	for (i = 0; i < BLOCK_HEIGHT; ++i)
		if (
			block[i] & 0x80 && !dire ||
			block[i] & 0x01 && dire ||
			screen[dropdis + i] & (dire ? block[i] >> 1 : block[i] << 1)) // Blocked
			return;
	for (i = 0; i < BLOCK_HEIGHT; ++i)
		if (dire)
			block[i] >>= 1;
		else
			block[i] <<= 1;
	movedis += dire ? 1 : -1;
}

#define OBKL blocks[blktype][blkdire][i]
#define MBKL (movedis > 0 ? OBKL >> movedis : OBKL << -movedis)
void change()
{
	char i;
	blkdire++;
	blkdire %= 4;
	for (i = 0; i < BLOCK_HEIGHT; i++)
	{
		if (dropdis + i >= HEIGHT && OBKL) // Bottomed
			return;
		movedis--;
		if (MBKL & 0x01) // Overflow
		{
			movedis++;
			blkdire = blkdire ? blkdire - 1 : 3;
			return;
		}
		movedis++;
		if (screen[dropdis + i] & MBKL) // Blocked
			return;
	}
	for (i = 0; i < BLOCK_HEIGHT; i++)
		block[i] = MBKL;
}

#define OVER 0
#define NEWB 1
#define DROP 2

bit playing = 1;
bit falling = 0;
bit quickly = 0;

void TRountine4Tetris()
{
	static unsigned int Count4Update = 0, Count4Btn = 0;
	Count4Update++;
	Count4Btn++;
	if (Count4Update >= 1000 - quickly * 500)
	{
		Count4Update = 0;
		if (falling)
		{
			falling = drop();
			if (!falling)
				fix();
		}
		else
		{
			del();
			falling = newb();
			playing = falling;
		}
	}
	if (Count4Btn >= 20)
		matrixBtnEventLoop(0);
}

void main4Tetris()
{
	unsigned char i, j, blockLine;
	unsigned char flush[HEIGHT], temp[HEIGHT];
	convertT_DS18B20();
	srand(read_XPT2046(XPT2046_XP) * read_XPT2046(XPT2046_YP) * read_XPT2046(XPT2046_VBAT) * (unsigned long)(readT_DS18B20() * 10000));
	while (playing)
	{
		for (i = 0; i < HEIGHT; i++)
		{
			blockLine = (dropdis + BLOCK_HEIGHT > i && i >= dropdis ? block[i - dropdis] : 0);
			flush[i] = screen[i] | blockLine;
		}
		for (i = 0; i < HEIGHT; i++)
			temp[i] = flush[i];
		for (i = 0; i < HEIGHT; ++i)
		{
			for (j = 0; j < HEIGHT; ++j)
			{
				if (temp[i] & (0x80 >> j))
					flush[j] |= (0x80 >> i);
				else
					flush[j] &= ~(0x80 >> i);
			}
		}
		matrixLED_LongShow(flush);
		if (TR1)
		{
			switch (matrixBtnScanByEventLoop())
			{
			case 1:
				move(0);
				break;
			case 2:
				move(1);
				break;
			case 3:
				change();
				break;
			case 4:
				while (drop())
				{
				};
				break;
			}
		}
		switch (independentBtnCheck())
		{
		case 1:
			TR1 = !TR1;
			break;
		case 2:
			quickly = !quickly;
			break;
		}
	}
	nixieDynamicShow(score);
}
/*
PROGRAM CODE END
OF Tetris
*/

/*
PROGRAM CODE START
[NAME] Snake
#include <REGX52.H>
included <INTRINS.H>
included <stdlib.h>
included "matrixled.h"
included "matrixbtn.h"
included "segdisplay.h"
included "T1.h"
included "IR.h"
*/
#include <INTRINS.H>
#include "T1.h"
#include "IR.h"

char idata p[8][8];
char pdata e[63][2];
char x = 0;
char y = 0;
char l = 1;
char mode = 7;
char cnt = 0;
char speed = 0;

void TRoutine4Snake()
{
	static unsigned int Count4Update = 0, Count4Btn = 0;
	char i, j, randint;
	Count4Update++;
	Count4Btn++;
	if (Count4Update >= 500 - speed * 100)
	{
		Count4Update = 0;
		cnt = 0;
		switch (mode)
		{
		case 2:
			y--;
			y += (y < 0) * 8;
			break;
		case 10:
			y++;
			y -= (y >= 8) * 8;
			break;
		case 5:
			x--;
			x += (x < 0) * 8;
			break;
		case 7:
			x++;
			x -= (x >= 8) * 8;
			break;
		}
		if (p[y][x] > 1)
			mode = 0;
		else if (p[y][x] < 0)
		{
			l++;
			for (i = 0; i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (p[i][j] > 0)
						p[i][j]++;
					else if (!(i == y && j == x))
					{
						e[cnt][0] = i;
						e[cnt][1] = j;
						cnt++;
					}
				}
			}
			randint = rand() % cnt;
			p[e[randint][0]][e[randint][1]] = -1;
		}
		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (p[i][j] > 0)
					p[i][j]--;
			}
		}
		p[y][x] = l;
	}
	if (Count4Btn >= 20)
		matrixBtnEventLoop(0);
}

void main4Snake()
{
	unsigned char i, j, dat, btnCode;
	init_IR();
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			p[i][j] = 0;
		}
	}
	p[y][x] = l;
	p[5][5] = -1;
	while (mode)
	{
		for (i = 0; i < 8; i++)
		{
			dat = 0x00;
			for (j = 0; j < 8; j++)
			{
				if (p[j][i])
					dat |= (0x80 >> j);
			}
			matrixLED_ShowCol(i, dat);
		}
		btnCode = matrixBtnScanByEventLoop();
		if (getDF_IR())
		{
			switch (IR_Cmd)
			{
			case IR_2:
				btnCode = 2;
				break;
			case IR_8:
				btnCode = 10;
				break;
			case IR_4:
				btnCode = 5;
				break;
			case IR_6:
				btnCode = 7;
				break;
			case IR_TOG:
				TR1 = !TR1;
				break;
			case IR_MIN:
				speed--;
				break;
			case IR_ADD:
				speed++;
				break;
			}
		}
		if ((btnCode == 2 || btnCode == 10 || btnCode == 5 || btnCode == 7) && abs(mode - btnCode) != 2 && abs(mode - btnCode) != 8)
			mode = btnCode;
		else if (TR1)
		{
			switch (btnCode)
			{
			case 4:
				speed++;
				break;
			case 8:
				speed--;
				break;
			case 12:
				while (!P1_5)
					;
				TR1 = 0;
				break;
			}
		}
		else if (matrixBtnScanByFlip(1) == 12)
			TR1 = 1;
		P2 = 0xFE << (l - 1) % 8;
	}
	P2 = P0 = 0;
	nixieDynamicShow(l);
}
/*
PROGRAM CODE END
OF Snake
*/

/*
PROGRAM CODE START
[NAME] Stopwatch
#include <REGX52.H>
included "AT24C02.h"
included "segdisplay.h"
included "indpdtbtn.h"
included "delay.h"
*
#include "AT24C02.h"
#include "delay.h"

unsigned char btnCode, min, sec, cse;
bit flag;

void stopwatchLoop()
{
	if (flag)
	{
		cse++;
		if (cse >= 100)
		{
			cse = 0;
			sec++;
			if (sec >= 60)
			{
				sec = 0;
				min++;
				min %= 60;
			}
		}
	}
}

void TRoutine4Stopwatch()
{
	static unsigned int T0Count4IndpdtBtn = 0, T0Count4Stopwatch = 0;
	T0Count4IndpdtBtn++;
	T0Count4Stopwatch++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count4IndpdtBtn >= 20)
	{
		T0Count4IndpdtBtn = 0;
		btnEventLoop();
	}
	if (T0Count4Stopwatch >= 10)
	{
		T0Count4Stopwatch = 0;
		stopwatchLoop();
	}
	nixieDisplayLoop();
}

void main4Stopwatch()
{
	T0Set();
	while (1)
	{
		btnCode = independentBtnCheckByEventLoop();
		if (btnCode == 1)
			flag = !flag;
		else if (btnCode == 2)
		{
			min = 0;
			sec = 0;
			cse = 0;
		}
		else if (btnCode == 3)
		{
			write_AT24C02(0, min);
			delay(5);
			write_AT24C02(1, sec);
			delay(5);
			write_AT24C02(2, cse);
			delay(5);
		}
		else if (btnCode == 4)
		{
			min = read_AT24C02(0);
			sec = read_AT24C02(1);
			cse = read_AT24C02(2);
		}
		nixieDisplayBuf[0] = min / 10;
		nixieDisplayBuf[1] = min % 10;
		nixieDisplayBuf[2] = 11;
		nixieDisplayBuf[3] = sec / 10;
		nixieDisplayBuf[4] = sec % 10;
		nixieDisplayBuf[5] = 11;
		nixieDisplayBuf[6] = cse / 10;
		nixieDisplayBuf[7] = cse % 10;
	}
}
/*
PROGRAM CODE END
OF Stopwatch
*/

/*
PROGRAM CODE START
[NAME] BuzzerMusic
#include <REGX52.H>
included "delay.h"
#include "T0.h"
*

#define crotchet 200
#define P -1
#define L1 0
#define L1S 1
#define L2 2
#define L2S 3
#define L3 4
#define L4 5
#define L4S 6
#define L5 7
#define L5S 8
#define L6 9
#define L6S 10
#define L7 11
#define M1 12
#define M1S 13
#define M2 14
#define M2S 15
#define M3 16
#define M4 17
#define M4S 18
#define M5 19
#define M5S 20
#define M6 21
#define M6S 22
#define M7 23
#define H1 24
#define H1S 25
#define H2 26
#define H2S 27
#define H3 28
#define H4 29
#define H4S 30
#define H5 31
#define H5S 32
#define H6 33
#define H6S 34
#define H7 35

sbit buzzer = P2 ^ 5;

unsigned int freqArray[] = {
	0,
	63775, 63874, 63967, 64055, 64138, 64217, 64291, 64360, 64426, 64489, 64548, 64603,
	64655, 64705, 64751, 64795, 64837, 64876, 64913, 64948, 64981, 65012, 65042, 65069,
	65096, 65120, 65144, 65166, 65187, 65206, 65225, 65242, 65259, 65274, 65289, 65303};
/
// "Twinkle Twinkle Little Star"
char musicMap[] = {
	M1,4, M1,4, M5,4, M5,4,
	M6,4, M6,4, M5,8,
	M4,4, M4,4, M3,4, M3,4,
	M2,4, M2,4, M1,8,
	P, 0xFF
};
*
// "Laputa: Castle In The Sky"
char code musicMap[] = {
	P, 4, P, 4, P, 4, M6, 2, M7, 2,
	H1, 6, M7, 2, H1, 4, H3, 4,
	M7, 12, M3, 2, M3, 2,

	M6, 6, M5, 2, M6, 4, H1, 4,
	M5, 12, M3, 4,
	M4, 6, M3, 2, M4, 4, H1, 4,

	M3, 8, P, 2, H1, 2, H1, 2, H1, 2,
	M7, 6, M4S, 2, M4S, 4, M7, 4,
	M7, 8, P, 4, M6, 2, M7, 2,

	H1, 6, M7, 2, H1, 4, H3, 4,
	M7, 12, M3, 2, M3, 2,
	M6, 6, M5, 2, M6, 4, H1, 4,

	M5, 12, M2, 2, M3, 2,
	M4, 4, H1, 2, M7, 4, H1, 6,
	H2, 2, H2, 2, H3, 2, H1, 10,

	H1, 2, M7, 2, M6, 2, M6, 2, M7, 4, M5S, 4,
	M6, 12, H1, 2, H2, 2,
	H3, 6, H2, 2, H3, 4, H5, 4,

	H2, 12, M5, 2, M5, 2,
	H1, 6, M7, 2, H1, 4, H3, 4,
	H3, 16,

	M6, 2, M7, 2, H1, 4, M7, 4, H2, 2, H2, 2,
	H1, 6, M5, 10,
	H4, 4, H3, 4, H3, 4, H1, 4,

	H3, 12, H3, 4,
	H6, 8, H5, 4, H5, 4,
	H3, 2, H2, 2, H1, 8, P, 2, H1, 2,

	H2, 4, H1, 2, H2, 2, H2, 4, H5, 4,
	H3, 12, H3, 4,
	H6, 8, H5, 8,

	H3, 2, H2, 2, H1, 8, P, 2, H1, 2,
	H2, 4, H1, 2, H2, 6, M7, 4,
	M6, 12, P, 4,

	P, 0xFF};
unsigned char freqSelect, musicProg;

void TRoutine4BuzzerMusic()
{
	if (freqArray[freqSelect + 1])
	{
		TL0 = freqArray[freqSelect + 1] % 256;
		TH0 = freqArray[freqSelect + 1] / 256;
		buzzer = !buzzer;
	}
}

void main4BuzzerMusic()
{
	T0Set();
	while (1)
	{
		freqSelect = musicMap[musicProg];
		musicProg++;
		delay(crotchet / 4 * musicMap[musicProg]);
		musicProg++;
		TR0 = 0;
		delay(5);
		TR0 = 1;
		while (musicMap[musicProg] == 0xFF)
			;
	}
}
/*
PROGRAM CODE END
OF BuzzerMusic
*/

/*
PROGRAM CODE START
[NAME] TemperatureAlarm
#include <REGX52.H>
included "LCD1602.h"
included "DS18B20.h"
included "AT24C02.h"
included "indpdtbtn.h"
included "delay.h"
#include "T0.h"
*
#include "LCD1602.h"
#include "DS18B20.h"

float T;
char TH, TL;
unsigned char btnCode;

void TRoutine4TemperatureAlarm()
{
	static unsigned int T0Count = 0;
	T0Count++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count >= 20)
	{
		T0Count = 0;
		btnEventLoop();
	}
}

void reviseThreshold()
{
	if (TH > 125)
		TH = 125;
	else if (TH <= TL)
		TH++;
	else if (TL >= TH)
		TL--;
	else if (TL < -55)
		TL = -55;
	if (TH <= TL)
	{
		TH = 20;
		TL = 15;
	}
}

void main4TemperatureAlarm()
{
	convertT_DS18B20();
	T0Set();
	LCD_Init();
	LCD_ShowString(1, 1, "T:");
	LCD_ShowString(2, 1, "TH:");
	LCD_ShowString(2, 9, "TL:");
	TH = read_AT24C02(0);
	TL = read_AT24C02(1);
	reviseThreshold();
	delay(500);
	while (1)
	{
		convertT_DS18B20();
		T = readT_DS18B20();
		LCD_ShowSignedNum(1, 3, T, 3);
		LCD_ShowChar(1, 7, '.');
		LCD_ShowNum(1, 8, (unsigned long)(T * 10000) % 10000, 4);
		LCD_ShowSignedNum(2, 4, TH, 3);
		LCD_ShowSignedNum(2, 12, TL, 3);
		btnCode = independentBtnCheckByEventLoop();
		if (btnCode)
		{
			if (btnCode == 1)
				TH++;
			else if (btnCode == 2)
				TH--;
			else if (btnCode == 3)
				TL++;
			else if (btnCode == 4)
				TL--;
			reviseThreshold();
			write_AT24C02(0, TH);
			delay(5);
			write_AT24C02(1, TL);
			delay(5);
		}
		if (T > TH)
		{
			LCD_ShowString(1, 13, "OV:H");
		}
		else if (T < TL)
		{
			LCD_ShowString(1, 13, "OV:L");
		}
		else
		{
			LCD_ShowString(1, 13, "    ");
		}
	}
}
/*
PROGRAM CODE END
OF TemperatureAlarm
*/

/*
PROGRAM CODE START
[NAME] ModifiableClock
#include <REGX52.H>
included "LCD1602.h"
included "DS1302.h"
included "indpdtbtn.h"
#include "T0.h"
*
#include "DS1302.h"

unsigned char btnCode, selected, flashFlag; // unsigned char btnCode, mode, selected, flashFlag; ('mode': redefinition)
char *days[] = {"MON", "TUE", "THR", "FOU", "FRI", "SAT", "SUN"};
char *modifiable[] = {"SEC", "MIN", "HOU", "DAT", "MON", "DAY", "YEA"};

void show_DS1302Time()
{
	unsigned char i;
	get_DS1302();
	for (i = 0; i < 7; i++)
	{
		if (i == 5)
		{
			LCD_ShowString(1, 10, days[DS1302Time[i++] - 1]);
			LCD_ShowNum(1, 1, DS1302Time[i], 2);
		}
		else
			LCD_ShowNum((5 - i) / 3 + 1, 7 - i % 3 * 3, DS1302Time[i], 2);
	}
}

void revise_DS1302Time()
{
	if (DS1302Time[0] > 59)
		DS1302Time[0] = 0;
	else if (DS1302Time[0] < 0)
		DS1302Time[0] = 59;
	if (DS1302Time[1] > 59)
		DS1302Time[1] = 0;
	else if (DS1302Time[1] < 0)
		DS1302Time[1] = 59;
	if (DS1302Time[2] > 23)
		DS1302Time[2] = 0;
	else if (DS1302Time[2] < 0)
		DS1302Time[2] = 23;
	if (DS1302Time[4] > 12)
		DS1302Time[4] = 1;
	else if (DS1302Time[4] < 1)
		DS1302Time[4] = 12;
	if (DS1302Time[6] > 99)
		DS1302Time[6] = 0;
	else if (DS1302Time[6] < 0)
		DS1302Time[6] = 99;
	if (DS1302Time[4] == 1 || DS1302Time[4] == 3 || DS1302Time[4] == 5 || DS1302Time[4] == 7 || DS1302Time[4] == 8 || DS1302Time[4] == 10 || DS1302Time[4] == 12)
	{
		if (DS1302Time[3] > 31)
			DS1302Time[3] = 1;
		else if (DS1302Time[3] < 1)
			DS1302Time[3] = 31;
	}
	else if (DS1302Time[4] == 4 || DS1302Time[4] == 6 || DS1302Time[4] == 9 || DS1302Time[4] == 11)
	{
		if (DS1302Time[3] > 30)
			DS1302Time[3] = 1;
		else if (DS1302Time[3] < 1)
			DS1302Time[3] = 30;
	}
	else if (DS1302Time[4] == 2 && !DS1302Time[6] % 4)
	{
		if (DS1302Time[3] > 29)
			DS1302Time[3] = 1;
		else if (DS1302Time[3] < 1)
			DS1302Time[3] = 29;
	}
	else if (DS1302Time[3] > 28)
		DS1302Time[3] = 1;
	else if (DS1302Time[3] < 1)
		DS1302Time[3] = 28;
}

void modify_DS1302Time()
{
	if (btnCode == 2)
	{
		selected++;
		selected %= 7;
	}
	else if (btnCode == 3)
		DS1302Time[selected]++;
	else if (btnCode == 4)
		DS1302Time[selected]--;
	revise_DS1302Time();
	set_DS1302();
	if (flashFlag)
	{
		if (selected == 6)
			LCD_ShowString(1, 1, "  ");
		else if (selected == 5)
			LCD_ShowString(1, 10, "   ");
		else
			LCD_ShowString((5 - selected) / 3 + 1, 7 - selected % 3 * 3, "  ");
	}
	else
		show_DS1302Time();
	LCD_ShowString(2, 10, modifiable[selected]);
}

void TRoutine4ModifiableClock()
{
	static unsigned int T0Count = 0;
	T0Count++;
	TL0 = 0x66;
	TH0 = 0xFC;
	if (T0Count >= 500)
	{
		T0Count = 0;
		flashFlag = !flashFlag;
	}
}

void main4ModifiableClock()
{
	LCD_Init();
	init_DS1302();
	T0Set();
	set_DS1302();
	LCD_ShowString(1, 1, "  -  -  ");
	LCD_ShowString(2, 1, "  :  :  ");
	LCD_ShowString(2, 10, modifiable[selected]);
	while (1)
	{
		btnCode = independentBtnCheck();
		if (btnCode == 1)
			mode = !mode;
		if (mode)
			modify_DS1302Time();
		else
			show_DS1302Time();
	}
}
/*
PROGRAM CODE END
OF ModifiableClock
*/

/*
PROGRAM CODE START
[NAME] MotorIR
#include <REGX52.H>
included "LCD1602.h"
included "delay.h"
included "IR.h"
included "motor.h"
*
#include "motor.h"

void main4MotorIR()
{
	LCD_Init();
	LCD_ShowString(1, 1, "ADDR  CMD  NUM");
	LCD_ShowString(2, 1, "00    00   000");
	init_IR();
	init_Motor();
	while (1)
	{
		if (getDF_IR() || getRF_IR())
		{
			LCD_ShowHexNum(2, 1, IR_Addr, 2);
			LCD_ShowHexNum(2, 7, IR_Cmd, 2);
			if (IR_Cmd == IR_MIN)
				motorSpeed--;
			else if (IR_Cmd == IR_ADD)
				motorSpeed++;
			LCD_ShowNum(2, 12, motorSpeed, 3);
		}
	}
}
/*
PROGRAM CODE END
OF MotorIR
*/

/*
PROGRAM CODE START
[NAME] Resistors
#include <REGX52.H>
included "LCD1602.h"
included "delay.h"
included "XPT2046.h"
*
#include "XPT2046.h"

unsigned int adv;

void main4Resistors()
{
	LCD_Init();
	LCD_ShowString(1, 1, "ADJ  NTC  GR");
	while (1)
	{
		adv = read_XPT2046(XPT2046_XP);
		LCD_ShowNum(2, 1, adv, 3);
		adv = read_XPT2046(XPT2046_YP);
		LCD_ShowNum(2, 6, adv, 3);
		adv = read_XPT2046(XPT2046_VBAT);
		LCD_ShowNum(2, 11, adv, 3);
		delay(100);
	}
}
/*
PROGRAM CODE END
OF Resistors
*/

/* MAIN ENTER CODE BELOW */

unsigned char prog;

void T1Routine() interrupt 3
{
	TL1 = 0x66;
	TH1 = 0xFC;
	switch (prog)
	{
	case 1:
		TRoutine4Snake();
		break;
	case 2:
		TRountine4Tetris();
		break;
	default:
		break;
	}
}

void main()
{
	T1Set();
	while (1)
	{
		prog = independentBtnCheck();
		switch (prog)
		{
		case 1:
			main4Snake();
		case 2:
			main4Tetris();
		default:
			break;
		}
	}
}
