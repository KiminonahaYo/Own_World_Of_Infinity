//DxLib��p

#ifndef KEYCONTROL_DEFINED

#include "MyMath.h"

bool Stick(int, bool, int);			//�L�[�������ꂽ�u�Ԃɑ�����󂯕t����
bool MouseStick(int, bool, int);	//�}�E�X���N���b�N���ꂽ�u�Ԃɑ�����󂯕t����
int GetKey_whatis_push(void);
int Stick_whatis_push(void);
int Key_PressedCount(int);			//�������A�������u�Ԃɗ����Ă���/�����Ă������Ԃ��ǂꂭ�炢���Ԃ��֐�
int Mouse_PressedCount(int);		//�N���b�N�����A�������u�Ԃɗ����Ă���/�N���b�N���Ă������Ԃ��ǂꂭ�炢���Ԃ��֐�

int GetMouse(int);					//�}�E�X�N���b�N��Ԓ���

//�L�[���͊֐�

int GetKey_whatis_push(void)
{
	//�L�[���͏���ÓI�m��
	static char PushedKey[256];
	static int  PushCount[256];
	bool ret;

	int i;

	GetHitKeyStateAll(PushedKey);

	for (i = 0; i < 256; i++)
	{
		if (PushedKey[i] != 0) break;
	}
	if (i < 256)	return i;
	else			return -1;
}

int Stick_whatis_push(void)
{
	int pushedkey = GetKey_whatis_push();
	static char pushedflag = 1;
	int ret;

	if (pushedkey != -1)
	{
		if (pushedflag == 0)
		{
			pushedflag = 1;
			ret = pushedkey;
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		pushedflag = 0;
		ret = -1;
	}

	return ret;
}

//�L�[���������u�Ԃɑ�����󂯕t����
bool Stick(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	//�L�[���͏���ÓI�m��
	static char PushedKey[256];
	static int  PushCount[256];
	bool ret;

	//�L�[�������ꂽ��
	if (CheckHitKey(KeyCode))
	{
		PushCount[KeyCode]++;

		//�L�[���������u�Ԃ܂��͉������ςȂ��ő���ł��郂�[�h�̂Ƃ�
		if (PushedKey[KeyCode] == 0 || (ForSelectStickFlag && (PushCount[KeyCode] == 1 || PushCount[KeyCode] >= 30)))
		{
			//�L�[�����������ǂ����̃t���O�𗧂Ă�
			PushedKey[KeyCode] = 1;

			//�������ςȂ��ő���ł��郂�[�h�̂Ƃ�
			if (ForSelectStickFlag && PushCount[KeyCode] >= 30)
			{
				if (PushCount[KeyCode] % Select_kirikae_kankaku == 0)
					ret = TRUE;
				else
					ret = FALSE;
			}
			//�����ꂽ�u�Ԃ̂Ƃ�
			else
			{
				ret = TRUE;
			}
		}
		//�������u�ԈȊO�̂Ƃ�
		else
		{
			ret = FALSE;
		}
	}
	//�L�[��������Ă��Ȃ�������
	else
	{
		PushCount[KeyCode] = 0;
		PushedKey[KeyCode] = 0;
		ret = FALSE;
		//return FALSE;
	}
	return ret;
}

//�L�[�������ė����ꂽ/������ĉ����ꂽ�u�Ԃɉ�����/���������Ԃ�Ԃ��֐�
int Key_PressedCount(int KeyCode)
{
	//�L�[���͏���ÓI�m��
	static char PushFlag[256];
	static int  PushCount[256];
	int ret;

	//�����ꂽ��
	if (CheckHitKey(KeyCode))
	{
		//�����ꂽ�u�ԁA�����ꂽ���ԁi�}�C�i�X�j��Ԃ�
		if (PushFlag[KeyCode] == FALSE)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = TRUE;
			PushCount[KeyCode] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ăv���X�J�E���g
		else
		{
			ret = 0;
			PushFlag[KeyCode] = TRUE;
			PushCount[KeyCode]++;
		}
	}
	//�����ꂽ��
	else
	{
		//�����ꂽ�u�ԁA���������ԁi�v���X�j��Ԃ�
		if (PushFlag[KeyCode] == TRUE)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = FALSE;
			PushCount[KeyCode] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ă}�C�i�X�J�E���g
		else
		{
			ret = 0;
			PushFlag[KeyCode] = FALSE;
			PushCount[KeyCode]--;
		}
	}
	return ret;
}

