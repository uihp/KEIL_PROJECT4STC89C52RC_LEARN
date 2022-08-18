#include <REGX52.H>
#include "segdisplay.h"
#include "matrixbtn.h"

sbit RW=P2^5;

void main()
{
	int btnCode;
	unsigned char loc = 8;
	int temp = matrixBtnScanByRowColCheck();
	RW=0;
	while (1) {
		while (temp) {
			nixieStaticShow(loc--, temp % 10);
			temp /= 10;
		}
		loc = 8;
		btnCode = matrixBtnScanByRowColCheck();
		if (btnCode) {
			temp = btnCode;
		}
	}
}
