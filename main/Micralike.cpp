//�w�b�_�[�t�@�C���ǂݍ���
#include "DxLib.h"

#include "Micralike_set.h"

//�R���p�C�����̌x���\���}��
#pragma warning( disable:4305 4244 4305 )

//���C���֐�
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	//DxLib,�ϐ��̏����ݒ�
	Init();

	//�ėp�ϐ�
	int i,j;

	//�}�b�v�f�[�^�A�}�b�v�z��v�f���Ȃǂ̃}�b�v�֘A�̃f�[�^
	MAP_MAIN Map;

	//���@�A�J����
	JIKI   Jiki;
	CAMERA Cam;
	//int JikiP2x, JikiP2y;
	//int OldJikiP2x, OldJikiP2y;

	//���f���ԍ�
	int modelno = 1;

	//�Q�[�����̉��z����
	GAMETIME Gametime;

	//�e���v���[�g�t�@�C���ꗗ�擾�ϐ�
	SPLITDATA spdata;
	char filelist[2048];
	int spn;

	//�e���v���[�g�֘A

	//�e���v���[�g�z�u�t���O
	bool tempsetflag = FALSE;

	//�z�u����e���v���[�g�t�@�C�����ƃe���v���[�g�}�b�v�f�[�^
	char tempfilename[256] = "";
	MAP_TEMPLATE SetMapT;

	//�e���v���[�g�}�b�v�f�[�^
	MAP_TEMPLATE *MapT;

	/*
	//�e���v���[�g�t�@�C���ꗗ�擾�A�ǂݍ���
	dirlist(filelist, ".\\", "*.tempmap");
	spn = split(filelist, "\n", &spdata);
	MapT = (MAP_TEMPLATE*)malloc(sizeof(MAP_TEMPLATE)*spn);
	for (i = 0; i < spn; i++)
	{
		loadmap_Temp_formain(spdata[i], &MapT[i]);
	}
	*/

	//�}�b�v�f�[�^�����[�h
	loadmap("Map.dat", &Map, &Jiki, &Cam);

	//�L�[�R���t�B�O�ǂݍ���
	KeyConfig_Load();

	//�}�b�v���Ȃ�������V�����}�b�v���쐬
	if (Map.Data.youso == 0)
	{
		for (i = 0; i < 1000; i++)
			MakeMapCell(&Map, Jiki.pos);
	}

	//�ŏ��̍��W���}�X���W�ɕϊ�
	//GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

	start_mode(Map.Data);

	//���C�����[�v
	while (ProcessMessage() == 0 && endflag == 0)
	{
		ClsDrawScreen();		//�`�悷���ʁi���j�����N���A

		GetGameTime(&Gametime);	//�Q�[�����Ԃ��擾

		//���́A����---------------------------------------------------------------------

		Stick_whatis_push();

		//E�L�[�Ń��f���ԍ���I�������ʂɂ���
		if (Stick(BLOCKMENU_KEY))
			selmodel(&modelno);

		//�L�[�R���t�B�O���j���[���
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

			//�e���v���[�g�t�@�C���ꗗ�擾�A�ǂݍ���
			dirlist(filelist, ".\\", "*.tempmap");
			spn = split(filelist, "\n", &spdata);
			MapT = (MAP_TEMPLATE*)malloc(sizeof(MAP_TEMPLATE)*spn);
			for (i = 0; i < spn; i++)
			{
				loadmap_Temp_formain(spdata[i], &MapT[i]);
			}
			*/
		}

		//�E�N���b�N�Ńu���b�N��u��
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
		
		//���N���b�N�Ńu���b�N������
		if (MouseStick(BLOCK_DEL_MOUSE, TRUE, 8))
			ClickDelBlock(&Map.Data, Jiki, Cam);

		//���@�A�J�������W���ړ��A�J�������Z�b�g
		MoveJiki(Map.Data, &Jiki, Cam);
		MoveCam(&Cam, Jiki);
		SetCam(Jiki, Cam);

		//�O�̃}�X���W���L���A���̃}�X���W���擾
		//OldJikiP2x = JikiP2x;
		//OldJikiP2y = JikiP2y;
		//GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

		//�O��̃}�X�ƍ��̃}�X���������}�b�v�𐶐�
		//if (JikiP2x != OldJikiP2x || JikiP2y != OldJikiP2y)

		MakeMapCell(&Map, Jiki.pos);

		//�o��---------------------------------------------------------------------------

		//�w�i�i��j��`��
		DrawBox(0, 0, DisplayX, DisplayY, GetGameTimeColor(Gametime), TRUE);

		//�u���b�N��`��
		DispMap(Map.Data, Jiki.pos);

		//���ݑI�𒆂̃u���b�N�i�T���l�C���j�Ɛ����������ɕ\��
		DrawBox(15, DisplayY - 170, 145, DisplayY - 10, GetHSVColor(0, 0, 180), TRUE);
		drawthumb(30, DisplayY - 135, modelno);
		DrawFormatString(30, DisplayY - 160, STR_COLOR, "�I�𒆂̂��́F");
		DrawFormatString(30, DisplayY - 30, STR_COLOR, "%s", Model[modelno].name);

		DrawFormatString(10, 10, STR_COLOR, "���[�h�F%s, H�L�[�Ő؂�ւ�", tempsetflag ? "�e���v���[�g�z�u���[�h" : "�u���b�N�z�u���[�h");
		DrawFormatString(DisplayX - 50, DisplayY - 20, STR_COLOR, "%dFPS", GetFPS());

		DrawLine(DisplayX / 2 - 50, DisplayY / 2, DisplayX / 2 + 50, DisplayY / 2, GetColor(255, 255, 255));
		DrawLine(DisplayX / 2, DisplayY / 2 - 50, DisplayX / 2, DisplayY / 2 + 50, GetColor(255, 255, 255));

		if (strcmp(tempfilename, "") != 0 && tempsetflag)
		{
			DrawFormatString(10, 40, STR_COLOR, "�ǂݍ��񂾃e���v���[�g�t�@�C���F%s", tempfilename);
		}

		//�f�o�b�O�֌W
		clsDx();
		//DispDebug(Map.Data, Jiki, Cam, Gametime);
		//DispJiku();

		//�Q�[�����Ԃ��X�V
		GameTime_Update();

		ScreenFlip();
	}
	//���C�����[�v�I��

	//�}�b�v�f�[�^�A�L�[�R���t�B�O���Z�[�u�A���O��ۑ�
	//sortlog(&Map.Data);
	savemap("Map.dat", Map, Jiki, Cam);
	KeyConfig_Save();
	//datalog(Map.Data);

	del_mapmain_array(&Map);

	fclose(Prog_Log_fp);

	DxLib_End();
	return 0;
}

