#ifndef TIMEOFGAME_SET_H_DEFINED
#define TIMEOFGAME_SET_H_DEFINED

//���ԑђ�`
typedef enum
{
	GT_YORUtoASA,
	GT_ASA,
	GT_ASAtoHIRU,
	GT_HIRU,
	GT_HIRUtoYORU,
	GT_YUU,
	GT_YORU
}
GTAREA;

//���Ԃ�0:00:00���牽�b�o���������L�^
int64 gametimer;

void	GameTime_Update		(void);			//�Q�[�����Ԃ��X�V�i�i�߂�j
void	GetGameTime			(GAMETIME*);	//�Q�[�����Ԃ��擾
int		GetGameTimeColor	(GAMETIME);		//�Q�[�����Ԃɍ������F���擾
GTAREA	GetGameTimeArea		(GAMETIME);		//�Q�[�����Ԃ̎��ԑт��擾

#endif