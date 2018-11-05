#ifndef DEFINES_DEFINED

//------------------------------------------------------------

//わかりやすいように型定義
typedef unsigned char	BINDATA;	//マップデータ1つ
typedef long long int	int64;		//64ビット整数型
typedef int				HANDLER;	//DxLibハンドラ

//自機構造体
typedef struct
{
	VECTOR pos;
	bool CanFlySkyFlag;
}
JIKI;

//カメラ構造体
typedef struct
{
	VECTOR pos;
	float kyori;
	float r1, r2;
}
CAMERA;

//3D座標xyz構造体
typedef struct
{
	int x, y, z;
}
POS_3D;

//3D座標xyh構造体
typedef struct
{
	int x, y, h;
}
POS_3D_H;

//テンプレート配置場所予約用構造体
typedef struct
{
	POS_3D_H *value;
	int *kaku;
	int *no;

	int youso;
}
YOYAKU;

//マップ水平面xy座標上の1マス（マップセル）のデータ（高さの部分）
//x, y座標と-128 ～127までのデータ
typedef struct
{
	int x, y;
	BINDATA *data;
}
MAPCELL;

//マップのデータ
typedef struct
{
	MAPCELL *value;
	int youso;

	int pxmin, pxmax;
	int pymin, pymax;

	POS_3D_H BlockMin;
	POS_3D_H BlockMax;
}
MAP_DATA;

//主となるマップのデータセット
typedef struct
{
	MAP_DATA Data;

	YOYAKU Yoyaku;
	YOYAKU Area_Yoyaku;
}
MAP_MAIN;

//テンプレート用のマップのデータセット
typedef struct
{
	MAP_DATA Data;

	//int no;
}
MAP_TEMPLATE;

//モデルハンドラ構造体
typedef struct
{
	HANDLER modelh;
	HANDLER thumbnail;
	char *name;
}
MODEL_HANDLER;

//ゲーム時間構造体
typedef struct
{
	int hour, min, sec;
}
GAMETIME;

MODEL_HANDLER Model[256];		//モデルハンドラ　256個まで

//汎用ログファイルのファイルポインタ
FILE *Prog_Log_fp = fopen("Micralike_Log.log", "w");

//各種定義------------------------------------------------------------

#define B_NUM_START		(BINDATA)0x2		//マップデータ値使用開始値
#define B_ZIP_CODE		(BINDATA)0x1		//マップ圧縮コード
#define B_ERROR			(BINDATA)0x0		//マップデータ関連汎用エラーコード

#define MAP_ZERO		0x80				//マップ上で座標0になるところの配列の添字（128）
#define MAP_MAX			0x100				//マップの高さのサイズ（256個分、-128～127）

#define MASU_1			50.0f				//マス1つ分の大きさ
#define THUMB_SIZE		100.0				//サムネイル1つ分の大きさ

#define MAP_NULL		(B_NUM_START)		//何も置いていないマス
#define SOUGEN			(B_NUM_START+1)		//草原
#define DIRT			(B_NUM_START+2)		//土
#define STONE			(B_NUM_START+3)		//石

#define STR_COLOR		0xFFFFFF

//------------------------------------------------------------

//キーの設定
int MAE_KEY;
int USIRO_KEY;
int LEFT_KEY;
int RIGHT_KEY;
int BLOCKMENU_KEY;
int MOVEMODE_CHANGE_KEY;
int JYOSYO_KEY;
int KOUKA_KEY;
int JUMP_KEY;
int BLOCK_SET_MOUSE;
int BLOCK_DEL_MOUSE;
int KEYCONFIGMENU_KEY;
int TEMPLATEMENU_KEY;
int TEMPLATE_HAICHI_KIRIKAE_KEY;

//終了フラグ
int endflag = 0;
//------------------------------------------------------------

#define DEFINES_DEFINED
#endif