#include<Windows.h>
#include<iostream>
#include<tchar.h>
#include<TlHelp32.h>
using namespace std;

#define PROC_NAME (L"notepad.exe")
#define DLL_NAME (L"Dll1.dll")

DWORD FindProcessID(LPCTSTR szProcessName) {
	DWORD dwPid = 0xFFFFFFFF;
	HANDLE hSnapshot;
	BOOL bMore;
	PROCESSENTRY32 pe;

	// 获取当前系统快照snapshot
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	// 查找进程
	bMore = Process32First(hSnapshot, &pe);
	// 遍历
	for (; bMore; bMore = Process32Next(hSnapshot, &pe)) {
		if (!_tcsicmp(szProcessName, (LPCTSTR)pe.szExeFile)) {
			dwPid = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return dwPid;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege) {
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		cerr << "OpenProcessToken ERROR: " << GetLastError() << "\n";
		return FALSE;
	}
	if (!LookupPrivilegeValue(
		NULL, // on local system
		lpszPrivilege, // privilege to lookup
		&luid
	)) {
		cerr << "LookupPrivilegeValue ERROR: " << GetLastError() << "\n";
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege) {
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else {
		tp.Privileges[0].Attributes = 0;
	}

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp, // new state
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL
	)) {
		cerr << "AdjustTokenPrivileges ERROR: " << GetLastError() << "\n";
		return FALSE;
	}
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		cerr << "The token does not have the specified privilege!\n";
		return FALSE;
	}
	
	return TRUE;
}

BOOL EjectDll(DWORD dwPid,LPCTSTR szDllName) {
	BOOL bMore = FALSE, bFound = FALSE;
	HANDLE hSnapshot, hProcess, hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = { sizeof(me) };
	LPTHREAD_START_ROUTINE pThreadProc;

	// dwPid: notepad.exe进程的PID
	// 使用 TH32CS_SNAPMODULE参数，获取加载到notepad.exe进程的DLL名称
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	bMore = Module32First(hSnapshot, &me);
	// 遍历所有模块
	for (; bMore; bMore = Module32Next(hSnapshot, &me) ){
		if (!_tcsicmp((LPCTSTR)me.szModule, szDllName) ||
			!_tcsicmp((LPCTSTR)me.szExePath, szDllName )){
			bFound = TRUE;
			break;
		}
	}
	if (!bFound) {
		CloseHandle(hSnapshot);
		return FALSE;
	}
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid))) {
		cerr << "OpenProcess failed!\n";
		return FALSE;
	}

	hModule = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
	hThread = CreateRemoteThread(
		hProcess,
		NULL,
		0,
		pThreadProc,
		me.modBaseAddr,
		0,
		NULL
	);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);
	return TRUE;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwPid = 0xFFFFFFFF;
	dwPid = FindProcessID(PROC_NAME);
	if (dwPid == 0xFFFFFFFF) {
		cerr << "No process " << PROC_NAME << "!\n";
		return 1;
	}
	cout << "PID: " << dwPid << "\n";

	// 更改privilege
	if (!SetPrivilege(SE_DEBUG_NAME, TRUE)) // 设置DEBUG权限
		return 1;

	// Eject Dll
	if (EjectDll(dwPid, DLL_NAME))
		cout << "Success!\n";
	else
		cout << "Failed!\n";
	
	return 1 ^ 1;
}