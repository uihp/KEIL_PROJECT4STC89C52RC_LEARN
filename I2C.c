#include <REGX52.H>

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

void start_I2C() {
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
}

void stop_I2C() {
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}

void send_I2C(unsigned char dat) {
	unsigned char i;
	for (i=0;i<8;i++) {
		I2C_SDA = dat & (0x80>>i);
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}

unsigned char recv_I2C() {
	unsigned char i, dat = 0x00;
	I2C_SDA = 1;
	for (i=0;i<8;i++) {	
		I2C_SCL = 1;
		if (I2C_SDA) dat |= (0x80>>i);
		I2C_SCL = 0;
	}
	return dat;
}

void sendack_I2C(bit ack) {
	I2C_SDA = ack;
	I2C_SCL = 1;
	I2C_SCL = 0;
}

bit recvack_I2C() {
	bit ack;
	I2C_SDA = 1;
	I2C_SCL = 1;
	ack = I2C_SDA;
	I2C_SCL = 0;
	return ack;
}
