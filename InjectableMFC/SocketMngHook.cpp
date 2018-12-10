#include "stdafx.h"
#include "SocketMngHook.h"

CodeCave SocketMngHook::CC::Send;
CodeCave SocketMngHook::CC::RawSend;

// Initiate the hook.
bool SocketMngHook::Init(void) {
	// Find the executable.
	HMODULE hMod_Soma = NULL;

	for (; hMod_Soma == NULL; Sleep(10)) {
		hMod_Soma = GetModuleHandle(NULL);
	}

	//// Store the address of SocketMng::Send so that it can be called later.
	SocketMng::fSend = (tSend)((DWORD_PTR)hMod_Soma + 0x000D99D0);

	// Hook SocketMng.Send.
	//CC::Send.Hook((LPVOID)((DWORD_PTR)hMod_Soma + 0x000D99D0), MY::Send);

	// Hook SocketMng.RawSend.
	//CC::RawSend.Hook((LPVOID)((DWORD_PTR)hMod_Soma + 0x000D9DE0), MY::RawSend);

	return true;
}

void SocketMngHook::Exit(void) {
	if (CC::Send.IsHooked()) {
		CC::Send.Unhook();
	}
}

VOID WINAPI SocketMngHook::MY::Send(int size, LPBYTE bypData) {

	switch ((PacketType)bypData[0]) {
		case PacketType::ENCRYPTION_START_RESULT:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::ENCRYPTION_START_RESULT");
			break;
		case PacketType::LOGIN:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::LOGIN");
			break;
		case PacketType::ACOUNT_LOGIN:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::ACOUNT_LOGIN");
			break;
		case PacketType::LIVEBROADCAST:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::LIVEBROADCAST");
			break;
		case PacketType::GAMESTART:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::GAMESTART");
			break;
		case PacketType::USERMODIFY:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::USERMODIFY");
			break;
		case PacketType::NEW_HERO:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::NEW_HERO");
			break;
		case PacketType::QUEST_VIEW:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::QUEST_VIEW");
			break;
		case PacketType::MOVEFIRST:
		case PacketType::MOVEMIDDLE:
		case PacketType::MOVEEND:
		case PacketType::RUN_MOVEFIRST:
		case PacketType::RUN_MOVEMIDDLE:
		case PacketType::RUN_MOVEEND:
			Bot::Print(Debug::SOCKETMNG, "SendPacketType::PKT_MOVE");
			break;
		default: {
				Bot::Print(Debug::SOCKETMNG, "Sent unknown packet\tSize [%d]", size);
				for (int i = 0; i < size; i++) {
					if (i == 0) {
						printf_s("\t\t");
					} printf_s("%X ", bypData[i]);
				} printf_s("\n\n");
			} 
			break;
	}

	//Call the original function. 
	PREPARE_SOCKETMNG_CALL();
	((ttSend)CC::Send.OriginalFunction())(size, bypData);
}

VOID WINAPI SocketMngHook::MY::RawSend(int size, PBYTE bypData) {

	WORD RawPktType = *(WORD *)bypData;

	//printf_s("RawPktType [%X] Casted [%x] [%x]\n", RawPktType, (RawPacketType)RawPktType, RawPacketType::SM_GAME_REQ);

	switch (RawPktType) {
		case 0x8016:
			Bot::Print(Debug::SOCKETMNG, "SendRawPacketType::SM_DIR_REQ");
			break;
		case SM_LOGIN_REQ:
			Bot::Print(Debug::SOCKETMNG, "SendRawPacketType::SM_LOGIN_REQ");
			break;
		case SM_GAME_REQ:
			Bot::Print(Debug::SOCKETMNG, "SendRawPacketType::SM_GAME_REQ");
			break;
		case SM_AFK:
			Bot::Print(Debug::SOCKETMNG, "Prevented AFK packet");
			return;
		default:	
			Bot::Print(Debug::SOCKETMNG, "SendRawPacketType [%X] [%d]", RawPktType, RawPktType);
			break;
	}

	//Call the original function. 
	PREPARE_SOCKETMNG_CALL();
	((ttRawSend)CC::RawSend.OriginalFunction())(size, bypData);
}