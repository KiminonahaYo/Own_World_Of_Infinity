#ifndef KEYCONFIG_DEFINED
#define KEYCONFIG_DEFINED

#include "KeyConfig_Set.h"

void KeyConfig_Load(void);		//キーコンフィグ読み込み
void KeyConfig_Save(void);		//キーコンフィグ保存
void KeyConfig_sel(void);		//キーコンフィグ設定画面
int KeyKind_Len(void);
int ConfigKey_Len(void);

void KeyConfig_sel(void)
{
	int i, j;
	int selectkey_now = 0;

	SetMouseDispFlag(TRUE);

	while (ProcessMessage() == 0 && endflag == 0)
	{
		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		ClearDrawScreen();

		DrawBox(0, 0, DisplayX, DisplayY, GetHSVColor(160, 180, 100), TRUE);

		//入力
		if (Stick(KEYCONFIGMENU_KEY))
		{
			break;
		}
		else if (Stick(KEY_INPUT_UP, TRUE))
		{
			if (selectkey_now > 0) selectkey_now--;
		}
		else if (Stick(KEY_INPUT_DOWN, TRUE))
		{
			if (selectkey_now < KeyKind_Len() - 1) selectkey_now++;
		}
		else
		{
			int pushedkey = Stick_whatis_push();

			//いずれか押されたらそのキーを操作キーにする（設定可能キーであるか検索してから）
			if (pushedkey != -1)
			{
				//設定可能キーリストを検索
				for (j = 0; j < ConfigKey_Len(); j++)
				{
					if (ConfigKey[j].Key == pushedkey) break;
				}
				//設定可能キーであった場合設定
				if (ConfigKey[j].Guide[0] != '\0') *KeyKind[selectkey_now] = pushedkey;
			}
		}

		//出力
		for (i = 0; i < KeyKind_Len(); i++)
		{
			DrawBox(0, 0, DisplayX, 45, GetHSVColor(150, 180, 160), TRUE);
			DrawFormatString(10, 10, GetColor(0, 0, 0), "コンフィグキー設定　上下キーで選択、何かキーを押すとそのキーが反映されます。もう一度F1で決定");

			if (i == selectkey_now)
			{
				DrawBox(20, 50 + i * 25, 400, 50 + (i + 1) * 25 - 7, GetHSVColor(150, 200, 255), TRUE);
				DrawFormatString(20, 50 + i * 25, GetColor(0, 0, 0), "%s", KeyKind_Setsumei[i]);

				for (j = 0; j < ConfigKey_Len(); j++)
				{
					if (ConfigKey[j].Key == *KeyKind[i]) break;
				}
				if (ConfigKey[j].Guide[0] != '\0')
					DrawFormatString(350, 50 + i * 25, GetColor(0, 0, 0), "%s", ConfigKey[j].Guide);
			}
			else
			{
				DrawFormatString(20, 50 + i * 25, GetColor(255, 255, 255), "%s", KeyKind_Setsumei[i]);

				for (j = 0; j < ConfigKey_Len(); j++)
				{
					if (ConfigKey[j].Key == *KeyKind[i]) break;
				}
				if (ConfigKey[j].Guide[0] != '\0')
					DrawFormatString(350, 50 + i * 25, GetColor(255, 255, 255), "%s", ConfigKey[j].Guide);
			}
		}

		ScreenFlip();
	}
}

int KeyKind_Len(void)
{
	int i;

	for (i = 0; KeyKind[i] != NULL; i++);

	return i;
}

int ConfigKey_Len(void)
{
	int i;

	for (i = 0; ConfigKey[i].Guide[0]!='\0'; i++);

	return i;
}

//キーコンフィグ読み込み
void KeyConfig_Load(void)
{
	FILE *fp;

	fp = fopen("Keyconfig.dat", "rb");

	if (fp != NULL)
	{
		fread(&MAE_KEY, sizeof(int), 1, fp);
		fread(&USIRO_KEY, sizeof(int), 1, fp);
		fread(&LEFT_KEY, sizeof(int), 1, fp);
		fread(&RIGHT_KEY, sizeof(int), 1, fp);
		fread(&BLOCKMENU_KEY, sizeof(int), 1, fp);
		fread(&MOVEMODE_CHANGE_KEY, sizeof(int), 1, fp);
		fread(&JYOSYO_KEY, sizeof(int), 1, fp);
		fread(&KOUKA_KEY, sizeof(int), 1, fp);
		fread(&JUMP_KEY, sizeof(int), 1, fp);
		fread(&BLOCK_SET_MOUSE, sizeof(int), 1, fp);
		fread(&BLOCK_DEL_MOUSE, sizeof(int), 1, fp);
		fread(&TEMPLATE_HAICHI_KIRIKAE_KEY, sizeof(int), 1, fp);
		fread(&KEYCONFIGMENU_KEY, sizeof(int), 1, fp);
		fread(&TEMPLATEMENU_KEY, sizeof(int), 1, fp);

		fclose(fp);
	}
	else
	{
		MAE_KEY = KEY_INPUT_W;
		USIRO_KEY = KEY_INPUT_S;
		LEFT_KEY = KEY_INPUT_A;
		RIGHT_KEY = KEY_INPUT_D;
		BLOCKMENU_KEY = KEY_INPUT_E;
		MOVEMODE_CHANGE_KEY = KEY_INPUT_SPACE;
		JYOSYO_KEY = KEY_INPUT_Q;
		KOUKA_KEY = KEY_INPUT_Z;
		JUMP_KEY = KEY_INPUT_SPACE;
		BLOCK_SET_MOUSE = MOUSE_INPUT_RIGHT;
		BLOCK_DEL_MOUSE = MOUSE_INPUT_LEFT;
		TEMPLATE_HAICHI_KIRIKAE_KEY = KEY_INPUT_H;
		KEYCONFIGMENU_KEY = KEY_INPUT_F1;
		TEMPLATEMENU_KEY = KEY_INPUT_F2;
	}
}

//キーコンフィグ書き込み
void KeyConfig_Save(void)
{
	FILE *fp;

	fp = fopen("Keyconfig.dat", "wb");

	fwrite(&MAE_KEY, sizeof(int), 1, fp);
	fwrite(&USIRO_KEY, sizeof(int), 1, fp);
	fwrite(&LEFT_KEY, sizeof(int), 1, fp);
	fwrite(&RIGHT_KEY, sizeof(int), 1, fp);
	fwrite(&BLOCKMENU_KEY, sizeof(int), 1, fp);
	fwrite(&MOVEMODE_CHANGE_KEY, sizeof(int), 1, fp);
	fwrite(&JYOSYO_KEY, sizeof(int), 1, fp);
	fwrite(&KOUKA_KEY, sizeof(int), 1, fp);
	fwrite(&JUMP_KEY, sizeof(int), 1, fp);
	fwrite(&BLOCK_SET_MOUSE, sizeof(int), 1, fp);
	fwrite(&BLOCK_DEL_MOUSE, sizeof(int), 1, fp);
	fwrite(&TEMPLATE_HAICHI_KIRIKAE_KEY, sizeof(int), 1, fp);
	fwrite(&KEYCONFIGMENU_KEY, sizeof(int), 1, fp);
	fwrite(&TEMPLATEMENU_KEY, sizeof(int), 1, fp);

	fclose(fp);
}

#endif