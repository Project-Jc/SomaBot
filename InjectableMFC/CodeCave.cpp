#include "stdafx.h"

#include "CodeCave.h"

CodeCave::CodeCave(void) {
	_isHooked = false;
}

bool CodeCave::Hook(LPVOID dwTarget, LPVOID dwDestination) {

	try {

		if (_isHooked) {
			throw "The code is already hooked.";
		}

		// Get the length of instructions to copy. We need at least 5 BYTEs for a jump.
		// If this takes more than 5 attempts, something is wrong.
		DISASM myDisasm = { 0 };
		int maximumAttempts = 5;

		while (_originalBytesCount < 5 && maximumAttempts-- > 0) {
			myDisasm.EIP = (UIntPtr) dwTarget + _originalBytesCount;
			_originalBytesCount += Disasm(&myDisasm);
		}

		// Change the protection to allow read/write/execute.
		DWORD dwOldProtect;
		if (!VirtualProtect(dwTarget, _originalBytesCount, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
			throw "Could not change protection.";
		}

		// Allocate memory for the code cave.
		_originalFunction = VirtualAlloc(NULL, _originalBytesCount + 5, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (_originalFunction == NULL) {
			throw "Could not allocate memory.";
		}

		// Copy the original memory to the code cave.
		memcpy(_originalFunction, dwTarget, _originalBytesCount);

		// Calculate the address to return to in the original memory (right after the target address).
		_returnAddress = (LPVOID) ((DWORD_PTR) dwTarget + _originalBytesCount);

		// Write a jump back to the return address from the code cave.
		*(BYTE *) ((DWORD) _originalFunction + _originalBytesCount) = (BYTE) PrimaryOpcode::JUMP;
		*(DWORD *) ((DWORD) _originalFunction + _originalBytesCount + 1) = (DWORD) _returnAddress - (DWORD) ((DWORD) _originalFunction + _originalBytesCount) - 5;

		// Replace the original memory with a jump to the code cave.
		*(BYTE *) dwTarget = (BYTE) PrimaryOpcode::JUMP;
		*(DWORD *) ((DWORD) dwTarget + 1) = (DWORD) dwDestination - (DWORD) dwTarget - 5;

		// NOP any left over BYTEs in the original memory.
		for (int i = 1; i <= (_originalBytesCount - 5); i++)
			*(BYTE *) ((DWORD) dwTarget + 4 + i) = (BYTE) PrimaryOpcode::NOP;

		// Change the protection of the original memory back to whatever it was.
		if (!VirtualProtect(dwTarget, _originalBytesCount, dwOldProtect, &dwOldProtect)) {
			throw "Could not change protection back.";
		}

		// Store the address of the original memory.
		_originalAddress = dwTarget;

		_isHooked = true;

		return true;

	} catch (LPCSTR msg) {
		std::cerr << __FUNCTION__ << ": " << msg << std::endl;
		return false;
	}
}

bool CodeCave::Unhook(void) {
	try {
		if (!_isHooked) {
			throw "The code is not hooked.";
		}

		DWORD dwOldProtect;

		// Change the protection to allow read/write/execute.
		if (!VirtualProtect(_originalAddress, _originalBytesCount, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
			throw "Could not change protection.";
		}

		// Put the original BYTEs back.
		memcpy(_originalAddress, _originalFunction, 5);

		// Change the protection back to whatever it was.
		if (!VirtualProtect(_originalAddress, _originalBytesCount, dwOldProtect, &dwOldProtect)) {
			throw "Could not change protection back.";
		}

		if (!VirtualFree(_originalFunction, _originalBytesCount + 5, MEM_RELEASE)) {
			//throw "Could not free memory.";
		}

		_isHooked = false;

		return true;
	} catch (LPCSTR msg) {
		std::cerr << __FUNCTION__ << ": " << msg << std::endl;
		return false;
	}
}

CodeCave::~CodeCave(void) {
	//Unhook();
}