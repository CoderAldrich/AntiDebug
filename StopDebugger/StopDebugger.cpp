// StopDebugger.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "StopDebugger.h"
#include <windows.h>
#include <winternl.h>

typedef NTSTATUS (CALLBACK *NtSetInformationThreadPtr)(HANDLE threadHandle,
											  THREADINFOCLASS threadInformationClass,
											  PVOID threadInformation,
											  ULONG threadInformationLength);

// 调试时有断点就会崩溃
void WINAPI StopDebegger()
{
	HMODULE hModule = LoadLibrary(TEXT("ntdll.dll"));
	NtSetInformationThreadPtr NtSetInformationThread = (NtSetInformationThreadPtr)GetProcAddress(hModule, "NtSetInformationThread");

	NtSetInformationThread(OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId()), (THREADINFOCLASS)0x11, 0, 0);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	StopDebegger();
	MessageBoxW(NULL, L"stopped debugger", L"AntiDebugger", MB_OK);
	return 1;
}