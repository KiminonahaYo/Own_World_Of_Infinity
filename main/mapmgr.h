#ifndef TIMEOFGAME_H_DEFINED

//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "MapManager_Set.h"

//------------------------------------------------------------

//ブロックを置く
void ClickSetBlock(MAP_DATA *Mdata, BINDATA dat, JIKI Jiki, CAMERA Cam)
{
	int fx, fy, fh;
	int ax, ay, ah;

	bool sucf;

	sucf = FindViewBlockForSet(*Mdata, Cam, Jiki, Cam.kyori, &fx, &fy, &fh);

	if (sucf == TRUE)
	{
		SetBlock(*Mdata, dat, fx, fy, fh);

		//ブロック配置範囲最小/最大を求める
		if (fx > Mdata->BlockMax.x) Mdata->BlockMax.x = fx;
		if (fy > Mdata->BlockMax.y) Mdata->BlockMax.y = fy;
		if (fh > Mdata->BlockMax.h) Mdata->BlockMax.h = fh;
		if (fx < Mdata->BlockMin.x) Mdata->BlockMin.x = fx;
		if (fy < Mdata->BlockMin.y) Mdata->BlockMin.y = fy;
		if (fh < Mdata->BlockMin.h) Mdata->BlockMin.h = fh;
	}
}

//自機とカメラを基準にブロックを置くために基準となるブロックを探す（点を移動させてぶつかったところ）
bool FindViewBlockForSet(MAP_DATA Mdata, CAMERA Camera, JIKI Jiki, float kyori, int *xr, int *yr, int *hr)
{
	//m:masu, t:temp, c:check
	VECTOR Position = Camera.pos;
	VECTOR Target   = Jiki.pos;

	int xm, ym, hm;
	int txm, tym, thm;
	int cxm, cym, chm;
	int ax, ay, ah;

	//向きを設定
	float xd = Target.z - Position.z;
	float yd = Target.x - Position.x;
	float hd = Target.y - Position.y;

	//その他 b:block
	float xb, yb, hb;
	float dvi = kyori*1.2f;
	BINDATA tmp;

	//点の移動速度を1px以下にする
	xd /= dvi;
	yd /= dvi;
	hd /= dvi;

	//初めの位置のマス座標を記憶
	GetMasu3(Position.z, Position.x, Position.y, &txm, &tym, &thm);

	//ブロックが置いてあるところに自機がいないかの判定のために自機のマス座標を記憶
	GetMasu3(Target.z, Target.x, Target.y, &cxm, &cym, &chm);

	while (1)
	{
		//点を移動
		Target.z += xd;
		Target.x += yd;
		Target.y += hd;

		//移動させた点を
		GetMasu3(Target.z, Target.x, Target.y, &xm, &ym, &hm);

		//ブロックを取得、
		tmp = GetBlock(Mdata, xm, ym, hm);

		//遠い所か高すぎるところか低すぎるところ（限界を超えるところ）にブロックを置いたならば
		if (abs(xm - txm) >= 10 || abs(ym - tym) >= 10 || abs(hm - thm) >= 10 || tmp == B_ERROR)
		{
			//失敗フラグを返して抜ける
			fprintf(Prog_Log_fp, "自機と対象ブロックが離れています。\n");
			return FALSE;
		}
		//ブロックが置いてあるところにぶつかったらループを抜ける
		if (tmp != MAP_NULL) break;
	}

	ax = ay = ah = 0;

	xb = Amarif(Target.z, MASU_1);
	yb = Amarif(Target.x, MASU_1);
	hb = Amarif(Target.y, MASU_1);

	//基準となるブロックの隣のどこに置けばいいかを計算する
	if (xb >= MASU_1 - 1.0f || yb >= MASU_1 - 1.0f || xb <= 1.0f || yb <= 1.0f)
	{
		if (xb >= MASU_1 - 1.0f)	ax = 1;
		if (yb >= MASU_1 - 1.0f)	ay = 1;
		if (xb <= 1.0f)				ax = -1;
		if (yb <= 1.0f)				ay = -1;
	}
	else
	{
		if (hb >= MASU_1 - 1.0f)	ah = 1;
		if (hb <= 1.0f)				ah = -1;
	}

	//置くブロックがどこかを返す
	*xr = xm + ax;
	*yr = ym + ay;
	*hr = hm + ah;

	//置く場所に他のブロックがないか自機がいないかを確かめるところ
	tmp = GetBlock(Mdata, xm + (ax), ym + (ay), hm + (ah));

	if (tmp == MAP_NULL)
	{
		bool flag;

		//当たり判定準備
		SetBlock(Mdata, B_NUM_START + 1, xm + ax, ym + ay, hm + ah);	//試しに草原（本当は何でもいい）を置く
		flag = atari_hantei_jiki(Mdata, Jiki.pos);							//置いたものと自機の当たり判定
		SetBlock(Mdata, MAP_NULL, xm + ax, ym + ay, hm + ah);			//試しにおいた草原を消す

		if (flag == 0) return TRUE;
		else
		{
			fprintf(Prog_Log_fp, "xyh:Mdata(%d,%d,%d) 置いたところには自機があります。\n", xm + ax, ym + ay, hm + ah);
			return FALSE;
		}
	}
	else
	{
		fprintf(Prog_Log_fp, "xyh:Mdata(%d,%d,%d) 置いたところには既にブロックが置かれていました。\n", xm + ax, ym + ay, hm + ah);
		return FALSE;
	}
}

