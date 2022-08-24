#ifndef __I2C_H__
#define __I2C_H__

void start_I2C();
void stop_I2C();
void send_I2C(unsigned char dat);
unsigned char recv_I2C();
void sendack_I2C(bit ack);
bit recvack_I2C();

#endif
