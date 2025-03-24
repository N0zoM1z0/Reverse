// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

// include
#include "stdio.h"
#include "wchar.h"
#include "windows.h"


// typedef
typedef BOOL(WINAPI* PFSETWINDOWTEXTW)(HWND hWnd, LPWSTR lpString);


// globals
FARPROC g_pOrgFunc = NULL;


// 自定义hook 修改lpString
BOOL WINAPI MySetWindowTextW(HWND hWnd, LPWSTR lpString)
{
    const wchar_t* pNum = L"零一二三四五六七八九";
    wchar_t temp[2] = { 0, };
    int i = 0, nLen = 0, nIndex = 0;

    nLen = wcslen(lpString);
    for (i = 0; i < nLen; i++)
    {

        if (L'0' <= lpString[i] && lpString[i] <= L'9')
        {
            temp[0] = lpString[i];
            nIndex = _wtoi(temp);
            lpString[i] = pNum[nIndex];
        }
    }

    // 返回调用 user32.dll中的 SetWindowTextW()
    return ((PFSETWINDOWTEXTW)g_pOrgFunc)(hWnd, lpString);
}


// hook_iat
// pfnOrg -> pfnNew
// 其实可以用winAPI来获取IAT地址
BOOL hook_iat(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
    HMODULE hMod;
    LPCSTR szLibName;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    PIMAGE_THUNK_DATA pThunk;
    DWORD dwOldProtect, dwRVA;
    PBYTE pAddr;

    // hMod, pAddr = ImageBase of calc.exe
    //             = VA to MZ signature (IMAGE_DOS_HEADER)
    hMod = GetModuleHandle(NULL);
    pAddr = (PBYTE)hMod;

    // pAddr = VA to PE signature (IMAGE_NT_HEADERS)
    pAddr += *((DWORD*)&pAddr[0x3C]);

    // dwRVA = RVA to IMAGE_IMPORT_DESCRIPTOR Table
    dwRVA = *((DWORD*)&pAddr[0x80]);

    // pImportDesc = VA to IMAGE_IMPORT_DESCRIPTOR Table
    pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hMod + dwRVA);

    for (; pImportDesc->Name; pImportDesc++)
    {
        // szLibName = VA to IMAGE_IMPORT_DESCRIPTOR.Name
        szLibName = (LPCSTR)((DWORD)hMod + pImportDesc->Name);
        if (!_stricmp(szLibName, szDllName))
        {
            // pThunk = IMAGE_IMPORT_DESCRIPTOR.FirstThunk
            //        = VA to IAT(Import Address Table)
            pThunk = (PIMAGE_THUNK_DATA)((DWORD)hMod +
                pImportDesc->FirstThunk);

            // pThunk->u1.Function = VA to API
            for (; pThunk->u1.Function; pThunk++)
            {
                if (pThunk->u1.Function == (DWORD)pfnOrg)
                {
                    // 将这块内存改为可读可写可执行
                    VirtualProtect((LPVOID)&pThunk->u1.Function,
                        4,
                        PAGE_EXECUTE_READWRITE,
                        &dwOldProtect);

                    // 改IAT表
                    pThunk->u1.Function = (DWORD)pfnNew;

                    // 改回原来的权限
                    VirtualProtect((LPVOID)&pThunk->u1.Function,
                        4,
                        dwOldProtect,
                        &dwOldProtect);

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // original API 주소 저장
        g_pOrgFunc = GetProcAddress(GetModuleHandle(L"user32.dll"),
            "SetWindowTextW");

        // # hook
        //   user32!SetWindowTextW() 를 hookiat!MySetWindowText() 로 후킹
        hook_iat("user32.dll", g_pOrgFunc, (PROC)MySetWindowTextW);
        break;

    case DLL_PROCESS_DETACH:
        // # unhook
        //   calc.exe 의 IAT 를 원래대로 복원
        hook_iat("user32.dll", (PROC)MySetWindowTextW, g_pOrgFunc);
        break;
    }

    return TRUE;
}