//ブロックを消す
void ClickDelBlock(MAP_DATA *Mdata, JIKI Jiki, CAMERA Cam)
{
	int fx, fy, fh;

	bool sucf;

	sucf = FindViewBlockForDel(*Mdata, Cam, Jiki, Cam.kyori, &fx, &fy, &fh);

	if (sucf == TRUE)
	{
		SetBlock(*Mdata, MAP_NULL, fx, fy, fh);

		//ブロック配置範囲最小/最大を求める
		if (fx > Mdata->BlockMax.x) Mdata->BlockMax.x = fx;
		if (fy > Mdata->BlockMax.y) Mdata->BlockMax.y = fy;
		if (fh > Mdata->BlockMax.h) Mdata->BlockMax.h = fh;
		if (fx < Mdata->BlockMin.x) Mdata->BlockMin.x = fx;
		if (fy < Mdata->BlockMin.y) Mdata->BlockMin.y = fy;
		if (fh < Mdata->BlockMin.h) Mdata->BlockMin.h = fh;
	}
}

//自機とカメラを基準に消すブロックを探す（点を移動させてぶつかったところ）
bool FindViewBlockForDel(MAP_DATA Mdata, CAMERA Camera, JIKI Jiki, float kyori, int *xr, int *yr, int *hr)
{
	VECTOR Position = Camera.pos;
	VECTOR Target   = Jiki.pos;

	int xm, ym, hm;
	int txm, tym, thm;
	float xd = Target.z - Position.z;
	float yd = Target.x - Position.x;
	float hd = Target.y - Position.y;
	float dvi = kyori*1.2f;
	BINDATA tmp;

	xd /= dvi;
	yd /= dvi;
	hd /= dvi;

	GetMasu3(Position.z, Position.x, Position.y, &txm, &tym, &thm);

	while (1)
	{
		Target.z += xd;
		Target.x += yd;
		Target.y += hd;

		GetMasu3(Target.z, Target.x, Target.y, &xm, &ym, &hm);

		tmp = GetBlock(Mdata, xm, ym, hm);
		if (abs(xm - txm) >= 10 || abs(ym - tym) >= 10 || abs(hm - thm) >= 10 || tmp == B_ERROR)
			return FALSE;
		if (tmp != MAP_NULL) break;
	}

	*xr = xm;
	*yr = ym;
	*hr = hm;

	return TRUE;
}

//------------------------------------------------------------

//メインマップ用

//新しいマップを生成（拡張）
void MakeMapCell(MAP_MAIN *Map, VECTOR Jiki)
{
	int xi, yi, i, j, f, mf = 0;	//mf : 変更したか
	int youso2 = Map->Data.youso;	//新しくマップを作る前のデータ個数
	int youso_temp = Map->Data.youso;

	int jikix, jikiy;

	//jikix = int(Jiki.z/MASU_1);
	//jikiy = int(Jiki.x/MASU_1);
	GetMasu2(Jiki.z, Jiki.x, &jikix, &jikiy);

	for (xi = jikix - 30; xi <= jikix + 30; xi++)
	{
		for (yi = jikiy - 30; yi <= jikiy + 30; yi++)
		{
			f = mapfind(Map->Data, xi, yi);
			if (f == -1)
			{
				mf = 1;
				youso_temp++;	//配列の要素数カウントを1上げる

				//配列の初期設定
				Map->Data.value = (MAPCELL*)realloc(Map->Data.value, sizeof(MAPCELL)*(youso_temp + 1));
				Map->Data.value[youso_temp - 1].x = xi;
				Map->Data.value[youso_temp - 1].y = yi;

				//マップデータの初期設定
				Map->Data.value[youso_temp - 1].data = (BINDATA*)malloc(sizeof(BINDATA)*MAP_MAX);

				//ソートに使うための座標最大値／最小値の更新
				if (xi < Map->Data.pxmin) Map->Data.pxmin = xi;
				if (xi > Map->Data.pxmax) Map->Data.pxmax = xi;
				if (yi < Map->Data.pymin) Map->Data.pymin = yi;
				if (yi > Map->Data.pymax) Map->Data.pymax = yi;

				//マップ作成
				for (i = 0; i < MAP_MAX; i++)
				{
					if (-128 <= i - MAP_ZERO && i - MAP_ZERO < -10)		Map->Data.value[youso_temp - 1].data[i] = STONE;
					else if (-10 <= i - MAP_ZERO && i - MAP_ZERO < -1)	Map->Data.value[youso_temp - 1].data[i] = DIRT;
					else if (i - MAP_ZERO == -1)						Map->Data.value[youso_temp - 1].data[i] = SOUGEN;
					else												Map->Data.value[youso_temp - 1].data[i] = MAP_NULL;
				}

				/*
				//予約マップを配置
				for (i = 0; i < Map->Yoyaku.youso; i++)
				{
				int n;
				Template_No_Load(&n);

				for (j = 0; j < n; j++)
				{
				if (MapTAry[j].no == Map->Yoyaku.no[i]) break;
				}
				if (j < n)
				{
				set_maptemp_yoyaku(Map, MapTAry[j], i);
				}
				}
				*/
			}
		}
	}

MakeMapCell_BrkPoint:

	if (mf == 1)
	{
		Map->Data.youso = youso_temp;
		if (edit_makemapcell_flag == 1)
		{
			mapsort(&Map->Data, youso2, Map->Data.youso, TRUE);
			edit_makemapcell_flag = 0;
		}
		else
		{
			mapsort(&Map->Data, youso2, Map->Data.youso, FALSE);
		}
	}
}

