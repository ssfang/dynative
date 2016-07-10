// WinX64CallingConvention.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int func_with_four_args(char cArg1, float fArg2, long lArg3, double dArg4){
	return cArg1 + (int)fArg2 + lArg3 + (int)dArg4;
}

int func_with_six_args(char cArg1, float fArg2, long lArg3, double dArg4, float fArg5, short sArg6){
	return cArg1 + (int)fArg2 + lArg3 + (int)dArg4 + (int)fArg5 + sArg6;
}

int _tmain(int argc, _TCHAR* argv[])
{
	union IntFlt
	{
		int i;
		float f;
	};
	IntFlt arg2 = { 0x2222 }, arg4 = { 0x4444 }, arg5 = { 0x5555 };

	func_with_four_args(argc, arg2.f, 0x33, arg4.f);

	func_with_six_args(argc, arg2.f, 0x33, arg4.f, arg5.f, 0x66);
	return 0;
}

