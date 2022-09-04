#ifndef __MATRIXBTN_H__
#define __MATRIXBTN_H__

unsigned char matrixBtnScanByRowColCheck();
unsigned char matrixBtnScanByFlip(bit);
void matrixBtnEventLoop(bit press);
unsigned char matrixBtnScanByEventLoop();

#endif
