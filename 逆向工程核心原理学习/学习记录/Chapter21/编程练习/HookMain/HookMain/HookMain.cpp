// HookMain.cpp
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>

#define DLL_NAME "KeyHook.dll" // dll's name
#define HOOKSTART "HookStart" // start function in DLL
#define HOOKSTOP "HookStop" // stop function

typedef void(*P_FUNC)(); // 函数指针

int main() {
	HMODULE hDll = NULL;
	P_FUNC HookStart = NULL;
	P_FUNC HookStop = NULL;
	
	// 加载KeyHook.dll
	hDll = LoadLibraryA(DLL_NAME);

	// 获取导出函数地址
	HookStart = (P_FUNC)GetProcAddress(hDll, HOOKSTART);
	HookStop = (P_FUNC)GetProcAddress(hDll, HOOKSTOP);

	//
	HookStart();
	printf("press 'q' to quit!\n");
	while (_getch() != 'q');

	//
	HookStop();

	// 卸载KeyHook.dll
	FreeLibrary(hDll);
	return 0;
}