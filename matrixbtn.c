#include <REGX52.H>
#include "delay.h"

unsigned char matrixBtnScanByRowColCheck()
{
	unsigned char btnCode;
	P1 = 0xFF;
	P1_0 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=16;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=12;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=8;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=4;}
	P1 = 0xFF;
	P1_1 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=15;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=11;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=7;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=3;}
	P1 = 0xFF;
	P1_2 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=14;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=10;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=6;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=2;}
	P1 = 0xFF;
	P1_3 = 0;
	if (!P1_4) {delay(20);while(!P1_4);delay(20);btnCode=13;}
	if (!P1_5) {delay(20);while(!P1_5);delay(20);btnCode=9;}
	if (!P1_6) {delay(20);while(!P1_6);delay(20);btnCode=5;}
	if (!P1_7) {delay(20);while(!P1_7);delay(20);btnCode=1;}
	return btnCode;
}
