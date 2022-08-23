#include <REGX52.H>
#include "delay.h"

unsigned char independentBtnCheck() {
	unsigned char btnCode = 0;
	if(!P3_1) {delay(20);while(!P3_1);delay(20);btnCode=1;}
	if(!P3_0) {delay(20);while(!P3_0);delay(20);btnCode=2;}
	if(!P3_2) {delay(20);while(!P3_2);delay(20);btnCode=3;}
	if(!P3_3) {delay(20);while(!P3_3);delay(20);btnCode=4;}
	return btnCode;
}
