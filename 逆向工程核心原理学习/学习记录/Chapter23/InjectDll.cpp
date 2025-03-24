#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<tchar.h>
#include<iostream>
using namespace std;

void Inject(DWORD dwPid, WCHAR* szPath) {
	DWORD dwBufSize = (DWORD)(wcslen(szPath) + 1) * sizeof(WCHAR);
	// 1. OpenProcess()
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

	// 2. VirtualAllocEx()
	LPVOID pRemoteAddress = VirtualAllocEx(
		hProcess,
		NULL,
		dwBufSize,
		MEM_COMMIT,
		PAGE_READWRITE
	);

	// 3. WriteProcessMemory()
	WriteProcessMemory(
		hProcess, pRemoteAddress, szPath, dwBufSize, NULL
	);

	// 4. GET LoadLibraryW
	HMODULE hK32 = GetModuleHandle(L"kernel32.dll");
	LPVOID pLW = GetProcAddress(hK32,"LoadLibraryW");
	cout << "loadAdd:" << pLW << "\n";

	// 5. CreateRemoteThread()
	HANDLE hThread = CreateRemoteThread(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pLW,
		pRemoteAddress,
		0,
		NULL
	);
	if (!hThread)
	{
		printf("CreateRemoteThread Failed");
	}

	WaitForSingleObject(hThread, -1);
	VirtualFreeEx(hProcess, pRemoteAddress, dwBufSize, MEM_DECOMMIT);
}
int _tmain(int argc, _TCHAR* argv[]) {
	wchar_t wStr[] = L"C:\\Users\\Administrator\\Desktop\\逆向练习\\DLL Injection\\dll\\Dll1\\x64\\Debug\\Dll1.dll";
	DWORD dwPid = 0;
	HWND hNotepad = FindWindowA("Notepad", NULL); // 首字母大写。。

	// get PID
	DWORD dwRub = GetWindowThreadProcessId(hNotepad, &dwPid);
	printf("目标窗口的进程PID为 : %d\n", dwPid);

	Inject(dwPid, wStr);

	system("pause");
	return 0;
}