#ifndef PROGRAMCONTROL_DXLIB_DEFINED

#include <stdlib.h>

void End(int);

//�����I��
void End(int Code)
{
	DxLib_End();
	exit(Code);
}

#define PROGRAMCONTROL_DXLIB_DEFINED
#endif
