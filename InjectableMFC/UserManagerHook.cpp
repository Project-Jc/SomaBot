#include "stdafx.h"
#include "UserManagerHook.h"

// Initiate the hook.
bool UserManagerHook::Init(void) {
	// Find the executable.
	HMODULE hMod_Soma = NULL;

	for (; hMod_Soma == NULL; Sleep(10)) {
		hMod_Soma = GetModuleHandle(NULL);
	}

	// Store the address of UserManager::GetMe so that it can be called later.
	UserManager::fGetMe = (tGetMe)((DWORD_PTR)hMod_Soma + 0x00100F70);

	// Store the address of UserManager::GetUserWithUid so that it can be called later.
	UserManager::fGetUserWithUid = (tGetUserWithUid)((DWORD_PTR)hMod_Soma + 0x000F7FA0);

	// Store the address of PacketParser::MoveRequest so that it can be called later.
	UserManager::fMoveRequest = (tMoveRequest)((DWORD_PTR)hMod_Soma + 0x000D2CF0);

	// Store the address of PacketParser::MoveRunRequest so that it can be called later.
	UserManager::fMoveRunRequest = (tMoveRequest)((DWORD_PTR)hMod_Soma + 0x000D2F00);

	// Store the address of CPathFind::IsMovable so that it can be called later.
	UserManager::fIsMoveable = (tIsMoveable)((DWORD_PTR)hMod_Soma + 0x000D6780);

	return true;
}

void UserManagerHook::Exit(void) {

}