//メインマップをロード
void loadmap(char *filename, MAP_MAIN *Map, JIKI *Jiki, CAMERA *Cam)
{
	FILE *fp;
	int i;

	clsDx();
	printfDx("Now Loading.....\n");
	ScreenFlip();

	if ((fp = fopen(filename, "rb")) != NULL)
	{
		fseek(fp, 0, SEEK_SET);

		//ヘッダー
		fread(&Map->Data.youso, sizeof(int), 1, fp);
		fread(&Map->Yoyaku.youso, sizeof(int), 1, fp);
		fread(&Map->Data.pxmax, sizeof(int), 1, fp);
		fread(&Map->Data.pxmin, sizeof(int), 1, fp);
		fread(&Map->Data.pymax, sizeof(int), 1, fp);
		fread(&Map->Data.pymin, sizeof(int), 1, fp);
		fread(&Map->Data.BlockMax, sizeof(POS_3D_H), 1, fp);
		fread(&Map->Data.BlockMin, sizeof(POS_3D_H), 1, fp);
		fread(Jiki, sizeof(JIKI), 1, fp);
		fread(Cam, sizeof(CAMERA), 1, fp);
		fread(&gametimer, sizeof(int64), 1, fp);

		//マップデータ配列領域確保
		(Map->Data.value) = (MAPCELL*)malloc(0);
		(Map->Data.value) = (MAPCELL*)realloc(Map->Data.value, sizeof(MAPCELL)*(Map->Data.youso + 1));

		//テンプレート予約座標配列領域確保
		(Map->Yoyaku.value) = (POS_3D_H*)malloc(0);
		(Map->Yoyaku.value) = (POS_3D_H*)realloc(Map->Yoyaku.value, sizeof(POS_3D_H)*(Map->Yoyaku.youso));

		//テンプレート予約角度配列領域確保
		(Map->Yoyaku.kaku) = (int*)malloc(0);
		(Map->Yoyaku.kaku) = (int*)realloc(Map->Yoyaku.kaku, sizeof(int)*(Map->Yoyaku.youso));

		//テンプレート予約番号配列領域確保
		(Map->Yoyaku.no) = (int*)malloc(0);
		(Map->Yoyaku.no) = (int*)realloc(Map->Yoyaku.no, sizeof(int)*(Map->Yoyaku.youso));

		//テンプレート予約
		for (i = 0; i < Map->Yoyaku.youso; i++)
		{
			fread(&Map->Yoyaku.value[i], sizeof(POS_3D_H), 1, fp);
			fread(&Map->Yoyaku.no[i], sizeof(int), 1, fp);
			fread(&Map->Yoyaku.kaku[i], sizeof(int), 1, fp);
		}

		//マップデータ
		for (i = 0; i < Map->Data.youso; i++)
		{
			//マップデータ256個分の配列を確保
			Map->Data.value[i].data = (BINDATA*)malloc(sizeof(BINDATA)*MAP_MAX);

			//座標情報読み込み
			fread(&Map->Data.value[i].x, sizeof(int), 1, fp);
			fread(&Map->Data.value[i].y, sizeof(int), 1, fp);
			loadcell(fp, Map->Data.value[i].data);
		}
		fclose(fp);
	}
	else
	{
		//ファイルがなかったら初期設定
		Map->Data.value = (MAPCELL*)malloc(0);
		Map->Yoyaku.value = (POS_3D_H*)malloc(0);
		Map->Yoyaku.no = (int*)malloc(0);
		Map->Yoyaku.kaku = (int*)malloc(0);
		Map->Data.youso = 0;
		Map->Yoyaku.youso = 0;
		Map->Data.pxmax = -(4294967296 / 2);
		Map->Data.pxmin = (4294967296 / 2) - 1;
		Map->Data.pymax = -(4294967296 / 2);
		Map->Data.pymin = (4294967296 / 2) - 1;
		Map->Data.BlockMax.x = -(4294967296 / 2);
		Map->Data.BlockMax.y = -(4294967296 / 2);
		Map->Data.BlockMax.h = -(4294967296 / 2);
		Map->Data.BlockMin.x = (4294967296 / 2) - 1;
		Map->Data.BlockMin.y = (4294967296 / 2) - 1;
		Map->Data.BlockMin.h = (4294967296 / 2) - 1;
		(*Jiki).pos.x = 0.0;
		(*Jiki).pos.y = 50.0;
		(*Jiki).pos.z = 0.0;
		(*Jiki).CanFlySkyFlag = 0;
		(*Cam).kyori = 50.0;
		(*Cam).r1 = 0.0;
		(*Cam).r2 = 10.0;
		gametimer = 3600 * 6;
	}
	clsDx();
}

//メインマップをセーブ
void savemap(char *filename, MAP_MAIN Map, JIKI Jiki, CAMERA Cam)
{
	FILE *fp;
	int i;

	fp = fopen(filename, "wb");

	//ヘッダー情報読み込み
	fwrite(&Map.Data.youso, sizeof(int), 1, fp);
	fwrite(&Map.Yoyaku.youso, sizeof(int), 1, fp);
	fwrite(&Map.Data.pxmax, sizeof(int), 1, fp);
	fwrite(&Map.Data.pxmin, sizeof(int), 1, fp);
	fwrite(&Map.Data.pymax, sizeof(int), 1, fp);
	fwrite(&Map.Data.pymin, sizeof(int), 1, fp);
	fwrite(&Map.Data.BlockMax, sizeof(POS_3D_H), 1, fp);
	fwrite(&Map.Data.BlockMin, sizeof(POS_3D_H), 1, fp);
	fwrite(&Jiki, sizeof(JIKI), 1, fp);
	fwrite(&Cam, sizeof(CAMERA), 1, fp);
	fwrite(&gametimer, sizeof(int64), 1, fp);

	//テンプレート予約
	for (i = 0; i < Map.Yoyaku.youso; i++)
	{
		fread(&Map.Yoyaku.value[i], sizeof(POS_3D_H), 1, fp);
		fread(&Map.Yoyaku.no[i], sizeof(int), 1, fp);
		fread(&Map.Yoyaku.kaku[i], sizeof(int), 1, fp);
	}

	//マップ情報（x,yとマップ本体）を保存
	for (i = 0; i < Map.Data.youso; i++)
	{
		fwrite(&Map.Data.value[i].x, sizeof(int), 1, fp);
		fwrite(&Map.Data.value[i].y, sizeof(int), 1, fp);
		savecell(fp, Map.Data.value[i].data);
	}

	//ログを保存
	fprintf(Prog_Log_fp, "--------------------------------------------\n");
	fprintf(Prog_Log_fp,
		"youso=%d,\n"
		"Jiki(%.2f,%.2f,%.2f),\n"
		"Cam(%.2f,%.2f,%.2f),\n"
		"Kyori=%.2f,r(%.2f,%.2f),\n"
		"TimeCode=%d\n"
		"で保存しました。\n",
		Map.Data.youso,
		Jiki.pos.x,
		Jiki.pos.y,
		Jiki.pos.z,
		Cam.pos.x,
		Cam.pos.y,
		Cam.pos.z,
		Cam.kyori,
		Cam.r1,
		Cam.r2,
		gametimer
		);

	fclose(fp);
}

