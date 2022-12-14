#include <REGX52.H>
#include "delay.h"

unsigned char numberMap4Nixie[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x40};
unsigned char nixieDisplayBuf[] = {10,10,10,10,10,10,10,10};

void nixieStaticShow(unsigned char loc, number)
{
	switch (loc)
	{
		case 1: P2_4=1;P2_3=1;P2_2=1; break;
		case 2: P2_4=1;P2_3=1;P2_2=0; break;
		case 3: P2_4=1;P2_3=0;P2_2=1; break;
		case 4: P2_4=1;P2_3=0;P2_2=0; break;
		case 5: P2_4=0;P2_3=1;P2_2=1; break;
		case 6: P2_4=0;P2_3=1;P2_2=0; break;
		case 7: P2_4=0;P2_3=0;P2_2=1; break;
		case 8: P2_4=0;P2_3=0;P2_2=0; break;
	}
	P0 = numberMap4Nixie[number];
	delay(1);
	P0 = 0x00;
}

void nixieLongShow(unsigned char loc, number)
{
	switch (loc)
	{
		case 1: P2_4=1;P2_3=1;P2_2=1; break;
		case 2: P2_4=1;P2_3=1;P2_2=0; break;
		case 3: P2_4=1;P2_3=0;P2_2=1; break;
		case 4: P2_4=1;P2_3=0;P2_2=0; break;
		case 5: P2_4=0;P2_3=1;P2_2=1; break;
		case 6: P2_4=0;P2_3=1;P2_2=0; break;
		case 7: P2_4=0;P2_3=0;P2_2=1; break;
		case 8: P2_4=0;P2_3=0;P2_2=0; break;
	}
	P0 = numberMap4Nixie[number];
}

void nixieDynamicShow(int number) {
	unsigned char loc = 8;
	int temp = number;
	while (1) {
		while (temp) {
			nixieStaticShow(loc--, temp % 10);
			temp /= 10;
		}
		temp = number;
		loc = 8;
	}
}

void nixieDisplayLoop() {
	static unsigned char i;
	P0 = 0x00;
	nixieLongShow(i, nixieDisplayBuf[i++]);
	i %= 8;
}

void nixieLoopShow(int number) {
	unsigned char loc = 8;
	while (number) {
		nixieDisplayBuf[loc---1] = number % 10;
		number /= 10;
	}
}
