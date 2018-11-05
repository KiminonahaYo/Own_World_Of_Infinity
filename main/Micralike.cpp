//ヘッダーファイル読み込み
#include "DxLib.h"

#include "Micralike_set.h"

//コンパイル時の警告表示抑制
#pragma warning( disable:4305 4244 4305 )

//メイン関数
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	//DxLib,変数の初期設定
	Init();

	//汎用変数
	int i,j;

	//マップデータ、マップ配列要素数などのマップ関連のデータ
	MAP_MAIN Map;

	//自機、カメラ
	JIKI   Jiki;
	CAMERA Cam;
	//int JikiP2x, JikiP2y;
	//int OldJikiP2x, OldJikiP2y;

	//モデル番号
	int modelno = 1;

	//ゲーム内の仮想時間
	GAMETIME Gametime;

	//テンプレートファイル一覧取得変数
	SPLITDATA spdata;
	char filelist[2048];
	int spn;

	//テンプレート関連

	//テンプレート配置フラグ
	bool tempsetflag = FALSE;

	//配置するテンプレートファイル名とテンプレートマップデータ
	char tempfilename[256] = "";
	MAP_TEMPLATE SetMapT;

	//テンプレートマップデータ
	MAP_TEMPLATE *MapT;

	/*
	//テンプレートファイル一覧取得、読み込み
	dirlist(filelist, ".\\", "*.tempmap");
	spn = split(filelist, "\n", &spdata);
	MapT = (MAP_TEMPLATE*)malloc(sizeof(MAP_TEMPLATE)*spn);
	for (i = 0; i < spn; i++)
	{
		loadmap_Temp_formain(spdata[i], &MapT[i]);
	}
	*/

	//マップデータをロード
	loadmap("Map.dat", &Map, &Jiki, &Cam);

	//キーコンフィグ読み込み
	KeyConfig_Load();

	//マップがなかったら新しいマップを作成
	if (Map.Data.youso == 0)
	{
		for (i = 0; i < 1000; i++)
			MakeMapCell(&Map, Jiki.pos);
	}

	//最初の座標をマス座標に変換
	//GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

	start_mode(Map.Data);

	//メインループ
	while (ProcessMessage() == 0 && endflag == 0)
	{
		ClsDrawScreen();		//描画する画面（裏）を黒クリア

		GetGameTime(&Gametime);	//ゲーム時間を取得

		//入力、処理---------------------------------------------------------------------

		Stick_whatis_push();

		//Eキーでモデル番号を選択する画面にする
		if (Stick(BLOCKMENU_KEY))
			selmodel(&modelno);

		//キーコンフィグメニュー画面
		if (Stick(KEYCONFIGMENU_KEY))
			KeyConfig_sel();

		if (Stick(TEMPLATE_HAICHI_KIRIKAE_KEY))
			tempsetflag = !tempsetflag;

		if (Stick(TEMPLATEMENU_KEY))
		{
			if (tempsetflag)
			{
				Template_Sel('S', tempfilename);
				loadmap_Temp_formain(tempfilename, &SetMapT);
			}
			else
			{
				Template_Sel('E', tempfilename);
			}

			/*
			delsplit(spn, spdata);

			//テンプレートファイル一覧取得、読み込み
			dirlist(filelist, ".\\", "*.tempmap");
			spn = split(filelist, "\n", &spdata);
			MapT = (MAP_TEMPLATE*)malloc(sizeof(MAP_TEMPLATE)*spn);
			for (i = 0; i < spn; i++)
			{
				loadmap_Temp_formain(spdata[i], &MapT[i]);
			}
			*/
		}

		//右クリックでブロックを置く
		if (MouseStick(BLOCK_SET_MOUSE, TRUE, 8))
		{
			if (tempsetflag)
			{
				if (strcmp(tempfilename, "") != 0)
				{
					int xm, ym, hm;
					FindViewBlockForSet(Map.Data, Cam, Jiki, Cam.kyori, &xm, &ym, &hm);
					set_maptemp(&Map, SetMapT, xm, ym, hm, Cam);
				}
			}
			else
			{
				ClickSetBlock(&Map.Data, modelno + B_NUM_START, Jiki, Cam);
			}
		}
		
		//左クリックでブロックを消す
		if (MouseStick(BLOCK_DEL_MOUSE, TRUE, 8))
			ClickDelBlock(&Map.Data, Jiki, Cam);

		//自機、カメラ座標を移動、カメラをセット
		MoveJiki(Map.Data, &Jiki, Cam);
		MoveCam(&Cam, Jiki);
		SetCam(Jiki, Cam);

		//前のマス座標を記憶、今のマス座標を取得
		//OldJikiP2x = JikiP2x;
		//OldJikiP2y = JikiP2y;
		//GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

		//前回のマスと今のマスが違ったらマップを生成
		//if (JikiP2x != OldJikiP2x || JikiP2y != OldJikiP2y)

		MakeMapCell(&Map, Jiki.pos);

		//出力---------------------------------------------------------------------------

		//背景（空）を描画
		DrawBox(0, 0, DisplayX, DisplayY, GetGameTimeColor(Gametime), TRUE);

		//ブロックを描画
		DispMap(Map.Data, Jiki.pos);

		//現在選択中のブロック（サムネイル）と説明を左下に表示
		DrawBox(15, DisplayY - 170, 145, DisplayY - 10, GetHSVColor(0, 0, 180), TRUE);
		drawthumb(30, DisplayY - 135, modelno);
		DrawFormatString(30, DisplayY - 160, STR_COLOR, "選択中のもの：");
		DrawFormatString(30, DisplayY - 30, STR_COLOR, "%s", Model[modelno].name);

		DrawFormatString(10, 10, STR_COLOR, "モード：%s, Hキーで切り替え", tempsetflag ? "テンプレート配置モード" : "ブロック配置モード");
		DrawFormatString(DisplayX - 50, DisplayY - 20, STR_COLOR, "%dFPS", GetFPS());

		DrawLine(DisplayX / 2 - 50, DisplayY / 2, DisplayX / 2 + 50, DisplayY / 2, GetColor(255, 255, 255));
		DrawLine(DisplayX / 2, DisplayY / 2 - 50, DisplayX / 2, DisplayY / 2 + 50, GetColor(255, 255, 255));

		if (strcmp(tempfilename, "") != 0 && tempsetflag)
		{
			DrawFormatString(10, 40, STR_COLOR, "読み込んだテンプレートファイル：%s", tempfilename);
		}

		//デバッグ関係
		clsDx();
		//DispDebug(Map.Data, Jiki, Cam, Gametime);
		//DispJiku();

		//ゲーム時間を更新
		GameTime_Update();

		ScreenFlip();
	}
	//メインループ終了

	//マップデータ、キーコンフィグをセーブ、ログを保存
	//sortlog(&Map.Data);
	savemap("Map.dat", Map, Jiki, Cam);
	KeyConfig_Save();
	//datalog(Map.Data);

	del_mapmain_array(&Map);

	fclose(Prog_Log_fp);

	DxLib_End();
	return 0;
}