void del_mapmain_array(MAP_MAIN *Map)
{
	int i;

	for (i = 0; i < Map->Data.youso; i++)
	{
		free(Map->Data.value[i].data);
	}

	free(Map->Data.value);
}

//------------------------------------------------------------

//テンプレート用

//新しいマップを作成、拡張
void MakeMapCell_Temp(MAP_TEMPLATE *Map, VECTOR Jiki)
{
	int xi, yi, i, f, mf = 0;		//mf : 変更したか
	int xist, yist;
	int youso2 = Map->Data.youso;	//新しくマップを作る前のデータ個数
	int youso_temp = Map->Data.youso;

	int jikix, jikiy;

	//jikix = int(Jiki.z/MASU_1);
	//jikiy = int(Jiki.x/MASU_1);
	GetMasu2(Jiki.z, Jiki.x, &jikix, &jikiy);

	//平面座標のマイナス部分は拡張しない
	if (jikix - 20 >= 0)	xist = jikix - 20;
	else					xist = 0;
	if (jikiy - 20 >= 0)	yist = jikiy - 20;
	else					yist = 0;

	for (xi = xist; xi <= jikix + 20; xi++)
	{
		for (yi = yist; yi <= jikiy + 20; yi++)
		{
			f = mapfind(Map->Data, xi, yi);
			if (f == -1)
			{
				mf = 1;
				youso_temp++;	//配列の要素数カウントを1上げる

				//配列の初期設定
				Map->Data.value = (MAPCELL*)realloc(Map->Data.value, sizeof(MAPCELL)*(youso_temp + 1));
				Map->Data.value[youso_temp - 1].x = xi;
				Map->Data.value[youso_temp - 1].y = yi;

				//マップデータの初期設定
				Map->Data.value[youso_temp - 1].data = (BINDATA*)malloc(sizeof(BINDATA)*MAP_MAX);

				//ソートに使うための座標最大値／最小値の更新
				if (xi < Map->Data.pxmin) Map->Data.pxmin = xi;
				if (xi > Map->Data.pxmax) Map->Data.pxmax = xi;
				if (yi < Map->Data.pymin) Map->Data.pymin = yi;
				if (yi > Map->Data.pymax) Map->Data.pymax = yi;

				//マップ作成
				for (i = 0; i < MAP_MAX; i++)
				{
					if (-128 <= i - MAP_ZERO && i - MAP_ZERO < -10)		Map->Data.value[youso_temp - 1].data[i] = STONE;
					else if (-10 <= i - MAP_ZERO && i - MAP_ZERO < -1)	Map->Data.value[youso_temp - 1].data[i] = DIRT;
					else if (i - MAP_ZERO == -1)						Map->Data.value[youso_temp - 1].data[i] = SOUGEN;
					else												Map->Data.value[youso_temp - 1].data[i] = MAP_NULL;
				}
			}
		}
	}
	Map->Data.youso = youso_temp;
	if (mf == 1)
	{
		Map->Data.youso = youso_temp;
		if (edit_makemapcell_flag == 0)
		{
			mapsort(&Map->Data, youso2, Map->Data.youso, TRUE);
			edit_makemapcell_flag = 1;
		}
		else
		{
			mapsort(&Map->Data, youso2, Map->Data.youso, FALSE);
		}
	}
}

