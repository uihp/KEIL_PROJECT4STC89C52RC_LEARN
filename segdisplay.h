#ifndef __SEGDISPLAY_H__
#define __SEGDISPLAY_H__

extern unsigned char nixieDisplayBuf[];
unsigned char numberMap4Nixie[];
void nixieStaticShow(unsigned char loc, number);
void nixieLongShow(unsigned char loc, number);
void nixieDynamicShow(int number);
void nixieDisplayLoop();
void nixieLoopShow(int number);

#endif
