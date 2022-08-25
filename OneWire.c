#include <REGX52.H>

sbit OneWire_DQ = P3^7;

bit init_OneWire() {
	bit ack;
	unsigned char i;
	EA = 0; // disable all timers
	OneWire_DQ = 1;
	OneWire_DQ = 0;
	i = 227; while (--i); // delay 500us
	OneWire_DQ = 1;
	i = 29; while (--i); // delay 70us
	ack = OneWire_DQ;
	i = 227; while (--i); // delay 500us
	EA = 1; // re-enable all timers
	return ack;
}

void send_OneWire(bit dat) {
	unsigned char i;
	EA = 0;
	OneWire_DQ = 0;
	i = 4; while (--i); // delay 10us
	OneWire_DQ = dat;
	i = 22; while (--i); // delay 50us
	OneWire_DQ = 1;
	EA = 1;
}

bit recv_OneWire() {
	bit dat;
	unsigned char i;
	EA = 0;
	OneWire_DQ = 0;
	i = 2; while (--i); // delay 5us
	OneWire_DQ = 1;
	i = 2; while (--i); // delay 5us
	dat = OneWire_DQ;
	i = 22; while (--i); // delay 50us
	EA = 1;
	return dat;
}

void sendB_OneWire(unsigned char dat) {
	unsigned char i;
	for (i=0;i<8;i++) send_OneWire(dat&(0x01<<i));
}

unsigned char recvB_OneWire() {
	unsigned char i, dat = 0x00;
	for (i=0;i<8;i++) {
		if (recv_OneWire()) dat |= (0x01<<i);
	}
	return dat;
}