//スタート画面
void start_mode(MAP_DATA Mdata)
{
	float rot = 0.0;
	float posx = (Mdata.pxmax + Mdata.pxmin) / 2 * MASU_1;
	float posy = (Mdata.pymax + Mdata.pymin) / 2 * MASU_1;
	float hankei;

	if (Mdata.pxmax - Mdata.pxmin < Mdata.pymax - Mdata.pymin)
		hankei = (Mdata.pymax - Mdata.pymin) / 2 * (MASU_1 * 1.5);
	else
		hankei = (Mdata.pxmax - Mdata.pxmin) / 2 * (MASU_1 * 1.5);

	GAMETIME Gametime;

	SetMouseDispFlag(FALSE);

	SetFullSceneAntiAliasingMode(4, 5);		// フルシーンアンチエイリアスを設定、少し遅くなりますがきれいに表示されます。

	while (ProcessMessage() == 0 && endflag == 0)
	{
		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		ClsDrawScreen();

		GetGameTime(&Gametime);	//ゲーム時間を取得

		//背景（空）を描画
		DrawBox(0, 0, 1920, 1080, GetGameTimeColor(Gametime), TRUE);

		if (Stick(KEY_INPUT_RETURN)) break;

		SetCameraPositionAndTargetAndUpVec(
			VGet(
				Degsin(rot) * hankei + posy,
				MASU_1 * 30,
				Degcos(rot) * hankei + posx
			),
			VGet(
				posy,
				MASU_1 * 0,
				posx
			),
			VGet(0.0f, 1.0f, 0.0f)
			);

		DispMapForStart(Mdata);

		rot += 1.5;

		DrawFormatString(10, 10, STR_COLOR, "スタート画面　Enterでスタート");

		ScreenFlip();
	}

	SetFullSceneAntiAliasingMode(1, 1);		// フルシーンアンチエイリアスを設定、少し遅くなりますがきれいに表示されます。
}

//現在の描画速度を計測する
int GetFPS(void)
{
	static int oldtime = GetNowCount();
	int nowtime;
	static int cnt = 0;
	static int ret = 0;

	nowtime = GetNowCount();

	if (nowtime - oldtime >= 1000)
	{
		oldtime = nowtime;
		ret = cnt;
		cnt = 0;
	}
	cnt++;

	return ret;
}

//テンプレート編集
void Template_Edit(char filename[])
{
	//マップデータ、マップ配列要素数などのマップ関連のデータ

	MAP_TEMPLATE Map;

	//自機、カメラ
	JIKI   Jiki;
	CAMERA Cam;
	int JikiP2x, JikiP2y;
	int OldJikiP2x, OldJikiP2y;
	int jikimx, jikimy;

	//モデル番号
	int modelno = 1;

	//ゲーム内の仮想時間
	GAMETIME Gametime;

	//マップデータをロード
	loadmap_Temp(filename, &Map, &Jiki, &Cam);

	//マップがなかったら新しいマップを作成
	if (Map.Data.youso == 0) MakeMapCell_Temp(&Map, Jiki.pos);

	//最初の座標をマス座標に変換
	GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

	//メインループ
	while (ProcessMessage() == 0 && endflag == 0)
	{
		ClsDrawScreen();		//描画する画面（裏）を黒クリア

		GetGameTime(&Gametime);	//ゲーム時間を取得

		//入力、処理---------------------------------------------------------------------

		Stick_whatis_push();

		if (Stick(KEY_INPUT_RETURN))
			break;

		//Eキーでモデル番号を選択する画面にする
		if (Stick(BLOCKMENU_KEY))
			selmodel(&modelno);

		if (Stick(KEYCONFIGMENU_KEY))
			KeyConfig_sel();

		//右クリックでブロックを置く
		if (MouseStick(BLOCK_SET_MOUSE, TRUE, 8))
			ClickSetBlock(&Map.Data, modelno + B_NUM_START, Jiki, Cam);

		//左クリックでブロックを消す
		if (MouseStick(BLOCK_DEL_MOUSE, TRUE, 8))
			ClickDelBlock(&Map.Data, Jiki, Cam);

		//自機、カメラ座標を移動、カメラをセット
		MoveJiki(Map.Data, &Jiki, Cam);
		if (Jiki.pos.z < MASU_1 / 2)Jiki.pos.z = MASU_1 / 2;
		if (Jiki.pos.x < MASU_1 / 2)Jiki.pos.x = MASU_1 / 2;

		MoveCam(&Cam, Jiki);
		SetCam(Jiki, Cam);

		//前のマス座標を記憶、今のマス座標を取得
		OldJikiP2x = JikiP2x;
		OldJikiP2y = JikiP2y;
		GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

		//前回のマスと今のマスが違ったらマップを生成
		if (JikiP2x != OldJikiP2x || JikiP2y != OldJikiP2y)
			MakeMapCell_Temp(&Map, Jiki.pos);

		//出力---------------------------------------------------------------------------

		//背景（空）を描画
		DrawBox(0, 0, DisplayX, DisplayY, GetGameTimeColor(Gametime), TRUE);

		//ブロックを描画
		DispMap(Map.Data, Jiki.pos);

		//現在選択中のブロック（サムネイル）と説明を左下に表示
		DrawBox(15, DisplayY - 170, 145, DisplayY - 10, GetHSVColor(0, 0, 180), TRUE);
		drawthumb(30, DisplayY - 135, modelno);
		DrawFormatString(30, DisplayY - 160, STR_COLOR, "選択中のもの：");
		DrawFormatString(30, DisplayY - 30, STR_COLOR, "%s", Model[modelno].name);

		DrawFormatString(10, 10, STR_COLOR, "テンプレート編集モード Enterで終了");
		DrawFormatString(DisplayX - 50, DisplayY - 20, STR_COLOR, "%dFPS", GetFPS());

		DrawLine(DisplayX / 2 - 50, DisplayY / 2, DisplayX / 2 + 50, DisplayY / 2, GetColor(255, 255, 255));
		DrawLine(DisplayX / 2, DisplayY / 2 - 50, DisplayX / 2, DisplayY / 2 + 50, GetColor(255, 255, 255));

		//デバッグ関係
		clsDx();
		//DispDebug(Map.Data, Jiki, Cam, Gametime);

		//テンプレート編集のため、あえて軸を表示
		DispJiku();

		//ゲーム時間を更新
		GameTime_Update();

		ScreenFlip();
	}
	//メインループ終了

	//マップデータをセーブ、ログを保存
	//sortlog(&Map.Data);
	//datalog(Map.Data);
	savemap_Temp(filename, Map, Jiki, Cam);

	del_maptemp_array(&Map);
}

