// HeapTail.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HeapTail.h"
#include <windows.h>

BOOL WINAPI IsDebug()
{
	DWORD dwRet = 0;
	int flag[] = {0xabababab, 0xabababab};
	LPVOID buff = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 32);
	int temp = 0;

	temp = (int)buff + 32;

	// 堆末尾的8个字节是数据ABABABABABABABAB
	dwRet = memcmp((LPVOID)temp, (LPVOID)flag, 8);

	return dwRet == 0;
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