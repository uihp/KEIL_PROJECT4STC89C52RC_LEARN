#include <REGX52.H>
#include "T0.h"
#include "delay.h"

#define crotchet 200
#define P	-1
#define L1	0
#define L1S	1
#define L2	2
#define L2S	3
#define L3	4
#define L4	5
#define L4S	6
#define L5	7
#define L5S	8
#define L6	9
#define L6S	10
#define L7	11
#define M1	12
#define M1S	13
#define M2	14
#define M2S	15
#define M3	16
#define M4	17
#define M4S	18
#define M5	19
#define M5S	20
#define M6	21
#define M6S	22
#define M7	23
#define H1	24
#define H1S	25
#define H2	26
#define H2S	27
#define H3	28
#define H4	29
#define H4S	30
#define H5	31
#define H5S	32
#define H6	33
#define H6S	34
#define H7	35

sbit buzzer = P2^5;

unsigned int freqArray[] = {
	0,
	63775,63874,63967,64055,64138,64217,64291,64360,64426,64489,64548,64603,
	64655,64705,64751,64795,64837,64876,64913,64948,64981,65012,65042,65069,
	65096,65120,65144,65166,65187,65206,65225,65242,65259,65274,65289,65303
};
/*
// "Twinkle Twinkle Little Star"
char musicMap[] = {
	M1,4, M1,4, M5,4, M5,4,
	M6,4, M6,4, M5,8,
	M4,4, M4,4, M3,4, M3,4,
	M2,4, M2,4, M1,8,
	P, 0xFF
};
*/
// "Laputa: Castle In The Sky"
char code musicMap[] = {
	P,4, P,4, P,4, M6,2, M7,2,
	H1,6, M7,2, H1,4, H3,4,
	M7,12, M3,2, M3,2,

	M6,6, M5,2, M6,4,H1,4,
	M5,12, M3,4,
	M4,6, M3,2, M4,4, H1,4,

	M3,8, P,2, H1,2, H1,2, H1,2,
	M7,6, M4S,2, M4S,4, M7,4,
	M7,8, P,4, M6,2, M7,2,

	H1,6, M7,2, H1,4, H3,4,
	M7,12, M3,2, M3,2,
	M6,6, M5,2, M6,4, H1,4,

	M5,12, M2,2, M3,2,
	M4,4, H1,2, M7,4, H1,6,
	H2,2, H2,2, H3,2, H1,10,

	H1,2, M7,2, M6,2, M6,2, M7,4, M5S,4,
	M6,12, H1,2, H2,2,
	H3,6, H2,2, H3,4, H5,4,

	H2,12, M5,2, M5,2,
	H1,6, M7,2, H1,4, H3,4,
	H3,	16,

	M6,2, M7,2, H1,4, M7,4, H2,2, H2,2,
	H1,6, M5,10,
	H4,4, H3,4, H3,4, H1,4,

	H3,12, H3,4,
	H6,8, H5,4, H5,4,
	H3,2, H2,2, H1,8, P,2, H1,2,

	H2,4, H1,2, H2,2, H2,4, H5,4,
	H3,12, H3,4,
	H6,8, H5,8,

	H3,2, H2,2, H1,8, P,2, H1,2,
	H2,4, H1,2, H2,6, M7,4,
	M6,12, P,4,

	P, 0xFF
};
unsigned char freqSelect, musicProg;

void T0Routine() interrupt 1 {
	if (freqArray[freqSelect+1]) {
		TL0 = freqArray[freqSelect+1]%256;
		TH0 = freqArray[freqSelect+1]/256;
		buzzer = !buzzer;
	}
}

void main() {
	T0Set();
	while (1) {
		freqSelect = musicMap[musicProg];
		musicProg++;
		delay(crotchet/4*musicMap[musicProg]);
		musicProg++;
		TR0 = 0;
		delay(5);
		TR0 = 1;
		while (musicMap[musicProg] == 0xFF);
	}
}
