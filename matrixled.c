#include <REGX52.H>
#include "74HC595.h"
#include "delay.h"

void matrixLED_ShowCol(unsigned char col, dat)
{
	write_74HC595(dat);
	P0 = ~(0x80 >> col);
	delay(1);
	P0 = 0xFF;
}

void matrixLED_LongShow(unsigned char *dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
		matrixLED_ShowCol(i, *(dat + i));
}

void matrixLED_Show(unsigned char *dat)
{
	unsigned char i;
	while (1)
		for (i = 0; i < 8; i++)
			matrixLED_ShowCol(i, *(dat + i));
}
