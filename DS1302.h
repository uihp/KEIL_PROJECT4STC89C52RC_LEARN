#ifndef __DS1302_H__
#define __DS1302_H__

extern char DS1302Time[];
void init_DS1302();
void write_DS1302(unsigned char cmd, dat);
unsigned char read_DS1302(unsigned char cmd);
void get_DS1302();
void set_DS1302();

#endif
