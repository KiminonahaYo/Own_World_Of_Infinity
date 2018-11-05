#ifndef TIMEOFGAME_H_M_DEFINED

#include "TimeOfGame_Set.h"

//ゲーム時間を更新
void GameTime_Update(void)
{
	static double hasu = 0;
	double add1;

	add1 = 86400.0 / 1200.0 / 60.0;		//１呼び出しでつ進める秒数
	hasu += add1 - floor(add1);			//端数（切り捨てられた余り）
	gametimer += (int64)add1;			//時間を進める

	//端数が1秒を超えたら１秒進める
	if (hasu >= 1.0)
	{
		fprintf(Prog_Log_fp, "gametimer: Over 1 hasu value=%f\n", hasu);
		hasu -= 1.0;
		gametimer++;
	}
}

//ゲーム時間を取得
void GetGameTime(GAMETIME* Gametime)
{
	int DayofGametime = gametimer % (3600 * 24);

	Gametime->hour = DayofGametime / 3600;
	Gametime->min = DayofGametime / 60 % 60;
	Gametime->sec = DayofGametime % 60;
}

//ゲーム時間にあった色を取得
int GetGameTimeColor(GAMETIME Gametime)
{
	int col;

	switch (GetGameTimeArea(Gametime))
	{
		//夜から朝（5:00～5:59）、黒から水色に徐々に変わる
		case GT_YORUtoASA:
			col = GetHSVColor(
																					//hsvカラーコードの状態遷移
				180,																//hsv: h : 180 -> 180（そのまま）夜のカラーコードは、hが200（青）だが、はvが0（黒）なので関係ない
				160,																//hsv: s : 160 -> 160（そのまま）
				(int)((Gametime.min * 60 + Gametime.sec) / 3600.0*255.0)			//hsv: v :   0 -> 255
				);
			break;

		//朝（6:00～8:59）、水色
		case GT_ASA:
			col = GetHSVColor(180, 160, 255);
			break;

		//朝から昼（9:00～9:59）、１時間かけて水色から青に徐々に変わる
		case GT_ASAtoHIRU:
			col = GetHSVColor(
																					//hsvカラーコードの状態遷移
				180 + (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*20.0),		//hsv: h : 180 -> 200
				160 + (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*40.0),		//hsv: s : 160 -> 200
				255																	//hsv: v : 255 -> 255（そのまま）
				);
			break;

		//昼（10:00～17:59）、青
		case GT_HIRU:
			col = GetHSVColor(200, 200, 255);
			break;

		//昼から夜（18:00～18:59）、青から黒に徐々に変わる
		case GT_HIRUtoYORU:
			col = GetHSVColor(
																					//hsvカラーコードの状態遷移
				200,																//hsv: h : 200 -> 200（そのまま）
				200 - (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*40.0),		//hsv: s : 200 -> 160
				255 - (int)((Gametime.min * 60 + Gametime.sec) / 3600.0*255.0)		//hsv: v : 255 ->   0
				);
			break;

		//夜（19:00～4:59）、黒
		case GT_YORU:
			col = GetHSVColor(200, 160, 0);
			break;
	}

	return col;
}

//時間帯取得
GTAREA GetGameTimeArea(GAMETIME Gametime)
{
	if (5 <= Gametime.hour && Gametime.hour < 6)			return GT_YORUtoASA;	//夜から朝	（5:00～5:59）
	else if (6 <= Gametime.hour && Gametime.hour < 9)		return GT_ASA;			//朝		（6:00～8:59）
	else if (9 <= Gametime.hour && Gametime.hour < 10)		return GT_ASAtoHIRU;	//朝から昼	（9:00～9:59）
	else if (10 <= Gametime.hour && Gametime.hour < 18)		return GT_HIRU;			//昼		（10:00～17:59）
	else if (18 <= Gametime.hour && Gametime.hour < 19)		return GT_HIRUtoYORU;	//昼から夜	（18:00～18:59）
	else													return GT_YORU;			//夜		（19:00～4:59）
}

#define TIMEOFGAME_H_M_DEFINED
#endif