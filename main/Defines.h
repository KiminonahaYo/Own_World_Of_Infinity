#ifndef DEFINES_DEFINED

//------------------------------------------------------------

//�킩��₷���悤�Ɍ^��`
typedef unsigned char	BINDATA;	//�}�b�v�f�[�^1��
typedef long long int	int64;		//64�r�b�g�����^
typedef int				HANDLER;	//DxLib�n���h��

//���@�\����
typedef struct
{
	VECTOR pos;
	bool CanFlySkyFlag;
}
JIKI;

//�J�����\����
typedef struct
{
	VECTOR pos;
	float kyori;
	float r1, r2;
}
CAMERA;

//3D���Wxyz�\����
typedef struct
{
	int x, y, z;
}
POS_3D;

//3D���Wxyh�\����
typedef struct
{
	int x, y, h;
}
POS_3D_H;

//�e���v���[�g�z�u�ꏊ�\��p�\����
typedef struct
{
	POS_3D_H *value;
	int *kaku;
	int *no;

	int youso;
}
YOYAKU;

//�}�b�v������xy���W���1�}�X�i�}�b�v�Z���j�̃f�[�^�i�����̕����j
//x, y���W��-128 �`127�܂ł̃f�[�^
typedef struct
{
	int x, y;
	BINDATA *data;
}
MAPCELL;

//�}�b�v�̃f�[�^
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

//��ƂȂ�}�b�v�̃f�[�^�Z�b�g
typedef struct
{
	MAP_DATA Data;

	YOYAKU Yoyaku;
	YOYAKU Area_Yoyaku;
}
MAP_MAIN;

//�e���v���[�g�p�̃}�b�v�̃f�[�^�Z�b�g
typedef struct
{
	MAP_DATA Data;

	//int no;
}
MAP_TEMPLATE;

//���f���n���h���\����
typedef struct
{
	HANDLER modelh;
	HANDLER thumbnail;
	char *name;
}
MODEL_HANDLER;

//�Q�[�����ԍ\����
typedef struct
{
	int hour, min, sec;
}
GAMETIME;

MODEL_HANDLER Model[256];		//���f���n���h���@256�܂�

//�ėp���O�t�@�C���̃t�@�C���|�C���^
FILE *Prog_Log_fp = fopen("Micralike_Log.log", "w");

//�e���`------------------------------------------------------------

#define B_NUM_START		(BINDATA)0x2		//�}�b�v�f�[�^�l�g�p�J�n�l
#define B_ZIP_CODE		(BINDATA)0x1		//�}�b�v���k�R�[�h
#define B_ERROR			(BINDATA)0x0		//�}�b�v�f�[�^�֘A�ėp�G���[�R�[�h

#define MAP_ZERO		0x80				//�}�b�v��ō��W0�ɂȂ�Ƃ���̔z��̓Y���i128�j
#define MAP_MAX			0x100				//�}�b�v�̍����̃T�C�Y�i256���A-128�`127�j

#define MASU_1			50.0f				//�}�X1���̑傫��
#define THUMB_SIZE		100.0				//�T���l�C��1���̑傫��

#define MAP_NULL		(B_NUM_START)		//�����u���Ă��Ȃ��}�X
#define SOUGEN			(B_NUM_START+1)		//����
#define DIRT			(B_NUM_START+2)		//�y
#define STONE			(B_NUM_START+3)		//��

#define STR_COLOR		0xFFFFFF

//------------------------------------------------------------

//�L�[�̐ݒ�
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

//�I���t���O
int endflag = 0;
//------------------------------------------------------------

#define DEFINES_DEFINED
#endif