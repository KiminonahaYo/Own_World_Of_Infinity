#ifndef DEFINES_DEFINED

//------------------------------------------------------------

#define B_NUM_START		0x10

#define MAP_ZERO		128
#define MAP_MAX			256

//------------------------------------------------------------

typedef unsigned short BINDATA;
typedef long long int int64;

typedef struct
{
	int x,y;
	BINDATA data[256];
}
MAPCELL;

int pxmin,pxmax;
int pymin,pymax;

//------------------------------------------------------------

#define DEFINES_DEFINED
#endif