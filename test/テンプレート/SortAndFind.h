/*
	座標を指定して見つけるプログラムです。
	早く見つけるためにソートしています。
*/

//------------------------------------------------------------

#include "Defines.h"

//------------------------------------------------------------

void swap(MAPCELL *a, MAPCELL *b);
void mapsort(MAPCELL*, int);
void sortmain(MAPCELL*, int,int);
int findpivot(MAPCELL*, int,int);
int64 calc(int,int);

int mapfind(MAPCELL*, int, int, int);

//------------------------------------------------------------

//クイックソートで実行
void mapsort(MAPCELL Map[], int n)
{
	sortmain(Map, 0,n-1);
}

void sortmain(MAPCELL Map[], int left, int right)
{
	int lw=left,rw=right;
	int64 pivot;
	int pivotp;
	int64 temp;

	pivotp=findpivot(Map,left,right);
	pivot=calc(Map[pivotp].x,Map[pivotp].y);
	swap(&Map[left],&Map[pivotp]);

	while(lw<rw)
	{
		while(
			temp=calc(Map[lw].x,Map[lw].y),
			lw<=rw && temp<=pivot
		) lw++;
		while(
			temp=calc(Map[rw].x,Map[rw].y),
			lw<=rw && temp>pivot
		) rw--;
		if(lw<rw) swap(&Map[lw],&Map[rw]);
	}
	swap(&Map[left], &Map[rw]);

	if(left<rw-1) sortmain(Map, left,rw-1);
	if(rw+1<right)sortmain(Map, rw+1,right);
}

//------------------------------------------------------------

int findpivot(MAPCELL Map[], int left, int right)
{
	int i;
	int64 lnum   = calc(Map[left].x,Map[right].y);
	int64 lnump1 = calc(Map[left+1].x,Map[right+1].y);
	int64 num;

	if(lnum>lnump1) return left;

	for(i=left+1; i<right; i++)
	{
		num=calc(Map[i].x,Map[i].y);
		if(lnum<num) break;
	}

	return i;
}

//------------------------------------------------------------

void swap(MAPCELL *a, MAPCELL *b)
{
	MAPCELL temp=*a;
	*a=*b;
	*b=temp;
}

//------------------------------------------------------------

//二分探索法で実行
int mapfind(MAPCELL Map[], int n, int x,int y)
{
	int low=0,high=n-1, mid;
	int64 keyxy=calc(x,y);
	int64 midxy;

	while(low<=high)
	{
		mid=(low+high)/2;

		midxy=calc(Map[mid].x,Map[mid].y);
		if(keyxy<midxy)			high=mid-1;
		else if(keyxy>midxy)	low =mid+1;
		else					break;
	}
	if(x==Map[mid].x && y==Map[mid].y)	return mid;
	else								return -1;
}

//------------------------------------------------------------

int64 calc(int x,int y)
{
	int xt=x-pxmin;
	int yt=y-pymin;
	int xm=pxmax-pxmin;

	return yt*xm+xt;
}

//------------------------------------------------------------
