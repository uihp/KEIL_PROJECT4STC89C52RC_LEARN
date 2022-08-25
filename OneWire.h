#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

bit init_OneWire();
void send_OneWire(bit dat);
bit recv_OneWire();
void sendB_OneWire(unsigned char dat);
unsigned char recvB_OneWire();

#endif
