#ifndef MAPMANAGER_SET_H_DEFINED

//------------------------------------------------------------

#include "TimeOfGame_Set.h"

//------------------------------------------------------------

//�}�b�v�Ǘ�
//���W���}�X���W�ɕϊ�
void	GetMasu2			(float, float, int*, int*);
void	GetMasu3			(float, float, float, int*, int*, int*);

//------------------------------------------------------------

//�u���b�N��z�u
void	ClickSetBlock		(MAP_DATA*, BINDATA, JIKI, CAMERA);
bool	FindViewBlockForSet	(MAP_DATA, CAMERA, JIKI, float, int*, int*, int*);

//�u���b�N���폜
void	ClickDelBlock		(MAP_DATA*, JIKI, CAMERA);
bool	FindViewBlockForDel	(MAP_DATA, CAMERA, JIKI, float, int*, int*, int*);

//------------------------------------------------------------

//------------------------------------------------------------

//���C���}�b�v�p

//�V�����}�b�v���쐬�A�g��
void	MakeMapCell			(MAP_MAIN*, VECTOR);
//�}�b�v�̃��[�h�A�Z�[�u
void	loadmap				(char *, MAP_MAIN*, JIKI*, CAMERA*);
void	savemap				(char *, MAP_MAIN, JIKI, CAMERA);
void	del_mapmain_array	(MAP_MAIN*);

//------------------------------------------------------------

//�e���v���[�g�p

//�V�����}�b�v���쐬�A�g��
void	MakeMapCell_Temp	(MAP_TEMPLATE*, VECTOR);

//�}�b�v�̃��[�h�A�Z�[�u
void	loadmap_Temp		(char *, MAP_TEMPLATE*, JIKI*, CAMERA*);
void	savemap_Temp		(char *, MAP_TEMPLATE,  JIKI, CAMERA);
void	loadmap_Temp_formain(char *, MAP_TEMPLATE*);

void	del_maptemp_array	(MAP_TEMPLATE*);

//�e���v���[�g�����C���}�b�v�ɔz�u
void	set_maptemp			(MAP_MAIN*, MAP_TEMPLATE, int, int, int, CAMERA);
//void	set_maptemp_yoyaku	(MAP_MAIN*, MAP_TEMPLATE, int);

void	Template_No_Load	(int *);
void	Template_No_Save	(int);

//�t�@�C���|�C���^�̃J�[�\�����w�肵���o�C�g���i�߂�A�߂�
void	fnext(FILE*, long);
void	fback(FILE*, long);

//------------------------------------------------------------

//�}�b�v�𐅕���x,y���̂P�̍��W�̃}�b�v�f�[�^256�������[�h�A�Z�[�u
void	loadcell			(FILE*, BINDATA*);
void	savecell			(FILE*, BINDATA*);

//------------------------------------------------------------

//�t�@�C���|�C���^�̃J�[�\�����}�b�v�f�[�^1���i�߂�A�߂�
void	mnext				(FILE*);
void	mback				(FILE*);

//char *filename = "Map.dat";

//------------------------------------------------------------

//�}�b�v�Z�������֘A

//�\�[�g�i�q�[�v�\�[�g�j
void	mapsort				(MAP_DATA*, int, int, bool);
void	buildtree			(MAP_DATA*, int, int);
void	addtree				(MAP_DATA*, int);


//------------------------------------------------------------

//�}�b�v�Z���f�[�^�̓���ւ�
void	swap				(MAPCELL*, MAPCELL*);

//------------------------------------------------------------

int edit_makemapcell_flag = 0;

#define MAPMANAGER_SET_H_DEFINED
#endif
