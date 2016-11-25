// ApiAntiDebug.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ApiAntiDebug.h"
#include <windows.h>

BOOL WINAPI IsDebug()
{
	DWORD dwRet = 0;
	BOOL bRet = FALSE;

	bRet = IsDebuggerPresent();

	__asm
	{
		// IsDebuggerPresent函数原型，获取PEB地址，PEB第三个字节存放的调试标志
		mov     eax, fs:[0x30]
		movzx   eax,byte ptr ds:[eax+2]
		mov		bRet, eax
	}

	// 查看制定进程是否被调试，本质还是调用NtQueryInformationProcess查看debugport
	CheckRemoteDebuggerPresent(OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId()), &bRet);

	return (bRet == 1) || (dwRet == 1);
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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