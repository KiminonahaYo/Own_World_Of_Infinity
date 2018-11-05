#ifndef MASU_DEFINED

#define MASU_DEFINED

#include "Defines.h"

//二次元上のマスを取得
void GetMasu2(float x, float y, int *xr, int *yr)
{
	if (x >= 0) *xr = int(x / MASU_1);
	else		*xr = int(x / MASU_1) - 1;
	if (y >= 0) *yr = int(y / MASU_1);
	else		*yr = int(y / MASU_1) - 1;
}

//三次元上のマスを取得
void GetMasu3(float x, float y, float h, int *xr, int *yr, int *hr)
{
	if (x >= 0) *xr = int(x / MASU_1);
	else		*xr = int(x / MASU_1) - 1;
	if (y >= 0) *yr = int(y / MASU_1);
	else		*yr = int(y / MASU_1) - 1;
	if (h >= 0) *hr = int(h / MASU_1);
	else		*hr = int(h / MASU_1) - 1;
}

#endif