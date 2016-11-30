// PEBNtGlobalFlags.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PEBNtGlobalFlags.h"

BOOL WINAPI IsDebug()
{
	BOOL bRet = FALSE;
	// 4. 查看PEB的NtGlobalFlags标志位
	__asm
	{
		mov eax, fs:[30h]
		mov eax, [eax+68h]
		and eax, 0x70
		mov bRet, eax
	}

	return bRet != 0;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	if (IsDebug())
	{
		MessageBoxW(NULL, L"process is being debugged", L"AntiDebug", MB_OK);
	}
	else
	{
		MessageBoxW(NULL, L"process is not being debugged", L"AntiDebug", MB_OK);
	}
	return 1;
}