//テンプレート選択画面
void Template_Sel(char mode, char filename[])
{
	SPLITDATA spdata;
	char filelist[2048];
	int spn;
	int selno = 0;
	int i;

	SetMouseDispFlag(TRUE);

	while (ProcessMessage() == 0 && endflag == 0)
	{
		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		ClearDrawScreen();

		//ファイル一覧取得
		dirlist(filelist, ".\\", "*.tempmap");
		spn = split(filelist, "\n", &spdata);

		//入力
		if (Stick(TEMPLATEMENU_KEY))
		{
			break;
		}
		else if (Stick(KEY_INPUT_RETURN))
		{
			//編集モードで呼び出されたら
			if (mode == 'E')
			{
				if (exist(spdata[selno]) == TRUE)
				{
					if (spn - 1 > 0)
						Template_Edit(spdata[selno]);
				}
			}
			//配置モードで呼び出されたら
			else if (mode == 'S')
			{
				if (exist(spdata[selno]) == TRUE)
				{
					sprintf_s(filename, 256, "%s", spdata[selno]);
					break;
				}
			}
		}
		else if (Stick(KEY_INPUT_N))
		{
			char newname[256] = "";
			InputName(newname,"tempmap");
			Template_Edit(newname);
		}
		else if (Stick(KEY_INPUT_UP, TRUE))
		{
			if (selno > 0) selno--;
		}
		else if (Stick(KEY_INPUT_DOWN, TRUE))
		{
			if (selno < spn - 2) selno++;
		}

		//出力
		DrawBox(0, 0, DisplayX, DisplayY, GetHSVColor(180, 255, 100), TRUE);

		DrawBox(0, 0, DisplayX, 45, GetHSVColor(180, 180, 180), TRUE);
		DrawFormatString(10, 10, GetColor(0, 0, 0), "テンプレート選択　新規作成：Nキー　%s　もう一度F2で戻る Enterで決定", (mode == 'E') ? "編集モード" : ((mode == 'S') ? "配置モード" : "その他") );

		for (i = 0; i < spn; i++)
		{
			if (i == selno)
			{
				DrawBox(10, 50 + i * 25, 500, 50 + (i + 1) * 25 - 5, GetHSVColor(180, 255, 255), TRUE);
				DrawFormatString(10, 50 + i * 25, GetColor(0, 0, 0), "%s", spdata[i]);
			}
			else
			{
				DrawFormatString(10, 50 + i * 25, GetColor(255, 255, 255), "%s", spdata[i]);
			}
		}

		delsplit(spn, spdata);

		ScreenFlip();
	}
}

//名前入力画面
void InputName(char name[], char *kakuchoshi)
{
	int cursor = 0;
	char c;

	ClearInputCharBuf();
	while (ProcessMessage() == 0 && endflag == 0)
	{
		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		ClearDrawScreen();

		c = GetInputChar(TRUE);

		if (Stick(KEY_INPUT_RETURN))
		{
			break;
		}
		else if (c >= CTRL_CODE_CMP)
		{
			name[cursor] = c;
			name[cursor+1] = '\0';
			cursor++;
		}
		else if (c == CTRL_CODE_BS)
		{
			if (cursor > 0)
			{
				cursor--;
				name[cursor] = '\0';
			}
		}

		//出力
		DrawBox(0, 0, DisplayX, DisplayY, GetHSVColor(250, 255, 100), TRUE);

		DrawBox(0, 0, DisplayX, 50, GetColor(100, 100, 100), TRUE);
		DrawFormatString(10, 10, GetColor(255, 255, 255), "ファイル名を入力してください(拡張子：*.%s) Enterで決定", kakuchoshi);

		DrawFormatString(10, 55, GetColor(255, 255, 255), "ファイル名：%s.%s", name, kakuchoshi);

		ScreenFlip();
	}

	sprintf_s(name, 256, "%s.%s", name, kakuchoshi);
}

//モデル配列の長さを求める関数
int modellen(void)
{
	int i;

	for (i = 1; Model[i].name[0] != '\0'; i++);

	//要素が1から始まるので1を引いている
	return i - 1;
}

