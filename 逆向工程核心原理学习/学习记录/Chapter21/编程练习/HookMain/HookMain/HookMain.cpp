// HookMain.cpp
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>

#define DLL_NAME "KeyHook.dll" // dll's name
#define HOOKSTART "HookStart" // start function in DLL
#define HOOKSTOP "HookStop" // stop function

typedef void(*P_FUNC)(); // ����ָ��

int main() {
	HMODULE hDll = NULL;
	P_FUNC HookStart = NULL;
	P_FUNC HookStop = NULL;
	
	// ����KeyHook.dll
	hDll = LoadLibraryA(DLL_NAME);

	// ��ȡ����������ַ
	HookStart = (P_FUNC)GetProcAddress(hDll, HOOKSTART);
	HookStop = (P_FUNC)GetProcAddress(hDll, HOOKSTOP);

	//
	HookStart();
	printf("press 'q' to quit!\n");
	while (_getch() != 'q');

	//
	HookStop();

	// ж��KeyHook.dll
	FreeLibrary(hDll);
	return 0;
}