//�X�^�[�g���
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

	SetFullSceneAntiAliasingMode(4, 5);		// �t���V�[���A���`�G�C���A�X��ݒ�A�����x���Ȃ�܂������ꂢ�ɕ\������܂��B

	while (ProcessMessage() == 0 && endflag == 0)
	{
		if (Stick(KEY_INPUT_ESCAPE)) endflag = 1;

		ClsDrawScreen();

		GetGameTime(&Gametime);	//�Q�[�����Ԃ��擾

		//�w�i�i��j��`��
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

		DrawFormatString(10, 10, STR_COLOR, "�X�^�[�g��ʁ@Enter�ŃX�^�[�g");

		ScreenFlip();
	}

	SetFullSceneAntiAliasingMode(1, 1);		// �t���V�[���A���`�G�C���A�X��ݒ�A�����x���Ȃ�܂������ꂢ�ɕ\������܂��B
}

//���݂̕`�摬�x���v������
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

//�e���v���[�g�ҏW
void Template_Edit(char filename[])
{
	//�}�b�v�f�[�^�A�}�b�v�z��v�f���Ȃǂ̃}�b�v�֘A�̃f�[�^

	MAP_TEMPLATE Map;

	//���@�A�J����
	JIKI   Jiki;
	CAMERA Cam;
	int JikiP2x, JikiP2y;
	int OldJikiP2x, OldJikiP2y;
	int jikimx, jikimy;

	//���f���ԍ�
	int modelno = 1;

	//�Q�[�����̉��z����
	GAMETIME Gametime;

	//�}�b�v�f�[�^�����[�h
	loadmap_Temp(filename, &Map, &Jiki, &Cam);

	//�}�b�v���Ȃ�������V�����}�b�v���쐬
	if (Map.Data.youso == 0) MakeMapCell_Temp(&Map, Jiki.pos);

	//�ŏ��̍��W���}�X���W�ɕϊ�
	GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

	//���C�����[�v
	while (ProcessMessage() == 0 && endflag == 0)
	{
		ClsDrawScreen();		//�`�悷���ʁi���j�����N���A

		GetGameTime(&Gametime);	//�Q�[�����Ԃ��擾

		//���́A����---------------------------------------------------------------------

		Stick_whatis_push();

		if (Stick(KEY_INPUT_RETURN))
			break;

		//E�L�[�Ń��f���ԍ���I�������ʂɂ���
		if (Stick(BLOCKMENU_KEY))
			selmodel(&modelno);

		if (Stick(KEYCONFIGMENU_KEY))
			KeyConfig_sel();

		//�E�N���b�N�Ńu���b�N��u��
		if (MouseStick(BLOCK_SET_MOUSE, TRUE, 8))
			ClickSetBlock(&Map.Data, modelno + B_NUM_START, Jiki, Cam);

		//���N���b�N�Ńu���b�N������
		if (MouseStick(BLOCK_DEL_MOUSE, TRUE, 8))
			ClickDelBlock(&Map.Data, Jiki, Cam);

		//���@�A�J�������W���ړ��A�J�������Z�b�g
		MoveJiki(Map.Data, &Jiki, Cam);
		if (Jiki.pos.z < MASU_1 / 2)Jiki.pos.z = MASU_1 / 2;
		if (Jiki.pos.x < MASU_1 / 2)Jiki.pos.x = MASU_1 / 2;

		MoveCam(&Cam, Jiki);
		SetCam(Jiki, Cam);

		//�O�̃}�X���W���L���A���̃}�X���W���擾
		OldJikiP2x = JikiP2x;
		OldJikiP2y = JikiP2y;
		GetMasu2(Jiki.pos.z, Jiki.pos.x, &JikiP2x, &JikiP2y);

		//�O��̃}�X�ƍ��̃}�X���������}�b�v�𐶐�
		if (JikiP2x != OldJikiP2x || JikiP2y != OldJikiP2y)
			MakeMapCell_Temp(&Map, Jiki.pos);

		//�o��---------------------------------------------------------------------------

		//�w�i�i��j��`��
		DrawBox(0, 0, DisplayX, DisplayY, GetGameTimeColor(Gametime), TRUE);

		//�u���b�N��`��
		DispMap(Map.Data, Jiki.pos);

		//���ݑI�𒆂̃u���b�N�i�T���l�C���j�Ɛ����������ɕ\��
		DrawBox(15, DisplayY - 170, 145, DisplayY - 10, GetHSVColor(0, 0, 180), TRUE);
		drawthumb(30, DisplayY - 135, modelno);
		DrawFormatString(30, DisplayY - 160, STR_COLOR, "�I�𒆂̂��́F");
		DrawFormatString(30, DisplayY - 30, STR_COLOR, "%s", Model[modelno].name);

		DrawFormatString(10, 10, STR_COLOR, "�e���v���[�g�ҏW���[�h Enter�ŏI��");
		DrawFormatString(DisplayX - 50, DisplayY - 20, STR_COLOR, "%dFPS", GetFPS());

		DrawLine(DisplayX / 2 - 50, DisplayY / 2, DisplayX / 2 + 50, DisplayY / 2, GetColor(255, 255, 255));
		DrawLine(DisplayX / 2, DisplayY / 2 - 50, DisplayX / 2, DisplayY / 2 + 50, GetColor(255, 255, 255));

		//�f�o�b�O�֌W
		clsDx();
		//DispDebug(Map.Data, Jiki, Cam, Gametime);

		//�e���v���[�g�ҏW�̂��߁A�����Ď���\��
		DispJiku();

		//�Q�[�����Ԃ��X�V
		GameTime_Update();

		ScreenFlip();
	}
	//���C�����[�v�I��

	//�}�b�v�f�[�^���Z�[�u�A���O��ۑ�
	//sortlog(&Map.Data);
	//datalog(Map.Data);
	savemap_Temp(filename, Map, Jiki, Cam);

	del_maptemp_array(&Map);
}

