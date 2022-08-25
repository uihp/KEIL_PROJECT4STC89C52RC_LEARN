#include <REGX52.H>
#include "delay.h"

sbit buzzer = P2^5;

void buzzer500Hz(unsigned int ms) {
	unsigned int i;
	for (i=0;i<ms*2;i++) {
		buzzer = !buzzer;
		delay(1);
	}
}
