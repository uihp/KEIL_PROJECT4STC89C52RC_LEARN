#ifndef __IR_H__
#define __IR_H__

#define IR_POW 0x45
#define IR_MOD 0x46
#define IR_MUT 0x47
#define IR_TOG 0x44
#define IR_PRE 0x40
#define IR_NEX 0x43
#define IR_EQK 0x07
#define IR_MIN 0x15
#define IR_ADD 0x09
#define IR_RPT 0x19
#define IR_USD 0x0D
#define IR_0 0x16
#define IR_1 0x0C
#define IR_2 0x18
#define IR_3 0x5E
#define IR_4 0x08
#define IR_5 0x1C
#define IR_6 0x5A
#define IR_7 0x42
#define IR_8 0x52
#define IR_9 0x4A

extern unsigned char IR_Addr;
extern unsigned char IR_Cmd;
void init_IR();
bit getRF_IR();
bit getDF_IR();

#endif
