#ifndef TIMEOFGAME_SET_H_DEFINED
#define TIMEOFGAME_SET_H_DEFINED

//時間帯定義
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

//時間が0:00:00から何秒経ったかを記録
int64 gametimer;

void	GameTime_Update		(void);			//ゲーム時間を更新（進める）
void	GetGameTime			(GAMETIME*);	//ゲーム時間を取得
int		GetGameTimeColor	(GAMETIME);		//ゲーム時間に合った色を取得
GTAREA	GetGameTimeArea		(GAMETIME);		//ゲーム時間の時間帯を取得

#endif