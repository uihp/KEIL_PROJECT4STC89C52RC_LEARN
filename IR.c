#include <REGX52.H>
#include <stdlib.h>
#include "T0.h"
#include "I0.h"

//@11.0592MHz
#define START 12442
#define REPEAT 10368
#define DATA0 1032
#define DATA1 2074

bit IR_Busy;
bit IR_Read;
bit IR_DF;
bit IR_RF;
unsigned int IR_Time;
unsigned char IR_Data[4];
unsigned char IR_Pos;
unsigned char IR_Addr;
unsigned char IR_Cmd;

void init_IR() {
	T0InitCounter();
	I0Init();
}

bit getDF_IR() {
	if (IR_DF) {
		IR_DF = 0;
		return 1;
	}
	return 0;
}

bit getRF_IR() {
	if (IR_RF) {
		IR_RF = 0;
		return 1;
	}
	return 0;
}

void I0Routine() interrupt 0 {
	if (!IR_Busy) {
		T0SetCounter(0);
		TR0 = 1;
		IR_Busy = 1;
	} else {
		IR_Time = T0GetCounter();
		T0SetCounter(0);
		if (!IR_Read) {
			if (abs(IR_Time-START) < 500) IR_Read = 1;
			else if (abs(IR_Time-REPEAT) < 500) {
				TR0 = 0;
				IR_Busy = 0;
				IR_RF = 1;
			}
		} else {
			if (abs(IR_Time-DATA0) < 500) IR_Data[IR_Pos/8]&=~(0x01<<((IR_Pos++)%8));
			else if (abs(IR_Time-DATA1) < 500) IR_Data[IR_Pos/8]|=0x01<<((IR_Pos++)%8);
			if (IR_Pos >= 32) {
				IR_Pos = 0;
				if((IR_Data[0]==~IR_Data[1]) && (IR_Data[2]==~IR_Data[3])) {
					IR_Addr = IR_Data[0];
					IR_Cmd = IR_Data[2];
					IR_DF = 1;
				}
				TR0 = 0;
				IR_Read = 0;
				IR_Busy = 0;
			}
		} 
	}
}