//�e���v���[�g�I�����
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

		//�t�@�C���ꗗ�擾
		dirlist(filelist, ".\\", "*.tempmap");
		spn = split(filelist, "\n", &spdata);

		//����
		if (Stick(TEMPLATEMENU_KEY))
		{
			break;
		}
		else if (Stick(KEY_INPUT_RETURN))
		{
			//�ҏW���[�h�ŌĂяo���ꂽ��
			if (mode == 'E')
			{
				if (exist(spdata[selno]) == TRUE)
				{
					if (spn - 1 > 0)
						Template_Edit(spdata[selno]);
				}
			}
			//�z�u���[�h�ŌĂяo���ꂽ��
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

		//�o��
		DrawBox(0, 0, DisplayX, DisplayY, GetHSVColor(180, 255, 100), TRUE);

		DrawBox(0, 0, DisplayX, 45, GetHSVColor(180, 180, 180), TRUE);
		DrawFormatString(10, 10, GetColor(0, 0, 0), "�e���v���[�g�I���@�V�K�쐬�FN�L�[�@%s�@������xF2�Ŗ߂� Enter�Ō���", (mode == 'E') ? "�ҏW���[�h" : ((mode == 'S') ? "�z�u���[�h" : "���̑�") );

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

//���O���͉��
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

		//�o��
		DrawBox(0, 0, DisplayX, DisplayY, GetHSVColor(250, 255, 100), TRUE);

		DrawBox(0, 0, DisplayX, 50, GetColor(100, 100, 100), TRUE);
		DrawFormatString(10, 10, GetColor(255, 255, 255), "�t�@�C��������͂��Ă�������(�g���q�F*.%s) Enter�Ō���", kakuchoshi);

		DrawFormatString(10, 55, GetColor(255, 255, 255), "�t�@�C�����F%s.%s", name, kakuchoshi);

		ScreenFlip();
	}

	sprintf_s(name, 256, "%s.%s", name, kakuchoshi);
}