//モデルを選択する画面
void selmodel(int *modelno)
{
	int i, posx = DisplayX/2 - 150 * 2, posy = 50;

	int len = modellen();

	int modelno_temp = *modelno;

	SetMouseDispFlag(TRUE);

	while (ProcessMessage() == 0 && endflag == 0)
	{
		ClearDrawScreen();

		DrawBox(0, 0, DisplayX, DisplayY , GetHSVColor(0, 0, 100), TRUE);

		//もう一度押して決定
		if (Stick(BLOCKMENU_KEY)) break;

		//キーの上下左右の選択
		if (Stick(KEY_INPUT_LEFT, TRUE))
		{
			if (modelno_temp > 1)	modelno_temp--;
		}

		if (Stick(KEY_INPUT_RIGHT, TRUE))
		{
			if (modelno_temp < len) modelno_temp++;
		}

		if (Stick(KEY_INPUT_UP, TRUE))
		{
			if (modelno_temp > 4)	modelno_temp -= 4;
			else					modelno_temp = 1;
		}

		if (Stick(KEY_INPUT_DOWN, TRUE))
		{
			if (modelno_temp < len - 4)	modelno_temp += 4;
			else						modelno_temp = len;
		}

		//選択位置が端まで来た時に表示位置をずらす
		if ((modelno_temp - 1) / 4 >= DisplayY / 130)	posy = 130 * (DisplayY / 130 - 1) - ((modelno_temp - 1) / 4) * 130 + 50;
		else											posy = 50;

		//サムネイル（モデルの表面画像）とモデルの名前を表示
		for (i = 1; i <= len; i++)
		{
			//選択中のものは背景色と文字の色を変える
			if (i == modelno_temp)
			{
				DrawBox(
					posx + ((i - 1) % 4) * 150,
					posy + ((i - 1) / 4) * 130,
					posx + ((i - 1) % 4) * 150 + 150,
					posy + ((i - 1) / 4) * 130 + 130,
					GetHSVColor(180, 50, 255),
					TRUE
					);

				DrawFormatString(
					posx + ((i - 1) % 4) * 150 + 25,
					posy + ((i - 1) / 4) * 130 + 105,
					GetColor(0, 0, 0),
					"%s", Model[i].name
					);
			}
			else
			{
				DrawFormatString(
					posx + ((i - 1) % 4) * 150 + 25,
					posy + ((i - 1) / 4) * 130 + 105,
					GetColor(255, 255, 255),
					"%s", Model[i].name
					);
			}

			drawthumb(
				posx + ((i - 1) % 4) * 150 + 25,
				posy + ((i - 1) / 4) * 130 + 5,
				i
				);
		}

		//タイトルを上に書くように表示
		DrawBox(0, 0, DisplayX, 45, GetHSVColor(30, 180, 230), TRUE);
		DrawFormatString(10, 10, GetColor(0, 0, 0), "素材選択　上下左右キーで選択、Eをまた押して決定");

		ScreenFlip();
	}

	*modelno = modelno_temp;
}

//サムネイル1枚を描く
void drawthumb(int x, int y, int modelno)
{
	DrawExtendGraph(x, y, x + THUMB_SIZE, y + THUMB_SIZE, Model[modelno].thumbnail, FALSE);
}

//初期設定関数
void Init(void)
{
	//画面サイズとウィンドウモード設定(どれかひとつを選択)
	//	SetGraphMode(  320, 240, 32 );

	SetDoubleStartValidFlag(FALSE);

	//	SetGraphMode(  640, 480, 32 );
	//  SetGraphMode(800, 600, 32);
	//	SetGraphMode( 1366, 768, 32 );
	//	SetGraphMode( 1280,1024, 32 );
	//	SetGraphMode( 1920,1080, 32 );	//フルスクリーンの解像度

	SetGraphMode(DisplayX, DisplayY, 32);

	ChangeWindowMode(FALSE);									//ウィンドウモードで起動、コメントアウトでフルスクリーン

	SetFullSceneAntiAliasingMode(4, 2);						// フルシーンアンチエイリアスを設定、少し遅くなりますがきれいに表示されます。

	if (DxLib_Init() == -1) exit(-1);						//DXライブラリ初期化

	ChangeLightTypeDir(VGet(-1.0, -1.0, 0.0));				//追加する照明タイプ設定
	L1 = CreateDirLightHandle(VGet(10.0, 10.0, 0.0));		//照明を追加

	SetBackgroundColor(0, 0, 0);							//背景色
	SetDrawScreen(DX_SCREEN_BACK);							//裏画面に描画

	SetOutApplicationLogValidFlag(FALSE);

	//SetCameraNearFar(1.0f, 3000.0f);

	LoadModel();
}

