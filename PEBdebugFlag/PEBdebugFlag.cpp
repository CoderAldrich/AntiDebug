// PEBdebugFlag.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PEBdebugFlag.h"

BOOL WINAPI IsDebug()
{
	DWORD bRet = 0;

	__asm
	{
		// IsDebuggerPresent函数原型，获取PEB地址，PEB第三个字节存放的调试标志
		mov     eax, fs:[0x30]
		movzx   eax,byte ptr ds:[eax+2]
		mov		bRet, eax
	}

	return bRet == TRUE;
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