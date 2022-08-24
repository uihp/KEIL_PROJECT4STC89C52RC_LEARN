#include <REGX52.H>
#include "I2C.h"

#define AT24C02_ADDR 0xA0

void write_AT24C02(unsigned char addr, dat) {
	start_I2C();
	send_I2C(AT24C02_ADDR);
	recvack_I2C();
	send_I2C(addr);
	recvack_I2C();
	send_I2C(dat);
	recvack_I2C();
	stop_I2C();
}

unsigned char read_AT24C02(unsigned char addr) {
	unsigned char dat;
	start_I2C();
	send_I2C(AT24C02_ADDR);
	recvack_I2C();
	send_I2C(addr);
	recvack_I2C();
	start_I2C();
	send_I2C(AT24C02_ADDR|0x01);
	recvack_I2C();
	dat = recv_I2C();
	sendack_I2C(1);
	stop_I2C();
	return dat;
}