//モデルとサムネイルと名前を読み込み
void LoadModel(void)
{
	int i;

	//3Dモデル
	Model[1].modelh = MV1LoadModel("Model/Sougen.mqo");
	Model[2].modelh = MV1LoadModel("Model/Dirt.mqo");
	Model[3].modelh = MV1LoadModel("Model/Stone.mqo");
	Model[4].modelh = MV1LoadModel("Model/one_color_1.mqo");
	Model[5].modelh = MV1LoadModel("Model/one_color_2.mqo");
	Model[6].modelh = MV1LoadModel("Model/one_color_3.mqo");
	Model[7].modelh = MV1LoadModel("Model/one_color_4.mqo");
	Model[8].modelh = MV1LoadModel("Model/one_color_5.mqo");
	Model[9].modelh = MV1LoadModel("Model/one_color_6.mqo");
	Model[10].modelh = MV1LoadModel("Model/one_color_7.mqo");
	Model[11].modelh = MV1LoadModel("Model/one_color_8.mqo");
	Model[12].modelh = MV1LoadModel("Model/one_color_9.mqo");
	Model[13].modelh = MV1LoadModel("Model/one_color_10.mqo");
	Model[14].modelh = MV1LoadModel("Model/one_color_11.mqo");
	Model[15].modelh = MV1LoadModel("Model/one_color_12.mqo");
	Model[16].modelh = MV1LoadModel("Model/one_color_13.mqo");
	Model[17].modelh = MV1LoadModel("Model/one_color_14.mqo");
	Model[18].modelh = MV1LoadModel("Model/one_color_15.mqo");
	Model[19].modelh = MV1LoadModel("Model/one_color_16.mqo");
	Model[20].modelh = MV1LoadModel("Model/one_color_17.mqo");
	Model[21].modelh = MV1LoadModel("Model/one_color_18.mqo");
	Model[22].modelh = MV1LoadModel("Model/one_color_19.mqo");
	Model[23].modelh = MV1LoadModel("Model/one_color_20.mqo");
	Model[24].modelh = MV1LoadModel("Model/one_color_21.mqo");
	Model[25].modelh = MV1LoadModel("Model/one_color_22.mqo");
	Model[26].modelh = MV1LoadModel("Model/one_color_23.mqo");
	Model[27].modelh = MV1LoadModel("Model/one_color_24.mqo");
	//Model[28].modelh = MV1LoadModel("Model/mado.mqo");

	//サムネイル
	Model[1].thumbnail = LoadGraph("Model/Sougen.png");
	Model[2].thumbnail = LoadGraph("Model/Dirt.png");
	Model[3].thumbnail = LoadGraph("Model/Stone.png");
	Model[4].thumbnail = LoadGraph("Model/one_color_1.png");
	Model[5].thumbnail = LoadGraph("Model/one_color_2.png");
	Model[6].thumbnail = LoadGraph("Model/one_color_3.png");
	Model[7].thumbnail = LoadGraph("Model/one_color_4.png");
	Model[8].thumbnail = LoadGraph("Model/one_color_5.png");
	Model[9].thumbnail = LoadGraph("Model/one_color_6.png");
	Model[10].thumbnail = LoadGraph("Model/one_color_7.png");
	Model[11].thumbnail = LoadGraph("Model/one_color_8.png");
	Model[12].thumbnail = LoadGraph("Model/one_color_9.png");
	Model[13].thumbnail = LoadGraph("Model/one_color_10.png");
	Model[14].thumbnail = LoadGraph("Model/one_color_11.png");
	Model[15].thumbnail = LoadGraph("Model/one_color_12.png");
	Model[16].thumbnail = LoadGraph("Model/one_color_13.png");
	Model[17].thumbnail = LoadGraph("Model/one_color_14.png");
	Model[18].thumbnail = LoadGraph("Model/one_color_15.png");
	Model[19].thumbnail = LoadGraph("Model/one_color_16.png");
	Model[20].thumbnail = LoadGraph("Model/one_color_17.png");
	Model[21].thumbnail = LoadGraph("Model/one_color_18.png");
	Model[22].thumbnail = LoadGraph("Model/one_color_19.png");
	Model[23].thumbnail = LoadGraph("Model/one_color_20.png");
	Model[24].thumbnail = LoadGraph("Model/one_color_21.png");
	Model[25].thumbnail = LoadGraph("Model/one_color_22.png");
	Model[26].thumbnail = LoadGraph("Model/one_color_23.png");
	Model[27].thumbnail = LoadGraph("Model/one_color_24.png");
	//Model[28].thumbnail = LoadGraph("Model/mado.png");

	//名前
	Model[1].name = "草原";
	Model[2].name = "土";
	Model[3].name = "石";
	Model[4].name = "単色1";
	Model[5].name = "単色2";
	Model[6].name = "単色3";
	Model[7].name = "単色4";
	Model[8].name = "単色5";
	Model[9].name = "単色6";
	Model[10].name = "単色7";
	Model[11].name = "単色8";
	Model[12].name = "単色9";
	Model[13].name = "単色10";
	Model[14].name = "単色11";
	Model[15].name = "単色12";
	Model[16].name = "単色13";
	Model[17].name = "単色14";
	Model[18].name = "単色15";
	Model[19].name = "単色16";
	Model[20].name = "単色17";
	Model[21].name = "単色18";
	Model[22].name = "単色19";
	Model[23].name = "単色20";
	Model[24].name = "単色21";
	Model[25].name = "単色22";
	Model[26].name = "単色23";
	Model[27].name = "単色24";
	//Model[28].name = "窓";

	//終わりを示す空文字列
	Model[28].name = "";

	for (i = 1; i <= modellen(); i++)
	{
		MV1SetUseZBuffer(Model[i].modelh, TRUE);
	}
}

//カメラ移動
void MoveCam(CAMERA *Cam, JIKI Jiki)
{
	static int cnt = 0;
	static int mvflag = 1;

	if (mvflag)
	{
		SetMouseDispFlag(FALSE);

		int msex, msey;
		int movx, movy;

		if (cnt == 0) SetMousePoint(DisplayX/2, DisplayY/2);

		GetMousePoint(&msex, &msey);
		SetMousePoint(DisplayX/2, DisplayY/2);

		movx = msex - DisplayX/2;
		movy = msey - DisplayY/2;

		Cam->r1 -= movx / 10;
		Cam->r2 += movy / 10;
		if (Cam->r2 < -89.0) Cam->r2 = -89.0;
		if (Cam->r2 > 89.0)  Cam->r2 = 89.0;

		if (Stick(KEY_INPUT_ESCAPE)) mvflag = 0;

		cnt++;
	}
	else
	{
		SetMouseDispFlag(TRUE);

		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		if (GetMouse(MOUSE_INPUT_LEFT))
		{
			mvflag = 1;
			cnt = 0;
		}
	}

	Cam->pos.x = (Cam->kyori)*Degcos(Cam->r1)*Degcos(Cam->r2) + Jiki.pos.x;//Degsin(Cam.rot)*Cam. + JikiP.x;
	Cam->pos.y = (Cam->kyori)*Degsin(Cam->r2)                 + Jiki.pos.y;//JikiP.y+40.0;
	Cam->pos.z = (Cam->kyori)*Degsin(Cam->r1)*Degcos(Cam->r2) + Jiki.pos.z;//Degcos(Cam.rot)*CamKyori + JikiP.z;

}

