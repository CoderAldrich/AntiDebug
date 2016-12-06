// SeDebugPrivilege.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SeDebugPrivilege.h"
#include <Tlhelp32.h>
#include <windows.h>

BOOL WINAPI IsDebug()
{
	HANDLE hProcess = NULL;
	PROCESSENTRY32 Pe32 = {0};
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	Pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &Pe32))
	{
		do
		{
			if (_wcsicmp(L"csrss.exe", Pe32.szExeFile) == 0)
			{
				HANDLE hProcess = OpenProcess(
					PROCESS_ALL_ACCESS,
					FALSE,
					Pe32.th32ProcessID
					);

				if (hProcess)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
				CloseHandle(hProcess);
			}
		}
		while (Process32Next(hProcessSnap, &Pe32));
	}
	CloseHandle(hProcessSnap);
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