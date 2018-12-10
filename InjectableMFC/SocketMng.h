#pragma once

#define PREPARE_SOCKETMNG_CALL() __asm mov ecx, 0x006DC190;

#define CLIENT_NOTIFY() ::PostMessage(socketSink, WM_SOCKET_NOTIFY, GameSocket, FD_WRITE);

typedef void(WINAPI *tSend)(int size, LPBYTE bypData);

struct SHOP_ITEM {
	short Index;
	short Quantity;
	short Weight;
	DWORD Price;
	byte NameLength;
	CString Name;
	int pQ;

	bool operator==(const CString &name) { return Name == name; }
};

class SocketMng {
public:
	static tSend fSend;

	static DWORD GameSocket;
	static HWND socketSink;

	static void SendLogin();
	static void SendAccountLogin();

	// Items
	static void SendUseItem(short ItemIndex);
	static void PickUpItem(ITEMFIELD_DATA data);
	static void SwitchItem(short FromIndex, short ToIndex);
	static void SendRepair(short ItemIndex);

	// Npc
	static void OpenNpc(CUser *pUser);
	static void NpcDialogueSelect(short Selection);
	static void SendCraftItem();
	static void DepositItem(short InventoryIndex, short Amount);
	static void WithdrawItem(short WarehouseIndex, short Amount);
	static void ShopBuy(short ShopNumber, SHOP_ITEM Item);
	static void ShopSell(short ShopNumber, short ItemIndex, short Amount);

	// User
	static void ToggleStamina(bool OnOff);
	static void SendAttack();
	static void SendRevival(byte Type);
	static void SendLogOut(void);
	static void SendMagicAttack(int TargetUid, short SpellId);
	static void SendAura(SPECIALATTACK_DATA* Aura);
	static void SetStance(Stance Stance);
	static void ChangeDirection(Direction Direction);
	static void SendChat(CString Message);
	static void SendPm(CString User, CString Message);
	static void SendChat(byte Type, CString Recipient, CString Message);
	static void ThrowItem(short Index, short Count);
	static void ThrowMoney(DWORD Amount);
	static void PartyInviteResponse(byte Response);

	static void Test();
};