//自機移動
void MoveJiki(MAP_DATA Mdata, JIKI *Jiki, CAMERA Cam)
{
	VECTOR JikiPosWill;
	char JumpFlag = 0;
	//static char CanFlySkyFlag = 0;

	//前
	if (CheckHitKey(MAE_KEY))
	{
		JikiPosWill =
			VGet(
			Jiki->pos.x - Degcos(Cam.r1)*5.0,
			Jiki->pos.y,
			Jiki->pos.z - Degsin(Cam.r1)*5.0
			);

		if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
		{
			Jiki->pos.x -= Degcos(Cam.r1)*5.0;
			Jiki->pos.z -= Degsin(Cam.r1)*5.0;
		}
		else
		{
			//階段のように登れる状態でキーを押したらオートジャンプ
			JikiPosWill =
				VGet(
				Jiki->pos.x - Degcos(Cam.r1)*5.0,
				Jiki->pos.y + 50,
				Jiki->pos.z - Degsin(Cam.r1)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				JumpFlag = 1;
			}

			JikiPosWill =
				VGet(
				Jiki->pos.x - Degcos(Cam.r1)*5.0,
				Jiki->pos.y,
				Jiki->pos.z
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.x -= Degcos(Cam.r1)*5.0;
			}
			JikiPosWill =
				VGet(
				Jiki->pos.x,
				Jiki->pos.y,
				Jiki->pos.z - Degsin(Cam.r1)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.z -= Degsin(Cam.r1)*5.0;
			}
		}
	}
	//後ろ
	if (CheckHitKey(USIRO_KEY))
	{
		JikiPosWill =
			VGet(
			Jiki->pos.x + Degcos(Cam.r1)*5.0,
			Jiki->pos.y,
			Jiki->pos.z + Degsin(Cam.r1)*5.0
			);

		if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
		{
			Jiki->pos.x += Degcos(Cam.r1)*5.0;
			Jiki->pos.z += Degsin(Cam.r1)*5.0;
		}
		else
		{
			//階段のように登れる状態でキーを押したらオートジャンプ
			JikiPosWill =
				VGet(
				Jiki->pos.x + Degcos(Cam.r1)*5.0,
				Jiki->pos.y + 50,
				Jiki->pos.z + Degsin(Cam.r1)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				JumpFlag = 1;
			}

			JikiPosWill =
				VGet(
				Jiki->pos.x + Degcos(Cam.r1)*5.0,
				Jiki->pos.y,
				Jiki->pos.z
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.x += Degcos(Cam.r1)*5.0;
			}
			JikiPosWill =
				VGet(
				Jiki->pos.x,
				Jiki->pos.y,
				Jiki->pos.z + Degsin(Cam.r1)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.z += Degsin(Cam.r1)*5.0;
			}
		}
	}
	//左
	if (CheckHitKey(LEFT_KEY))
	{
		JikiPosWill =
			VGet(
			Jiki->pos.x + Degcos(Cam.r1 - 90)*5.0,
			Jiki->pos.y,
			Jiki->pos.z + Degsin(Cam.r1 - 90)*5.0
			);

		if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
		{
			Jiki->pos.x += Degcos(Cam.r1 - 90)*5.0;
			Jiki->pos.z += Degsin(Cam.r1 - 90)*5.0;
		}
		else
		{
			JikiPosWill =
				VGet(
				Jiki->pos.x + Degcos(Cam.r1 - 90)*5.0,
				Jiki->pos.y,
				Jiki->pos.z
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.x += Degcos(Cam.r1 - 90)*5.0;
			}
			JikiPosWill =
				VGet(
				Jiki->pos.x,
				Jiki->pos.y,
				Jiki->pos.z + Degsin(Cam.r1 - 90)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.z += Degsin(Cam.r1 - 90)*5.0;
			}
		}
	}
	//右
	if (CheckHitKey(RIGHT_KEY))
	{
		JikiPosWill =
			VGet(
			Jiki->pos.x + Degcos(Cam.r1 + 90)*5.0,
			Jiki->pos.y,
			Jiki->pos.z + Degsin(Cam.r1 + 90)*5.0
			);

		if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
		{
			Jiki->pos.x += Degcos(Cam.r1 + 90)*5.0;
			Jiki->pos.z += Degsin(Cam.r1 + 90)*5.0;
		}
		else
		{
			JikiPosWill =
				VGet(
				Jiki->pos.x + Degcos(Cam.r1 + 90)*5.0,
				Jiki->pos.y,
				Jiki->pos.z
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.x += Degcos(Cam.r1 + 90)*5.0;
			}
			JikiPosWill =
				VGet(
				Jiki->pos.x,
				Jiki->pos.y,
				Jiki->pos.z + Degsin(Cam.r1 + 90)*5.0
				);
			if (atari_hantei_jiki(Mdata, JikiPosWill) == FALSE)
			{
				Jiki->pos.z += Degsin(Cam.r1 + 90)*5.0;
			}
		}
	}
	//空を飛べるモード
	if (Jiki->CanFlySkyFlag)
	{
		int presscount;
		static char press_1spaceflag = 0;	//スペースキーを一瞬だけ何回押したかのフラグ（最高２回）

		//押して離された/離されて押された、瞬間の時間（呼び出し回数、１秒あたり約60呼び出し）を返す関数
		presscount = Key_PressedCount(MOVEMODE_CHANGE_KEY);
		if (presscount > 0)
		{
			//一瞬だけ押されたらカウント
			if (presscount < (int)(0.20 * 60))
			{
				if (press_1spaceflag < 2)
				{
					press_1spaceflag++;
				}
			}
		}
		//離されて押された瞬間（上の関数の戻り値はマイナス）
		else if (presscount < 0)
		{
			//ダブルクリックのようにスペースを押さなかった場合最初の操作に戻る
			if (presscount <= -(int)(0.20 * 60)) press_1spaceflag = 0;
		}

		if (press_1spaceflag == 2)
		{
			press_1spaceflag = 0;
			Jiki->CanFlySkyFlag = 0;
		}

		//スペースで上昇
		if (CheckHitKey(JYOSYO_KEY))
		{
			if (
				atari_hantei_jiki(
				Mdata,
				VGet(
				Jiki->pos.x,
				Jiki->pos.y + 5.0,
				Jiki->pos.z
				)
				) == FALSE
				)
			{
				Jiki->pos.y += 5.0;
			}
			//}
		}
		//左シフトで降下
		if (CheckHitKey(KOUKA_KEY))
		{
			if (
				atari_hantei_jiki(
				Mdata,
				VGet(
				Jiki->pos.x,
				Jiki->pos.y - 5.0,
				Jiki->pos.z
				)
				) == FALSE
				)
			{
				Jiki->pos.y -= 5.0;
			}
			else
			{
				press_1spaceflag = 0;
				Jiki->CanFlySkyFlag = 0;
			}
			//}
		}
	}
	//重力モード
	else
	{
		static double kasokudo = -0.2;		//加速度
		static double speed = 0.0;			//速さ
		static char press_1spaceflag = 0;	//スペースキーを一瞬だけ何回押したかのフラグ（最高２回）
		int presscount;

		//押して離された/離されて押された、瞬間の時間（呼び出し回数、１秒あたり約60呼び出し）を返す関数
		presscount = Key_PressedCount(MOVEMODE_CHANGE_KEY);

		//押して離された瞬間（上の関数の戻り値はプラス）
		if (presscount > 0)
		{
			//一瞬だけ押されたらカウント
			if (presscount < (int)(0.20 * 60))
			{
				if(press_1spaceflag < 2) press_1spaceflag++;
			}
		}
		//離されて押された瞬間（上の関数の戻り値はマイナス）
		else if (presscount < 0)
		{
			//ダブルクリックのようにスペースを押さなかった場合最初の操作に戻る
			if (presscount <= -(int)(0.20 * 60)) press_1spaceflag = 0;
		}
		//ダブルクリックのようにスペースを押されたら少し（2マス分）上昇して空を飛べるモードにする
		//ただしその途中天井にぶつかったら上昇を停止して、空を飛べるモードにする
		if (press_1spaceflag == 2)
		{
			static int up_count = 0;

			if (
				atari_hantei_jiki(
					Mdata,
					VGet(
						Jiki->pos.x,
						Jiki->pos.y + 10.0,
						Jiki->pos.z
					)
				)
				== FALSE
			)
			//通常
			{
				Jiki->pos.y += 10.0;			//10.0px上昇させる
				up_count++;
			}
			//天井衝突時
			else
			{
				up_count = 0;

				kasokudo = -0.2;
				speed = 0.0;
				press_1spaceflag = 0;

				Jiki->CanFlySkyFlag = 1;
			}

			//2マス上昇したら
			//10.0px/10 = 1px -> 1px * １マス分 * 2 で 2マス分
			if (up_count >= (int)(MASU_1 * 2) / 10)
			{
				up_count = 0;

				kasokudo = -0.2;
				speed = 0.0;
				press_1spaceflag = 0;

				Jiki->CanFlySkyFlag = 1;
			}
		}
		else
		{
			//スペースキーでジャンプ
			if (Stick(JUMP_KEY))
			{
				JumpFlag = 1;
			}
			
			if (JumpFlag && kasokudo == 0)
			{
				JumpFlag = 0;

				//ジャンプ開始時は加速度を-0.2に戻す
				kasokudo = -0.2;

				//最終速度、距離、加速度から初速度を求める
				//v^2 - v0^2 =  2 * a * x
				//変形して、v0 = √( v^2 - 2 * a * x )
				//v0 : speed, √ : sqrt, v : 0, a : kasokudo, x : 75.0
				//つまり、ジャンプして75px上昇したら、降下する
				speed = sqrt(0.0 - 2.0*kasokudo*75.0);

				//速さを加速度の倍数にする
				speed = speed - Amarif(speed, -kasokudo);
			}

			//降下時
			if (speed <= 0.0)
			{
				//床に当たった時
				if (
					atari_hantei_jiki(
						Mdata,
						VGet(
							Jiki->pos.x,
							Jiki->pos.y - 0.2,
							Jiki->pos.z
						)
					) == TRUE
				)
				{
					//当たらない位置まで上昇
					while (atari_hantei_jiki(Mdata, Jiki->pos) == TRUE)
					{
						Jiki->pos.y += 0.01;
					}

					//スピードを0にする
					speed = 0.0;

					//床に到達時加速度を0にする
					kasokudo = 0.0;
				}
				else
				{
					//自機の下が床ではなくなった時、自由落下させるように加速度を-0.2に戻す
					kasokudo = -0.2;
				}
			}
			//上昇時
			else
			{
				//ジャンプして天井に当たった時
				if (
					atari_hantei_jiki(
						Mdata,
						VGet(
							Jiki->pos.x,
							Jiki->pos.y + 0.2,
							Jiki->pos.z
						)
					) == TRUE
				)
				{
					//当たらない位置まで降下
					while (atari_hantei_jiki(Mdata, Jiki->pos) == TRUE)
					{
						Jiki->pos.y -= 0.01;
					}

					//跳ね返る
					speed = -speed;
				}
			}
			//自機の座標を速さ分だけ移動

			Jiki->pos.y += speed;

			speed += kasokudo;
		}
	}
}

