#include <REGX52.H>

sbit XPT2046_CS = P3^5;
sbit XPT2046_DCLK = P3^6;
sbit XPT2046_DIN = P3^4;
sbit XPT2046_DOUT = P3^7;

unsigned int read_XPT2046(unsigned char cmd) {
	unsigned int adv = 0;
	unsigned char i;
	XPT2046_DCLK = 0;
	XPT2046_CS = 0;
	for (i=0;i<8;i++) {
		XPT2046_DIN = cmd & (0x80>>i);
		XPT2046_DCLK = 1;
		XPT2046_DCLK = 0;
	}
	for(i=0;i<16;i++) {
		XPT2046_DCLK = 1;
		XPT2046_DCLK = 0;
		if (XPT2046_DOUT) adv |= (0x8000>>i);
	}
	XPT2046_CS=1;
	return adv>>8;
}
