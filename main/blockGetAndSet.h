#ifndef BLOCKGETANDSET_DEFINED
#define BLOCKGETANDSET_DEFINED

//ブロックを置く、調べる
void    SetBlock(MAP_DATA, BINDATA, int, int, int);
BINDATA GetBlock(MAP_DATA, int, int, int);

//探索
int		mapfind(MAP_DATA, int, int);

//ソートの材料となる種の値を計算
int64	calc(MAP_DATA, int, int);

//ブロックを置く
void SetBlock(MAP_DATA Mdata, BINDATA block, int x, int y, int h)
{
	int p;

	p = mapfind(Mdata, x, y);

	if (p != -1 && h >= -128 && h < 128)
		Mdata.value[p].data[h + MAP_ZERO] = block;
}

//ブロックを取得する
BINDATA GetBlock(MAP_DATA Mdata, int x, int y, int h)
{
	int p;

	p = mapfind(Mdata, x, y);

	if (p != -1 && h >= -128 && h < 128)
		return Mdata.value[p].data[h + MAP_ZERO];
	else
		return B_ERROR;
}


//指定した座標（水平面座標）のデータを検索する

//二分探索法で実行
int mapfind(MAP_DATA Mdata, int x, int y)
{
	int low = 0, high = Mdata.youso - 1, mid;
	int64 keyxy = calc(Mdata, x, y);
	int64 midxy;

	while (low <= high)
	{
		mid = (low + high) / 2;

		midxy = calc(Mdata, Mdata.value[mid].x, Mdata.value[mid].y);
		if (keyxy<midxy)		high = mid - 1;
		else if (keyxy>midxy)	low = mid + 1;
		else					break;
	}
	if (Mdata.youso > 0 && x == Mdata.value[mid].x && y == Mdata.value[mid].y)	return mid;
	else																		return -1;
}

//------------------------------------------------------------

//ソートするための材料（数値）を計算する
int64 calc(MAP_DATA MData, int x, int y)
{
	int xt = x - MData.pxmin;//pxmin;
	int yt = y - MData.pymin;//pymin;
	int xm = MData.pxmax - MData.pxmin + 1;//pxmax - pxmin + 1;

	return yt*xm + xt;
}

//------------------------------------------------------------

#endif