//カメラセット
void SetCam(JIKI Jiki, CAMERA Cam)
{
	SetCameraPositionAndTargetAndUpVec(
		Cam.pos,
		Jiki.pos,
		VGet(0.0f, 1.0f, 0.0f)
		);
}

//モデルを表示
void DispMap(MAP_DATA Mdata, VECTOR Jiki)
{
	int i, j, k, Hdlp;			//カウンタ、モデルハンドル配列の添字
	int jikix, jikiy, jikih;	//自機のマス座標

	GetMasu3(Jiki.z, Jiki.x, Jiki.y, &jikix, &jikiy, &jikih);

	for (i = 0; i < Mdata.youso; i++)
	{

		if (
			jikix - 20 < Mdata.value[i].x && Mdata.value[i].x < jikix + 20 &&
			jikiy - 20 < Mdata.value[i].y && Mdata.value[i].y < jikiy + 20
			)

		{
			for (j = 0; j < MAP_MAX; j++)
			{
				bool hantei;

				if (jikih >= 0)	hantei = (-2 <= j - MAP_ZERO);
				else{
					hantei = 
					(jikih - 4 <= j - MAP_ZERO) && (jikih + 4 >= j - MAP_ZERO)&&
					jikix - 7 < Mdata.value[i].x && Mdata.value[i].x < jikix + 7 &&
					jikiy - 7 < Mdata.value[i].y && Mdata.value[i].y < jikiy + 7
					;
				}

				if (hantei)
				{
					//モデルハンドルの配列の添え字を取得
					Hdlp = Mdata.value[i].data[j] - B_NUM_START;

					//ブロックありの場合
					if (Hdlp != 0)
					{
						//ブロックの座標をセット
						MV1SetPosition(
							Model[Hdlp].modelh,
							VGet(
							Mdata.value[i].y*MASU_1,
							(j - MAP_ZERO)*MASU_1,
							Mdata.value[i].x*MASU_1
							)
							);

						//ブロックを描画
						MV1DrawModel(Model[Hdlp].modelh);
					}
				}
			}
		}
	}
}

