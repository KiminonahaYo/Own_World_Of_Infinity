#ifndef MYTIMEFORSTD_DEFINED

#include <time.h>
#include <stdio.h>
#include <string.h>

//���Ԏ擾�̂��߂̍\����
typedef struct
{
	int Year, Mon, Day;
	int Hour, Min, Sec;
	int Week;
}
DateAndTime;

//�v���g�^�C�v�錾
void GetTime(DateAndTime *);
char *GetWeek(int, char *);

int  Get12Hour(int);
char *GetAMorPM(int);

char *GetJapaneseYear(int, char *);

//���Ԏ擾�����₷������֐�
void GetTime(DateAndTime *Date)
{
	time_t timer;
	struct tm local;

	timer = time(NULL);
	localtime_s(&local, &timer);

	Date->Year = local.tm_year + 1900;
	Date->Mon = local.tm_mon + 1;
	Date->Day = local.tm_mday;
	Date->Hour = local.tm_hour;
	Date->Min = local.tm_min;
	Date->Sec = local.tm_sec;
	Date->Week = local.tm_wday;

	return;
}

//GetTime()�֐��œ����j���̃t���O��^���ĕ�����ŕԂ��֐�
//JS_S:���{��V���[�g�A	JS:���{��
//US_S:�p��V���[�g�A	US:�p��
char *GetWeek(int WeekDayFlag, char *Mode)
{
	if (strcmp(Mode, "JS_S") == 0)
	{
		switch (WeekDayFlag)
		{
		case 0: return "��"; break;
		case 1: return "��"; break;
		case 2: return "��"; break;
		case 3: return "��"; break;
		case 4: return "��"; break;
		case 5: return "��"; break;
		case 6: return "�y"; break;
		default: return ""; break;
		}
	}
	else if (strcmp(Mode, "JS") == 0)
	{
		switch (WeekDayFlag)
		{
		case 0: return "���j��"; break;
		case 1: return "���j��"; break;
		case 2: return "�Ηj��"; break;
		case 3: return "���j��"; break;
		case 4: return "�ؗj��"; break;
		case 5: return "���j��"; break;
		case 6: return "�y�j��"; break;
		default: return ""; break;
		}
	}
	else if (strcmp(Mode, "US_S") == 0)
	{
		switch (WeekDayFlag)
		{
		case 0: return "Sun"; break;
		case 1: return "Mon"; break;
		case 2: return "Tue"; break;
		case 3: return "Wed"; break;
		case 4: return "Thu"; break;
		case 5: return "Fri"; break;
		case 6: return "Sat"; break;
		default: return ""; break;
		}
	}
	else if (strcmp(Mode, "US") == 0)
	{
		switch (WeekDayFlag)
		{
		case 0: return "Sunday"; break;
		case 1: return "Monday"; break;
		case 2: return "Tuesday"; break;
		case 3: return "Wednesday"; break;
		case 4: return "Thursday"; break;
		case 5: return "Friday"; break;
		case 6: return "Saturday"; break;
		default: return ""; break;
		}
	}
	else
	{
		return "";
	}
}

//12���ԕ\���ɂ���
int Get12Hour(int Hour)
{
	return Hour % 12;
}

//AM-PM���擾����
char *GetAMorPM(int Hour)
{
	return (Hour / 12 == 0 ? "AM" : (Hour / 12 == 1 ? "PM" : ""));
}

//�N�����擾����
char *GetJapaneseYear(int Year, char *Mode)
{
	static char Ret[64];
	char *Nengo;
	int JYear;

	if (Year >= 1900)
	{
		if (strcmp(Mode, "A") == 0)
		{
			if (Year < 1912)		Nengo = "M";
			else if (Year < 1926)	Nengo = "T";
			else if (Year < 1989)	Nengo = "S";
			else				Nengo = "H";
		}
		else if (strcmp(Mode, "K") == 0)
		{
			if (Year < 1912)		Nengo = "����";
			else if (Year < 1926)	Nengo = "�吳";
			else if (Year < 1989)	Nengo = "���a";
			else				Nengo = "����";
		}
		else return "";

		if (Year < 1912)		JYear = Year - 1868 + 1;
		else if (Year < 1926)	JYear = Year - 1912 + 1;
		else if (Year < 1989)	JYear = Year - 1926 + 1;
		else				JYear = Year - 1989 + 1;

		sprintf_s(Ret, 64, "%s%d", Nengo, JYear);

		return Ret;
	}
	else
	{
		return "";
	}
}

#define MYTIMEFORSTD_DEFINED
#endif
