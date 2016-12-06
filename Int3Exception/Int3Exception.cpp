// Int3Exception.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Int3Exception.h"
#include <windows.h>

// 人为制造一个异常，当程序处于调试状态时会把异常传递给调试器，当正常运行程序时，所需要跑的正常流程放在异常处理中得到执行
int WINAPI AntiDebug()
{
	
	__asm 
	{
		// __try{}__exception{}的汇编形式
		push   offset exception_handler; //set exception handler
			push  dword ptr fs:[0h]
		mov    dword ptr fs:[0h],esp  
			xor   eax,eax;//reset EAX invoke int3
			int    3h
			pop    dword ptr fs:[0h];//restore exception handler
			add   esp,4

			test   eax,eax;// check the flag 
			je    rt_label
			jmp    rf_label

exception_handler:
		mov   eax,dword ptr [esp+0xc];//EAX = ContextRecord
			mov    dword ptr [eax+0xb0],0xffffffff;//set flag (ContextRecord.EAX)
			inc   dword ptr [eax+0xb8];//set ContextRecord.EIP
			xor   eax,eax
			retn

rt_label:
		xor eax,eax
			inc eax
			mov esp,ebp
			pop ebp
			retn
rf_label:
		xor eax,eax
			mov esp,ebp
			pop ebp
			retn
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	if (*(BYTE*)AntiDebug == 0xCC || *(BYTE*)AntiDebug == 0x64)
	{
		return 0;
	}
	else if (AntiDebug())
	{
		MessageBoxW(NULL, L"process is being debugged", L"AntiDebug", MB_OK);
	}
	else
	{
		MessageBoxW(NULL, L"process is not being debugged", L"AntiDebug", MB_OK);
	}
	return 1;
}