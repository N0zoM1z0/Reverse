#include<windows.h>
#include<stdio.h>
/*
定义好结构，简化操作
*/
typedef struct _THREAD_PARAMETER {
	FARPROC pFunc[2]; // Func
	char szBuf[4][128]; // Parameter
}THREAD_PARAM, * PTHREAD_PARAM;

// LoadLibraryA
typedef HMODULE(WINAPI* PFLoadLibraryA)(
	LPCSTR lpLibFileName
	);

// GetProcAddress
typedef FARPROC(WINAPI* PFGetProcAddress)(
	HMODULE hModule,
	LPCSTR lpProcName
	);

// MessageBoxA
typedef int(WINAPI* PFMessageBoxA)(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType
	);

DWORD WINAPI ThreadProc(LPVOID lParam) {
	PTHREAD_PARAM pParam = (PTHREAD_PARAM)lParam;
	HMODULE hModule = NULL;
	FARPROC pFunc = NULL;

	// LoadLibraryA("user32.dll")
	hModule = ((PFLoadLibraryA)(pParam->pFunc[0]))(pParam->szBuf[0]);

	// GetProcAddress("MessageBoxA")
	pFunc = (FARPROC)((PFGetProcAddress)(pParam->pFunc[1]))(hModule, pParam->szBuf[1]);

	// MessageBoxA(0,text,caption,MB_OK)
	((PFMessageBoxA)pFunc)(NULL, pParam->szBuf[2], pParam->szBuf[3], MB_OK);

	return 0;
}

BOOL InjectCode(DWORD dwPid) {
	HMODULE hModule = NULL;
	THREAD_PARAM param = { 0, };
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	LPVOID pRemoteBuf[2] = { 0, };
	DWORD dwSize = 0;

	// 设置参数

	hModule = GetModuleHandleA("kernel32.dll");
	param.pFunc[0] = GetProcAddress(hModule, "LoadLibraryA");
	param.pFunc[1] = GetProcAddress(hModule, "GetProcAddress");
	strcpy_s(param.szBuf[0], "user32.dll");
	strcpy_s(param.szBuf[1], "MessageBoxA");
	strcpy_s(param.szBuf[2], "INJECTED!");
	strcpy_s(param.szBuf[3], "N0zoM1z0");

	// OpenProcess
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return FALSE;
	}
	// VirtualAllocEx For Param
	dwSize = sizeof(THREAD_PARAM);
	pRemoteBuf[0] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);

	// WriteProcessMemory
	BOOL OK = WriteProcessMemory(hProcess, pRemoteBuf[0], (LPVOID)&param, dwSize, NULL);
	if (!OK) {
		return FALSE;
	}
	// VirtualAllocEx For ThreadProc
	dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;
	pRemoteBuf[1] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	// WriteProcessMemory
	OK = WriteProcessMemory(hProcess, pRemoteBuf[1], (LPVOID)ThreadProc, dwSize, NULL);
	if (!OK) {
		return FALSE;
	}
	// CreateRemoteThread
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteBuf[1], pRemoteBuf[0], 0, NULL);
	if (hThread == NULL) {
		return FALSE;
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

int main(int argc, CHAR* argv[]) {
	if (argc < 2) {
		printf("Usage: %s pid\n", argv[0]);
		return 1;
	}

	DWORD dwPid = (DWORD)atol((LPCSTR)argv[1]);
	InjectCode(dwPid);
	return 0;
}