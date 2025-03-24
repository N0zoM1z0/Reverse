#include<iostream>
#include<Windows.h>

#define LOAD_DLL "KeyHook.dll"
typedef void(*func)(); // 函数指针
signed main() {
	HMODULE hDll = NULL;
	func HookStart = NULL;
	func HookStop = NULL;
	char ch = 0;

	hDll = LoadLibraryA(LOAD_DLL);
	HookStart = (func)GetProcAddress(hDll, "HookStart");
	HookStop = (func)GetProcAddress(hDll, "HookStop");
	HookStart();
	HookStop();
	FreeLibrary(hDll);
	return 0;
}