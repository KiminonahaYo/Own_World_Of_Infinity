#ifndef FILECONTROL_DEFINED

#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <malloc.h>
#include <shellapi.h>
#include <windows.h>

#define FILE_PATH_LENGTH 256

#define DATAFILE 0
#define APPFILE 1

//プロトタイプ宣言
int filecopy(char *, char *);
void MakeDir(char *);
bool exist(char *);
void ExecCmd(char *, int, char *);

void dirlist(char[], char *, char *);
char *GetCurName(void);

//ファイルコピー
int filecopy(char *CopyMotoFileName, char *CopySakiFileName)
{
	FILE *MotoFilePtr;
	FILE *SakiFilePtr;
	unsigned char c;

	fopen_s(&MotoFilePtr, CopyMotoFileName, "rb");

	if (MotoFilePtr != NULL && strcmp(CopyMotoFileName, CopySakiFileName) != 0)
	{
		fopen_s(&SakiFilePtr, CopySakiFileName, "wb");

		if (SakiFilePtr != NULL)
		{
			while (fread(&c, sizeof(unsigned char), 1, MotoFilePtr) != 0)
			{
				fwrite(&c, sizeof(unsigned char), 1, SakiFilePtr);
			}

			//ファイルポインタ後始末
			fclose(SakiFilePtr);
		}

		//ファイルポインタ後始末
		fclose(MotoFilePtr);

		return 0;
	}
	else
	{
		return -1;
	}
}

void MakeDir(char *DirName)
{
	_mkdir(DirName);
}

bool exist(char *FileName)
{
	FILE *fp;
	fopen_s(&fp, FileName, "r");
	if (fp == NULL){
		return FALSE;
	}
	else{
		fclose(fp);
		return TRUE;
	}
}

void ExecCmd(char *command, int Mode, char *param = NULL)
{
	if (Mode)	ShellExecute(NULL, NULL, command, NULL, NULL, SW_SHOWNORMAL);
	else		ShellExecute(NULL, "open", command, param, NULL, SW_SHOWNORMAL);
}

void dirlist(char Dest[], char *FindDir, char *FindFile)
{
	WIN32_FIND_DATA find;
	HANDLE hFind;
	TCHAR szPath[MAX_PATH];
	LPTSTR p;

	//GetModuleFileName(NULL, szPath, MAX_PATH);	//実行ファイルのあるパスを取得
	lstrcpy(szPath, FindDir);

	p = _tcsrchr(szPath, _T('\\'));					//最後の\マークを検出
	*p = '\0';										//最後の\マークを消去
	lstrcat(szPath, TEXT("\\"));
	lstrcat(szPath, TEXT(FindFile));

	hFind = FindFirstFile(szPath, &find);			//実行ファイルのあるディレクトリを検索

	strcpy(Dest, "");

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//現在のディレクトリ(.)や前のディレクトリ(..)がパスとして出てしまうので除外
			if (lstrcmpi(find.cFileName, TEXT(".")) != 0 && lstrcmpi(find.cFileName, TEXT("..")) != 0)
			{
				//printfDx("%s\n", find.cFileName);	//ここで出力
				sprintf(Dest, "%s%s\n", Dest, find.cFileName);
			}
		} while (FindNextFile(hFind, &find));

		FindClose(hFind);
	}
}

char *GetCurName(void)
{
	static TCHAR szPath[MAX_PATH];

	GetModuleFileName(NULL, szPath, MAX_PATH);	//実行ファイルのあるパスを取得

	return szPath;
}

#define FILECONTROL_DEFINED
#endif
