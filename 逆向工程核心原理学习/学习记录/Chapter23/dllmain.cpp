// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include<Windows.h>
#include<urlmon.h>

#pragma comment (lib,"Urlmon.lib")
#define URL (L"https://www.runoob.com")
#define FILENAME (L"C:\\Users\\Administrator\\Desktop\\down.html")

DWORD WINAPI down(LPVOID lpParam) {
    HRESULT hResult = URLDownloadToFileW(NULL, URL, FILENAME, 0, NULL);
    if (hResult == S_OK) {
        OutputDebugString(L"下载成功！\n");
    }
    else {
        OutputDebugString(L"下载失败！\n");
        return 1;
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(NULL, "DLL Injected!\n", "REVERSING", MB_OK);
        HANDLE hThread = CreateThread(NULL, 0, &down, NULL, 0, NULL);
        CloseHandle(hThread);
        break;
    }
    return TRUE;
}