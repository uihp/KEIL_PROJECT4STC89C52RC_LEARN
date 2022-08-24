#ifndef __MATRIXBTN_H__
#define __MATRIXBTN_H__

unsigned char matrixBtnScanByRowColCheck();
unsigned char matrixBtnScanByFlip(bit);
void matrixBtnEventLoop();
unsigned char matrixBtnScanByEventLoop();

#endif
