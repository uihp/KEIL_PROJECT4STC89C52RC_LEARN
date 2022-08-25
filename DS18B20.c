#include <REGX52.H>
#include "OneWire.h"

#define DS18B20_SKIP_ROM 0xCC
#define DS18B20_CONVERT_T 0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

void convertT_DS18B20() {
	init_OneWire();
	sendB_OneWire(DS18B20_SKIP_ROM);
	sendB_OneWire(DS18B20_CONVERT_T);
}

float readT_DS18B20() {
	unsigned char LSB, MSB;
	init_OneWire();
	sendB_OneWire(DS18B20_SKIP_ROM);
	sendB_OneWire(DS18B20_READ_SCRATCHPAD);
	LSB = recvB_OneWire();
	MSB = recvB_OneWire();
	return ((MSB<<8)|LSB)/16.0;
}
