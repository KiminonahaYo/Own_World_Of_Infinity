#ifndef MYSTRING_DEFINED

#include <malloc.h>

typedef char** SPLITDATA;

//�v���g�^�C�v�錾
void delsplit(int, SPLITDATA);
int split(char*, char*, SPLITDATA*);

//split�Ŋm�ۂ����z����J��
void delsplit(int len, SPLITDATA SplitedData)
{
	int i;
	for (i = 0; i < len; i++)
		free(SplitedData[i]);
	free(SplitedData);
}

//��؂蕶�����w�肵�āAsplitedData�ɋ敪�����Ċi�[
int split(char *str, char *kugiri, SPLITDATA *splitedData)
{
	const int slen = strlen(str);
	const int klen = strlen(kugiri);
	char *temp;

	int i, j;
	int strc = 1, chrc = 1;

	(*splitedData) = (char**)malloc(sizeof(char*));
	(*splitedData)[0] = (char*)malloc(sizeof(char));
	(*splitedData)[0][0] = '\0';

	temp = (char*)malloc(sizeof(char)*klen);

	for (i = 0; i < slen;/* i++ */)
	{

		for (j = 0; j < klen && i + klen - 1 < slen; j++)
		{
			temp[j] = str[i + j];
		}
		temp[j] = '\0';
		if (i + klen - 1 < slen && strcmp(kugiri, temp) == 0)
		{
			strc++;
			chrc = 1;
			(*splitedData) = (char**)realloc(*splitedData, sizeof(char*)*strc);
			(*splitedData)[strc - 1] = (char*)malloc(sizeof(char));
			(*splitedData)[strc - 1][0] = '\0';
			i += klen;
		}
		else
		{
			chrc++;
			(*splitedData)[strc - 1] = (char*)realloc((*splitedData)[strc - 1], sizeof(char)*chrc);
			(*splitedData)[strc - 1][chrc - 2] = str[i];
			(*splitedData)[strc - 1][chrc - 1] = '\0';
			i++;
		}

	}

	return strc;
}

#define MYSTRING_DEFINED
#endif
