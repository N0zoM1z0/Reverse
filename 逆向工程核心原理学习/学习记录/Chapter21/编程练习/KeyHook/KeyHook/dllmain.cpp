// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

// KeyHook.cpp
#include<stdio.h>
#include<Windows.h>

constexpr auto PROCESS_NAME = "notepad.exe";
HINSTANCE hInstance = NULL;
HHOOK hHook = NULL;
HWND hWnd = NULL;

BOOL APIENTRY DllMain( HMODULE hinstDLL,
                       DWORD  dwReason,
                       LPVOID lpReserved
                     )
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        hInstance = hinstDLL;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    char szPath[MAX_PATH] = { 0 };
    char* p = NULL;

    if (nCode == 0) {
        // bit 31 —— 0 = key press   ;   1 = key release
        if ((lParam & 0x80000000) == 0) {
            GetModuleFileNameA(NULL, szPath, MAX_PATH); // 获取当前进程的可执行文件路径
            p = strrchr(szPath, '\\');
            
            // 比较当前进程名称，若为`notepad.exe`，则消息不会传递给下一个钩子。
            if (!_stricmp(p + 1, PROCESS_NAME))
                return 1;
        }
    }

    // 若不是`notepad.exe`，则将消息传递给下一个钩子。
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport)void HookStart(){
        hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInstance, 0);
}
    __declspec(dllexport)void HookStop() {
        if (hHook) {
            // 注销挂钩
            UnhookWindowsHookEx(hHook);
            hHook = NULL;
        }
    }

#ifdef __cplusplus
}
#endif