//���f���z��̒��������߂�֐�
int modellen(void)
{
	int i;

	for (i = 1; Model[i].name[0] != '\0'; i++);

	//�v�f��1����n�܂�̂�1�������Ă���
	return i - 1;
}

//���f����I��������
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

		//������x�����Č���
		if (Stick(BLOCKMENU_KEY)) break;

		//�L�[�̏㉺���E�̑I��
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

		//�I���ʒu���[�܂ŗ������ɕ\���ʒu�����炷
		if ((modelno_temp - 1) / 4 >= DisplayY / 130)	posy = 130 * (DisplayY / 130 - 1) - ((modelno_temp - 1) / 4) * 130 + 50;
		else											posy = 50;

		//�T���l�C���i���f���̕\�ʉ摜�j�ƃ��f���̖��O��\��
		for (i = 1; i <= len; i++)
		{
			//�I�𒆂̂��͔̂w�i�F�ƕ����̐F��ς���
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

		//�^�C�g������ɏ����悤�ɕ\��
		DrawBox(0, 0, DisplayX, 45, GetHSVColor(30, 180, 230), TRUE);
		DrawFormatString(10, 10, GetColor(0, 0, 0), "�f�ޑI���@�㉺���E�L�[�őI���AE���܂������Č���");

		ScreenFlip();
	}

	*modelno = modelno_temp;
}

//�T���l�C��1����`��
void drawthumb(int x, int y, int modelno)
{
	DrawExtendGraph(x, y, x + THUMB_SIZE, y + THUMB_SIZE, Model[modelno].thumbnail, FALSE);
}