//テンプレートマップのロード
void loadmap_Temp(char *filename, MAP_TEMPLATE* Map, JIKI *Jiki, CAMERA *Cam)
{
	FILE *fp;
	int i;

	clsDx();
	printfDx("Now Loading.....\n");
	ScreenFlip();

	if ((fp = fopen(filename, "rb")) != NULL)
	{
		fseek(fp, 0, SEEK_SET);

		//ヘッダー
		fread(&Map->Data.youso, sizeof(int), 1, fp);
		fread(&Map->Data.pxmax, sizeof(int), 1, fp);
		fread(&Map->Data.pxmin, sizeof(int), 1, fp);
		fread(&Map->Data.pymax, sizeof(int), 1, fp);
		fread(&Map->Data.pymin, sizeof(int), 1, fp);
		fread(&Map->Data.BlockMax, sizeof(POS_3D_H), 1, fp);
		fread(&Map->Data.BlockMin, sizeof(POS_3D_H), 1, fp);
		//fread(&Map->no, sizeof(int), 1, fp);
		fread(Jiki, sizeof(JIKI), 1, fp);
		fread(Cam, sizeof(CAMERA), 1, fp);
		fread(&gametimer, sizeof(int64), 1, fp);

		//マップデータ配列領域確保
		(Map->Data.value) = (MAPCELL*)malloc(0);
		(Map->Data.value) = (MAPCELL*)realloc(Map->Data.value, sizeof(MAPCELL)*(Map->Data.youso + 1));

		//マップデータ
		for (i = 0; i < Map->Data.youso; i++)
		{
			//マップデータ256個分の配列を確保
			Map->Data.value[i].data = (BINDATA*)malloc(sizeof(BINDATA)*MAP_MAX);

			//座標情報読み込み
			fread(&Map->Data.value[i].x, sizeof(int), 1, fp);
			fread(&Map->Data.value[i].y, sizeof(int), 1, fp);
			loadcell(fp, Map->Data.value[i].data);
		}
		fclose(fp);
	}
	else
	{
		//ファイルがなかったら初期設定
		Map->Data.value = (MAPCELL*)malloc(0);
		Map->Data.youso = 0;
		Map->Data.pxmax = -(4294967296 / 2);
		Map->Data.pxmin = (4294967296 / 2) - 1;
		Map->Data.pymax = -(4294967296 / 2);
		Map->Data.pymin = (4294967296 / 2) - 1;
		Map->Data.BlockMax.x = -(4294967296 / 2);
		Map->Data.BlockMax.y = -(4294967296 / 2);
		Map->Data.BlockMax.h = -(4294967296 / 2);
		Map->Data.BlockMin.x = (4294967296 / 2) - 1;
		Map->Data.BlockMin.y = (4294967296 / 2) - 1;
		Map->Data.BlockMin.h = (4294967296 / 2) - 1;
		//Template_No_Load(&Map->no);
		//Template_No_Save(Map->no + 1);
		(*Jiki).pos.x = MASU_1;
		(*Jiki).pos.y = 50.0;
		(*Jiki).pos.z = MASU_1;
		(*Jiki).CanFlySkyFlag = 0;
		(*Cam).kyori = 50.0;
		(*Cam).r1 = 0.0;
		(*Cam).r2 = 10.0;
		gametimer = 3600 * 6;
	}
	clsDx();
}

//テンプレートマップのセーブ
void savemap_Temp(char *filename, MAP_TEMPLATE Map, JIKI Jiki, CAMERA Cam)
{
	FILE *fp;
	int i;

	fp = fopen(filename, "wb");

	//ヘッダー情報読み込み
	fwrite(&Map.Data.youso, sizeof(int), 1, fp);
	fwrite(&Map.Data.pxmax, sizeof(int), 1, fp);
	fwrite(&Map.Data.pxmin, sizeof(int), 1, fp);
	fwrite(&Map.Data.pymax, sizeof(int), 1, fp);
	fwrite(&Map.Data.pymin, sizeof(int), 1, fp);
	fwrite(&Map.Data.BlockMax, sizeof(POS_3D_H), 1, fp);
	fwrite(&Map.Data.BlockMin, sizeof(POS_3D_H), 1, fp);
	//fwrite(&Map.no, sizeof(int), 1, fp);
	fwrite(&Jiki, sizeof(JIKI), 1, fp);
	fwrite(&Cam, sizeof(CAMERA), 1, fp);
	fwrite(&gametimer, sizeof(int64), 1, fp);

	//マップ情報（x,yとマップ本体）を保存
	for (i = 0; i < Map.Data.youso; i++)
	{
		fwrite(&Map.Data.value[i].x, sizeof(int), 1, fp);
		fwrite(&Map.Data.value[i].y, sizeof(int), 1, fp);
		savecell(fp, Map.Data.value[i].data);
	}

	//ログを保存
	fprintf(Prog_Log_fp, "--------------------------------------------\n");
	fprintf(Prog_Log_fp,
		"youso=%d,\n"
		"Jiki(%.2f,%.2f,%.2f),\n"
		"Cam(%.2f,%.2f,%.2f),\n"
		"Kyori=%.2f,r(%.2f,%.2f),\n"
		"TimeCode=%d\n"
		"で保存しました。\n",
		Map.Data.youso,
		Jiki.pos.x,
		Jiki.pos.y,
		Jiki.pos.z,
		Cam.pos.x,
		Cam.pos.y,
		Cam.pos.z,
		Cam.kyori,
		Cam.r1,
		Cam.r2,
		gametimer
		);

	fclose(fp);
}

//メインマップに貼り付けるためのテンプレートマップのロード
void loadmap_Temp_formain(char *filename, MAP_TEMPLATE *Map)
{
	FILE *fp;
	int i;

	clsDx();
	printfDx("Now Loading.....\n");
	ScreenFlip();

	if ((fp = fopen(filename, "rb")) != NULL)
	{
		fseek(fp, 0, SEEK_SET);

		//ヘッダー
		fread(&Map->Data.youso, sizeof(int), 1, fp);
		fread(&Map->Data.pxmax, sizeof(int), 1, fp);
		fread(&Map->Data.pxmin, sizeof(int), 1, fp);
		fread(&Map->Data.pymax, sizeof(int), 1, fp);
		fread(&Map->Data.pymin, sizeof(int), 1, fp);
		fread(&Map->Data.BlockMax, sizeof(POS_3D_H), 1, fp);
		fread(&Map->Data.BlockMin, sizeof(POS_3D_H), 1, fp);
		//fread(&Map->no, sizeof(int), 1, fp);

		//fread(Jiki, sizeof(JIKI), 1, fp);
		//fread(Cam, sizeof(CAMERA), 1, fp);
		//fread(&gametimer, sizeof(int64), 1, fp);

		fnext(fp, sizeof(JIKI)+sizeof(CAMERA)+sizeof(int64));

		//マップデータ配列領域確保
		(Map->Data.value) = (MAPCELL*)malloc(0);
		(Map->Data.value) = (MAPCELL*)realloc(Map->Data.value, sizeof(MAPCELL)*(Map->Data.youso + 1));

		//マップデータ
		for (i = 0; i < Map->Data.youso; i++)
		{
			//マップデータ256個分の配列を確保
			Map->Data.value[i].data = (BINDATA*)malloc(sizeof(BINDATA)*MAP_MAX);

			//座標情報読み込み
			fread(&Map->Data.value[i].x, sizeof(int), 1, fp);
			fread(&Map->Data.value[i].y, sizeof(int), 1, fp);
			loadcell(fp, Map->Data.value[i].data);
		}
		fclose(fp);
	}
}

