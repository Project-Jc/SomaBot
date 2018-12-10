#include "stdafx.h"
#include "PacketParserHook.h"

tSend SocketMng::fSend = NULL;

HWND SocketMng::socketSink = NULL;
DWORD SocketMng::GameSocket = NULL;

DWORD LastAttackTick;

void SocketMng::SendLogin() {
	//BYTE buf[100];
	//int offset = 0;

	//MP_AddByte(buf, offset, PKT_LOGIN);
	//MP_AddByte(buf, offset, m_szLoginId.GetLength());
	//MP_AddString(buf, offset, (PBYTE)(LPCTSTR)m_szLoginId, m_szLoginId.GetLength());
	////TRACE("Sent Character Login... ID: %s\n", (LPCTSTR)m_szLoginId);
	//ASSERT(offset<100);
	//Send(offset, buf);

	Bot::Print(Debug::USER, "Sending Login");

	CString Id = "ASSUME";

	BYTE buff[20];

	buff[0] = (BYTE)PacketType::LOGIN;
	buff[1] = (BYTE)Id.GetLength();

	CopyMemory(buff + 1, (PBYTE)(LPCTSTR)Id, (BYTE)Id.GetLength());

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendAccountLogin() {
	//MP_AddByte(buf, offset, PKT_ACOUNT_LOGIN);
	//MP_AddByte(buf, offset, m_szAcountLoginId.GetLength());
	//MP_AddString(buf, offset, (PBYTE)(LPCTSTR)m_szAcountLoginId, m_szAcountLoginId.GetLength());
	//TRACE("Sent Account Login ID: %s\n", (LPCTSTR)m_szAcountLoginId);
	//MP_AddByte(buf, offset, m_szPasswd.GetLength());
	//MP_AddString(buf, offset, (PBYTE)(LPCTSTR)m_szPasswd, m_szPasswd.GetLength());
	//ASSERT(offset<100);
	//Send(offset, buf);

	Bot::Print(Debug::USER, "Sending Account Login");

	CString AccId = "brecon111", PassWd = "qweasdzxc";

	BYTE buff[20];

	buff[0] = (BYTE)PacketType::ACOUNT_LOGIN;
	buff[1] = AccId.GetLength();
	buff[2] = 1;
	buff[7] = PassWd.GetLength();
	buff[8] = 2;

	PREPARE_SOCKETMNG_CALL();

	fSend(9, buff);

	CLIENT_NOTIFY();
}

// Items.
void SocketMng::SendUseItem(short ItemIndex) {

	BYTE buff[10] = { 
		
		(BYTE)PacketType::ITEM_USE, 
		
		ItemIndex >= INV_COUNT ? 
			(ItemIndex -= INV_COUNT, buff[1] = BELT) : 
			buff[1] = INVENTORY, 
			
			((byte*)&ItemIndex)[0], ((byte*)&ItemIndex)[1] 
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(4, buff);

	CLIENT_NOTIFY();
}
void SocketMng::PickUpItem(ITEMFIELD_DATA data) {

	Bot::Print(Debug::LOOT, "Picking up [%s] from [X: %d Y: %d]", data.Name, data.Point, data.Point);

	BYTE buff[10] = { 
		
		(BYTE)PacketType::ITEM_PICKUP, 
		
		((byte*)&data.Point.x)[0], ((byte*)&data.Point.x)[1],
		
		((byte*)&data.Point.y)[0], ((byte*)&data.Point.y)[1] 
	
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();

	Bot::Loot.Increment();
}
void SocketMng::SwitchItem(short FromIndex, short ToIndex) {

	Bot::Print(Debug::USER, "Sending item [%s] from index [%d] to [%d]", BotUser::Inventory.Item.Name(FromIndex), FromIndex, ToIndex);

	BYTE buff[10] = { (BYTE)PacketType::CHANGE_ITEM_INDEX, ((byte*)&FromIndex)[0], ((byte*)&FromIndex)[1], ((byte*)&ToIndex)[0], ((byte*)&ToIndex)[1] };

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendRepair(short ItemIndex) {

	Bot::Print(Debug::USER, "Repairing item [%s] at index [%d]", BotUser::Inventory.Item.Name(ItemIndex), ItemIndex);

	BYTE buff[10] = { 
		
		(BYTE)PacketType::REPAIR_REQ, 
		
		((byte*)&ItemIndex)[0], ((byte*)&ItemIndex)[1] 
	
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(3, buff);

	CLIENT_NOTIFY();
}

// Npc.
void SocketMng::OpenNpc(CUser *pUser) {

	if (!Bot::State.IsOpeningNpc) {

		Bot::Print(Debug::NPC, "Opening NPC [%s] [Uid: %d] [X: %d] [Y: %d]", pUser->Data->Name.GetString(), pUser->Data->Uid, pUser->Data->CellPos.x, pUser->Data->CellPos.y);

		int Uid = pUser->Data->Uid;

		BYTE buff[10] = { 
			
			(BYTE)PacketType::CLIENTEVENT,
			
			0x02, 
			
			((byte*)&Uid)[0], ((byte*)&Uid)[1], ((byte*)&Uid)[2], ((byte*)&Uid)[3]
		
		};

		PREPARE_SOCKETMNG_CALL();

		fSend(6, buff);

		CLIENT_NOTIFY();

		Bot::State.IsOpeningNpc = true;
	}
}
void SocketMng::NpcDialogueSelect(short Selection) {

	Bot::Print(Debug::GENERAL, "Sending dialogue selection [%d]", Selection);

	BYTE buff[10] = { (BYTE)PacketType::NPC_DIALOGUE_SELECTION, Selection };

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendCraftItem() {

	int Size;
	BYTE buff[100] = { (byte)PacketType::CRAFTITEM };

	// Need to update index to use the exact index where the materials are located.

	// Armour.
	if (CraftBot::Get.Type() == "Armour") {

		if (CraftBot::Get.Recipe() == "Cheap Shoes") {

			// The type of craft.
			buff[1] = (short)Craft::SHOES;
			// Gender.
			buff[3] = (byte)Gender::FEMALE;

			CString Name = "Shoe";
			// Name length.
			buff[4] = (byte)Name.GetLength();
			// Name.
			CopyMemory(buff + 5, (PBYTE)(LPCTSTR)Name, buff[4]);

			// Total number of individual items to send.
			buff[9] = 2;
			// The first items inventory index.
			buff[10] = (short)10;
			// The amount of the first item to send.
			buff[12] = (short)1;
			// The second items inventory index.
			buff[14] = (short)11;
			// The amount of the second item to send.
			buff[16] = (short)2;

			Size = 18;
		}
	}

	// Weapon.
	else if (CraftBot::Get.Type() == "Weapon") {

		if (CraftBot::Get.Recipe() == "Dagger") {

			// The type of craft.
			buff[1] = (short)Craft::SWORD;
			// Gender.
			buff[3] = (byte)Gender::UNISEX;

			CString Name = "Weap";
			// Name length.
			buff[4] = (byte)Name.GetLength();
			// Name.
			CopyMemory(buff + 5, (PBYTE)(LPCTSTR)Name, buff[4]);

			// Total number of individual items to send.
			buff[9] = 2;
			// The first items inventory index.
			buff[10] = (short)10;
			// The amount of the first item to send.
			buff[12] = (short)1;
			// The second items inventory index.
			buff[14] = (short)11;
			// The amount of the second item to send.
			buff[16] = (short)3;

			Size = 18;
		}
	}

	// Accessory.
	else if (CraftBot::Get.Type() == "Accessory") {

		if (CraftBot::Get.Recipe() == "Ring") {

			//Bot::Print(Debug::CRAFTING, "Sending craft item [RING]");

			buff[1] = (short)Craft::RING;
			buff[3] = (byte)Gender::MALE;

			CString Name = "Ring";
			buff[4] = (byte)Name.GetLength();
			CopyMemory(buff + 5, (PBYTE)(LPCTSTR)Name, buff[4]);

			// Number of items to send
			buff[9] = 1;
			// The items inventory index
			buff[10] = (short)10;
			// The amount of the item to send
			buff[12] = (short)3;

			Size = 14;
		}
	}

	// Cooking.
	else if (CraftBot::Get.Type() == "Cooking") {

		if (CraftBot::Get.Recipe() == "Beef Jerkey") {

			//Bot::Print(Debug::CRAFTING, "Sending craft item [Beef Jerkey]");

			// The type of craft.
			buff[1] = (short)Craft::COOK;
			//buff[3] = (byte)Gender::UNISEX;

			// Name.
			CString Name = "loll";
			buff[4] = (BYTE)Name.GetLength();	
			CopyMemory(buff + 5, (PBYTE)(LPCTSTR)Name, (BYTE)Name.GetLength());

			// Total number of individual items to send.
			buff[9] = 1;
			// The items inventory index.
			buff[10] = (short)10;

			// The amount of the item to send.
			buff[12] = (short)2;

			Size = 14;
		}
	}

	// Potion.
	else if (CraftBot::Get.Type() == "Potion") {

		if (CraftBot::Get.Recipe() == "10HP") {

			//Bot::Print(Debug::CRAFTING, "Sending craft item [10HP]");

			// The type of craft.
			buff[1] = (short)Craft::POTION;
			//buff[3] = (byte)Gender::UNISEX;

			// Name.
			CString Name = "loll";
			buff[4] = (BYTE)Name.GetLength();
			CopyMemory(buff + 5, (PBYTE)(LPCTSTR)Name, (BYTE)Name.GetLength());

			// Total number of individual items to send.
			buff[9] = 2;
			// The first items inventory index.
			buff[10] = (short)10;
			// The amount of the first item to send.
			buff[12] = (short)1;
			// The second items inventory index.
			buff[14] = (short)11;
			// The amount of the second item to send.
			buff[16] = (short)1;

			Size = 18;
		}
	}

	PREPARE_SOCKETMNG_CALL();

	fSend(Size, buff);

	CLIENT_NOTIFY();
}
void SocketMng::DepositItem(short InventoryIndex, short Amount) {

	Bot::Print(Debug::CRAFTING, "Depositing [%d] items from inventory index [%d]", Amount, InventoryIndex);

	BYTE buff[10] = { (BYTE)PacketType::DEPOSIT_ITEM, ((byte*)&InventoryIndex)[0], ((byte*)&InventoryIndex)[1], ((byte*)&Amount)[0], ((byte*)&Amount)[1] };

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();
}
void SocketMng::WithdrawItem(short WarehouseIndex, short Amount) {

	Bot::Print(Debug::CRAFTING, "Withdrawing [%d] [%s] from index [%d]", Amount, BotUser::WareHouse.GetByIndex(WarehouseIndex)->Name, WarehouseIndex);

	BYTE buff[10] = { (BYTE)PacketType::WITHDRAW_ITEM, ((byte*)&WarehouseIndex)[0], ((byte*)&WarehouseIndex)[1], ((byte*)&Amount)[0], ((byte*)&Amount)[1] };

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();
}
void SocketMng::ShopBuy(short ShopNumber, SHOP_ITEM Item) {

	Bot::Print(Debug::CRAFTING, "Buying [%d] [%s] with index [%d] at shop [%d]", Item.pQ, Item.Name, Item.Index, ShopNumber);

	BYTE buff[10] = { 
		
		(BYTE)PacketType::SHOP_BUY, 
		
		((byte*)&ShopNumber)[0], ((byte*)&ShopNumber)[1], 
		
		((byte*)&Item.Index)[0], ((byte*)&Item.Index)[1],
		
		((byte*)&Item.pQ)[0], ((byte*)&Item.pQ)[1]
	
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(7, buff);

	CLIENT_NOTIFY();
}
void SocketMng::ShopSell(short ShopNumber, short ItemIndex, short Amount) {

	BYTE buff[10] = { 
		
		(BYTE)PacketType::SELL_SHOP, 
		
		((byte*)&ShopNumber)[0], ((byte*)&ShopNumber)[1], 
		
		((byte*)&ItemIndex)[0], ((byte*)&ItemIndex)[1], 
		
		((byte*)&Amount)[0], ((byte*)&Amount)[1]
	
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(7, buff);

	CLIENT_NOTIFY();
}

//User.
void SocketMng::ToggleStamina(bool TrueOnFalseOff) {

	Bot::Print(Debug::STAMINA, "Toggling stamina [%s]", TrueOnFalseOff ? "ON" : "OFF");

	BYTE buff[10] = { (BYTE)PacketType::SETRUNMODE, TrueOnFalseOff };

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendAttack() {

	if ((GetTickCount() - LastAttackTick) >= 10) {

		if (BotUser::IsValid(BotUser::Me->Target)) {

			int TargetUid = BotUser::Me->Target->Data->Uid;

			Direction Direction = UserManager::GetDirection(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos);

			Bot::Print(Debug::TARGET, "Attacking [%s] [Uid: %d]", BotUser::Me->Target->Data->Name, TargetUid);

			BYTE buff[10] = { (BYTE)PacketType::ATTACK, ((byte*)&TargetUid)[0], ((byte*)&TargetUid)[1], ((byte*)&TargetUid)[2], ((byte*)&TargetUid)[3], (byte)Direction, 0 };

			PREPARE_SOCKETMNG_CALL();

			fSend(7, buff);

			CLIENT_NOTIFY();

			LastAttackTick = GetTickCount();
		}
	}
}
void SocketMng::SendRevival(byte Type) {

	Bot::Print(Debug::USER, "Sending revival [T: %d]", Type);

	if (Type == 2) {
		// Reset the attack state here because if the botuser's target is not cleared here it will not be removed from the UserList when the user revives in town. This causes a crash.
		Bot::State.Attack.Reset();
	}

	Bot::State.HasEnteredPortal = false;

	BYTE buff[10] = { 
		
		(BYTE)PacketType::LIFE, 
		
		Type 
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendLogOut(void) {

	if (Bot::State.IsRunning) {
		Bot::State.Terminate(Debug::GENERAL, NULL);
	}
	
	Bot::Print(Debug::USER, "Logging out.");

	BYTE buff[10] = { (BYTE)PacketType::LOGOUT, 1 };

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendMagicAttack(int TargetUid, short SpellId) {

	if (BotUser::Me->IsCharStop() OR BotUser::Me->Data->AnimationIndex == USER::Motion::ATTACKING) {

		Direction Direction = BotUser::Me->Data->Direction;

		if (TargetUid != BotUser::Me->Data->Uid) {
			if (BotUser::IsValid(BotUser::Me->Target)) {
				Direction = UserManager::GetDirection(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos);
				Bot::Print(Debug::TARGET, "Attacking [%s] [Uid: %d]", BotUser::Me->Target->Data->Name, TargetUid);
			}
		} else {
			Bot::Print(Debug::TARGET, "Healing [%s] [Uid: %d]", BotUser::Me->Data->Name, TargetUid);
		}

		BYTE buff[20] = {

			(BYTE)PacketType::MAGIC_READY,

			((byte*)&TargetUid)[0], ((byte*)&TargetUid)[1], ((byte*)&TargetUid)[2], ((byte*)&TargetUid)[3],

			((byte*)&SpellId)[0], ((byte*)&SpellId)[1],

			(byte)(((byte)Direction + 1) % 8),

			(byte)Direction, 0
		};

		PREPARE_SOCKETMNG_CALL();

		fSend(10, buff);

		CLIENT_NOTIFY();

		switch ((Magic)SpellId) {
			case Magic::WEAKENING_LV1:
			case Magic::WEAKENING_LV2:
			case Magic::WEAKENING_LV3:
			case Magic::WEAKENING_LV4:
			case Magic::HEALING_LV1:
			case Magic::HEALING_LV2:
			case Magic::HEALING_LV3:
			case Magic::HEALING_LV4:
			return;
		}

		if (Bot::Configuration.CastFast) {

			BYTE buffbuff[20] = {

				(BYTE)PacketType::MAGIC_ATTACK,

				(byte)Direction, 0,

				((byte*)&TargetUid)[0], ((byte*)&TargetUid)[1], ((byte*)&TargetUid)[2], ((byte*)&TargetUid)[3],

				((byte*)&SpellId)[0], ((byte*)&SpellId)[1],
			};

			PREPARE_SOCKETMNG_CALL();

			fSend(9, buffbuff);

			CLIENT_NOTIFY();
		}
	}
}
void SocketMng::SendAura(SPECIALATTACK_DATA* Aura) {

	Bot::Print(Debug::USER, "Using aura [%s] [Id: %d]", Aura->Name.GetString(), Aura->Id);

	int TargetUid = BotUser::Me->Data->Uid;

	Direction Direction = BotUser::Me->Data->Direction;

	// Detect an attack aura.
	if (Aura->TargetMethod == 3 OR Aura->Range >= 1) {
		if (BotUser::IsValid(BotUser::Me->Target)) {
			TargetUid = BotUser::Me->Target->Data->Uid;
			Direction = UserManager::GetDirection(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos);
		}
	}

	BYTE buff[20] = { 
		
		(BYTE)PacketType::SPECIAL_ATTACK, 
		
		((byte*)&TargetUid)[0], ((byte*)&TargetUid)[1], ((byte*)&TargetUid)[2], ((byte*)&TargetUid)[3], 
		
		(byte)Direction, 0, 
		
		((byte*)&Aura->Id)[0], ((byte*)&Aura->Id)[1] 
	
	};

	if (Aura->Name == "Multiple Attack") {
		// Make sure it's safe to use 3rd aura.
		for (auto item = UserManager::UserList.Begin(); item != UserManager::UserList.End(); item++) {
			CUser *pUser = item->second;
			if (BotUser::IsValid(pUser)) {
				if (!pUser->Data->Dead) {
					if (!pUser->IsPlayer()) {
						if (!Bot::Configuration.TargetList.Contains(pUser->Data->Name.GetString())) {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) == 2) {
								Bot::Print(Debug::USER, "Not safe to use aura because of nearby user [%s] [Uid: %d]", pUser->Data->Name.GetString(), pUser->Data->Uid); return;
							}
						}
					}
				}
			}
		} buff[0] = (BYTE)PacketType::SPECIAL_ATTACK_CIRCLE;
	}

	PREPARE_SOCKETMNG_CALL();

	fSend(9, buff);

	CLIENT_NOTIFY();

	// Update this here because if the bot sends more than one aura packet it removes the effect of the aura server side.
	Bot::Configuration.AuraLastUseTime = GetTickCount();

}
void SocketMng::SetStance(Stance Stance) {

	BYTE buff[10] = { (BYTE)PacketType::BATTLE, (byte)Stance };

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}
void SocketMng::ChangeDirection(Direction Direction) {

	int Uid = BotUser::Me->Data->Uid;

	BYTE buff[10] = { (BYTE)PacketType::CHANGEDIR, ((byte*)&Uid)[0], ((byte*)&Uid)[1], ((byte*)&Uid)[2], ((byte*)&Uid)[3], (byte)Direction, 0 };

	PREPARE_SOCKETMNG_CALL();

	fSend(7, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendChat(CString Text) {

	//Bot::Print(Debug::USER, "Sending chat message [%s]", Text);

	BYTE buff[100] = { 
		
		(BYTE)PacketType::CHAT, 
		
		(BYTE)ChatType::NORMAL 
	
	};

	CopyMemory(buff + 2, (PBYTE)(LPCTSTR)Text, (BYTE)Text.GetLength());

	PREPARE_SOCKETMNG_CALL();

	fSend((Text.GetLength() + 2), buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendPm(CString User, CString Message) {

	Bot::Print(Debug::USER, "Sending private message [%s] to [%s]", Message, User);

	BYTE buff[50] = { (BYTE)PacketType::CHAT, (BYTE)ChatType::PRIVATE_INCOMING, (BYTE)User.GetLength() };

	//CopyMemory(buff + 3, (PBYTE)(LPCTSTR)User, (BYTE)User.GetLength());

	////printf_s("\n\n");
	////for (int i = 0; i < 50; i++) {
	////	printf_s("%X ", buff[i]);
	////}

	//CopyMemory(buff + 3 + (BYTE)User.GetLength(), (PBYTE)(LPCTSTR)Message, (BYTE)Message.GetLength());

	//printf_s("\n\n");
	//for (int i = 0; i < 50; i++) {
	//	printf_s("%X ", buff[i]);
	//}

	//for (int i = 3; i < 50; i++) {
	//	buff[i] = 0;
	//}

	CopyMemory(buff + 3, (PBYTE)(LPCTSTR)(User + Message), (BYTE)(User.GetLength() + Message.GetLength()));

	/*printf_s("\n\n");
	for (int i = 0; i < 50; i++) {
		printf_s("%X ", buff[i]);
	}*/
	
	PREPARE_SOCKETMNG_CALL();

	int size = 3 + User.GetLength();// +Message.GetLength();
	//size += Message.GetLength();
	//printf_s("Size: %d\n", size);
	//return;
	fSend(size, buff);

	CLIENT_NOTIFY();
}
void SocketMng::SendChat(byte Type, CString Recipient, CString Message) {
	switch ((ChatType)Type) {

		case ChatType::NORMAL:
			break;

		case ChatType::PRIVATE_INCOMING:
			break;

		case ChatType::PARTY: {

			BYTE buff[50] { (BYTE)PacketType::CHAT, (BYTE)ChatType::NORMAL };

			Message.Insert(0, '>');

			CopyMemory(buff + 2, (PBYTE)(LPCTSTR)Message, (BYTE)Message.GetLength());

			PREPARE_SOCKETMNG_CALL();

			fSend((Message.GetLength() + 2), buff);

			CLIENT_NOTIFY();

		} break;
	}
}
void SocketMng::ThrowItem(short Index, short Count) {

	//Bot::Print(Debug::USER, "Dropping item [%s] from index [%d] Count [%d]", BotUser::Inventory.Item.Name(Index), Index, Count);

	CPoint MyPoint = BotUser::Me->Data->CellPos;

	BYTE buff[10] = { 
		
		(BYTE)PacketType::ITEM_THROW, 
		
		(byte)INVENTORY, 
		
		((byte*)&Index)[0], ((byte*)&Index)[1],

		((byte*)&Count)[0], ((byte*)&Count)[1],
		
		((byte*)&MyPoint.x)[0], ((byte*)&MyPoint.x)[1],

		((byte*)&MyPoint.y)[0], ((byte*)&MyPoint.y)[1]
	
	};

	PREPARE_SOCKETMNG_CALL();

	fSend(10, buff);

	CLIENT_NOTIFY();
}
void SocketMng::ThrowMoney(DWORD Amount) {

	//Bot::Print(Debug::USER, "Dropping [%d] barr", Amount);

	CPoint MyPoint = BotUser::Me->Data->CellPos;

	BYTE buff[10] = { 
		
		(BYTE)PacketType::ITEM_DUMP, 
		
		((byte*)&Amount)[0], ((byte*)&Amount)[1], ((byte*)&Amount)[2], ((byte*)&Amount)[3], 
		
		((byte*)&MyPoint.x)[0], ((byte*)&MyPoint.x)[1], 
		
		((byte*)&MyPoint.y)[0], ((byte*)&MyPoint.y)[1]

	};

	PREPARE_SOCKETMNG_CALL();

	fSend(9, buff);

	CLIENT_NOTIFY();
}
void SocketMng::PartyInviteResponse(byte Response) {

	Bot::Print(Debug::HEAL, "Sending party invite response [%d]", Response);

	BYTE buff[2] = { (BYTE)PacketType::PARTY_INVITE_RESULT, (BYTE)Response };

	PREPARE_SOCKETMNG_CALL();

	fSend(2, buff);

	CLIENT_NOTIFY();
}

void SocketMng::Test() {

	//BYTE buff[10] = {

	//	(BYTE)PacketType::PUTITEM_BELT,

	//	10, 0,

	//	0, 0

	//};

	BYTE buff[20] = {

		(BYTE)PacketType::TAKEBACKMONEY,

		1, 0, 0, 0

	};

	PREPARE_SOCKETMNG_CALL();

	fSend(5, buff);

	CLIENT_NOTIFY();
}                                                                   