//�����ݒ�֐�
void Init(void)
{
	//��ʃT�C�Y�ƃE�B���h�E���[�h�ݒ�(�ǂꂩ�ЂƂ�I��)
	//	SetGraphMode(  320, 240, 32 );

	SetDoubleStartValidFlag(FALSE);

	//	SetGraphMode(  640, 480, 32 );
	//  SetGraphMode(800, 600, 32);
	//	SetGraphMode( 1366, 768, 32 );
	//	SetGraphMode( 1280,1024, 32 );
	//	SetGraphMode( 1920,1080, 32 );	//�t���X�N���[���̉𑜓x

	SetGraphMode(DisplayX, DisplayY, 32);

	ChangeWindowMode(FALSE);									//�E�B���h�E���[�h�ŋN���A�R�����g�A�E�g�Ńt���X�N���[��

	SetFullSceneAntiAliasingMode(4, 2);						// �t���V�[���A���`�G�C���A�X��ݒ�A�����x���Ȃ�܂������ꂢ�ɕ\������܂��B

	if (DxLib_Init() == -1) exit(-1);						//DX���C�u����������

	ChangeLightTypeDir(VGet(-1.0, -1.0, 0.0));				//�ǉ�����Ɩ��^�C�v�ݒ�
	L1 = CreateDirLightHandle(VGet(10.0, 10.0, 0.0));		//�Ɩ���ǉ�

	SetBackgroundColor(0, 0, 0);							//�w�i�F
	SetDrawScreen(DX_SCREEN_BACK);							//����ʂɕ`��

	SetOutApplicationLogValidFlag(FALSE);

	//SetCameraNearFar(1.0f, 3000.0f);

	LoadModel();
}

//���f���ƃT���l�C���Ɩ��O��ǂݍ���
void LoadModel(void)
{
	int i;

	//3D���f��
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

	//�T���l�C��
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

	//���O
	Model[1].name = "����";
	Model[2].name = "�y";
	Model[3].name = "��";
	Model[4].name = "�P�F1";
	Model[5].name = "�P�F2";
	Model[6].name = "�P�F3";
	Model[7].name = "�P�F4";
	Model[8].name = "�P�F5";
	Model[9].name = "�P�F6";
	Model[10].name = "�P�F7";
	Model[11].name = "�P�F8";
	Model[12].name = "�P�F9";
	Model[13].name = "�P�F10";
	Model[14].name = "�P�F11";
	Model[15].name = "�P�F12";
	Model[16].name = "�P�F13";
	Model[17].name = "�P�F14";
	Model[18].name = "�P�F15";
	Model[19].name = "�P�F16";
	Model[20].name = "�P�F17";
	Model[21].name = "�P�F18";
	Model[22].name = "�P�F19";
	Model[23].name = "�P�F20";
	Model[24].name = "�P�F21";
	Model[25].name = "�P�F22";
	Model[26].name = "�P�F23";
	Model[27].name = "�P�F24";
	//Model[28].name = "��";

	//�I���������󕶎���
	Model[28].name = "";

	for (i = 1; i <= modellen(); i++)
	{
		MV1SetUseZBuffer(Model[i].modelh, TRUE);
	}
}

//�J�����ړ�
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

