// GuartPage.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GuartPage.h"
#include <windows.h>

// 当执行一个属性为PAGE_GUARD的页面时，触发EXCEPTION_GUARD_PAGE的异常，如果存在调试器一般都是由调试器处理
BOOL WINAPI IsDebug()
{
	SYSTEM_INFO sSysInfo;
	DWORD dwPageSize = 0;
	DWORD OldProtect = 0;
	GetSystemInfo(&sSysInfo);
	dwPageSize = sSysInfo.dwPageSize;
	LPVOID lpvBase = VirtualAlloc(NULL, dwPageSize, MEM_COMMIT, PAGE_READWRITE);
	if (lpvBase == NULL)
	{
		return FALSE;
	}
	PBYTE lptmpB = (PBYTE)lpvBase;
	*lptmpB = 0xc3; 
	VirtualProtect(lpvBase, dwPageSize, PAGE_EXECUTE_READ | PAGE_GUARD, &OldProtect);
	__try
	{
		__asm  call dword ptr[lpvBase];
		VirtualFree(lpvBase, 0, MEM_RELEASE);
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		VirtualFree(lpvBase, 0, MEM_RELEASE);
		return FALSE;
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	if (*(BYTE*)IsDebug == 0xCC || *(BYTE*)IsDebug == 0x64)
	{
		return 0;
	}
	else if (IsDebug())
	{
		MessageBoxW(NULL, L"process is being debugged", L"AntiDebug", MB_OK);
	}
	else
	{
		MessageBoxW(NULL, L"process is not being debugged", L"AntiDebug", MB_OK);
	}
	return 1;
}