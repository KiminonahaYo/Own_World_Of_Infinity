#ifndef BLOCKGETANDSET_DEFINED
#define BLOCKGETANDSET_DEFINED

//�u���b�N��u���A���ׂ�
void    SetBlock(MAP_DATA, BINDATA, int, int, int);
BINDATA GetBlock(MAP_DATA, int, int, int);

//�T��
int		mapfind(MAP_DATA, int, int);

//�\�[�g�̍ޗ��ƂȂ��̒l���v�Z
int64	calc(MAP_DATA, int, int);

//�u���b�N��u��
void SetBlock(MAP_DATA Mdata, BINDATA block, int x, int y, int h)
{
	int p;

	p = mapfind(Mdata, x, y);

	if (p != -1 && h >= -128 && h < 128)
		Mdata.value[p].data[h + MAP_ZERO] = block;
}

//�u���b�N���擾����
BINDATA GetBlock(MAP_DATA Mdata, int x, int y, int h)
{
	int p;

	p = mapfind(Mdata, x, y);

	if (p != -1 && h >= -128 && h < 128)
		return Mdata.value[p].data[h + MAP_ZERO];
	else
		return B_ERROR;
}


//�w�肵�����W�i�����ʍ��W�j�̃f�[�^����������

//�񕪒T���@�Ŏ��s
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

//�\�[�g���邽�߂̍ޗ��i���l�j���v�Z����
int64 calc(MAP_DATA MData, int x, int y)
{
	int xt = x - MData.pxmin;//pxmin;
	int yt = y - MData.pymin;//pymin;
	int xm = MData.pxmax - MData.pxmin + 1;//pxmax - pxmin + 1;

	return yt*xm + xt;
}

//------------------------------------------------------------

#endif