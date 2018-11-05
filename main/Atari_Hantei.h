#ifndef ATARI_HANTEI_DEFINED
#define ATARI_HANTEI_DEFINED

#include "Defines.h"

bool atari_hantei_jiki(MAP_DATA, VECTOR);
bool atari_hantei(MAP_DATA, VECTOR, float);
bool atari_hantei_Cell(MAP_DATA, VECTOR, float, float);

bool atari_hantei_jiki(MAP_DATA Mdata, VECTOR Jiki)
{
	return atari_hantei(Mdata, Jiki, 100.0);
}

//自機やカメラなどとブロックの当たり判定
bool atari_hantei(MAP_DATA Mdata, VECTOR Object, float length)
{
	/*
	int i, j, k, Hdlp;			//カウンタ、モデルハンドル配列の添字
	int objmx, objmy, objmh;	//自機に限らずオブジェクトのマス座標

	static bool passed = FALSE;

	//当たり判定

	MV1_COLL_RESULT_POLY_DIM blockatari;	//当たり判定のための構造体
	bool flag = FALSE;						//当たり判定の当たりフラグをFALSEと仮定する

	GetMasu3(Object.z, Object.x, Object.y, &objmx, &objmy, &objmh);

	if (passed == FALSE)
	{
		for (k = 1; k <= 27; k++)
		{
			MV1SetupCollInfo(Model[k].modelh, -1);
		}
	}

	for (i = 0; i < Mdata.youso; i++)
	{
		//ifは簡略化のための条件、自機に対して水平面上のでx,yで1マス以内の場合判定
		if (
			objmx >= Mdata.value[i].x - 1 && Mdata.value[i].x + 1 >= objmx &&
			objmy >= Mdata.value[i].y - 1 && Mdata.value[i].y + 1 >= objmy
			)
		{
			for (j = 0; j < MAP_MAX; j++)
			{
				//モデルハンドルの配列の添え字を取得
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//ブロックありの場合
				if (Hdlp != 0)
				{
					//当たり判定（ifは簡略化のための条件、自機に対して高さが1マス以内の場合判定）
					if (objmh >= j - MAP_ZERO - 1 && objmh <= j - MAP_ZERO + 1)
					{
						//ブロックの座標をセット
						MV1SetPosition(
							Model[Hdlp].modelh,
							VGet(
							Mdata.value[i].y*MASU_1,
							(j - MAP_ZERO)*MASU_1,
							Mdata.value[i].x*MASU_1
							)
							);

						MV1RefreshCollInfo(Model[Hdlp].modelh, -1);

						
						hitflag = atari_hantei_cube(
							Model[Hdlp].modelh,
							Mdata,
							Object,
							cube_length
							);
							

						//カプセル型として判定
						blockatari = MV1CollCheck_Capsule(
							Model[Hdlp].modelh,
							-1,
							VGet(Object.x, Object.y - length/2.0, Object.z),
							VGet(Object.x, Object.y + length/2.0, Object.z),
							18
							);

						//もしオブジェクトとブロックが当たっていたら
						if (blockatari.HitNum!=0)
						{
							flag = TRUE;
							goto hantei_brkpoint;
						}

						//当たり判定のための構造体の後始末
						MV1CollResultPolyDimTerminate(blockatari);
					}
				}
			}
		}
	}
hantei_brkpoint:

	passed = TRUE;

	return flag;
	*/
	int i, j, k, Hdlp;			//カウンタ、モデルハンドル配列の添字
	int objmx, objmy, objmh;	//自機に限らずオブジェクトのマス座標

	GetMasu3(Object.z, Object.x, Object.y, &objmx, &objmy, &objmh);

	for (i = 0; i < Mdata.youso; i++)
	{
		//ifは簡略化のための条件、自機に対して水平面上のでx,yで1マス以内の場合判定
		if (
			objmx >= Mdata.value[i].x - 1 && Mdata.value[i].x + 1 >= objmx &&
			objmy >= Mdata.value[i].y - 1 && Mdata.value[i].y + 1 >= objmy
			)
		{
			for (j = 0; j < MAP_MAX; j++)
			{
				//モデルハンドルの配列の添え字を取得
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//ブロックありの場合
				if (Hdlp != 0)
				{
					//当たり判定（ifは簡略化のための条件、自機に対して高さが1マス以内の場合判定）
					if (objmh >= j - MAP_ZERO - 1 && objmh <= j - MAP_ZERO + 1)
					{
						if (atari_hantei_Cell(Mdata, Object, length, 18) == TRUE)
							return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

bool atari_hantei_Cell(MAP_DATA Mdata, VECTOR Object, float length, float hankei)
{
	float nowrot;
	float nowlen;
	VECTOR chkpos;
	POS_3D_H chkmpos;

	for (nowlen = -length / 2.0; nowlen < length / 2.0; nowlen += 5.0)
	{
		for (nowrot = 0.0; nowrot <= 360.0; nowrot += 45.0)
		{
			chkpos.z = Degcos(nowrot) * hankei + Object.z;
			chkpos.x = Degsin(nowrot) * hankei + Object.x;
			chkpos.y = nowlen + Object.y;
			GetMasu3(chkpos.z, chkpos.x, chkpos.y, &chkmpos.x, &chkmpos.y, &chkmpos.h);
			if (GetBlock(Mdata, chkmpos.x, chkmpos.y, chkmpos.h) != MAP_NULL) return TRUE;
		}
	}
	return FALSE;
}

#endif