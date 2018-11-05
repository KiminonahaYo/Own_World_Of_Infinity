#ifndef MICRALIKE_SET_H_DEFINED

void Init(void);
void LoadModel(void);

int DisplayX = int(1920/1.5), DisplayY = int(1080/1.5);

#include "Defines.h"
#include "UsefulHeaders/useful.h"

#include "BlockGetAndSet.h"

#include "KeyConfig.h"

#include "Masu.h"
#include "Atari_Hantei.h"

#include "mapmgr.h"
#include "TimeOfGame.h"

//�J�����E���@�֘A
void SetCam			(JIKI, CAMERA);
void MoveCam		(CAMERA*, JIKI);
void MoveJiki		(MAP_DATA, JIKI*, CAMERA);

//�o�͊֘A
void DispMap		(MAP_DATA, VECTOR);
void DispDebug		(MAP_DATA, JIKI, CAMERA, GAMETIME);
void DispJiku		(void);
void sortlog		(MAP_DATA*);
void datalog		(MAP_DATA);

//�u���b�N�I���֘A
int modellen		(void);
void selmodel		(int *);
void drawthumb		(int, int, int);

//�e���v���[�g�֘A
void Template_Edit(void);				//�e���v���[�g�ҏW
void Template_Sel(char, char[]);		//�e���v���[�g�I�����

void start_mode(MAP_DATA);
void DispMapForStart(MAP_DATA);

//���O���͉��
void InputName(char[], char*);

int GetFPS(void);

int L1;

//bool atariflag[6] = {FALSE};

#define MICRALIKE_SET_H_DEFINED
#endif