//指定したバイト数ファイルのカーソルを進める(file next)
void fnext(FILE *fp, long bytes)
{
	fseek(fp, bytes, SEEK_CUR);
}

//指定したバイト数ファイルのカーソルを戻す(file back)
void fback(FILE *fp, long bytes)
{
	fseek(fp, -bytes, SEEK_CUR);
}

void del_maptemp_array(MAP_TEMPLATE *Map)
{
	int i;

	for (i = 0; i < Map->Data.youso; i++)
	{
		free(Map->Data.value[i].data);
	}

	free(Map->Data.value);
}

//テンプレートをメインマップに配置
void set_maptemp(MAP_MAIN *MapM, MAP_TEMPLATE MapT, int setx, int sety, int seth, CAMERA Cam)
{
	int xi, yi, hi;
	int xim, yim;
	int ximst, yimst, ximp, yimp;
	int kakumode;

	bool setflag = 1;

	if (Amari((int)Cam.r1, 360) < 45)
	{
		kakumode = 1;
	}
	else if (Amari((int)Cam.r1, 360) < 135)
	{
		kakumode = 2;
	}
	else if (Amari((int)Cam.r1, 360) < 225)
	{
		kakumode = 3;
	}
	else if (Amari((int)Cam.r1, 360) < 315)
	{
		kakumode = 4;
	}
	else
	{
		kakumode = 1;
	}

	if (kakumode == 1)
	{
		ximst = MapT.Data.BlockMin.x;
		yimst = MapT.Data.BlockMin.y;
		ximp = -1;
		yimp = -1;
	}
	else if (kakumode == 2)
	{
		ximst = MapT.Data.BlockMin.y;
		yimst = MapT.Data.BlockMin.x;
		ximp = -1;
		yimp = +1;
	}
	else if (kakumode == 3)
	{
		ximst = MapT.Data.BlockMin.x;
		yimst = MapT.Data.BlockMin.y;
		ximp = +1;
		yimp = +1;
	}
	else if (kakumode == 4)
	{
		ximst = MapT.Data.BlockMin.y;
		yimst = MapT.Data.BlockMin.x;
		ximp = +1;
		yimp = -1;
	}

	if (kakumode == 1 || kakumode == 3)
	{
		for (xi = MapT.Data.BlockMin.x, xim = ximst; xi <= MapT.Data.BlockMax.x; xi++, xim += ximp)
		{
			for (yi = MapT.Data.BlockMin.y, yim = yimst; yi <= MapT.Data.BlockMax.y; yi++, yim += yimp)
			{
				for (hi = MapT.Data.BlockMin.h; hi <= MapT.Data.BlockMax.h; hi++)
				{
					if (mapfind(MapM->Data, setx + xim, sety + yim) == -1 || GetBlock(MapM->Data, setx + xim, sety + yim, seth + hi) != MAP_NULL)
					{
						if (hi >= 0)
						{
							setflag = 0;
							goto settemp_brkpoint1;
						}
					}
				}
			}
		}
	settemp_brkpoint1:

		if (setflag)
		{
			for (xi = MapT.Data.BlockMin.x, xim = ximst; xi <= MapT.Data.BlockMax.x; xi++, xim += ximp)
			{
				for (yi = MapT.Data.BlockMin.y, yim = yimst; yi <= MapT.Data.BlockMax.y; yi++, yim += yimp)
				{
					for (hi = MapT.Data.BlockMin.h; hi <= MapT.Data.BlockMax.h; hi++)
					{
						BINDATA setb;

						setb = GetBlock(MapT.Data, xi, yi, hi);
						SetBlock(MapM->Data, setb, setx + xim, sety + yim, seth + hi);
						fprintf(Prog_Log_fp, "LoadPos(%d,%d,%d), setpos(%d,%d,%d)\n", xi, yi, hi, xim, yim, hi);
					}
				}
			}

			/*
			//テンプレートの位置を予約して未配置のブロックも拡張時に配置できるようにする
			MapM->Yoyaku.youso++;
			MapM->Yoyaku.value = (POS_3D_H*)realloc(MapM->Yoyaku.value, sizeof(POS_3D_H)*MapM->Yoyaku.youso);
			MapM->Yoyaku.no = (int*)realloc(MapM->Yoyaku.no, sizeof(int)*MapM->Yoyaku.youso);
			MapM->Yoyaku.kaku = (int*)realloc(MapM->Yoyaku.kaku, sizeof(int)*MapM->Yoyaku.youso);

			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].x = setx;
			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].y = sety;
			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].h = seth;
			MapM->Yoyaku.no[MapM->Yoyaku.youso - 1] = MapT.no;
			MapM->Yoyaku.kaku[MapM->Yoyaku.youso - 1] = kakumode;
			*/
		}
	}
	else
	{
		for (xi = MapT.Data.BlockMin.x, yim = yimst; xi <= MapT.Data.BlockMax.x; xi++, yim += yimp)
		{
			for (yi = MapT.Data.BlockMin.y, xim = ximst; yi <= MapT.Data.BlockMax.y; yi++, xim += ximp)
			{
				for (hi = MapT.Data.BlockMin.h; hi <= MapT.Data.BlockMax.h; hi++)
				{
					if (GetBlock(MapT.Data, xi, yi, hi) != MAP_NULL)
					{
						if (mapfind(MapM->Data, setx + xim, sety + yim) == -1 || GetBlock(MapM->Data, setx + xim, sety + yim, seth + hi) != MAP_NULL)
						{
							if (hi >= 0)
							{
								setflag = 0;
								goto settemp_brkpoint1;
							}
						}
					}
				}
			}
		}
	settemp_brkpoint2:

		if (setflag)
		{
			for (xi = MapT.Data.BlockMin.x, yim = yimst; xi <= MapT.Data.BlockMax.x; xi++, yim += yimp)
			{
				for (yi = MapT.Data.BlockMin.y, xim = ximst; yi <= MapT.Data.BlockMax.y; yi++, xim += ximp)
				{
					for (hi = MapT.Data.BlockMin.h; hi <= MapT.Data.BlockMax.h; hi++)
					{
						if (GetBlock(MapT.Data, xi, yi, hi) != MAP_NULL)
						{
							BINDATA setb;

							setb = GetBlock(MapT.Data, xi, yi, hi);
							SetBlock(MapM->Data, setb, setx + xim, sety + yim, seth + hi);
							fprintf(Prog_Log_fp, "LoadPos(%d,%d,%d), setpos(%d,%d,%d)\n", xi, yi, hi, xim, yim, hi);
						}
					}
				}
			}

			/*
			//テンプレートの位置を予約して未配置のブロックも拡張時に配置できるようにする
			MapM->Yoyaku.youso++;
			MapM->Yoyaku.value = (POS_3D_H*)realloc(MapM->Yoyaku.value, sizeof(POS_3D_H)*MapM->Yoyaku.youso);
			MapM->Yoyaku.no = (int*)realloc(MapM->Yoyaku.no, sizeof(int)*MapM->Yoyaku.youso);
			MapM->Yoyaku.kaku = (int*)realloc(MapM->Yoyaku.kaku, sizeof(int)*MapM->Yoyaku.youso);

			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].x = setx;
			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].y = sety;
			MapM->Yoyaku.value[MapM->Yoyaku.youso - 1].h = seth;
			MapM->Yoyaku.no[MapM->Yoyaku.youso - 1] = MapT.no;
			MapM->Yoyaku.kaku[MapM->Yoyaku.youso - 1] = kakumode;
			*/
		}
	}
}
/*
void set_maptemp_yoyaku(MAP_MAIN *MapM, MAP_TEMPLATE MapT, int yoyakusoeji)
{
	int xi, yi, hi;
	int xim, yim;
	int setx, sety, seth;
	int ximst, yimst, ximp, yimp;
	int kakumode;

	bool setflag = 1;

	kakumode = MapM->Yoyaku.kaku[yoyakusoeji];
	setx = MapM->Yoyaku.value[yoyakusoeji].x;
	sety = MapM->Yoyaku.value[yoyakusoeji].y;
	seth = MapM->Yoyaku.value[yoyakusoeji].h;

	if (kakumode == 1)
	{
		ximst = MapT.Data.BlockMin.x;
		yimst = MapT.Data.BlockMin.y;
		ximp = -1;
		yimp = -1;
	}
	else if (kakumode == 2)
	{
		ximst = MapT.Data.BlockMin.y;
		yimst = MapT.Data.BlockMin.x;
		ximp = -1;
		yimp = +1;
	}
	else if (kakumode == 3)
	{
		ximst = MapT.Data.BlockMin.x;
		yimst = MapT.Data.BlockMin.y;
		ximp = +1;
		yimp = +1;
	}
	else if (kakumode == 4)
	{
		ximst = MapT.Data.BlockMin.y;
		yimst = MapT.Data.BlockMin.x;
		ximp = +1;
		yimp = -1;
	}

	for (xi = MapT.Data.BlockMin.x, xim = ximst; xi <= MapT.Data.BlockMax.x; xi++, xim += ximp)
	{
		for (yi = MapT.Data.BlockMin.y, yim = yimst; yi <= MapT.Data.BlockMax.y; yi++, yim += yimp)
		{
			for (hi = MapT.Data.BlockMin.x; hi <= MapT.Data.BlockMax.x; hi++)
			{
				if (mapfind(MapM->Data, setx + xim, sety + yim) != -1 && GetBlock(MapM->Data, setx + xim, sety + yim, seth + hi) != MAP_NULL)
				{
					setflag = 0;
					goto settemp_brkpoint1;
				}
			}
		}
	}
settemp_brkpoint1:

	if (setflag)
	{
		for (xi = MapT.Data.BlockMin.x, xim = ximst; xi <= MapT.Data.BlockMax.x; xi++, xim += ximp)
		{
			for (yi = MapT.Data.BlockMin.y, yim = yimst; yi <= MapT.Data.BlockMax.y; yi++, yim += yimp)
			{
				for (hi = MapT.Data.BlockMin.x; hi <= MapT.Data.BlockMax.x; hi++)
				{
					BINDATA setb;

					if (mapfind(MapM->Data, setx + xim, sety + yim) != -1)
					{
						setb = GetBlock(MapT.Data, xi, yi, hi);
						SetBlock(MapM->Data, setb, setx + xim, sety + yim, seth + hi);
					}
				}
			}
		}
	}
}
*/

