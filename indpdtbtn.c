#include <REGX52.H>
#include "delay.h"

unsigned char globalBtnCode;

unsigned char independentBtnCheck() {
	unsigned char btnCode = 0;
	if(!P3_1) {delay(20);while(!P3_1);delay(20);btnCode=1;}
	if(!P3_0) {delay(20);while(!P3_0);delay(20);btnCode=2;}
	if(!P3_2) {delay(20);while(!P3_2);delay(20);btnCode=3;}
	if(!P3_3) {delay(20);while(!P3_3);delay(20);btnCode=4;}
	return btnCode;
}

unsigned char independentBtnState() {
	unsigned char btnCode = 0;
	if(!P3_1) {btnCode=1;}
	if(!P3_0) {btnCode=2;}
	if(!P3_2) {btnCode=3;}
	if(!P3_3) {btnCode=4;}
	return btnCode;
}

void btnEventLoop() {
	static unsigned char state, last;
	last = state;
	state = independentBtnState();
	if (!state && last) globalBtnCode = last;
}

unsigned char independentBtnCheckByEventLoop() {
	unsigned char tmp = globalBtnCode;
	globalBtnCode = 0;
	return tmp;
}
