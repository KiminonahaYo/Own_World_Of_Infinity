//DxLib専用

#ifndef KEYCONTROL_DEFINED

#include "MyMath.h"

bool Stick(int, bool, int);			//キーが押された瞬間に操作を受け付ける
bool MouseStick(int, bool, int);	//マウスがクリックされた瞬間に操作を受け付ける
int GetKey_whatis_push(void);
int Stick_whatis_push(void);
int Key_PressedCount(int);			//押した、離した瞬間に離していた/押していた時間がどれくらいか返す関数
int Mouse_PressedCount(int);		//クリックした、離した瞬間に離していた/クリックしていた時間がどれくらいか返す関数

int GetMouse(int);					//マウスクリック状態調査

//キー入力関数

int GetKey_whatis_push(void)
{
	//キー入力情報を静的確保
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

//キーを押した瞬間に操作を受け付ける
bool Stick(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	//キー入力情報を静的確保
	static char PushedKey[256];
	static int  PushCount[256];
	bool ret;

	//キーが押されたら
	if (CheckHitKey(KeyCode))
	{
		PushCount[KeyCode]++;

		//キーを押した瞬間または押しっぱなしで操作できるモードのとき
		if (PushedKey[KeyCode] == 0 || (ForSelectStickFlag && (PushCount[KeyCode] == 1 || PushCount[KeyCode] >= 30)))
		{
			//キーを押したかどうかのフラグを立てる
			PushedKey[KeyCode] = 1;

			//押しっぱなしで操作できるモードのとき
			if (ForSelectStickFlag && PushCount[KeyCode] >= 30)
			{
				if (PushCount[KeyCode] % Select_kirikae_kankaku == 0)
					ret = TRUE;
				else
					ret = FALSE;
			}
			//押された瞬間のとき
			else
			{
				ret = TRUE;
			}
		}
		//押した瞬間以外のとき
		else
		{
			ret = FALSE;
		}
	}
	//キーが押されていなかったら
	else
	{
		PushCount[KeyCode] = 0;
		PushedKey[KeyCode] = 0;
		ret = FALSE;
		//return FALSE;
	}
	return ret;
}

//キーを押して離された/離されて押された瞬間に押した/離した時間を返す関数
int Key_PressedCount(int KeyCode)
{
	//キー入力情報を静的確保
	static char PushFlag[256];
	static int  PushCount[256];
	int ret;

	//押されたら
	if (CheckHitKey(KeyCode))
	{
		//押された瞬間、離された時間（マイナス）を返す
		if (PushFlag[KeyCode] == FALSE)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = TRUE;
			PushCount[KeyCode] = 0;
		}
		//それ以外はダミーを返してプラスカウント
		else
		{
			ret = 0;
			PushFlag[KeyCode] = TRUE;
			PushCount[KeyCode]++;
		}
	}
	//離されたら
	else
	{
		//離された瞬間、押した時間（プラス）を返す
		if (PushFlag[KeyCode] == TRUE)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = FALSE;
			PushCount[KeyCode] = 0;
		}
		//それ以外はダミーを返してマイナスカウント
		else
		{
			ret = 0;
			PushFlag[KeyCode] = FALSE;
			PushCount[KeyCode]--;
		}
	}
	return ret;
}

//マウスをクリックして離した/離してクリックした瞬間にクリックした/離した時間を返す関数
int Mouse_PressedCount(int KeyCode)
{
	//キー入力情報を静的確保
	static char PushFlag[8];
	static int  PushCount[8];
	int ret;

	int Key = (int)(log((double)KeyCode) / log(2.0));

	//クリックされたら
	if (GetMouse(KeyCode))
	{
		//クリックされた瞬間、離された時間（マイナス）を返す
		if (PushFlag[Key] == FALSE)
		{
			ret = PushCount[Key];
			PushFlag[Key] = TRUE;
			PushCount[Key] = 0;
		}
		//それ以外はダミーを返してプラスカウント
		else
		{
			ret = 0;
			PushFlag[Key] = TRUE;
			PushCount[Key]++;
		}
	}
	//離されたら
	else
	{
		//離された瞬間、クリックした時間（プラス）を返す
		if (PushFlag[Key] == TRUE)
		{
			ret = PushCount[Key];
			PushFlag[Key] = FALSE;
			PushCount[Key] = 0;
		}
		//それ以外はダミーを返してマイナスカウント
		else
		{
			ret = 0;
			PushFlag[Key] = FALSE;
			PushCount[Key]--;
		}
	}
	return ret;
}

//マウスをクリックした瞬間に操作を受け付ける
bool MouseStick(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	static char PushedKey[8];
	static int  PushCount[8];
	bool ret;

	int Key = (int)(log((double)KeyCode) / log(2.0));

	//マウスがクリックされているとき
	if (GetMouse(KeyCode))
	{
		PushCount[Key]++;

		//マウスがクリックされた瞬間またはマウスをクリックしたままでも操作できるモードのとき
		if (PushedKey[Key] == 0 || (ForSelectStickFlag && (PushCount[Key] == 1 || PushCount[Key] >= 30)))
		{
			PushedKey[Key] = 1;

			//マウスをクリックしたままでも操作できるモードのとき
			if (ForSelectStickFlag && PushCount[Key] >= 30)
			{
				if (PushCount[Key] % Select_kirikae_kankaku == 0)
					ret = TRUE;
				else
					ret = FALSE;
			}
			//マウスをクリックされた瞬間のとき
			else
			{
				ret = TRUE;
			}
		}
		//マウスをクリックされた瞬間以外のとき
		else
		{
			ret = FALSE;
		}
	}
	//マウスがクリックされていないとき
	else
	{
		PushCount[Key] = 0;
		PushedKey[Key] = 0;
		ret = FALSE;
	}

	return ret;
}

//マウスのクリック状態を調べる関数
int GetMouse(int KeyCode)
{
	int InputState = GetMouseInput();
	int KeyFlag = InputState & KeyCode;

	return KeyFlag;
}

#define KEYCONTROL_DEFINED
#endif
