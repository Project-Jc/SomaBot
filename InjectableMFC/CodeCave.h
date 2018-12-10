#pragma once

enum class PrimaryOpcode : BYTE {
	NOP = 0x90,
	CALL = 0xE8,
	JUMP = 0xE9
};

class CodeCave {
private:
	bool _isHooked;

	LPVOID _originalAddress;
	LPVOID _originalFunction;

	int _originalBytesCount;

	LPVOID _returnAddress;
public:
	CodeCave(void);

	bool Hook(LPVOID dwTarget, LPVOID dwDestination);
	bool Unhook(void);

	bool IsHooked(void) { return _isHooked; }
	LPVOID OriginalFunction(void) { return _originalFunction; }

	~CodeCave(void);
};