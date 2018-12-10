#pragma once

typedef VOID(WINAPI *ttSend)(int size, LPBYTE bypData);
typedef VOID(WINAPI *ttRawSend)(int size, PBYTE bypData);

class SocketMngHook {
private:
public:
	static bool Init(void);
	static void Exit(void);

	struct CC {
		static CodeCave Send;
		static CodeCave RawSend;
	};

	struct MY {
		static VOID WINAPI Send(int size, LPBYTE bypData);
		static VOID WINAPI RawSend(int size, LPBYTE bypData);
	};
};