//スタート画面用のモデル表示関数
void DispMapForStart(MAP_DATA Mdata)
{
	int i, j, k, Hdlp;			//カウンタ、モデルハンドル配列の添字

	for (i = 0; i < Mdata.youso; i++)
	{
		for (j = 0; j < MAP_MAX; j++)
		{
			if (j - MAP_ZERO >= -2)
			{
				//モデルハンドルの配列の添え字を取得
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//ブロックありの場合
				if (Hdlp != 0)
				{
					//ブロックの座標をセット
					MV1SetPosition(
						Model[Hdlp].modelh,
						VGet(
						Mdata.value[i].y*MASU_1,
						(j - MAP_ZERO)*MASU_1,
						Mdata.value[i].x*MASU_1
						)
						);

					//ブロックを描画
					MV1DrawModel(Model[Hdlp].modelh);
				}
			}
		}
	}
}

//デバッグを表示
void DispDebug(MAP_DATA Mdata, JIKI Jiki, CAMERA Cam, GAMETIME Gametime)
{
	int i;
	int xm, ym, hm;

	GetMasu3(Jiki.pos.z, Jiki.pos.x, Jiki.pos.y, &xm, &ym, &hm);

	printfDx("n=%d\n\n", Mdata.youso);

	printfDx("JikiMasu[xyh] (%d,%d,%d)\n\n", xm, ym, hm);

	printfDx("Now is %d:%02d:%02d\n\n", Gametime.hour, Gametime.min, Gametime.sec);

	printfDx("Jiki(%.7f,%.7f,%.7f)\n", Jiki.pos.x, Jiki.pos.y, Jiki.pos.z);
	printfDx("Cam(%.1f,%.1f,%.1f) r1=%.0f, r2=%.0f\n\n", Cam.pos.x, Cam.pos.y, Cam.pos.z, Cam.r1, Cam.r2);
	printfDx("Min=(%d,%d), Max=(%d,%d)\n\n", Mdata.pxmin, Mdata.pymin, Mdata.pxmax, Mdata.pymax);

	printfDx("\n");

	/*
	printfDx("Atariflag = ");
	for (i = 0; i < 6; i++)
	{
		printfDx("%d, ", atariflag[i]);
	}
	printfDx("\n");
	*/
	/*
	for(i=0; i<20; i++)
	{
	printfDx("Map[%d](%d,%d), data={%X,%X,%X,...,%X,...,%X,%X,%X}\n",
	i,
	Map[i].x,
	Map[i].y,
	Map[i].data[0],
	Map[i].data[1],
	Map[i].data[2],
	Map[i].data[MAP_ZERO-1],
	Map[i].data[MAP_MAX-2-1],
	Map[i].data[MAP_MAX-1-1],
	Map[i].data[MAP_MAX-0-1]
	);
	}
	*/
}

//3D座標軸を表示
void DispJiku(void)
{
	//座標軸。赤がx軸, 緑がy軸, 青がz軸
	DrawLine3D(VGet(500.0, 0.0, 0.0), VGet(-500.0, 0.0, 0.0), GetColor(255, 0, 0)); //x
	DrawLine3D(VGet(0.0, 500.0, 0.0), VGet(0.0, -500.0, 0.0), GetColor(0, 255, 0)); //y
	DrawLine3D(VGet(0.0, 0.0, 500.0), VGet(0.0, 0.0, -500.0), GetColor(0, 0, 255)); //z

	//座標軸の先端のコーン（円錐）
	DrawCone3D(VGet(600.0, 0.0, 0.0), VGet(500.0, 0.0, 0.0), 20.0, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(VGet(0.0, 600.0, 0.0), VGet(0.0, 500.0, 0.0), 20.0, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(VGet(0.0, 0.0, 600.0), VGet(0.0, 0.0, 500.0), 20.0, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);

	DrawFormatString(450, 10, GetColor(0,0,0), "軸の見方：赤がx軸, 緑がy軸, 青がz軸");
}

//マップデータがちゃんとソートされているかどうか調べるデバッグ用ログを汎用ログファイルに出力
void sortlog(MAP_DATA *Mdata)
{
	int i;

	fprintf(Prog_Log_fp, "SortLog--------------------------------------------------\n");
	for (i = 0; i < Mdata->youso; i++)
	{
		fprintf(Prog_Log_fp, "Map[%d].Pos(%d,%d), Keynum=%lld\n",
			i,
			Mdata->value[i].x,
			Mdata->value[i].y,
			calc(*Mdata, Mdata->value[i].x, Mdata->value[i].y)
			);
	}
	fprintf(Prog_Log_fp, "EndOfSortLog---------------------------------------------\n");
}

void datalog(MAP_DATA Mdata)
{
	int xi, yi, i, hi, f;

	fprintf(Prog_Log_fp, "DataLog--------------------------------------------------\n");

	fprintf(Prog_Log_fp, "番号の見かた：\n");

	for (i = 1; i <= modellen(); i++)
	{
		fprintf(Prog_Log_fp, "%d番：%s\n", i+B_NUM_START, Model[i].name);
	}

	fprintf(Prog_Log_fp, "\n");

	for (xi = Mdata.pxmin; xi < Mdata.pxmax; xi++)
	{
		for (yi = Mdata.pymin; yi < Mdata.pymax; yi++)
		{
			f = mapfind(Mdata, xi, yi);

			if (f != -1)
			{
				fprintf(Prog_Log_fp, "マス座標：(%d, %d)：\n", xi, yi);
				for (hi = 0; hi < MAP_MAX; hi++)
				{
					fprintf(Prog_Log_fp, " %3d", Mdata.value[f].data[hi]);
				}
				fprintf(Prog_Log_fp, "\n");
			}
			else
			{
				fprintf(Prog_Log_fp, "マス座標：(%d, %d)：無し\n", xi, yi);
			}
		}
	}
	fprintf(Prog_Log_fp, "EndOfDataLog--------------------------------------------------\n");
}