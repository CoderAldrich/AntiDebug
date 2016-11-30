// PEBHeapFlags.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PEBHeapFlags.h"

BOOL WINAPI IsDebug()
{
	DWORD dwRet = 0;

	__asm
	{
		// heapflag正常情况下是2
		mov     eax, fs:[0x30]
		mov		eax, DWORD ptr [eax + 0x18] // PEB.ProcessHeap
		mov		eax, DWORD ptr [eax + 0x0c] // heap flag
		mov		dwRet, eax
	}

	return dwRet != 2;
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