//���@�ړ�
void MoveJiki(MAP_DATA Mdata, JIKI *Jiki, CAMERA Cam)
{
	VECTOR JikiPosWill;
	char JumpFlag = 0;
	//static char CanFlySkyFlag = 0;

	//�O
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
			//�K�i�̂悤�ɓo����ԂŃL�[����������I�[�g�W�����v
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
	//���
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
			//�K�i�̂悤�ɓo����ԂŃL�[����������I�[�g�W�����v
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
	//��
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
	//�E
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
	//����ׂ郂�[�h
	if (Jiki->CanFlySkyFlag)
	{
		int presscount;
		static char press_1spaceflag = 0;	//�X�y�[�X�L�[����u�������񉟂������̃t���O�i�ō��Q��j

		//�����ė����ꂽ/������ĉ����ꂽ�A�u�Ԃ̎��ԁi�Ăяo���񐔁A�P�b�������60�Ăяo���j��Ԃ��֐�
		presscount = Key_PressedCount(MOVEMODE_CHANGE_KEY);
		if (presscount > 0)
		{
			//��u���������ꂽ��J�E���g
			if (presscount < (int)(0.20 * 60))
			{
				if (press_1spaceflag < 2)
				{
					press_1spaceflag++;
				}
			}
		}
		//������ĉ����ꂽ�u�ԁi��̊֐��̖߂�l�̓}�C�i�X�j
		else if (presscount < 0)
		{
			//�_�u���N���b�N�̂悤�ɃX�y�[�X�������Ȃ������ꍇ�ŏ��̑���ɖ߂�
			if (presscount <= -(int)(0.20 * 60)) press_1spaceflag = 0;
		}

		if (press_1spaceflag == 2)
		{
			press_1spaceflag = 0;
			Jiki->CanFlySkyFlag = 0;
		}

		//�X�y�[�X�ŏ㏸
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
		//���V�t�g�ō~��
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
	//�d�̓��[�h
	else
	{
		static double kasokudo = -0.2;		//�����x
		static double speed = 0.0;			//����
		static char press_1spaceflag = 0;	//�X�y�[�X�L�[����u�������񉟂������̃t���O�i�ō��Q��j
		int presscount;

		//�����ė����ꂽ/������ĉ����ꂽ�A�u�Ԃ̎��ԁi�Ăяo���񐔁A�P�b�������60�Ăяo���j��Ԃ��֐�
		presscount = Key_PressedCount(MOVEMODE_CHANGE_KEY);

		//�����ė����ꂽ�u�ԁi��̊֐��̖߂�l�̓v���X�j
		if (presscount > 0)
		{
			//��u���������ꂽ��J�E���g
			if (presscount < (int)(0.20 * 60))
			{
				if(press_1spaceflag < 2) press_1spaceflag++;
			}
		}
		//������ĉ����ꂽ�u�ԁi��̊֐��̖߂�l�̓}�C�i�X�j
		else if (presscount < 0)
		{
			//�_�u���N���b�N�̂悤�ɃX�y�[�X�������Ȃ������ꍇ�ŏ��̑���ɖ߂�
			if (presscount <= -(int)(0.20 * 60)) press_1spaceflag = 0;
		}
		//�_�u���N���b�N�̂悤�ɃX�y�[�X�������ꂽ�班���i2�}�X���j�㏸���ċ���ׂ郂�[�h�ɂ���
		//���������̓r���V��ɂԂ�������㏸���~���āA����ׂ郂�[�h�ɂ���
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
			//�ʏ�
			{
				Jiki->pos.y += 10.0;			//10.0px�㏸������
				up_count++;
			}
			//�V��Փˎ�
			else
			{
				up_count = 0;

				kasokudo = -0.2;
				speed = 0.0;
				press_1spaceflag = 0;

				Jiki->CanFlySkyFlag = 1;
			}

			//2�}�X�㏸������
			//10.0px/10 = 1px -> 1px * �P�}�X�� * 2 �� 2�}�X��
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
			//�X�y�[�X�L�[�ŃW�����v
			if (Stick(JUMP_KEY))
			{
				JumpFlag = 1;
			}
			
			if (JumpFlag && kasokudo == 0)
			{
				JumpFlag = 0;

				//�W�����v�J�n���͉����x��-0.2�ɖ߂�
				kasokudo = -0.2;

				//�ŏI���x�A�����A�����x���珉���x�����߂�
				//v^2 - v0^2 =  2 * a * x
				//�ό`���āAv0 = ��( v^2 - 2 * a * x )
				//v0 : speed, �� : sqrt, v : 0, a : kasokudo, x : 75.0
				//�܂�A�W�����v����75px�㏸������A�~������
				speed = sqrt(0.0 - 2.0*kasokudo*75.0);

				//�����������x�̔{���ɂ���
				speed = speed - Amarif(speed, -kasokudo);
			}

			//�~����
			if (speed <= 0.0)
			{
				//���ɓ���������
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
					//������Ȃ��ʒu�܂ŏ㏸
					while (atari_hantei_jiki(Mdata, Jiki->pos) == TRUE)
					{
						Jiki->pos.y += 0.01;
					}

					//�X�s�[�h��0�ɂ���
					speed = 0.0;

					//���ɓ��B�������x��0�ɂ���
					kasokudo = 0.0;
				}
				else
				{
					//���@�̉������ł͂Ȃ��Ȃ������A���R����������悤�ɉ����x��-0.2�ɖ߂�
					kasokudo = -0.2;
				}
			}
			//�㏸��
			else
			{
				//�W�����v���ēV��ɓ���������
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
					//������Ȃ��ʒu�܂ō~��
					while (atari_hantei_jiki(Mdata, Jiki->pos) == TRUE)
					{
						Jiki->pos.y -= 0.01;
					}

					//���˕Ԃ�
					speed = -speed;
				}
			}
			//���@�̍��W�𑬂��������ړ�

			Jiki->pos.y += speed;

			speed += kasokudo;
		}
	}
}

