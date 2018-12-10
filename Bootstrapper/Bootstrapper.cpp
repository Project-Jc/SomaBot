#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <conio.h>
#include <Windows.h>

#define szDLLName "Dabura.dll"

int _tmain(int argc, _TCHAR* argv[]) {

	STARTUPINFO startupInfo = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION processInfo = { 0 };

	try {

		BOOL bProcessCreated = CreateProcess(L"C:\\Myth of Soma\\SomaWindow.exe", L"SomaWindow.exe 192.99.150.23", NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo);
		if (!bProcessCreated) {
			throw "Could not create process.";
		}

		FARPROC fLoadLibrary = GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryA");
		if (fLoadLibrary == NULL) {
			throw "Could not find LoadLibrary.";
		}

		LPVOID pAllocatedMemory = VirtualAllocEx(processInfo.hProcess, NULL, strlen(szDLLName) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (pAllocatedMemory == NULL) {
			throw "Could not allocate memory.";
		}

		BOOL bDataWritten = WriteProcessMemory(processInfo.hProcess, pAllocatedMemory, szDLLName, strlen(szDLLName) + 1, nullptr);
		if (!bDataWritten) {
			throw "Could not write memory.";
		}

		HANDLE hRemoteThread = CreateRemoteThread(processInfo.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)fLoadLibrary, pAllocatedMemory, NULL, NULL);
		if (hRemoteThread == NULL) {
			throw "Could not create remote thread.";
		}

		Sleep(1000);

		//_getch();

		DWORD dwSuspendCount = ResumeThread(processInfo.hThread);
		if (dwSuspendCount == -1) {
			throw "Could not resume process.";
		}

		//std::cout << "Bootstrapping completed successfully!" << std::endl;
	}
	catch (LPCSTR msg) {
		std::cerr << "Exception: " << std::endl << "\t" << __FUNCTION__ << std::endl << "\t" << msg << std::endl;
		_getch();
	}
	return 0;
}