void Template_No_Load(int *no)
{
	FILE *fp;

	fp = fopen("TempNo.dat", "rb");

	if (fp != NULL)
	{
		fread(no, sizeof(int), 1, fp);
		fclose(fp);
	}
	else
	{
		*no = 0;
	}
}

void Template_No_Save(int no)
{
	FILE *fp;

	fp = fopen("TempNo.dat", "wb");

	fwrite(&no, sizeof(int), 1, fp);
	fclose(fp);
}

//------------------------------------------------------------

//1マップセル読み込み

//圧縮マップセルデータをロード
void loadcell(FILE *fp, BINDATA data[])
{
	int i, j;
	BINDATA dtmp;
	BINDATA cnt;
	BINDATA zipf;

	for (i = 0; i < MAP_MAX;)
	{
		//解凍作業
		fread(&dtmp, sizeof(BINDATA), 1, fp);
		fread(&zipf, sizeof(BINDATA), 1, fp);
		if (zipf == B_ZIP_CODE)
		{
			fread(&cnt, sizeof(BINDATA), 1, fp);
			for (j = i; j < MAP_MAX && j < i + cnt + 1; j++)
			{
				data[j] = dtmp;
			}
			i += cnt + 1;
		}
		else
		{
			data[i] = dtmp;
			mback(fp);
			i++;
		}
	}

	//for(i=0; i<n; i++)
	//{
	//fwscanf(fp,L"%2lc",&data[i]);
	//}
}

