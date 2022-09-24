#include <REGX52.H>
#include <stdlib.h>
#include "matrixled.h"
#include "matrixbtn.h"
#include "segdisplay.h"
#include "T0.h"

#define HEIGHT 8
#define BLOCK_HEIGHT 4
#define BLOCK_KINDNUM 4

int score;
char screen[HEIGHT];
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
	0x18, 0x10, 0x10, 0};
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
		if (screen[i] & block[i])
			return 0; // Occupied
	}
	return 1;
}

bit drop()
{
	char i;
	for (i = 0; i < BLOCK_HEIGHT; i++)
		if (dropdis + i + 1 >= HEIGHT && block[i] || screen[dropdis + i + 1] & block[i])
			return 0; // Bottomed || Touched
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
			screen[dropdis + i] & (dire ? block[i] >> 1 : block[i] << 1))
			return; // Blocked
	for (i = 0; i < BLOCK_HEIGHT; ++i)
		if (dire)
			block[i] >>= 1;
		else
			block[i] <<= 1;
	movedis += dire ? 1 : -1;
}

void change()
{
	char i;
	blkdire++;
	blkdire %= 4;
#define OBKL blocks[blktype][blkdire][i]
#define MBKL (movedis > 0 ? OBKL >> movedis : OBKL << -movedis)
	for (i = 0; i < BLOCK_HEIGHT; i++)
	{
		if (dropdis + i >= HEIGHT && OBKL)
			return; // Bottomed
		movedis--;
		if (MBKL & 0x01)
		{
			movedis++;
			blkdire = blkdire ? blkdire - 1 : 3;
			return;
		} // Overflow
		movedis++;
		if (screen[dropdis + i] & MBKL)
			return; // Blocked
	}
	for (i = 0; i < BLOCK_HEIGHT; i++)
		block[i] = MBKL;
}

#define OVER 0
#define NEWB 1
#define DROP 2

bit playing = 1;
bit falling = 0;

void TRountine4Tetris()
{
	static unsigned int Count4Update = 0, Count4Btn = 0;
	Count4Update++;
	Count4Btn++;
	if (Count4Update >= 1000)
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

void T0Routine() interrupt 1
{
	TL0 = 0x66;
	TH0 = 0xFC;
	TRountine4Tetris();
}

void main()
{
	unsigned char i, j, blockLine;
	unsigned char flush[HEIGHT], temp[HEIGHT];
	T0Set();
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
		for (i = 0; i < HEIGHT; i++)
			matrixLED_ShowCol(i, flush[i]);
		if (TR0)
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
					;
				break;
			case 5:
				while (!P1_6)
					;
				TR0 = 0;
				break;
			}
		}
		else if (matrixBtnScanByFlip(1) == 5)
			TR0 = 1;
	}
	nixieDynamicShow(score);
}
