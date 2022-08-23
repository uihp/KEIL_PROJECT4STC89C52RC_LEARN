#include <REGX52.H>

sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

unsigned char DS1302Time[] = {0,0,0,1,1,1,0};

void init_DS1302() {
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}

void write_DS1302(unsigned char cmd, dat) {
	unsigned char i;
	DS1302_CE = 1;
	for (i=0;i<8;i++) {
		DS1302_IO = cmd & (0x01<<i);
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	for (i=0;i<8;i++) {
		DS1302_IO = dat & (0x01<<i);
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	DS1302_CE = 0;
}

unsigned char read_DS1302(unsigned char cmd) {
	unsigned char i, dat = 0x00;
	cmd |= 0x01;
	DS1302_CE = 1;
	for (i=0;i<8;i++) {
		DS1302_IO = cmd & (0x01<<i);
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
	for (i=0;i<8;i++) {
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		if (DS1302_IO) dat |= (0x01<<i);
	}
	DS1302_CE = 0;
	DS1302_IO = 0;
	return dat;
}

unsigned char BCD2Dec(unsigned char bcd) { return bcd/16*10+bcd%16; }
unsigned char Dec2BCD(unsigned char dec) { return dec/10*16+dec%10; }

void set_DS1302() {
	unsigned char i;
	for (i=0;i<7;i++) write_DS1302(0x80|(i<<1), Dec2BCD(DS1302Time[i]));
}

void get_DS1302() {
	unsigned char i;
	for (i=0;i<7;i++) DS1302Time[i] = BCD2Dec(read_DS1302(0x80|(i<<1)));
}