//�J�����Z�b�g
void SetCam(JIKI Jiki, CAMERA Cam)
{
	SetCameraPositionAndTargetAndUpVec(
		Cam.pos,
		Jiki.pos,
		VGet(0.0f, 1.0f, 0.0f)
		);
}

//���f����\��
void DispMap(MAP_DATA Mdata, VECTOR Jiki)
{
	int i, j, k, Hdlp;			//�J�E���^�A���f���n���h���z��̓Y��
	int jikix, jikiy, jikih;	//���@�̃}�X���W

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
					//���f���n���h���̔z��̓Y�������擾
					Hdlp = Mdata.value[i].data[j] - B_NUM_START;

					//�u���b�N����̏ꍇ
					if (Hdlp != 0)
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

						//�u���b�N��`��
						MV1DrawModel(Model[Hdlp].modelh);
					}
				}
			}
		}
	}
}

//�X�^�[�g��ʗp�̃��f���\���֐�
void DispMapForStart(MAP_DATA Mdata)
{
	int i, j, k, Hdlp;			//�J�E���^�A���f���n���h���z��̓Y��

	for (i = 0; i < Mdata.youso; i++)
	{
		for (j = 0; j < MAP_MAX; j++)
		{
			if (j - MAP_ZERO >= -2)
			{
				//���f���n���h���̔z��̓Y�������擾
				Hdlp = Mdata.value[i].data[j] - B_NUM_START;

				//�u���b�N����̏ꍇ
				if (Hdlp != 0)
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

					//�u���b�N��`��
					MV1DrawModel(Model[Hdlp].modelh);
				}
			}
		}
	}
}

//�f�o�b�O��\��
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

//3D���W����\��
void DispJiku(void)
{
	//���W���B�Ԃ�x��, �΂�y��, ��z��
	DrawLine3D(VGet(500.0, 0.0, 0.0), VGet(-500.0, 0.0, 0.0), GetColor(255, 0, 0)); //x
	DrawLine3D(VGet(0.0, 500.0, 0.0), VGet(0.0, -500.0, 0.0), GetColor(0, 255, 0)); //y
	DrawLine3D(VGet(0.0, 0.0, 500.0), VGet(0.0, 0.0, -500.0), GetColor(0, 0, 255)); //z

	//���W���̐�[�̃R�[���i�~���j
	DrawCone3D(VGet(600.0, 0.0, 0.0), VGet(500.0, 0.0, 0.0), 20.0, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(VGet(0.0, 600.0, 0.0), VGet(0.0, 500.0, 0.0), 20.0, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
	DrawCone3D(VGet(0.0, 0.0, 600.0), VGet(0.0, 0.0, 500.0), 20.0, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);

	DrawFormatString(450, 10, GetColor(0,0,0), "���̌����F�Ԃ�x��, �΂�y��, ��z��");
}

//�}�b�v�f�[�^�������ƃ\�[�g����Ă��邩�ǂ������ׂ�f�o�b�O�p���O��ėp���O�t�@�C���ɏo��
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

	fprintf(Prog_Log_fp, "�ԍ��̌������F\n");

	for (i = 1; i <= modellen(); i++)
	{
		fprintf(Prog_Log_fp, "%d�ԁF%s\n", i+B_NUM_START, Model[i].name);
	}

	fprintf(Prog_Log_fp, "\n");

	for (xi = Mdata.pxmin; xi < Mdata.pxmax; xi++)
	{
		for (yi = Mdata.pymin; yi < Mdata.pymax; yi++)
		{
			f = mapfind(Mdata, xi, yi);

			if (f != -1)
			{
				fprintf(Prog_Log_fp, "�}�X���W�F(%d, %d)�F\n", xi, yi);
				for (hi = 0; hi < MAP_MAX; hi++)
				{
					fprintf(Prog_Log_fp, " %3d", Mdata.value[f].data[hi]);
				}
				fprintf(Prog_Log_fp, "\n");
			}
			else
			{
				fprintf(Prog_Log_fp, "�}�X���W�F(%d, %d)�F����\n", xi, yi);
			}
		}
	}
	fprintf(Prog_Log_fp, "EndOfDataLog--------------------------------------------------\n");
}