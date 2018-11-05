#ifndef TIMEOFGAME_H_M_DEFINED

#include "TimeOfGame_Set.h"

//�Q�[�����Ԃ��X�V
void GameTime_Update(void)
{
	static double hasu = 0;
	double add1;

	add1 = 86400.0 / 1200.0 / 60.0;		//�P�Ăяo���łi�߂�b��
	hasu += add1 - floor(add1);			//�[���i�؂�̂Ă�ꂽ�]��j
	gametimer += (int64)add1;			//���Ԃ�i�߂�

	//�[����1�b�𒴂�����P�b�i�߂�
	if (hasu >= 1.0)
	{
		fprintf(Prog_Log_fp, "gametimer: Over 1 hasu value=%f\n", hasu);
		hasu -= 1.0;
		gametimer++;
	}
}

//�Q�[�����Ԃ��擾
void GetGameTime(GAMETIME* Gametime)
{
	int DayofGametime = gametimer % (3600 * 24);

	Gametime->hour = DayofGametime / 3600;
	Gametime->min = DayofGametime / 60 % 60;
	Gametime->sec = DayofGametime % 60;
}

//�Q�[�����Ԃɂ������F���擾
int GetGameTimeColor(GAMETIME Gametime)
{
	int col;

	switch (GetGameTimeArea(Gametime))
	{
		//�邩�璩�i5:00�`5:59�j�A�����琅�F�ɏ��X�ɕς��
		case GT_YORUtoASA:
			col = GetHSVColor(
																					//hsv�J���[�R�[�h�̏�ԑJ��
				180,																//hsv: h : 180 -> 180�i���̂܂܁j��̃J���[�R�[�h�́Ah��200�i�j�����A��v��0�i���j�Ȃ̂Ŋ֌W�Ȃ�
				160,																//hsv: s : 160 -> 160�i���̂܂܁j
				(int)((Gametime.min * 60 + Gametime.sec) / 3600.0*255.0)			//hsv: v :   0 -> 255
				);
			break;

		//���i6:00�`8:59�j�A���F
		case GT_ASA:
			col = GetHSVColor(180, 160, 255);
			break;

		//�����璋�i9:00�`9:59�j�A�P���Ԃ����Đ��F����ɏ��X�ɕς��
		case GT_ASAtoHIRU:
			col = GetHSVColor(
																					//hsv�J���[�R�[�h�̏�ԑJ��
				180 + (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*20.0),		//hsv: h : 180 -> 200
				160 + (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*40.0),		//hsv: s : 160 -> 200
				255																	//hsv: v : 255 -> 255�i���̂܂܁j
				);
			break;

		//���i10:00�`17:59�j�A��
		case GT_HIRU:
			col = GetHSVColor(200, 200, 255);
			break;

		//�������i18:00�`18:59�j�A���獕�ɏ��X�ɕς��
		case GT_HIRUtoYORU:
			col = GetHSVColor(
																					//hsv�J���[�R�[�h�̏�ԑJ��
				200,																//hsv: h : 200 -> 200�i���̂܂܁j
				200 - (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*40.0),		//hsv: s : 200 -> 160
				255 - (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*255.0)		//hsv: v : 255 ->   0
				);
			break;

		//��i19:00�`4:59�j�A��
		case GT_YORU:
			col = GetHSVColor(200, 160, 0);
			break;
	}

	return col;
}

//���ԑю擾
GTAREA GetGameTimeArea(GAMETIME Gametime)
{
	if (5 <= Gametime.hour && Gametime.hour < 6)			return GT_YORUtoASA;	//�邩�璩	�i5:00�`5:59�j
	else if (6 <= Gametime.hour && Gametime.hour < 9)		return GT_ASA;			//��		�i6:00�`8:59�j
	else if (9 <= Gametime.hour && Gametime.hour < 10)		return GT_ASAtoHIRU;	//�����璋	�i9:00�`9:59�j
	else if (10 <= Gametime.hour && Gametime.hour < 18)		return GT_HIRU;			//��		�i10:00�`17:59�j
	else if (18 <= Gametime.hour && Gametime.hour < 19)		return GT_HIRUtoYORU;	//�������	�i18:00�`18:59�j
	else													return GT_YORU;			//��		�i19:00�`4:59�j
}

#define TIMEOFGAME_H_M_DEFINED
#endif