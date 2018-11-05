#ifndef KEYCONFIG_SET_DEFINED

#define KEYCONFIG_SET_DEFINED

typedef struct
{
	int Key;
	char *Guide;
}
CONFIGKEY;

char *KeyKind_Setsumei[] =
{
	"↑キー",
	"↓キー",
	"←キー",
	"→キー",
	"ブロック選択画面表示",
	"重力/空飛び移動切り替え",
	"空飛び移動上昇キー",
	"空飛び移動降下キー",
	"重力移動ジャンプキー",
	"テンプレート配置モードON/OFFキー"
	""								//終わりを示す
};

int *KeyKind[] =
{
	&MAE_KEY,
	&USIRO_KEY,
	&LEFT_KEY,
	&RIGHT_KEY,
	&BLOCKMENU_KEY,
	&MOVEMODE_CHANGE_KEY,
	&JYOSYO_KEY,
	&KOUKA_KEY,
	&JUMP_KEY,
	&TEMPLATE_HAICHI_KIRIKAE_KEY,
	NULL
};

CONFIGKEY ConfigKey[] =
{
	//各種キー
	{ KEY_INPUT_A, "A" },
	{ KEY_INPUT_B, "B" },
	{ KEY_INPUT_C, "C" },
	{ KEY_INPUT_D, "D" },
	{ KEY_INPUT_E, "E" },
	{ KEY_INPUT_F, "F" },
	{ KEY_INPUT_G, "G" },
	{ KEY_INPUT_H, "H" },
	{ KEY_INPUT_I, "I" },
	{ KEY_INPUT_J, "J" },
	{ KEY_INPUT_K, "K" },
	{ KEY_INPUT_L, "L" },
	{ KEY_INPUT_M, "M" },
	{ KEY_INPUT_N, "N" },
	{ KEY_INPUT_O, "O" },
	{ KEY_INPUT_P, "P" },
	{ KEY_INPUT_Q, "Q" },
	{ KEY_INPUT_R, "R" },
	{ KEY_INPUT_S, "S" },
	{ KEY_INPUT_T, "T" },
	{ KEY_INPUT_U, "U" },
	{ KEY_INPUT_V, "V" },
	{ KEY_INPUT_W, "W" },
	{ KEY_INPUT_X, "X" },
	{ KEY_INPUT_Y, "Y" },
	{ KEY_INPUT_Z, "Z" },
	{ KEY_INPUT_SPACE, "Space" },
	{ KEY_INPUT_LSHIFT, "LShift" },
	{ KEY_INPUT_RSHIFT, "RShift" },
	{ KEY_INPUT_F3, "F3" },
	{ KEY_INPUT_F4, "F4" },
	{ KEY_INPUT_F5, "F5" },
	{ KEY_INPUT_F6, "F6" },
	{ KEY_INPUT_F7, "F7" },
	{ KEY_INPUT_F8, "F8" },
	{ KEY_INPUT_F9, "F9" },
	{ KEY_INPUT_F10, "F10" },
	{ KEY_INPUT_F11, "F11" },
	{ KEY_INPUT_F12, "F12" },

	//終わりを示すデータ
	{ 0, "" },
};

#endif