//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mapmgr.h"

//------------------------------------------------------------

void print(MAPCELL*, int);
void setrnd(void);
void getrnd(MAPCELL*, int);

//------------------------------------------------------------

int main(void)
{
	int n=10;
	MAPCELL Map[10];
	int x,y,p;

	setrnd();

	printf("読み込んだデータ-------\n");
	loadmap(Map,n);
	print(Map,n);

	printf("乱数を発生-------\n");
	getrnd(Map,n);
	print(Map,n);
	x=Map[0].x;
	y=Map[0].y;

	printf("ソート----------\n");
	mapsort(Map,n);
	print(Map,n);

	printf("探索-------------\n");
	p=mapfind(Map,n,x,y);
	if(p!=-1)	printf("p(%d,%d)は要素[%d]に見つかりました\n",x,y, p);
	else		printf("要素は見つかりませんでした\n");

	printf("min=(%d,%d), max=(%d,%d)\n",pxmin,pymin, pxmax,pymax);

	savemap(Map,n);
	printf("保存しました。\n");

	return 0;
}

//------------------------------------------------------------

void setrnd(void)
{
	time_t timer;
	struct tm *ltime;

	timer=time(NULL);
	ltime=localtime(&timer);

	srand(
		ltime->tm_hour*3600+
		ltime->tm_min *  60+
		ltime->tm_sec
	);
}

void getrnd(MAPCELL *Map, int n)
{
	int i,j;

	pxmin=pymin=(4294967296/2-1);
	pxmax=pymax=-(4294967296/2);

	for(i=0; i<n; i++)
	{
		Map[i].x=rand()%10000-5000;
		Map[i].y=rand()%10000-5000;
		if(Map[i].x<pxmin) pxmin=Map[i].x;
		if(Map[i].x>pxmax) pxmax=Map[i].x;
		if(Map[i].y<pymin) pymin=Map[i].y;
		if(Map[i].y>pymax) pymax=Map[i].y;
		for(j=0; j<MAP_MAX; j++)
		{
			Map[i].data[j]=rand()%0x1000+B_NUM_START;
		}
	}
}

void print(MAPCELL Map[], int n)
{
	int i,j;

	for(i=0; i<n; i++)
	{
		printf("Map[%d].Pos=(%d,%d)------------------------\n",i,Map[i].x,Map[i].y);

		for(j=0; j<MAP_MAX; j++)
		{
			printf(" %04X",Map[i].data[j]);
			if((j+1)%20==0) printf("\n");
		}
		if(j%20!=0) printf("\n");
	}
}

//------------------------------------------------------------