//�}�E�X���N���b�N���ė�����/�����ăN���b�N�����u�ԂɃN���b�N����/���������Ԃ�Ԃ��֐�
int Mouse_PressedCount(int KeyCode)
{
	//�L�[���͏���ÓI�m��
	static char PushFlag[8];
	static int  PushCount[8];
	int ret;

	int Key = (int)(log((double)KeyCode) / log(2.0));

	//�N���b�N���ꂽ��
	if (GetMouse(KeyCode))
	{
		//�N���b�N���ꂽ�u�ԁA�����ꂽ���ԁi�}�C�i�X�j��Ԃ�
		if (PushFlag[Key] == FALSE)
		{
			ret = PushCount[Key];
			PushFlag[Key] = TRUE;
			PushCount[Key] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ăv���X�J�E���g
		else
		{
			ret = 0;
			PushFlag[Key] = TRUE;
			PushCount[Key]++;
		}
	}
	//�����ꂽ��
	else
	{
		//�����ꂽ�u�ԁA�N���b�N�������ԁi�v���X�j��Ԃ�
		if (PushFlag[Key] == TRUE)
		{
			ret = PushCount[Key];
			PushFlag[Key] = FALSE;
			PushCount[Key] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ă}�C�i�X�J�E���g
		else
		{
			ret = 0;
			PushFlag[Key] = FALSE;
			PushCount[Key]--;
		}
	}
	return ret;
}

//�}�E�X���N���b�N�����u�Ԃɑ�����󂯕t����
bool MouseStick(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	static char PushedKey[8];
	static int  PushCount[8];
	bool ret;

	int Key = (int)(log((double)KeyCode) / log(2.0));

	//�}�E�X���N���b�N����Ă���Ƃ�
	if (GetMouse(KeyCode))
	{
		PushCount[Key]++;

		//�}�E�X���N���b�N���ꂽ�u�Ԃ܂��̓}�E�X���N���b�N�����܂܂ł�����ł��郂�[�h�̂Ƃ�
		if (PushedKey[Key] == 0 || (ForSelectStickFlag && (PushCount[Key] == 1 || PushCount[Key] >= 30)))
		{
			PushedKey[Key] = 1;

			//�}�E�X���N���b�N�����܂܂ł�����ł��郂�[�h�̂Ƃ�
			if (ForSelectStickFlag && PushCount[Key] >= 30)
			{
				if (PushCount[Key] % Select_kirikae_kankaku == 0)
					ret = TRUE;
				else
					ret = FALSE;
			}
			//�}�E�X���N���b�N���ꂽ�u�Ԃ̂Ƃ�
			else
			{
				ret = TRUE;
			}
		}
		//�}�E�X���N���b�N���ꂽ�u�ԈȊO�̂Ƃ�
		else
		{
			ret = FALSE;
		}
	}
	//�}�E�X���N���b�N����Ă��Ȃ��Ƃ�
	else
	{
		PushCount[Key] = 0;
		PushedKey[Key] = 0;
		ret = FALSE;
	}

	return ret;
}

//�}�E�X�̃N���b�N��Ԃ𒲂ׂ�֐�
int GetMouse(int KeyCode)
{
	int InputState = GetMouseInput();
	int KeyFlag = InputState & KeyCode;

	return KeyFlag;
}

#define KEYCONTROL_DEFINED
#endif
