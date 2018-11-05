#ifndef MAPMANAGER_SET_H_DEFINED

//------------------------------------------------------------

#include "TimeOfGame_Set.h"

//------------------------------------------------------------

//マップ管理
//座標をマス座標に変換
void	GetMasu2			(float, float, int*, int*);
void	GetMasu3			(float, float, float, int*, int*, int*);

//------------------------------------------------------------

//ブロックを配置
void	ClickSetBlock		(MAP_DATA*, BINDATA, JIKI, CAMERA);
bool	FindViewBlockForSet	(MAP_DATA, CAMERA, JIKI, float, int*, int*, int*);

//ブロックを削除
void	ClickDelBlock		(MAP_DATA*, JIKI, CAMERA);
bool	FindViewBlockForDel	(MAP_DATA, CAMERA, JIKI, float, int*, int*, int*);

//------------------------------------------------------------

//------------------------------------------------------------

//メインマップ用

//新しいマップを作成、拡張
void	MakeMapCell			(MAP_MAIN*, VECTOR);
//マップのロード、セーブ
void	loadmap				(char *, MAP_MAIN*, JIKI*, CAMERA*);
void	savemap				(char *, MAP_MAIN, JIKI, CAMERA);
void	del_mapmain_array	(MAP_MAIN*);

//------------------------------------------------------------

//テンプレート用

//新しいマップを作成、拡張
void	MakeMapCell_Temp	(MAP_TEMPLATE*, VECTOR);

//マップのロード、セーブ
void	loadmap_Temp		(char *, MAP_TEMPLATE*, JIKI*, CAMERA*);
void	savemap_Temp		(char *, MAP_TEMPLATE,  JIKI, CAMERA);
void	loadmap_Temp_formain(char *, MAP_TEMPLATE*);

void	del_maptemp_array	(MAP_TEMPLATE*);

//テンプレートをメインマップに配置
void	set_maptemp			(MAP_MAIN*, MAP_TEMPLATE, int, int, int, CAMERA);
//void	set_maptemp_yoyaku	(MAP_MAIN*, MAP_TEMPLATE, int);

void	Template_No_Load	(int *);
void	Template_No_Save	(int);

//ファイルポインタのカーソルを指定したバイト分進める、戻す
void	fnext(FILE*, long);
void	fback(FILE*, long);

//------------------------------------------------------------

//マップを水平面x,y軸の１つの座標のマップデータ256個分をロード、セーブ
void	loadcell			(FILE*, BINDATA*);
void	savecell			(FILE*, BINDATA*);

//------------------------------------------------------------

//ファイルポインタのカーソルをマップデータ1つ分進める、戻す
void	mnext				(FILE*);
void	mback				(FILE*);

//char *filename = "Map.dat";

//------------------------------------------------------------

//マップセル検索関連

//ソート（ヒープソート）
void	mapsort				(MAP_DATA*, int, int, bool);
void	buildtree			(MAP_DATA*, int, int);
void	addtree				(MAP_DATA*, int);


//------------------------------------------------------------

//マップセルデータの入れ替え
void	swap				(MAPCELL*, MAPCELL*);

//------------------------------------------------------------

int edit_makemapcell_flag = 0;

#define MAPMANAGER_SET_H_DEFINED
#endif