//マップセルデータを圧縮してセーブ
void savecell(FILE *fp, BINDATA data[])
{
	int i, j;
	BINDATA cnt;
	BINDATA zip = B_ZIP_CODE;

	for (i = 0; i < MAP_MAX;)
	{
		//圧縮作業
		cnt = 0;	//0=1文字
		for (j = i; j < MAP_MAX - 1 && data[j] == data[j + 1]; j++)
		{
			cnt++;
		}
		if (cnt >= 3)
		{
			fwrite(&data[i], sizeof(BINDATA), 1, fp);
			fwrite(&zip, sizeof(BINDATA), 1, fp);
			fwrite(&cnt, sizeof(BINDATA), 1, fp);
			i += cnt + 1;
		}
		else
		{
			fwrite(&data[i], sizeof(BINDATA), 1, fp);
			i++;
		}
	}

	//for(i=0; i<n; i++)
	//{
	//fputc(data[i],fp);
	//}
}

//------------------------------------------------------------

//ファイルの現在位置をマップ上のブロック1つ分進める(map next)
void mnext(FILE *fp)
{
	fseek(fp, (long)sizeof(BINDATA), SEEK_CUR);
}

//ファイルの現在位置をマップ上のブロック1つ分戻す(map back)
void mback(FILE *fp)
{
	fseek(fp, -(long)sizeof(BINDATA), SEEK_CUR);
}

//------------------------------------------------------------

//マップ情報を二分法で探索するためソート----------------------

//ヒープソートで実行
void mapsort(MAP_DATA *Mdata, int baddn, int aaddn, bool resetflag = 0)//before,after
{
	static int passed = 0;
	static MAP_DATA temp;
	int i;
	int n;

	if (resetflag && passed != 0)
	{
		free(temp.value);
		passed = 0;
	}

	//初回通過時は一時変数を初期化
	if (passed == 0) temp.value = (MAPCELL*)malloc(0);

	temp.pxmax = Mdata->pxmax;
	temp.pxmin = Mdata->pxmin;
	temp.pymax = Mdata->pymax;
	temp.pymin = Mdata->pymin;

	temp.value = (MAPCELL*)realloc(temp.value, sizeof(MAPCELL)*(aaddn + 1));

	//初回ソート時
	if (passed == 0)
	{
		//Mdata->temp（配列1番地から）
		for (i = 0; i < aaddn; i++)
		{
			temp.value[i + 1] = Mdata->value[i];
		}

		//ニ分木を作成
		for (i = aaddn / 2; i >= 1; i--)
		{
			buildtree(&temp, aaddn, i);
		}
	}
	//初回以外
	else
	{
		//ニ分木に追加
		for (i = baddn; i < aaddn; i++)
		{
			temp.value[i + 1] = Mdata->value[i];
			addtree(&temp, i + 1);
		}
	}

	//temp->Mdata（配列１番地から）
	for (i = 0; i < aaddn; i++)
	{
		Mdata->value[i + 1] = temp.value[i + 1];
	}

	//ニ分木からソート
	n = aaddn;
	while (n > 1)
	{
		swap(&Mdata->value[1], &Mdata->value[n]);
		n--;
		buildtree(Mdata, n, 1);
	}

	//配列0番地からに直す
	for (i = 0; i < aaddn; i++)
	{
		Mdata->value[i] = Mdata->value[i + 1];
	}

	passed = 1;
}

//二分木を作成
void buildtree(MAP_DATA *Mdata, int n, int oya)
{
	int child = oya * 2;
	int64 chn, chn_p1, oyan;

	while (child <= n)
	{
		if (child < n)
		{
			chn = calc(*Mdata, Mdata->value[child].x, Mdata->value[child].y);
			chn_p1 = calc(*Mdata, Mdata->value[child + 1].x, Mdata->value[child + 1].y);
			if (chn < chn_p1) child++;
		}
		chn = calc(*Mdata, Mdata->value[child].x, Mdata->value[child].y);
		oyan = calc(*Mdata, Mdata->value[oya].x, Mdata->value[oya].y);

		if (chn < oyan) break;

		swap(&Mdata->value[child], &Mdata->value[oya]);

		oya = child;
		child = oya * 2;
	}
}

//二分木に情報を追加
void addtree(MAP_DATA *Mdata, int child)
{
	int oya = child / 2;
	int64 chn, oyan;

	while (oya >= 1)
	{
		chn = calc(*Mdata, Mdata->value[child].x, Mdata->value[child].y);
		oyan = calc(*Mdata, Mdata->value[oya].x, Mdata->value[oya].y);

		if (chn < oyan) break;

		swap(&Mdata->value[child], &Mdata->value[oya]);

		child = oya;
		oya = child / 2;
	}
}

//------------------------------------------------------------

//マップセル情報を入れ替え
void swap(MAPCELL *a, MAPCELL *b)
{
	MAPCELL temp = *a;
	*a = *b;
	*b = temp;
}

//------------------------------------------------------------

#define TIMEOFGAME_H_DEFINED
#endif