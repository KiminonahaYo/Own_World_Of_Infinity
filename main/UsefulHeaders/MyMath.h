#ifndef MYMATH_DEFINED

#include <math.h>
#include <stdlib.h>
#include "GetTimeForStd.h"

//プロトタイプ宣言
double Degsin(double);
double Degcos(double);
double round(double);

double GetMaxf(double, double);
double GetMinf(double, double);

int Amari(int, int);
double Amarif(double, double);

int RangeRand(int, int);
void InitRand(void);

//標準rand()関数の初期化
void InitRand(void)
{
	DateAndTime Date;
	GetTime(&Date);
	srand(
		Date.Hour * 3600 +
		Date.Min * 60 +
		Date.Sec
		);
	return;
}

//範囲を指定して乱数を発生
int RangeRand(int min, int max)
{
	if (min <= max)	return rand() % (max - min + 1) + min;
	else			return -1;
}

double GetMaxf(double num1, double num2)
{
	if (num1 > num2)	return num1;
	else			return num2;
}

double GetMinf(double num1, double num2)
{
	if (num1 < num2)	return num1;
	else			return num2;
}

int Amari(int num, int waru)
{
	int ans = num;

	if (num < 0)
	{
		while (ans < 0)		ans += waru;
	}
	else if (num >= waru)
	{
		while (ans >= waru)	ans -= waru;
	}
	return ans;
}

double Amarif(double num, double waru)
{
	double ans = num;

	if (num < 0.0)
	{
		while (ans < 0.0)	ans += waru;
	}
	else if (num >= waru)
	{
		while (ans >= waru)	ans -= waru;
	}

	return ans;
}

//デグリーで指定するsin
double Degsin(double num)
{
	double RAD1 = 3.14159 / 180.0;
	return sin(RAD1*num);
}

//デグリーで指定するcos
double Degcos(double num)
{
	double RAD1 = 3.14159 / 180.0;
	return cos(RAD1*num);
}

//四捨五入
double round(double num)
{
	if (num >= 0)
	{
		if (num - int(num) >= 0.5)	return double(int(num) + 1);
		else					return double(int(num));
	}
	else
	{
		if (num - int(num) >= -0.5)	return double(int(num) - 1);
		else					return double(int(num));
	}
}

#define MYMATH_DEFINED
#endif
