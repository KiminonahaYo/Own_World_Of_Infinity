//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "Defines.h"
#include "SortAndFind.h"

//------------------------------------------------------------

void loadmap(MAPCELL*, int);
void savemap(MAPCELL*, int);
void loadcell(FILE*, BINDATA*, int, int);
void savecell(FILE*, BINDATA*, int);

char *filename = "test.txt";

//------------------------------------------------------------

void loadmap(MAPCELL Map[], int n)
{
	FILE *fp;
	int i,j;

	if((fp=fopen(filename,"rb")) != NULL)
	{
		for(i=0; i<n; i++)
		{
			fseek(fp,i*(MAP_MAX+40+2)*2+10*2,SEEK_SET);
			fwscanf(fp,L"%20d %20d ",&Map[i].x,&Map[i].y);
			loadcell(fp, Map[i].data, MAP_MAX, i);
		}
		fclose(fp);
	}
	else
	{
		for(i=0; i<n; i++)
		{
			Map[i].x=0;
			Map[i].y=0;
			for(j=0; j<MAP_MAX; j++)
			{
				Map[i].data[j]=MAP_ZERO;
			}
		}
	}
}

void savemap(MAPCELL Map[], int n)
{
	FILE *fp;
	int i;

	fp=fopen(filename,"wb");

	fwprintf(fp,L"%-10d",n);
	for(i=0; i<n; i++)
	{
		fwprintf(fp,L"%-20d %-20d ",Map[i].x,Map[i].y);
		savecell(fp, Map[i].data, MAP_MAX);
	}

	fclose(fp);
}

//------------------------------------------------------------

void loadcell(FILE *fp, BINDATA data[], int y, int no)
{
	int i;

	for(i=0; i<y; i++)
	{
		fseek(fp,(i+40+2)*2+no*(MAP_MAX+40+2)*2+10*2,SEEK_SET);
		fwscanf(fp,L"%2lc",&data[i]);
	}
}

void savecell(FILE *fp, BINDATA data[], int y)
{
	int i;

	for(i=0; i<y; i++)
	{
		fputwc(data[i],fp);
	}
}

//------------------------------------------------------------
