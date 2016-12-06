// POPF.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "POPF.h"

LONG WINAPI ExceptionFilter(
	PEXCEPTION_POINTERS ExceptionInfo
	)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		OutputDebugStringW(L"EXCEPTION_SINGLE_STEP");
		MessageBoxW(NULL, L"process is not being debugged", L"AntiDebug", MB_OK);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else
	{
		OutputDebugStringW(L"not EXCEPTION_SINGLE_STEP");
	}
}

int WINAPI AntiDebug()
{
	LPTOP_LEVEL_EXCEPTION_FILTER lpsetun;

	lpsetun = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);

	__asm
	{
		pushf 
		mov dword ptr [esp], 0x100
		popf
	}

	SetUnhandledExceptionFilter(lpsetun);

	return 1;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	LPTOP_LEVEL_EXCEPTION_FILTER lpsetun;

	lpsetun = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);
	if (*(BYTE*)AntiDebug == 0xCC || *(BYTE*)AntiDebug == 0x64)
	{
		return 0;
	}
	AntiDebug();
	SetUnhandledExceptionFilter(lpsetun);
	return 1;
}