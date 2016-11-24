// DebugPort.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DebugPort.h"
#include <windows.h>
#include <Winternl.h>

typedef NTSTATUS (CALLBACK *NTQUERYINFORMATIONPROCESS)(  
	HANDLE ProcessHandle,   
	PROCESSINFOCLASS processInfo,
	PVOID ProcessInformation,  
	ULONG ProcessInformationLength,  
	PULONG ReturnLength OPTIONAL  
	);

BOOL  WINAPI  DetectDebugPort();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	if (DetectDebugPort())
	{
		MessageBoxW(NULL, L"Process is being debugged", L"NOTE", MB_OK);
	}
	else
	{
		MessageBoxW(NULL, L"Process is not being debugged", L"NOTE", MB_OK);
	}

	return 1;
}

// 检测调试端口，判断是否处于调试状态
BOOL WINAPI DetectDebugPort()
{
	BOOL bRet = FALSE;
	HMODULE hNtdll = NULL;
	NTQUERYINFORMATIONPROCESS pNtQueryInformationProcess = NULL;
	DWORD dwDebugPort = 0;

	__try
	{
		hNtdll = LoadLibraryW(L"ntdll.dll");
		if (NULL == hNtdll)
		{
			OutputDebugStringW(L"get ntdll handle failed!");
			__leave;
		}

		pNtQueryInformationProcess = (NTQUERYINFORMATIONPROCESS)GetProcAddress(hNtdll, "NtQueryInformationProcess");
		if (NULL == pNtQueryInformationProcess)
		{
			OutputDebugStringW(L"get NtQueryInformationProcess address failed!");
			__leave;
		}

		pNtQueryInformationProcess(GetCurrentProcess(), ProcessDebugPort, &dwDebugPort, sizeof(dwDebugPort), NULL);

		dwDebugPort == -1? bRet = TRUE:bRet = FALSE;
	}

	__finally
	{

	}

	return bRet;
}