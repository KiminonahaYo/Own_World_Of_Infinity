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

//���@��J�����Ȃǂƃu���b�N�̓����蔻��
bool atari_hantei(MAP_DATA Mdata, VECTOR Object, float length)
{
	/*
	int i, j, k, Hdlp;			//�J�E���^�A���f���n���h���z��̓Y��
	int objmx, objmy, objmh;	//���@�Ɍ��炸�I�u�W�F�N�g�̃}�X���W

	static bool passed = FALSE;

	//�����蔻��

	MV1_COLL_RESULT_POLY_DIM blockatari;	//�����蔻��̂��߂̍\����
	bool flag = FALSE;						//�����蔻��̓�����t���O��FALSE�Ɖ��肷��

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
		//if�͊ȗ����̂��߂̏����A���@�ɑ΂��Đ����ʏ�̂�x,y��1�}�X�ȓ��̏ꍇ����
		if (
			objmx >= Mdata.value[i].x - 1 && Mdata.value[i].x + 1 >= objmx &&
			objmy >= Mdata.value[i].y - 1 && Mdata.value[i].y + 1 >= objmy
			)
		{
			for (j = 0; j < MAP_MAX; j++)
			{
				//���f���n���h���̔z��̓Y�������擾
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//�u���b�N����̏ꍇ
				if (Hdlp != 0)
				{
					//�����蔻��iif�͊ȗ����̂��߂̏����A���@�ɑ΂��č�����1�}�X�ȓ��̏ꍇ����j
					if (objmh >= j - MAP_ZERO - 1 && objmh <= j - MAP_ZERO + 1)
					{
						//�u���b�N�̍��W���Z�b�g
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
							

						//�J�v�Z���^�Ƃ��Ĕ���
						blockatari = MV1CollCheck_Capsule(
							Model[Hdlp].modelh,
							-1,
							VGet(Object.x, Object.y - length/2.0, Object.z),
							VGet(Object.x, Object.y + length/2.0, Object.z),
							18
							);

						//�����I�u�W�F�N�g�ƃu���b�N���������Ă�����
						if (blockatari.HitNum!=0)
						{
							flag = TRUE;
							goto hantei_brkpoint;
						}

						//�����蔻��̂��߂̍\���̂̌�n��
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
	int i, j, k, Hdlp;			//�J�E���^�A���f���n���h���z��̓Y��
	int objmx, objmy, objmh;	//���@�Ɍ��炸�I�u�W�F�N�g�̃}�X���W

	GetMasu3(Object.z, Object.x, Object.y, &objmx, &objmy, &objmh);

	for (i = 0; i < Mdata.youso; i++)
	{
		//if�͊ȗ����̂��߂̏����A���@�ɑ΂��Đ����ʏ�̂�x,y��1�}�X�ȓ��̏ꍇ����
		if (
			objmx >= Mdata.value[i].x - 1 && Mdata.value[i].x + 1 >= objmx &&
			objmy >= Mdata.value[i].y - 1 && Mdata.value[i].y + 1 >= objmy
			)
		{
			for (j = 0; j < MAP_MAX; j++)
			{
				//���f���n���h���̔z��̓Y�������擾
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//�u���b�N����̏ꍇ
				if (Hdlp != 0)
				{
					//�����蔻��iif�͊ȗ����̂��߂̏����A���@�ɑ΂��č�����1�}�X�ȓ��̏ꍇ����j
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