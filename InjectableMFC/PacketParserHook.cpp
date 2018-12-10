#include "stdafx.h"
#include "PacketParserHook.h"
#include "DDrawHook.h"
#include <algorithm>

CodeCave PacketParserHook::CC::PacketProcess;
CodeCave PacketParserHook::CC::RawPacketProcess;

bool PacketParserHook::Init(void) {
	// Find the executable.
	HMODULE hMod_Soma = NULL;

	for (; hMod_Soma == NULL; Sleep(10)) {
		hMod_Soma = GetModuleHandle(NULL);
	}

	// Hook PacketParser.PacketProcess.
	CC::PacketProcess.Hook((LPVOID)((DWORD_PTR)hMod_Soma + 0x000CF4D0), MY::PacketProcess);

	// Hook PacketParser.SessionPacketProcess.
	//CC::RawPacketProcess.Hook((LPVOID)((DWORD_PTR)hMod_Soma + 0x000CEDA0), MY::RawPacketProcess);

	return true;
}
void PacketParserHook::Exit(void) {
	if (CC::PacketProcess.IsHooked()) {
		CC::PacketProcess.Unhook();
	}
}

BOOL OriginalProcess;

string decryptUser(string msg)
{
	//printf_s("Before [%s]\n", msg.c_str());

	string New;
	for (int i = 0, hex = 0x3; i < msg.size(); i++, hex++) {

		char Char = msg[i] - hex;

		if (Char > 0x7E)
			Char = 0x4F;

		//printf_s("Old [%c]\tNew [%c] [0x%X]\n", msg[i], Char, Char);

		New += Char;
	}

	//printf_s("After [%s]\n", New.c_str());

	return New;
}

// User control.
void HandleUserDelete(PBYTE bypData) {

	PARAM_USERMODIFY *data = (PARAM_USERMODIFY *)bypData;

	if (data->Type == PARAM_USERMODIFY::Type::REMOVE) {

		Bot::Print(Debug::USERLIST, "ReceivePacketType::USERMODIFY [REMOVE]");

		// Do not delete me.
		if (data->Uid != BotUser::Me->Data->Uid) {

			// If the user to be deleted is my target and the bot is running, do not delete the user as this will be handled on the bot thread.
			if (data->Uid == BotUser::Me->TargetUid) {
				if (!Bot::State.IsRunning) {
					if (Bot::Configuration.TrackTarget) {
						BotUser::Me->ResetTarget();
					}
				} else {
					return;
				}
			}

			Bot::Print(Debug::USERLIST, "Deleting user [Uid: %d] from the UserList.\n", data->Uid);
			UserManager::UserList.Delete(data->Uid);
		}
	} 
}
void HandleUserAddModify(PBYTE bypData) {

	PARAM_USERMODIFY *data = (PARAM_USERMODIFY *)bypData;

	if (data->Type == PARAM_USERMODIFY::Type::MODIFY) {

		Bot::Print(Debug::USERLIST, "ReceivePacketType::USERMODIFY [MODIFY]");

		if (!UserManager::UserList.Contains(data->Uid)) {

			// Get the new user.
			PUSER user = UserManager::GetUserWithUid(data->Uid);

			// Add a new user.
			if (user) {

				// Create a CUser based on the game data.
				CUser *pUser = new CUser(user);

				// Fill the BotUser::Me ptr if the new user is me.
				if (user == UserManager::GetMe()) {

					// Security measure.
					for (auto xt = BotUser::Subscribers.PermittedUsers.begin();; xt++) {
						if (xt == BotUser::Subscribers.PermittedUsers.end()) {
							Bot::Print(Debug::USER, "Illegal user [%s]", pUser->Data->Name);
							ExitProcess(NULL);
						} 
						//else if (string(pUser->Data->Name) == decryptUser(*xt)) 
						{

							BotUser::Me = pUser;

							Bot::Print(Debug::USER, "New user [%s] [Uid: %d]", BotUser::Me->Data->Name, BotUser::Me->Data->Uid);

							BotUser::Me->Race = (int*)((DWORD)GetModuleHandle(NULL) + 0x1362F4);
							BotUser::Me->Stamina = (byte*)((DWORD)GetModuleHandle(NULL) + 0x136780);

							// Reset these booleans here because the bot user gets reset when changing zones.
							if (Bot::State.IsEnteringPortal) {
								Bot::State.IsOpeningNpc = false;
								Bot::State.IsEnteringPortal = false;
								Bot::State.HasEnteredPortal = true;
							} break;
						}
					}				
				}
				else { // Having this code on an else statement prevents the bot user being detected as a nearby notify user when going through portals.
					
					if (Bot::State.IsRunning) {

						// Nearby user detection.
						if (Bot::Configuration.mode == Mode::LEVEL) {
							if (Bot::NearbyUserDetection.IsEnabled) {
								if (pUser->IsPlayer()) {
									// Do not detect users if we're doing repairs & restocks or if we're running the prelvl path.
									if (!Bot::State.PotsAndRepairs AND !Bot::State.IsPathRunning) {
										// Do not act on this user if they are on the exclusion list.
										if (!Bot::NearbyUserDetection.ExclusionList.Contains(pUser->Data->Name.GetString()) AND !Bot::NearbyUserDetection.ExclusionList.Contains(pUser->Data->Guild.GetString())) {

											Bot::Print(Debug::OUTPUT, "Detected nearby user [%s]", pUser->Data->Name);

											switch (Bot::NearbyUserDetection.DetectedUsrResponse) {

												case Response::DISCONNECT: {
													SocketMng::SendLogOut();
												} break;

												case Response::NOTIFY: {
													if (!Bot::NearbyUserDetection.DetectedUsers.Contains(pUser->Data->Name.GetString())) {
														Bot::NearbyUserDetection.DetectedUsers.Add(pUser->Data->Name.GetString());
														Bot::NearbyUserDetection.IsNotifying = true;
														Bot::OutPut.SoundPlay(IDR_NEARBYUSER);
													}
												} break;
											}
										}
									}
								}
							}
						}

						
					}
				}

				// Add the user to the UserList.
				Bot::Print(Debug::USERLIST, "Adding user [%s] [Uid: %d] to the UserList.\n", pUser->Data->Name, data->Uid);
				UserManager::UserList.Add(pUser);
			}
		}
	}
}
void HandleAccountLogin(PBYTE bypData) {
	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::ACOUNT_LOGIN");

	// Clear the UserList and reset BotUser on the character selection screen.

	Bot::Print(Debug::USERLIST, "Clearing the UserList.");
	UserManager::UserList.ClearAll();

	Bot::Print(Debug::USERLIST, "Resetting Me.\n");
	BotUser::Me = nullptr;
}
void HandleMove(BYTE Kind, PBYTE bypData) {

	PARAM_MOVE *data = (PARAM_MOVE *)bypData;

	if (data->Uid == BotUser::Me->Data->Uid)
		Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::PKT_MOVE\n\t\tSuccess: %d\n\t\tUid: %d", data->Result, data->Uid);

	// Reset the user's target on successful movement.
	if (data->Result == PARAM_MOVE::Result::SUCCESS) {

		CUser *pUser = nullptr;
		if (UserManager::UserList.Get(data->Uid, pUser)) {

			// Do not alter the bot users variables.
			if (data->Uid != BotUser::Me->Data->Uid) {

				// No need to reset the target of target if there isn't one already.
				if (pUser->TargetUid != 0) {
					Bot::Print(Debug::TARGET, "Reset target of [%s] [Uid: %d]", pUser->Data->Name.GetString(), pUser->Data->Uid);
					pUser->Target = nullptr;
					pUser->TargetUid = 0;
				}
			}

			// Code evaluations for when only the bot is running.
			if (Bot::State.IsRunning) {

				// Custom condition to attack annoyance mobs whenever they move into range (If we're on Abias hunting Nightmare).	
				if (Bot::State.IsTrainingInt) {
					if (Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
						if (Bot::Zone.Current() == Map::ABIAS) {
							if (Bot::State.HasTarget) {		
								// Only switch targets if we're already attacking (Prevents switching targets when running).
								if (Bot::State.Attack.IsAttacking)  {
									if (!Bot::Configuration.IgnoreList.Contains(pUser->Data->Name.GetString())) {
										// Do not attempt to attack a target that we already have targetted.
										if (BotUser::Me->TargetUid != 0 AND BotUser::Me->TargetUid != pUser->Data->Uid) {
											if ((pUser->Data->Name == "SWAMP BEAST" OR pUser->Data->Name == "STIGE") AND pUser->RangeFromCell(BotUser::Me->Data->CellPos) <= ((BotUser::MagicList.AtkSpell->Range - 2) * 2)) {
												Bot::Print(Debug::TARGET, "Switching to target [%s] [Uid: %d [RFM: %d]", pUser->Data->Name, pUser->Data->Uid, pUser->RangeFromCell(BotUser::Me->Data->CellPos));
												BotUser::Me->Target = pUser;
												BotUser::Me->TargetUid = pUser->Data->Uid;
											}
										}
									}
								}
							}
						}
					}
				}

				// Determine if it safe to heal when a safe heal is required.
				if (Bot::State.Heal.SafeHealRequired) {
					//if (data->Uid == BotUser::Me->Data->Uid) 
					{
						for (auto xt = UserManager::UserList.Begin();;xt++) {
							// No aggressive mobs have been found so disable SafeHealRequired and let healing commence (Heal.WepChangeRequired and Heal.Required are TRUE at this point).
							if (xt == UserManager::UserList.End()) {
								Bot::State.Heal.SafeHealRequired = false; break;
							} else {
								CUser *pUser = xt->second;
								if (BotUser::IsValid(pUser) AND !pUser->Data->IsMe AND !pUser->IsMagicMob() AND pUser->IsAggressive() AND pUser->TargetUid == 0) {
									if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) < (pUser->AggroRange + 4)) {
										// Break out of the loop so heal will not get run if an aggressive mob is found that is within range.
										Bot::Print(Debug::GENERAL, "Not safe to heal because of [%s] [Uid: %d] [RFM: %d]", pUser->Data->Name, pUser->Data->Uid, UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos));
										break;
									}
								}
							}
						}
					}
				}

				// If we're stood still regaining stamina, check for impending aggressive mobs.
				if (Bot::State.IsRegainingStamina) {
					if (!pUser->IsPlayer() AND pUser->IsAggressive()) {
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) <= 6) { 
							Bot::Print(Debug::GENERAL, "Detected nearby aggressive mob [%s] [Uid: %d] while regaining stamina.", pUser->Data->Name, data->Uid);
							Bot::State.IsRegainingStamina = false;
						}
					}
				}
			}
		}

		// Reset the blacklisted uid on successful movement (Movement usually solves bugged mobs).
		if (Bot::Configuration.BlackList.Contains(data->Uid, 0)) {
			Bot::Configuration.BlackList.Remove(data->Uid);
		}
	}
}
void HandleLogin() {
	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::LOGIN");

	Bot::Zone.Number = (int*)((DWORD)GetModuleHandle(NULL) + 0x1316B4);

	BotUser::MagicList.Belt.BeltSelIndex = (int*)BotUser::ResolvePointer((DWORD)GetModuleHandle(NULL) + 0x1366AC, 0);
	BotUser::MagicList.Belt.Belt = (MAGICBELT_DATA**)BotUser::ResolvePointer((DWORD)GetModuleHandle(NULL) + 0x136678, 0);

	SocketMng::socketSink = ::FindWindow(NULL, "Socket Notification Sink");
	SocketMng::GameSocket = *(DWORD*)BotUser::ResolvePointer((DWORD)GetModuleHandle(NULL) + 0x2DC1B4, 1, 0x4);
}

// User.
void HandleChangeItemInfo(PBYTE bypData) {
	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::ITEM_CHANGE_INFO");

	if (Bot::State.IsRunning) {

		PARAM_CHANGE_ITEM_INFO* data = (PARAM_CHANGE_ITEM_INFO *)bypData;

		//printf_s("Location [%s]\nIndex [%d]\nType: [0x%X]", (data->Location == PARAM_CHANGE_ITEM_INFO::Location::BLT ? "BELT" : "INV"), data->Index, data->Type);

		// Dura information.
		if (data->Type == PARAM_CHANGE_ITEM_INFO::Type::DURA AND data->Index < 10) {

			bypData += sizeof(PARAM_CHANGE_ITEM_INFO);

			short NewCurrentDura = ParseData(bypData, short);

			// Use a different DuraThreshold if the item is Hero, Upgraded or Medusa.
			int DuraThreshold = Bot::Configuration.DuraThreshold;
			if (BotUser::Inventory.Item.IsHero(data->Index) OR BotUser::Inventory.Item.IsUpgraded(data->Index) OR BotUser::Inventory.Item.Name(data->Index).Find("Medusa") != -1) {
				DuraThreshold = 5;
			}

			if (NewCurrentDura < (BotUser::Inventory.Item.DuraOrigMax(data->Index) * DuraThreshold / 100)) {

				Bot::Print(Debug::GENERAL, "Item [%s] at index [%d] needs replacing.", BotUser::Inventory.Item.Name(data->Index), data->Index);

				for (int i = 10; i < INV_COUNT; i++) {

					// Same type.
					if (BotUser::Inventory.Item.Type(i) == BotUser::Inventory.Item.Type(data->Index)) {

						// Dura is above (DuraThreshold + 5).
						if (BotUser::Inventory.Item.Dura(i) > (BotUser::Inventory.Item.DuraOrigMax(i) * (Bot::Configuration.DuraThreshold + 5) / 100)) {

							// ARMOR.
							if (BotUser::Inventory.Item.Type(i) == ItemType::ARMOR) {

								// Make sure the new item is of the right gender. Or if it's unisex.
								if (BotUser::Inventory.Item.Gender(i) == Gender::UNISEX OR BotUser::Inventory.Item.Gender(i) == (Gender)(BotUser::Me->Data->Gender + 1)) {

									// Same armour position (Prevents gay issues).
									if (BotUser::Inventory.Item.ArmPos(i) == BotUser::Inventory.Item.ArmPos(data->Index)) {

										// Make sure we have enough stats to equip the item.
										if (Bot::State.IsTrainingInt) {
											if (BotUser::Inventory.Item.Requirement.Intelligence(i) <= BotUser::Me->Data->Stats.Intelligence) {
												SocketMng::SwitchItem(i, data->Index); return;
											}
										} else {
											if (BotUser::Inventory.Item.Requirement.Strength(i) <= BotUser::Me->Data->Stats.Strength) {
												SocketMng::SwitchItem(i, data->Index); return;
											}
										}
									}
								}
							}

							// WEAPON.
							else if (BotUser::Inventory.Item.ArmPos(i) == Armor::HANDS OR BotUser::Inventory.Item.ArmPos(i) == Armor::RIGHT_HAND) {

								// Check if the user has the right stats to equip the item.
								if ((BotUser::Inventory.Item.Requirement.Strength(i) <= BotUser::Me->Data->Stats.Strength) && (BotUser::Inventory.Item.Requirement.Dexterity(i) <= BotUser::Me->Data->Stats.Dexterity)) {

									bool TryChange;
									switch (BotUser::Inventory.Item.Type(i)) {
										case ItemType::SWORD:
											if (BotUser::Inventory.Item.Requirement.Skill(i) <= BotUser::Me->Data->WeaponSkills.Sword)
												TryChange = true;
											break;
										case ItemType::SPEAR:
											if (BotUser::Inventory.Item.Requirement.Skill(i) <= BotUser::Me->Data->WeaponSkills.Spear)
												TryChange = true;
											break;
										case ItemType::AXE:
											if (BotUser::Inventory.Item.Requirement.Skill(i) <= BotUser::Me->Data->WeaponSkills.Axe)
												TryChange = true;
											break;
										case ItemType::KNUCKLE:
											if (BotUser::Inventory.Item.Requirement.Skill(i) <= BotUser::Me->Data->WeaponSkills.Knuck)
												TryChange = true;
											break;
										case ItemType::BOW:
											if (BotUser::Inventory.Item.Requirement.Skill(i) <= BotUser::Me->Data->WeaponSkills.Bow)
												TryChange = true;
											break;
									}
									if (TryChange) {

										// Remove the users shield if the weapon to be equipped is a 2H weapon.
										if (BotUser::Inventory.Item.ArmPos(i) == Armor::HANDS AND BotUser::Inventory.Item.IsValid(7)) {
											if (Bot::Items.ShieldIndex != -1) {
												Bot::Items.ShieldIndex = BotUser::Inventory.NearestEmpty(NULL);
											}
											SocketMng::SwitchItem(7, -1); 
											return;
										}
			
										SocketMng::SwitchItem(i, data->Index); 

										// If we are equipping a 1H item when we have previously unequipped a shield, then re-equip the shield too.
										if (BotUser::Inventory.Item.ArmPos(i) == Armor::RIGHT_HAND AND Bot::Items.ShieldIndex != -1) {
											SocketMng::SwitchItem(Bot::Items.ShieldIndex, 7);
										}

										return;

									} else {
										Bot::Print(Debug::GENERAL, "Bot user lacks the skill required for item [%s]", BotUser::Inventory.Item.Name(i).MakeUpper());
									}
								}
							}

							// ACCESSORY.
							else if (BotUser::Inventory.Item.Type(i) == ItemType::ACC) {
								if (BotUser::Inventory.Item.ArmPos(i) == BotUser::Inventory.Item.ArmPos(data->Index)) {
									SocketMng::SwitchItem(i, data->Index); return;
								}
							}
						}
					}
				}

				// Item replacement search has failed.
				Bot::Print(Debug::GENERAL, "Failed to find a replacement for item [%s] at index [%d].", BotUser::Inventory.Item.Name(data->Index), data->Index);

				// If the item is an accessory, simply remove it.
				if (BotUser::Inventory.Item.Type(data->Index) == ItemType::ACC) {
					Bot::Print(Debug::GENERAL, "[%s] detected as accessory... Will de-equip.", BotUser::Inventory.Item.Name(data->Index));
					if (BotUser::Inventory.SpaceAvailable() > 0) {
						SocketMng::SwitchItem(data->Index, -1);
					} else {
						Bot::Print(Debug::GENERAL, "Player lacks the inventory space to remove the item.");
						Bot::State.Warp.FlagReq();
					}
				} else {
					// We check NO_WARP here because; as the bot never warps if NO_WARP is true, the bot will continue to use a low dura item until it has broken .
					if (Bot::Configuration.NoWarp) {
						SocketMng::SendLogOut();
					} else {
						Bot::State.Warp.FlagReq();
					}
				}
			}
		}
	}
}
void HandleCharterData(PBYTE bypData) {
	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::CHARTER_DATA");

	PARAM_CHARTERDATA* data = (PARAM_CHARTERDATA *)bypData;

	if (Bot::State.IsRunning) {

		if (data->Uid == BotUser::Me->Data->Uid) {

			if (Bot::State.HealthLow) {
				if (data->Health.Current > (data->Health.Maximum * (Bot::State.Heal.Enabled ? (Bot::Configuration.HpThreshold + 20) : 90) / 100)) {
					Bot::Print(Debug::GENERAL, "Potting up done.");
					Bot::State.HealthLow = false;
				}
			}
			else if (Bot::State.ManaLow) {
				if (data->Mana.Current > (data->Mana.Maximum * 80 / 100)) {
					Bot::Print(Debug::GENERAL, "Potting up done.");
					Bot::State.ManaLow = false;
				}
			}
		}
	}
}
void HandleDeadUser(PBYTE bypData) {
	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::DEAD");

	int Uid = ParseData(bypData, int);

	if (Uid == BotUser::Me->Data->Uid) {
		if (Bot::State.IsRunning) {
			// Screenshot?
			Bot::Print(Debug::USER, "Player has died.");
			Bot::OutPut.SoundPlay(IDR_DEAD);
			Bot::Configuration.Death.Delay = BotUser::RandomNumberGenerator(3000, 7500);
			Bot::State.AwaitingRevival = true;
		}
	} 
	else {
		// Only delete my target if the bot is not running and we're tracking it.
		if (Uid == BotUser::Me->TargetUid) {
			if (!Bot::State.IsRunning) {
				if (Bot::Configuration.TrackTarget) {
					BotUser::Me->ResetTarget();
				}
			} else {
				return;
			}
		}

		// Delete the dead user from the UserList.
		Bot::Print(Debug::USERLIST, "Deleting dead user [Uid: %d] from the UserList.", Uid);
		UserManager::UserList.Delete(Uid);
	}
}
void HandleZoneChange(PBYTE bypData) {

	PARAM_ZONECHANGE *data = (PARAM_ZONECHANGE *)bypData;

	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::ZONECHANGE\n\t\tUid: %d\tMap: %d\n", data->Uid, data->MapNum);

	if (data->Uid == BotUser::Me->Data->Uid) {
		// Purge the UserList because we're now in a new zone (Source client does this also).
		Bot::Print(Debug::USERLIST, "Clearing the UserList.");
		UserManager::UserList.ClearAll();
	}
}
void HandleTownPortal(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::TOWNPORTAL");

	if (Bot::State.IsRunning) {

		PacketType pktType = ParseEnum(bypData, PacketType);
		int Uid = ParseData(bypData, int);

		if (Uid == BotUser::Me->Data->Uid) {

			switch (pktType) {
				case PacketType::TOWNPORTALREQ: {
					Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::TOWNPORTALREQ");
					Bot::Print(Debug::USER, "Warping...");
					Bot::State.Warp.InProgress = true;
				} break;

			case PacketType::TOWNPORTALEND: {
					Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::TOWNPORTALEND");
					if (!Bot::State.Warp.Done) {
						Bot::Print(Debug::USER, "Warp done.");
						Bot::State.Warp.Done = true;
					}
				} break;
			}
		}
	}
}
void HandleMagicAll(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::MAGIC_ALL");

	byte Type = ParseData(bypData, byte);
	short TotalCount = ParseData(bypData, short);

	Bot::Print(Debug::PACKETPROCESS, "Type: %d\tCount: %d\n", Type, TotalCount);

	switch (Type) {

		// Magic.
		case 1: {
			for (int i = 0; i < TotalCount; i++) {

				MAGIC_DATA magicData;

				magicData.Id = ParseData(bypData, short);
				byte MethodOne = ParseData(bypData, byte);
				byte MethodTwo = ParseData(bypData, byte);
				magicData.CastTime = ParseData(bypData, short);
				magicData.Class = ParseData(bypData, byte);
				byte TargetMethod = ParseData(bypData, byte);
				byte NameLength = ParseData(bypData, byte);
				magicData.Name = ParseString(bypData, NameLength);
				byte ExplanationLength = ParseData(bypData, byte);
				CString Explanation = ParseString(bypData, ExplanationLength);
				magicData.Range = ParseData(bypData, short);
				magicData.MpUsage = ParseData(bypData, short);
				magicData.Duration = ParseData(bypData, int);
				magicData.Power = ParseData(bypData, short);
				bypData += 2;
				short MinIntReq = ParseData(bypData, short);
				bypData += 4;

				BotUser::MagicList.Add(magicData);
			}
		} break;

		// Aura.
		case 2: {
			for (int i = 0; i < TotalCount; i++) {

				SPECIALATTACK_DATA SpecialAtkData;

				SpecialAtkData.Id = ParseData(bypData, short);
				SpecialAtkData.TargetMethod = ParseData(bypData, byte);
				byte NameLength = ParseData(bypData, byte);
				SpecialAtkData.Name = ParseString(bypData, NameLength);
				byte ExplanationLength = ParseData(bypData, byte);
				CString Explanation = ParseString(bypData, ExplanationLength);
				SpecialAtkData.Range = ParseData(bypData, short);
				SpecialAtkData.Usage.Health = ParseData(bypData, short);
				SpecialAtkData.Usage.Mana = ParseData(bypData, short);
				SpecialAtkData.Usage.Stamina = ParseData(bypData, short);
				SpecialAtkData.Duration = ParseData(bypData, int);
				SpecialAtkData.CoolDown = ParseData(bypData, int);
				SpecialAtkData.Power = ParseData(bypData, short);
				short MinExp = ParseData(bypData, short);
				SpecialAtkData.Class = (ItemType)ParseData(bypData, byte);
				int SomeTerm = ParseData(bypData, int);
				short HpDecreaseOverTime = ParseData(bypData, short);
				short MpDecreaseOverTime = ParseData(bypData, short);
				bypData += 2;

				// Remove the trailing whitespace from the aura name.
				SpecialAtkData.Name.TrimRight();

				BotUser::SpecialAttackList.Add(SpecialAtkData);
			}
		} break;
	}
}
void HandleGetItem(PBYTE bypData) {
	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::GETITEM");

	if (Bot::State.IsRunning) {
		if (Bot::Configuration.mode == Mode::RESTOCK) {
			// Update the inventory index of the restock material for when we deposit later.
			short Index = ParseData(bypData, short);
			RestockBot::Set.ItemIndex(Index);
		}
	}
}
void HandleAreaState(PBYTE bypData) {

	//PARAM_AREASTATE* data = (PARAM_AREASTATE *)bypData;

	////Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::AREASTATE\n\t\tUid: %d State: %d", data->Uid, data->Type);

	//if (data->Uid == BotUser::Me->Data->Uid) {
	//	switch (data->Type) {
	//		case PARAM_AREASTATE::Type::HEALTH:
	//		case PARAM_AREASTATE::Type::STAMINA: {
	//			//Bot::Print(Debug::GENERAL, "Nullifying area state [%d]", data->Type);
	//			//OriginalProcess = false;
	//		} break;
	//	}
	//}
}
void HandleRecItemToBelt(PBYTE bypData) {

	//bypData[0] = 1;

	//BYTE Result = ParseData(bypData, byte);

	//Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::PUTITEM_BELT\n\t\tResult: %d\n", Result);
}
void HandleItemUse(PBYTE bypData) {

	/*BYTE Result = ParseData(bypData, byte);
	BYTE Belt = ParseData(bypData, byte);
	short Index = ParseData(bypData, short);

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::ITEM_USE\n\t\tResult: %d\n\t\tBelt: %d\n\t\tIndex: %d\n", Result, Belt, Index);*/
}
void HandlePartyInvite(PBYTE bypData) {

	if (Bot::Configuration.mode == Mode::HEAL) {

		int LdrUid = ParseData(bypData, int);
		int MyUid = ParseData(bypData, int);

		BYTE LdrNameLen = ParseData(bypData, byte);
		CString LdrName = ParseString(bypData, LdrNameLen);

		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::PARTY_INVITE\n\t\tLdrUid: %d MyUid: %d LdrName: %s\n\n", LdrUid, MyUid, LdrName);

		CUser* pUser;
		if (UserManager::UserList.Get(LdrUid, pUser)) {

			Bot::Print(Debug::HEAL, "Accepting party invite from [%s] [Uid: %d]", LdrName, LdrUid);

			SocketMng::PartyInviteResponse(PARTYACCEPT);

			OriginalProcess = false;

			// Only continue if we have a healing spell.
			if (BotUser::MagicList.Contains("Healing")) {

				BotUser::Me->Target = pUser;
				BotUser::Me->TargetUid = pUser->Data->Uid;
				//BotUser::Me->Target->PrevCellPos = CPoint(pUser->Data->CellPos.x - 2, 0);

				BotUser::MagicList.AtkSpell = BotUser::MagicList.GetByName("Healing");
				Bot::Configuration.AttackDistance = (BotUser::MagicList.AtkSpell->Range * 2);

				SocketMng::SetStance(Stance::BATTLE);

				SocketMng::SendChat((BYTE)ChatType::PARTY, NULL, "I will prioritize healing on " + LdrName);

				Bot::State.IsRunning = true;

			} else {
				SocketMng::SendChat((BYTE)ChatType::PARTY, NULL, "I could not find any healing spells?");
			}
		} else {
			Bot::Print(Debug::HEAL, "Could not locate healing target [%s] [Uid: %d]", LdrName, LdrUid);
		}
	}
}

// Npc.
void HandleNpcDialogue(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::NPC_DIALOGUE_SELECTION");

	if (Bot::State.IsRunning) {

		short DialogueSelection = -1;

		// Do not show the NPC dialogue.
		OriginalProcess = false;

		// Parse the NPC introductory text.
		short TextLength = ParseData(bypData, short);
		CString Text = ParseString(bypData, TextLength);

		for (int i = 0; i < 4; i++) {

			TextLength = ParseData(bypData, short);
			Text = ParseString(bypData, TextLength);

			//printf_s("\nTextLength [%d]\tTextPosition [%d]\n\tText [%s]\n", TextLength, i, Text);

			switch (Bot::Configuration.mode) {

				case Mode::CRAFT: {
					if (CraftBot::State.NpcRequired AND Text.Find("Sell") != -1) {
						DialogueSelection = 1;
					} else if (CraftBot::Get.Type() == "Cooking" AND Text.Find("Cook") != -1) {
						DialogueSelection = i;
					} else if (CraftBot::Get.Type() == "Accessory" AND Text.Find("Manufacture") != -1) {
						DialogueSelection = i;
					} else if ((CraftBot::Get.Type() == "Weapon" OR CraftBot::Get.Type() == "Armour") AND Text.Find("Make") != -1) {
						DialogueSelection = i;
					} 
				} break;

				case Mode::RESTOCK: {
					if (Text.Find("Materials") != -1) {
						DialogueSelection = i;
					}
				} break;

				case Mode::LEVEL: {
					if (Bot::State.IsRepairing AND Text.Find("Repair") != -1) {
						DialogueSelection = i;
					} else if (Bot::State.IsRestocking AND Text.Find("Buy") != -1) {
						DialogueSelection = i;
					} else if (Bot::State.IsPathRunning) {
						// Portals.
						if (Text.Find("Yes") != -1) {
							DialogueSelection = i;
						}
						// Hwan Castle from Pandi.
						else if (Text.Find("Hwan Castle") != -1) {
							DialogueSelection = i;
						}
					} 
				} break;
			}

			// Discontinue the processing of NPC options and send the selection if we've found the option we want.
			if (DialogueSelection != -1) {
				break;
			}
		}

		// Send the dialogue selection
		SocketMng::NpcDialogueSelect(DialogueSelection);
	}
}
void HandleCraftItem(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::CRAFT_ITEM");

	if (Bot::State.IsRunning) {

		if (Bot::Configuration.mode == Mode::CRAFT) {

			byte Result = ParseData(bypData, byte);
			short TotalMaterialCount = ParseData(bypData, short);
			short FirstMaterialIndex = ParseData(bypData, short);
			short FirstMaterialCountNew = ParseData(bypData, short);
			short SecondMaterialIndex;
			short SecondMaterialCountNew;
			short CraftedItemIndex;

			// Parse more values if we're crafting with two different materials.
			if (TotalMaterialCount == 2) {
				SecondMaterialIndex = ParseData(bypData, short);
				SecondMaterialCountNew = ParseData(bypData, short);
				//Bot::Print(Debug::CRAFTING, "RecievePacketType::CRAFT_ITEM\n\t\tSuccess:\t%d\n\t\tCraftItemCount:\t%d\n\t\tMaterialIndex:\t%d\n\t\tMaterialCount:\t%d\n\t\tMaterialIndexTwo:\t%d\n\t\tMaterialCountTwo:\t%d\n\t\tCraftedItemIndex:\t%d\n", Result, TotalMaterialCount, FirstMaterialIndex, FirstMaterialCountNew, SecondMaterialIndex, SecondMaterialCountNew, CraftedItemIndex);
			} 

			CraftedItemIndex = ParseData(bypData, short);
			//Bot::Print(Debug::CRAFTING, "RecievePacketType::CRAFT_ITEM\n\t\tSuccess:\t%d\n\t\tCraftItemCount:\t%d\n\t\tMaterialIndex:\t%d\n\t\tMaterialCount:\t%d\n\t\tCraftedItemIndex:\t%d\n", Result, TotalMaterialCount, FirstMaterialIndex, FirstMaterialCountNew, CraftedItemIndex);
			
			// Succesful craft.
			if (Result == 1) {

				// Update the CraftedItemIndex so we know the inventory index from where to deposit the crafted item (Only used for Cooking and Potions).
				if (CraftBot::Get.Type() == "Cooking" OR CraftBot::Get.Type() == "Potion")
					CraftBot::Configuration.CraftedItemIndex = CraftedItemIndex;

				// Check the available inventory space and discontinue if none is left (Means we need to empty the inventory of crafted items).
				if (BotUser::Inventory.SpaceAvailable() <= 1) {
					Bot::Print(Debug::CRAFTING, "Production complete.");
					CraftBot::State.CancelProduction(false);
				}
			}
			
			// TotalMaterialCount shows 0 if the very first packet received is a craft failure
			if (TotalMaterialCount > 0) {
				if (FirstMaterialCountNew < CraftBot::Configuration.MinMaterialCount) {
					Bot::Print(Debug::CRAFTING, "Production complete.");
					CraftBot::State.CancelProduction(true);
				}
			}		
		}
	}
}
void HandleWarehouseOpen(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::WAREHOUSE_OPEN");

	if (Bot::State.IsRunning) {

		int WhMoney = ParseData(bypData, int);
		short WhItemsMax = ParseData(bypData, short);
		short WhItemsCount = ParseData(bypData, short);

		//Bot::Print("Money: %d\nItemsMax: %d\nItemsCount: %d\n", WhMoney, WhItemsMax, WhItemsCount);

		//for (int i = 0; i < WhItemsCount; i++) {
		//	short Index = ParseData(bypData, short);
		//	byte Type = ParseData(bypData, byte);
		//	byte ArmPos = ParseData(bypData, byte);
		//	short Class = ParseData(bypData, short);
		//	short PicNum = ParseData(bypData, short);
		//	byte Gender = ParseData(bypData, byte);
		//	short MinDamage = ParseData(bypData, short);
		//	short MaxDamage = ParseData(bypData, short);
		//	short Weight = ParseData(bypData, short);
		//	short Speed = ParseData(bypData, short);
		//	short Str = ParseData(bypData, short);
		//	short Int = ParseData(bypData, short);
		//	short Dex = ParseData(bypData, short);
		//	short Skill = ParseData(bypData, short);
		//	short HP = ParseData(bypData, short);
		//	short MP = ParseData(bypData, short);
		//	short Dura = ParseData(bypData, short);
		//	byte NameLength = ParseData(bypData, byte);
		//	CString Name = ParseString(bypData, NameLength);
		//	byte UpgradeLvl = ParseData(bypData, byte);
		//	bypData += 24;

		//	Bot::Print("Index: %d\nType: %d\nArmPos: %d\nClass: %d\nPicNum: %d\nGender: %d\nMinDmg: %d\nMaxDmg: %d\nWeight: %d\nSpeed: %d\nStr: %d\nInt: %d\nDex: %d\nSkill: %d\nHP: %d\nMP: %d\nDura: %d\nNameLength: %d\nName: %s\nUpgradeLvl: %d\n\n",
		//		Index, Type, ArmPos, Class, PicNum, Gender, MinDamage, MaxDamage, Weight, Speed, Str, Int, Dex, Skill, HP, MP, Dura, NameLength, Name, UpgradeLvl);
		//}

		// Do not show the warehouse dialogue.
		//OriginalProcess = false;

		if (Bot::Configuration.mode == Mode::RESTOCK) {
			SocketMng::DepositItem(RestockBot::Get.ItemIndex(), BotUser::Inventory.Item.Count(RestockBot::Get.ItemIndex()));
		}
		else if (Bot::Configuration.mode == Mode::CRAFT AND CraftBot::State.RestockRequired) {

			// Deposit the crafted item stack if we're Cooking or making Potions.
			if (CraftBot::Get.Type() == "Cooking" OR CraftBot::Get.Type() == "Potion") {
				if (WhItemsCount == WhItemsMax) {
					Bot::State.Terminate(Debug::CRAFTING, "Not enough warehouse space to deposit crafted material."); return;
				} else {
					SocketMng::DepositItem(CraftBot::Configuration.CraftedItemIndex, BotUser::Inventory.Item.Count(CraftBot::Configuration.CraftedItemIndex));
				}
			}

			// Remove the old warehouse data.
			BotUser::WareHouse.Flush();

			// Parse the new warehouse data.
			for (int i = 0; i < WhItemsCount; i++, bypData += 25) {
				WAREHOUSE_DATA data;
				data.Index = Parse(bypData, short, 27);
				data.Quantity = ParseData(bypData, short);
				byte NameLength = ParseData(bypData, byte);
				data.Name = ParseString(bypData, NameLength);
				BotUser::WareHouse.Add(data);
			}	

			// Attempt a restock.
			Bot::Print(Debug::GENERAL, "Attempting to restock.");
			
			// Cooking.
			if (CraftBot::Get.Type() == "Cooking") {

				if (CraftBot::Get.Recipe() == "Beef Jerkey") {

					int Quantity = (BotUser::Inventory.BwAvailable() - 3);

					if (BotUser::WareHouse.Contains("Wolf Meat")) {
						SocketMng::WithdrawItem(BotUser::WareHouse.GetByName("Wolf Meat")->Index, Quantity);
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Wolf Meat found."); return;
					}
				}                     
			}
			// Accessory.
			else if (CraftBot::Get.Type() == "Accessory") {

				if (CraftBot::Get.Recipe() == "Ring") {

					int Quantity = (BotUser::Inventory.BwAvailable() - 3);

					if (BotUser::WareHouse.Contains("Copper Metal")) {
						SocketMng::WithdrawItem(BotUser::WareHouse.GetByName("Copper Metal")->Index, Quantity);
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Copper Metal found."); 
						return;
					}
				}
			}
			// Armour.
			else if (CraftBot::Get.Type() == "Armour") {

				if (CraftBot::Get.Recipe() == "Cheap Shoes") {

					// Calculate the amount to withdraw.
					int AmountToWithdraw = ((BotUser::Inventory.BwAvailable() / 3) - 2);

					// First find leather.
					WAREHOUSE_DATA* LthrData;

					if (BotUser::WareHouse.Contains("Basic Leather")) {
						LthrData = BotUser::WareHouse.GetByName("Basic Leather");
					} else if (BotUser::WareHouse.GetByName("Tiger Leather")) {
						LthrData = BotUser::WareHouse.GetByName("Tiger Leather");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Leather found.");
						return;
					}

					// Make sure there is enough leather to withdraw.
					if (LthrData->Quantity >= AmountToWithdraw) {
						SocketMng::WithdrawItem(LthrData->Index, AmountToWithdraw);
					} else {
						SocketMng::WithdrawItem(LthrData->Index, LthrData->Quantity);
					}

					// Then find linen.
					WAREHOUSE_DATA* LinenData;

					if (BotUser::WareHouse.Contains("Linen")) {
						LinenData = BotUser::WareHouse.GetByName("Linen");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Linen found.");
						return;
					}

					// Make sure there is enough Linen to withdraw.
					if (LinenData->Quantity >= (AmountToWithdraw * 2)) {
						SocketMng::WithdrawItem(LinenData->Index, (AmountToWithdraw * 2));
					} else {
						SocketMng::WithdrawItem(LinenData->Index, LinenData->Quantity);
					}
				}
			}
			// Weapon.
			else if (CraftBot::Get.Type() == "Weapon") {

				if (CraftBot::Get.Recipe() == "Dagger") {

					int AmountToWithdraw = ((BotUser::Inventory.BwAvailable() / 4) - 2);

					// First find leather.
					WAREHOUSE_DATA* LthrData;

					if (BotUser::WareHouse.Contains("Basic Leather")) {
						LthrData = BotUser::WareHouse.GetByName("Basic Leather");
					} else if (BotUser::WareHouse.GetByName("Tiger Leather")) {
						LthrData = BotUser::WareHouse.GetByName("Tiger Leather");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Leather found."); 
						return;
					}

					// Make sure there is enough leather to withdraw.
					if (LthrData->Quantity >= AmountToWithdraw) {
						SocketMng::WithdrawItem(LthrData->Index, AmountToWithdraw);
					} else {
						SocketMng::WithdrawItem(LthrData->Index, LthrData->Quantity);
					}

					// Then find metal.
					WAREHOUSE_DATA* MetalData;

					if (BotUser::WareHouse.Contains("Orcish Metal")) {
						MetalData = BotUser::WareHouse.GetByName("Orcish Metal");
					} else if (BotUser::WareHouse.GetByName("Copper Metal")) {
						MetalData = BotUser::WareHouse.GetByName("Copper Metal");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Metal found.");
						return;
					}

					// Make sure there is enough metal to withdraw.
					if (MetalData->Quantity >= (AmountToWithdraw * 3)) {
						SocketMng::WithdrawItem(MetalData->Index, (AmountToWithdraw * 3));
					} else {
						SocketMng::WithdrawItem(MetalData->Index, MetalData->Quantity);
					}
				}
			}
			// Potion.
			else if (CraftBot::Get.Type() == "Potion") {

				if (CraftBot::Get.Recipe() == "10HP") {

					WAREHOUSE_DATA* ItemData;

					int AmountToWithdraw = ((BotUser::Inventory.BwAvailable() / 2) - 2);
				
					// First find horns.
					if (BotUser::WareHouse.Contains("Leocrot Horn")) {
						ItemData = BotUser::WareHouse.GetByName("Leocrot Horn");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Leocrot Horn found.");
						return;
					}

					// Make sure there are enough horns to withdraw.
					if (ItemData->Quantity >= AmountToWithdraw) {
						SocketMng::WithdrawItem(ItemData->Index, AmountToWithdraw);
					} else {
						SocketMng::WithdrawItem(ItemData->Index, ItemData->Quantity);
					}

					// Then find livers.
					if (BotUser::WareHouse.Contains("Wolf Liver")) {
						ItemData = BotUser::WareHouse.GetByName("Wolf Liver");
					} else {
						Bot::State.Terminate(Debug::CRAFTING, "No Metal found.");
						return;
					}

					// Make sure there is enough metal to withdraw.
					if (ItemData->Quantity >= AmountToWithdraw) {
						SocketMng::WithdrawItem(ItemData->Index, AmountToWithdraw);
					} else {
						SocketMng::WithdrawItem(ItemData->Index, ItemData->Quantity);
					}
				}
			}
			CraftBot::State.RestockRequired = false;
			Bot::Print(Debug::GENERAL, "Restock succeeded.");
		}
		Bot::State.IsOpeningNpc = false;
	}
}
void HandleShopOpen(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SHOP_OPEN");

	if (Bot::State.IsRunning) {

		PARAM_SHOPOPEN *data = (PARAM_SHOPOPEN *)bypData;

		//Bot::Print(Debug::GENERAL, "RecievePacketType::SHOP_OPEN\n\t\tShopNumber: %d\n\t\tShopBehaviour: %d\n\t\tBuyRate: %d\n\t\tSellRate: %d\n\t\tItemCount: %d", data->ShopNumber, data->ShopBehaviour, data->BuyRate, data->SellRate, data->ItemCount);

		// Do not show the shop dialogue.
		OriginalProcess = false;

		Bot::State.IsOpeningNpc = false;

		// Npc items if we're crafting.
		if (Bot::Configuration.mode == Mode::CRAFT AND CraftBot::State.NpcRequired) {

			Bot::Print(Debug::CRAFTING, "Emptying inventory.\n");

			for (int i = (CraftBot::State.RestockPending ? 10 : CraftBot::Get.Type() == "Accessory" ? 11 : 12); i < INV_COUNT; i++) {
				if (BotUser::Inventory.Item.IsValid(i)) {
					SocketMng::ShopSell(data->ShopNumber, i, 1);
				}
			} 
			
			Bot::OutPut.TextColourChange();

			CraftBot::State.RestockPending ? 
				CraftBot::State.FlagRestockReq(), Bot::Print(Debug::CRAFTING, "A restock is now required\n") : 
				CraftBot::State.BeginProduction();
		} else {

			bypData += sizeof(PARAM_SHOPOPEN);

			// Parse all of the shops items into a list.
			vector<SHOP_ITEM> ShopItemList;
			for (int i = 0; i < data->ItemCount; i++, bypData += 25) {

				SHOP_ITEM Item;

				Item.Index = ParseData(bypData, short);
				Item.Quantity = Parse(bypData, short, 10);
				Item.Weight = Parse(bypData, short, 16);
				Item.Price = ParseData(bypData, DWORD);
				Item.NameLength = ParseData(bypData, byte);
				Item.Name = ParseString(bypData, Item.NameLength);

				ShopItemList.push_back(Item);
			}

			if (Bot::Configuration.mode == Mode::RESTOCK) {

				// Attempt to find the restock materials.
				for (auto xt = RestockBot::Get.MaterialListBegin();; xt++) {

					// Material amount is zero.
					if (xt == RestockBot::Get.MaterialListEnd()) {			
						if (Bot::State.IsWaiting) {
							Bot::Print(Debug::CRAFTING, "Restock material quantity is still zero.");
						} else {
							Bot::Print(Debug::CRAFTING, "Restock material quantity is zero... Will check for materials again when the soma hour reaches [%d].", (Bot::Configuration.GameHour + 1));
							Bot::State.SetWait(true);
						} break;
					} else {
						vector<SHOP_ITEM>::iterator ShopItem = find(ShopItemList.begin(), ShopItemList.end(), *xt);
						if (ShopItem != ShopItemList.end()) {
							if (ShopItem->Quantity > 0) {
								// Determine the amount of material to buy.
								ShopItem->pQ = (ShopItem->Quantity >= BotUser::Inventory.BwAvailable() ? BotUser::Inventory.BwAvailable() : ShopItem->Quantity);

								// Make sure we have enough money to buy that amount of material.
								if (BotUser::Me->Data->Barr < (ShopItem->Price * ShopItem->pQ)) {
									Bot::State.Terminate(Debug::CRAFTING, "Not enough money to restock."); break;
								} else {
									// Tell the bot thread to wait and then to buy the material (Buying too soon can be seen from other clients and is suspicious).
									RestockBot::Set.ShopEntry(*ShopItem, data->ShopNumber);

									// Wake the bot from wait mode if we have been waiting.
									if (Bot::State.IsWaiting) {
										Bot::State.SetWait(false);
									} break;
								}
							}
						}
					}
				}
			}
			else if (Bot::Configuration.mode == Mode::LEVEL) {
				if (Bot::State.IsRestocking) {

					vector<SHOP_ITEM>::iterator 
						HpPot = BotUser::Me->IsDevil() ? find(ShopItemList.begin(), ShopItemList.end(), "Medium Heal Potion") : find(ShopItemList.begin(), ShopItemList.end(), "Healing Potion"),
						MpPot = BotUser::Me->IsDevil() ? find(ShopItemList.begin(), ShopItemList.end(), "Medium Mana Potion") : find(ShopItemList.begin(), ShopItemList.end(), "Mana Potion"),
						Tp = find(ShopItemList.begin(), ShopItemList.end(), "Town Portal Scroll");

					HpPot->pQ = ((BotUser::Inventory.BwAvailable() / HpPot->Weight) - 10);

					// Buy less potions when melee hunting WRAITH.
					if (Bot::Configuration.TargetList.Contains("WRAITH")) {
						if (!Bot::State.IsTrainingInt) {
							HpPot->pQ = (HpPot->pQ / 4);
						}
					}

					// Buy +50 MP instead of +25 if we're hunting: Prism, NM or LL and our Int is over 77.
					if (Bot::Configuration.TargetList.Contains("PRISMATIC SHADOW") OR Bot::Configuration.TargetList.Contains("NIGHTMARE") OR Bot::Configuration.TargetList.Contains("LICH LORD")) {
						if (Bot::Zone.Current() == Map::ABIAS) {
							if (BotUser::Me->Data->Stats.Intelligence >= 770) {
								MpPot = find(ShopItemList.begin(), ShopItemList.end(), "Large Mana Potion");
							}
						}
					}

					// Buy POTIONS.
					if (Bot::State.IsTrainingInt) {
						if (BotUser::Me->Data->Barr >= (MpPot->Price * ((BotUser::Inventory.BwAvailable() / MpPot->Weight) - 10))) {
							MpPot->pQ = ((BotUser::Inventory.BwAvailable() / MpPot->Weight) - 5);
							SocketMng::ShopBuy(data->ShopNumber, *MpPot);
						} else {
							Bot::State.Terminate(Debug::GENERAL, "Not enough barr to buy MP potions");
						}
					} else {
						if (BotUser::Me->Data->Barr >= (HpPot->Price * HpPot->pQ)) {
							SocketMng::ShopBuy(data->ShopNumber, *HpPot);
						} else {
							Bot::State.Terminate(Debug::GENERAL, "Not enough barr to buy HP potions");
						}
					}

					// Buy TP's.
					if (BotUser::Me->Data->Barr >= (Tp->Price * 2)) {
						ITEM_DATA* Item;
						if (BotUser::Inventory.GetByName(Tp->Name, Item)) {
							if (Item->Dura.Current <= 2) {
								Tp->pQ = Item->Dura.Current == 2 ? 1 : 2;
							}
						} else {
							Tp->pQ = 2;
						} SocketMng::ShopBuy(data->ShopNumber, *Tp);					
					}
				}
			}		
		} 
	}
}
void HandleRepairOpen() {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::REPAIR_OPEN");

	if (Bot::State.IsRunning) {
		if (Bot::State.IsRepairing) {
	
			// Do not show the repair dialogue.
			OriginalProcess = false;

			Bot::State.IsOpeningNpc = false;

			// Repair items.
			for (int i = 0; i < INV_COUNT; i++) {
				if (BotUser::Inventory.Item.IsRepairable(i)) {
					SocketMng::SendRepair(i);
				}
			} 
		}
	}
}

// Attacks.
void HandleAttack(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::MELEE_ATTACK");

	PARAM_ATTACK *data = (PARAM_ATTACK *)bypData;

	if (data->Uid.Src == BotUser::Me->Data->Uid) {
		Bot::Print(Debug::PACKETPROCESS, "Success: %d\nSrcUid: %d\tDstUid: %d\nNewHp: %d\tNewMaxHp: %d\nExp: %d\n",
			data->Result, data->Uid.Src, data->Uid.Dst, data->Health.NewCurrent, data->Health.NewMax, data->ExpGiven);
	}

	CUser *pAttacker = nullptr;
	if (UserManager::UserList.Get(data->Uid.Src, pAttacker)) {

		// Set the attacking user's target.
		CUser *pTarget = nullptr;
		if (UserManager::UserList.Get(data->Uid.Dst, pTarget)) {

			// But only if the attacker is not the bot user.
			if (data->Uid.Src != BotUser::Me->Data->Uid) {
				if (pAttacker->TargetUid == 0) {
					Bot::Print(Debug::TARGET, "Set [%s] [Uid: %d] target to [%s] [Uid: %d]", pAttacker->Data->Name.GetString(), pAttacker->Data->Uid, pTarget->Data->Name.GetString(), pTarget->Data->Uid);
					pAttacker->Target = pTarget;
					pAttacker->TargetUid = pTarget->Data->Uid;
				}
			}

			//if (Bot::Configuration.DamageOverHead) {
			//	if (data->Uid.Dst == BotUser::Me->Data->Uid) {
			//		// Damage.
			//		if ((BotUser::Me->Data->Health.Current - data->Health.NewCurrent) > 0) {
			//			//DDrawHook::BotUserDmg.Add(DmgInfo((BotUser::Me->Data->Health.Current - data->Health.NewCurrent), GetTickCount(), DmgType::ATTACK, true));
			//		}
			//	}
			//	else if (data->Uid.Src == BotUser::Me->Data->Uid) {
			//		// Damage.
			//		if ((BotUser::Me->Data->Health.Current - data->Health.NewCurrent) > 0) {
			//			//DDrawHook::BotUserDmg.Add(DmgInfo((BotUser::Me->Data->Health.Current - data->Health.NewCurrent), GetTickCount(), DmgType::ATTACK, false));
			//		}
			//	}
			//}
		}

		// Special conditions.
		if (Bot::State.IsRunning) {
			if (data->Uid.Dst == BotUser::Me->Data->Uid) {
				if (Bot::State.IsTrainingInt) {
					if (Bot::State.HasTarget) {
						if (!Bot::Configuration.IgnoreList.Contains(pAttacker->Data->Name.GetString())) {
							// Custom condition to attack annoyance mobs when they're hitting us (If we're on Abias hunting Nightmare).
							if (Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
								if (BotUser::Me->TargetUid != 0 AND BotUser::Me->TargetUid != pAttacker->Data->Uid) {
									if ((pAttacker->Data->Name == "SWAMP BEAST" OR pAttacker->Data->Name == "STIGE") AND pAttacker->RangeFromCell(BotUser::Me->Data->CellPos) <= ((BotUser::MagicList.AtkSpell->Range - 1) * 2)) {
										Bot::Print(Debug::TARGET, "Switching target to [%s] [Uid: %d [R: %d]", pAttacker->Data->Name, pAttacker->Data->Uid, pAttacker->RangeFromCell(BotUser::Me->Data->CellPos));
										BotUser::Me->Target = pAttacker;
										BotUser::Me->TargetUid = pAttacker->Data->Uid;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (Bot::State.IsRunning) {

		if (Bot::State.HasTarget) {
			if (!Bot::State.Attack.IsAttacking) {
				// Determine whether or not another player is hitting our target as we're approaching it or that the target has not acquired a target other than us.
				if (data->Uid.Dst == BotUser::Me->TargetUid AND data->Uid.Src != BotUser::Me->Data->Uid) {
					Bot::Print(Debug::TARGET, "Another user has begun attacking my target... Resetting."); 
					Bot::State.Attack.Reset();
				} else if (data->Uid.Src == BotUser::Me->TargetUid AND data->Uid.Dst != BotUser::Me->Data->Uid) {
					Bot::Print(Debug::TARGET, "Target has acquired a new target... Resetting."); 
					Bot::State.Attack.Reset();
				}
			}
		}

		// Check if the attacker has been blacklisted and remove if so.
		if (data->Uid.Dst == BotUser::Me->Data->Uid) {
			if (Bot::Configuration.BlackList.Contains(data->Uid.Src, 1)) {
				Bot::Configuration.BlackList.Remove(data->Uid.Src);
			}
		}

		switch (data->Result) {

			case PARAM_ATTACK::Result::FAILURE:
			case PARAM_ATTACK::Result::UNKNOWN: {

				if (data->Uid.Src == BotUser::Me->Data->Uid) {
					if (Bot::State.Attack.TrackingFailures) {
						Bot::State.Attack.NoteFailure();
					}
				}	
			} break;

			case PARAM_ATTACK::Result::SUCCESS: {

				if (data->Uid.Dst == BotUser::Me->Data->Uid) {

					// Evalute the players new health value on a successful attack from something else.
					BotUser::EvaluateNewValues(data->Health.NewCurrent, 0);
				
					// Pk detection.
					if (data->Uid.Src < 10000) {
						Bot::State.PkDetected();
					}
				}
				else if (data->Uid.Src == BotUser::Me->Data->Uid) {

					// Discontinue the tracking of attack failures as this is a successful attack by us.
					if (Bot::State.Attack.TrackingFailures) {

						// Need this check because sometimes we get an attack packet after the mob has died.
						if (Bot::State.HasTarget)  {
							Bot::Print(Debug::TARGET, "Successful hit detected. Will no longer track attack failures.");
							Bot::State.Attack.TrackingFailures = false;
						}
					}
				}			
			} break;
		}
	}
}
void HandleSpecialAttack(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SPECIAL_ATTACK");

	PARAM_SPECIALATTACK *data = (PARAM_SPECIALATTACK *)bypData;

	//Bot::Print(Debug::GENERAL, "RecievePacketType::SPECIAL_ATTACK\n\t\tSuccess: %d\n\t\tSrcUid: %d\n\t\tDstUid: %d\n\t\tDuration: %d\n\t\tIndex: %d", data->Result, data->Src.Uid, data->Dst.Uid, data->Duration, data->AuraIndex);

	// Auras which have a nice repeated effect:
	/*
		27 HP Drain
		28 Master HP Drain
		29 Multiple Attack
		30 Master Multiple Attack
		*/

	/*1 0 HP Regen
		2 2 Dodging
		3 1 Perfect Defense
		4 3 Piercing Power
		5 4 Dodging
		6 0 Sword Aura
		7 2 Arrow Aura
		8 1 Axe Aura
		9 3 Attack Aura
		10 4 Blow Aura
		11 0 Double Attack
		12 2 Double Shooting
		13 1 Speed Up
		14 3 Double Piercing
		15 4 Repeated Blow
		16 0 HP Recovery
		17 0 MP Recovery
		18 5 Rapid MP Recovery
		19 5 Rapid Magic Casting
		20 5 MP Saver
		21 0 Multiple Attack
		22 0 Master Multiple Attack
		23 1 Paralyze
		24 1 Concussion
		25 2 Trace
		26 2 Master Trace
		27 3 HP Drain
		28 3 Master HP Drain
		29 4 Multiple Attack
		30 4 Master Multiple Attack*/

	if (data->Result == PARAM_SPECIALATTACK::Result::SUCCESS) {

		if (Bot::State.IsRunning) {

			if (data->Src.Uid == BotUser::Me->Data->Uid) {
				// Evaluate the new MP value for the bot user after a successful aura cast.
				BotUser::EvaluateNewValues(0, data->Src.NewMana);			
			}
			else if (data->Dst.Uid == BotUser::Me->Data->Uid) {
				// Pk detection.
				if (data->Src.Uid < 10000) {
					Bot::State.PkDetected();
				}
			}
		}

		// Modify auras.
		if (data->Src.Uid == BotUser::Me->Data->Uid) {
			switch (data->AuraIndex) {
				// Change trace aura to master trace.
				case 25: {
					data->AuraIndex = 26;
				} break;
			}
		}
	}
}
void HandleMagicAttack(PBYTE bypData) {
	//Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::MAGIC_ATTACK\n");

	PARAM_MAGICATTACK *data = (PARAM_MAGICATTACK *)bypData;

	if (data->Src.Uid == BotUser::Me->Data->Uid) {
		if (data->Result == PARAM_MAGICATTACK::Result::SUCCESS) {
			Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::MAGIC_ATTACK\n\t\tSuccess: %d\n\t\tSrcUid: %d\n\t\tDstUid: %d\n\t\tMagicID: %d\n\t\tSrcHp: %d\tSrcMp: %d\n\t\tDstHP: %d\tDstMaxHp: %d\n", data->Result, data->Src.Uid, data->Dst.Uid, data->Dst.MagicId, data->Src.NewHealth, data->Src.NewMana, data->Dst.NewHealth, data->Dst.NewHealthMax);
		}
	}

	switch (data->Result) {

		case PARAM_MAGICATTACK::Result::SUCCESS: {

				if (Bot::State.IsRunning) {

					// Source is me.
					if (data->Src.Uid == BotUser::Me->Data->Uid) {
						// Destination is me.
						if (data->Dst.Uid == BotUser::Me->Data->Uid) {
							// Player is healing him or herself so evaluate new HP & MP values.
							BotUser::EvaluateNewValues(data->Src.NewHealth, data->Src.NewMana);
						}
						else {

							// Player is casting offensive magic so evaluate new MP value.
							BotUser::EvaluateNewValues(0, data->Src.NewMana);

							// Check for a successful weakening cast if we're leveling int.
							if (Bot::State.IsTrainingInt) {

								switch ((Magic)data->Dst.MagicId) {

									case Magic::WEAKENING_LV1:
									case Magic::WEAKENING_LV2:
									case Magic::WEAKENING_LV3:
									case Magic::WEAKENING_LV4: {

										Bot::Print(Debug::USER, "Weakening succeeded.");

										if (BotUser::IsValid(BotUser::Me->Target)) {
											BotUser::Me->Target->Weakened = true;
										}

										Bot::State.Attack.WeakenCastTick = GetTickCount();
										Bot::State.Attack.MagicReadySuccess = false;

									} break;
								}			
							}
						}
					}		

					// BotUser is being attacked by magic.
					else if (data->Dst.Uid == BotUser::Me->Data->Uid) {

						// Check if the attacker has been blacklisted and remove if so.
						if (Bot::Configuration.BlackList.Contains(data->Src.Uid, 1)) {
							Bot::Configuration.BlackList.Remove(data->Src.Uid);
						}
					
						// Player is being attacked by offensive magic so evaluate new HP value.
						BotUser::EvaluateNewValues(data->Dst.NewHealth, 0);

						switch ((Magic)data->Dst.MagicId) {

							// Do not act on heal, giggle, protection, dispell or cure.
							case Magic::GIGGLING_LV1:
							case Magic::GIGGLING_LV2:
							case Magic::GIGGLING_LV3:
							case Magic::GIGGLING_LV4:
							case Magic::HEALING_LV1:
							case Magic::HEALING_LV2:
							case Magic::HEALING_LV3:
							case Magic::HEALING_LV4:
							case Magic::CUREWOUNDS_LV1:
							case Magic::CUREWOUNDS_LV2:
							case Magic::CUREWOUNDS_LV3:
							case Magic::CUREWOUNDS_LV4:
							case Magic::HEALINGWIND_LV1:
							case Magic::HEALINGWIND_LV2:
							case Magic::HEALINGWIND_LV3:
							case Magic::HEALINGWIND_LV4:
							case Magic::CURE_LV1:
							case Magic::CURE_LV2:
							case Magic::CURE_LV3:
							case Magic::CURE_LV4:
							case Magic::PROTECTION_LV1:
							case Magic::PROTECTION_LV2:
							case Magic::PROTECTION_LV3:
							case Magic::PROTECTION_LV4:
							case Magic::DISPELL_LV1:
								break;

							case Magic::CONFUSION_LV1:
							case Magic::CONFUSION_LV2:
							case Magic::CONFUSION_LV3:
							case Magic::CONFUSION_LV4: {
								// Remove the effect so the bot can hunt properly.
								OriginalProcess = false;
								// If we have been confused by a mob then do not attempt to cure.
								if (data->Src.Uid > 10000) {
									break;
								}
							}
							case Magic::SLOW_LV1:
							case Magic::SLOW_LV2:
							case Magic::SLOW_LV3:
							case Magic::SLOW_LV4: 
							case Magic::POISON_LV1:
							case Magic::POISON_LV2:
							case Magic::POISON_LV3:
							case Magic::POISON_LV4: {
								if (data->Src.Uid < 10000) {
									Bot::OutPut.SoundPlay(IDR_PKDETECT);
								}
								if (BotUser::MagicList.Contains("Cure LV")) {

								}
							} break;

							case Magic::BLINDNESS_LV1:
							case Magic::BLINDNESS_LV2:
							case Magic::BLINDNESS_LV3:
							case Magic::BLINDNESS_LV4: {
								if (data->Src.Uid < 10000) {
									if (BotUser::Inventory.Contains("Demon Water")) {
										Bot::OutPut.SoundPlay(IDR_PKDETECT);
										SocketMng::SendUseItem(BotUser::Inventory.GetByName("Demon Water")->Index);
									} else {
										Bot::State.PkDetected();
									}			
								}
							} break;
							
							case Magic::WEAKENING_LV1:
							case Magic::WEAKENING_LV2:
							case Magic::WEAKENING_LV3:
							case Magic::WEAKENING_LV4:
							case Magic::ZOMBIE_LV1:
							default: {
								// Pk detection.
								if (data->Src.Uid < 10000) {
									Bot::State.PkDetected();
								}						
							} break;
						}
					}
				}

				// Bot is not running.
				else {

					// Attempt to use a serum if a debuff is cast upon the bot user (Only evaluate if the bot is NOT running).
					if (data->Dst.Uid == BotUser::Me->Data->Uid) {		
						if (Bot::Configuration.AutoSerum) {
							switch ((Magic)data->Dst.MagicId) {
								case Magic::SLOW_LV1:
								case Magic::SLOW_LV2:
								case Magic::SLOW_LV3:
								case Magic::SLOW_LV4: {
									if (BotUser::Inventory.Contains("Heaven Water")) {
										ITEM_DATA* Item = BotUser::Inventory.GetByName("Heaven Water");
										SocketMng::SendUseItem(Item->Index);
										Bot::Print(Debug::GENERAL, "Debuff detected... Using [%s] at index [%d] remainder [%d]", Item->Name, Item->Index, Item->Dura.Current);
									}			
								} break;

								case Magic::BLINDNESS_LV1:
								case Magic::BLINDNESS_LV2:
								case Magic::BLINDNESS_LV3:
								case Magic::BLINDNESS_LV4:
								case Magic::ZOMBIE_LV1: {
									if (BotUser::Inventory.Contains("Demon Water")) {
										ITEM_DATA* Item = BotUser::Inventory.GetByName("Demon Water");
										SocketMng::SendUseItem(Item->Index);
										Bot::Print(Debug::GENERAL, "Debuff detected... Using [%s] at index [%d] remainder [%d]", Item->Name, Item->Index, Item->Dura.Current);
									}
								} break;

								case Magic::CONFUSION_LV1:
								case Magic::CONFUSION_LV2:
								case Magic::CONFUSION_LV3:
								case Magic::CONFUSION_LV4: {
									// Cancel the magic effect by not running the original PacketProcess.
									OriginalProcess = false;
								} break;

								case Magic::POISON_LV1:
								case Magic::POISON_LV2:
								case Magic::POISON_LV3:
								case Magic::POISON_LV4: {
									if (BotUser::Inventory.Contains("Antidote")) {
										ITEM_DATA* Item = BotUser::Inventory.GetByName("Antidote");
										SocketMng::SendUseItem(Item->Index);
										Bot::Print(Debug::GENERAL, "Debuff detected... Using [%s] at index [%d] remainder [%d]", Item->Name, Item->Index, Item->Dura.Current);
									}
								} break;

								case Magic::WEAKENING_LV1:
								case Magic::WEAKENING_LV2:
								case Magic::WEAKENING_LV3:
								case Magic::WEAKENING_LV4: {
									if (BotUser::Inventory.Contains("Forest Water")) {
										ITEM_DATA* Item = BotUser::Inventory.GetByName("Forest Water");
										SocketMng::SendUseItem(Item->Index);
										Bot::Print(Debug::GENERAL, "Debuff detected... Using [%s] at index [%d] remainder [%d]", Item->Name, Item->Index, Item->Dura.Current);
									}											  
								} break;
							}
						}
					}
				}

				// Set the attacking user's target.
				CUser *pAttacker = nullptr;
				if (UserManager::UserList.Get(data->Src.Uid, pAttacker)) {

					CUser *pTarget = nullptr;
					if (UserManager::UserList.Get(data->Dst.Uid, pTarget)) {

						// But only if the attacker is not the bot user.
						if (data->Src.Uid != BotUser::Me->Data->Uid) {

							// Determine whether or not another player is hitting our target as we're approaching it or that the target has not acquired a target other than us.
							if (Bot::State.IsRunning) {
								if (Bot::State.HasTarget) {
									if (!Bot::State.Attack.IsAttacking) {
										if (data->Dst.Uid == BotUser::Me->TargetUid AND data->Src.Uid != BotUser::Me->Data->Uid) {
											Bot::Print(Debug::TARGET, "User [%s] [%d] has begun attacking my target... Resetting.", pAttacker->Data->Name, data->Src.Uid);
											Bot::State.Attack.Reset();
										} else if (data->Src.Uid == BotUser::Me->TargetUid AND data->Dst.Uid != BotUser::Me->Data->Uid) {
											Bot::Print(Debug::TARGET, "Target has acquired a new target [%s] [%d]. Resetting.", pTarget->Data->Name, data->Dst.Uid);
											Bot::State.Attack.Reset();
										}
									}
								}
							}

							// Set the attacking user's target.
							if (pAttacker->TargetUid == 0) {
								Bot::Print(Debug::TARGET, "Set [%s] [Uid: %d] target to [%s] [Uid: %d]", pAttacker->Data->Name.GetString(), pAttacker->Data->Uid, pTarget->Data->Name.GetString(), pTarget->Data->Uid);
								pAttacker->Target = pTarget;
								pAttacker->TargetUid = pTarget->Data->Uid;
							}
						}  // Track my target if target tracking is enabled.
						else {
							if (Bot::Configuration.TrackTarget) {
								if (!Bot::State.IsRunning) {
									if (data->Dst.Uid != BotUser::Me->Data->Uid) {
										BotUser::Me->Target = pTarget;
										BotUser::Me->TargetUid = pTarget->Data->Uid;
									}
								}
							}
						}

						// Draw damage overhead.
						if (Bot::Configuration.DamageOverHead) {
							// Draw magic damage done to me.
							if (data->Dst.Uid == BotUser::Me->Data->Uid) {
								// Damage.
								if ((BotUser::Me->Data->Health.Current - data->Dst.NewHealth) > 0) {
									DDrawHook::BotUserDmg.FloatingDmgAdd(DmgInfo((BotUser::Me->Data->Health.Current - data->Dst.NewHealth), GetTickCount(), DmgType::ATTACK, true));
								} // Healing.
								else if ((BotUser::Me->Data->Health.Current - data->Dst.NewHealth) < 0) {
									DDrawHook::BotUserDmg.FloatingDmgAdd(DmgInfo((data->Dst.NewHealth - BotUser::Me->Data->Health.Current), GetTickCount(), DmgType::HEAL, true));
								}
							}
							// Draw magic damage done by me.
							else if (data->Src.Uid == BotUser::Me->Data->Uid) {
								//// Damage.
								//if (pTarget->IsPlayer()) {

								//} else {
								//	if (pTarget->Data->Health.Current != 0 AND pTarget->Data->Health.Current != 1) {
								//		if (pTarget->Data->Health.Current == pTarget->Data->Health.Maximum) {
								//			pTarget->PreviousHp = pTarget->Data->Health.Current;
								//		} else if (pTarget->Data->Health.Current < pTarget->PreviousHp) {
								//			//pTarget->PreviousHp = pTarget->Data->Health.Current;
								//		}
								//	}
								//	
								//	/*if ((pTarget->Data->Health.Maximum - pTarget->Data->Health.Current) > 0) {
								//		DDrawHook::BotUserDmg.FloatingDmgAdd(DmgInfo((pTarget->Data->Health.Maximum - pTarget->Data->Health.Current), GetTickCount(), DmgType::ATTACK, false));
								//	}*/
								//}
							}
						}
					}
				}		
			} break;

			case PARAM_MAGICATTACK::Result::FAILURE:
			case PARAM_MAGICATTACK::Result::UNKNOWN: {

				if (Bot::State.IsRunning) {
					// Check for a failed weakening cast if we're leveling int.
					if (Bot::State.IsTrainingInt) {
						switch ((Magic)data->Dst.MagicId) {
							// The MagicId value always shows zero for an unsuccessful cast.
							case Magic::WEAKENING_LV1:
							case Magic::WEAKENING_LV2:
							case Magic::WEAKENING_LV3:
							case Magic::WEAKENING_LV4: 
							default: {
								if (Bot::State.Attack.MagicReadySuccess) {
									Bot::Print(Debug::USER, "Weakening failed.");
									Bot::State.Attack.MagicReadySuccess = false;
									Bot::State.Attack.NoteFailure();
								}
							} break;
						}
					}
				} 
			} break;
	}
}
void HandleMagicReady(PBYTE bypData) {

	PARAM_MAGICREADY *data = (PARAM_MAGICREADY *)bypData;

	if (data->Uid.Src == BotUser::Me->Data->Uid) {
		if (data->Result == PARAM_MAGICREADY::Result::SUCCESS) {
			Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::MAGIC_READY\n\t\tSuccess: %d\n\t\tSrcUid: %d\n\t\tDstUid: %d\n\t\tMagicID: %d\n\t\tCastTime: %d\n", data->Result, data->Uid.Src, data->Uid.Dst, data->MagicId, data->StartTime);
		}
	}

	//if (data->Uid.Src == BotUser::Me->Data->Uid) {
	//	if (data->Result == PARAM_MAGICREADY::Result::SUCCESS) {
	//		switch ((Magic)data->MagicId) {
	//			case Magic::WEAKENING_LV1:
	//			case Magic::WEAKENING_LV2:
	//			case Magic::WEAKENING_LV3:
	//			case Magic::WEAKENING_LV4:
	//			case Magic::HEALING_LV1:
	//			case Magic::HEALING_LV2:
	//			case Magic::HEALING_LV3:
	//			case Magic::HEALING_LV4:
	//				//break;

	//			default: {
	//				data->StartTime -= Bot::Configuration.Test;
	//				printf_s("StartTime is now [%d]\n", data->StartTime);
	//			} break;
	//		}
	//	}
	//}

	if (data->Uid.Src == BotUser::Me->Data->Uid) {
		if (data->Result == PARAM_MAGICREADY::Result::SUCCESS) {	
		
			// Reduce the casting time.
			if (Bot::Configuration.CastReduction) {
				int Reduction = 120;
				data->StartTime -= Reduction;
			}

			// Check for a weakening cast from the bot user.
			if (Bot::State.IsRunning) {
				switch ((Magic)data->MagicId) {
					case Magic::WEAKENING_LV1:
					case Magic::WEAKENING_LV2:
					case Magic::WEAKENING_LV3:
					case Magic::WEAKENING_LV4: {
						if (!Bot::State.Attack.MagicReadySuccess) {
							Bot::Print(Debug::GENERAL, "MagicReadySuccess is now TRUE.");
							Bot::State.Attack.MagicReadySuccess = true;
						}
					} break;
				}
			}
		}
	}
}
void HandleMagicCircle(PBYTE bypData) {
	//Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::PARAM_MAGICCIRCLE\n");
	//PARAM_MAGICCIRCLE *data = (PARAM_MAGICCIRCLE *)bypData;
}

// Misc.
void HandleChat(PBYTE bypData) {

	ChatType chatType = ParseEnum(bypData, ChatType);

	switch (chatType) {

		case ChatType::NORMAL: {
			if (Bot::State.IsRunning AND Bot::Configuration.ChatNotify) {
				int SrcUid = ParseData(bypData, int);
				bypData += 2;

				BYTE SenderNameLen = ParseData(bypData, BYTE);
				CString SenderName = ParseString(bypData, SenderNameLen);

				BYTE MessageLen = ParseData(bypData, BYTE);
				CString Message = ParseString(bypData, MessageLen);

				//Bot::Print(Debug::CHAT, "[NPC] %s [Uid: %d]", Message, SrcUid);

				if (MessageLen > 0) {
					// This prevents chats from NPC's.
					if (SrcUid < 10000) {
						// Do not process messages from me.
						if (SrcUid != BotUser::Me->Data->Uid) {
							CUser *pUser = nullptr;
							if (UserManager::UserList.GetByName(SenderName, pUser)) {
								if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) <= 20) {
									Bot::Print(Debug::CHAT, "[NORMAL] %s: %s", SenderName, Message);
									Bot::OutPut.SoundPlay(IDR_CHAT);
								}
							}
						}
					}
				}
			}
		} break;

		case ChatType::SHOUT: {
			//int SrcUid = ParseData(bypData, int);
			//bypData += 2;
			//BYTE SenderNameLen = ParseData(bypData, BYTE);
			//CString SenderName = ParseString(bypData, SenderNameLen);
			//BYTE MessageLen = ParseData(bypData, BYTE);
			//CString Message = ParseString(bypData, MessageLen);
			////Bot::Print("[SHOUT] %s: %s", SenderName, Message);			
		} break;

		case ChatType::GUILD: {
			if (Bot::State.IsRunning AND Bot::Configuration.ChatNotify) {
				int SrcUid = ParseData(bypData, int);

				BYTE SenderNameLen = ParseData(bypData, BYTE);
				CString SenderName = ParseString(bypData, SenderNameLen);

				BYTE MessageLen = ParseData(bypData, BYTE);
				CString Message = ParseString(bypData, MessageLen);

				//Bot::Print("[GUILD] %s: %s", SenderName, Message);

				if (SrcUid < 10000 AND SrcUid != BotUser::Me->Data->Uid) {
					if (Message.Find(CString(BotUser::Me->Data->Name.GetString()).MakeLower()) != -1) {
						Bot::Print(Debug::CHAT, "[GUILD] %s: %s", SenderName, Message);
						Bot::OutPut.SoundPlay(IDR_CHAT);
					}
				}
			}
		} break;

		case ChatType::PARTY: {
								  if (Bot::Configuration.mode == Mode::HEAL) {

									  int SrcUid = ParseData(bypData, int);

									  BYTE SenderNameLen = ParseData(bypData, BYTE);
									  CString SenderName = ParseString(bypData, SenderNameLen);

									  BYTE MessageLen = ParseData(bypData, BYTE);
									  CString Message = ParseString(bypData, MessageLen);

									  //printf_s("[PARTY] %s", Message);
								  }
		} break;

		case ChatType::PRIVATE_INCOMING: {
			if (Bot::State.IsRunning) {
				if (Bot::Configuration.ChatNotify) {
					bypData += 3;

					BYTE SenderNameLen = ParseData(bypData, BYTE);
					CString SenderName = ParseString(bypData, SenderNameLen);

					BYTE MessageLen = ParseData(bypData, BYTE);
					CString Message = ParseString(bypData, MessageLen);

					if (MessageLen > 0)  {

						// Detect dabura specific messages.
						if (Message.Find("<B2D>") != -1) {

							Bot::Print(Debug::GENERAL, "Detected Dabura specific message.");

							// Do not show this message.
							OriginalProcess = false;

							if (Message.MakeLower().Find("wru") != -1) {
								CString Msg = "I am at X: " + BotUser::Me->Data->CellPos.x; Msg.Append(" Y: " + BotUser::Me->Data->CellPos.y);
								SocketMng::SendPm(BotUser::Me->Data->Name.GetString(), Msg);
							}
							else if (Message.Find("logout") != -1) {
								SocketMng::SendLogOut();
							}
							else if (Message.Find("stop bot") != -1) {
								Bot::State.Terminate(Debug::GENERAL, NULL);
							}
							else if (Message.Find("gimme barr") != -1) {
								SocketMng::ThrowMoney(BotUser::Me->Data->Barr / 4);
							}
							else if (Message.Find("get naked") != -1) {
								for (int i = 0; i < 10; i++) {
									SocketMng::ThrowItem(i, 1);
								}
							}
						}
						else {
							Bot::Print(Debug::CHAT, "[PM] %s: %s", SenderName, Message);
							Bot::OutPut.SoundPlay(IDR_CHAT);
						}
					}

					//if (Bot::Configuration.ChatRespond) {
					//	CString Greetings[] = {
					//		"hi", "hii", "yo", "yoo", "hy", "hey", "hello", "helllo", "helo", "elo", "lo",
					//		"sup", "whats up", "what up", "what's up",
					//		"alreet", "alrite", "alright",
					//		"salu", "greetings", "ciao"
					//		":p", ";p", ";o", ":o", "=]", ";)", ":)", ";]", ":]" };
					//	CString Deny[] = {
					//		"no", "nope", "nop", "nup", "nay"
					//		"nah", "na",
					//		"?", "?!?", "???" };
					//	CString Other[] = {
					//		"bot", "afk", "respond"
					//	};
					//	//Message.MakeLower();
					//	//SocketMng::SendPm(SenderName, Message);
					//	//for (int i = 0; i < 3; i++) {
					//	//	if (Message.Find(Other[i]) != -1) {
					//	//		Bot::Print(Debug::CHAT, "CString [%s] was found in the message.", Other[i]); break;
					//	//	}
					//	//}
					//	//for (int i = 0; i < 29; i++) {
					//	//	if (Message.Find(Greetings[i]) != -1) {
					//	//		Bot::Print(Debug::CHAT, "CString [%s] was found in the message.", Greetings[i]); break;
					//	//	}
					//	//}
					//	//printf_s("\n");
					//}			
				}
			}
		} break;

		case ChatType::PRIVATE_OUTGOING:
			break;

		case ChatType::GM_NOTICE:
			break;

		case ChatType::SYSTEM: {
			if (Bot::State.IsRunning) {
				if (Bot::Configuration.GmDetection.IsEnabled) {

					BYTE MessageLen = ParseData(bypData, BYTE);
					CString Message = ParseString(bypData, MessageLen);

					//Bot::Print(Debug::CHAT, "System Message [%s]", Message);

					//CString GmName = *Bot::Configuration.GmDetection.CurrentGm;
					//if (Message.Find(GmName) != -1) {

					//	if (Message.Find(GmName + " was last online: Not") != -1) {

					//		if (++Bot::Configuration.GmDetection.CurrentGm != Bot::Configuration.GmDetection.GmList.end()) {
					//			SocketMng::SendChat("/lastonline " + *Bot::Configuration.GmDetection.CurrentGm);
					//		}
					//		else {
					//			Bot::Configuration.GmDetection.CurrentGm = Bot::Configuration.GmDetection.GmList.begin();
					//		}

					//		// Do now show the messages ingame.
					//		OriginalProcess = false;
					//	} // Make sure the message actually matches so we don't detect wrong messages.
					//	else if (Message.Find(GmName + " was last online: Now")) {

					//		Bot::Print(Debug::GMDETECTION, "GM [%s] detected as online.\n", GmName);

					//		switch (Bot::Configuration.GmDetection.LiveResponse) {

					//			case Response::DISCONNECT: {
					//				Bot::OutPut.SoundPlay(IDR_NEARBYUSER);
					//				// Kill all soma clients if we're heal wind botting.
					//				if (Bot::Configuration.mode == Mode::HEALBOT) {
					//					system("taskkill /F /T /IM SomaWindow.exe");
					//				} else {
					//					SocketMng::SendLogOut();
					//				} return;
					//			} break;

					//			case Response::NOTIFY: {
					//				Bot::NearbyUserDetection.IsNotifying = true;
					//			} break;
					//		}
					//	}
					//}
				}
			}
		} break;

		case ChatType::YELLOW_STAT:
		case ChatType::BLUE_STAT: {
			if (Bot::StatTracker.IsEnabled) {

				BYTE MessageLen = ParseData(bypData, BYTE);
				CString Message = ParseString(bypData, MessageLen);

				if (Message.Find("Strength") != -1) {
					printf_s("\n\n\t\t%s\n", Message);
					Bot::StatTracker.Add(Stat::STR);
				} else if (Message.Find("Dexterity") != -1) {
					printf_s("\n\n\t\t%s\n", Message);
					Bot::StatTracker.Add(Stat::DEX);
				} else if (Message.Find("Intelligence") != -1) {
					printf_s("\n\n\t\t%s\n", Message);
					Bot::StatTracker.Add(Stat::INT);
				} else if (Message.Find("Constitution") != -1) {
					printf_s("\n\n\t\t%s\n", Message);
					Bot::StatTracker.Add(Stat::CON);
				}
			}
		} break;

		case ChatType::ZONE: {
								 /*int SrcUid = ParseData(bypData, int);
								 bypData += 2;
								 ColorRGB rgb = ParseData(bypData, ColorRGB);
								 BYTE SenderNameLen = ParseData(bypData, BYTE);
								 CString SenderName = ParseString(bypData, SenderNameLen);
								 BYTE MessageLen = ParseData(bypData, BYTE);
								 CString Message = ParseString(bypData, MessageLen);*/
								 //Bot::Print("[ZONE] %s: %s", SenderName, Message);
		} break;

		default: {
			//BYTE MessageLen = ParseData(bypData, BYTE);
			//CString Message = ParseString(bypData, MessageLen);
			//Bot::Print(Debug::CHAT, "[UNKNOWN] %s", Message);				
		} break;
	}
}
void HandleSetTime(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SET_TIME");

	PARAM_SETTIME *data = (PARAM_SETTIME *)bypData;

	// Update the game hour.
	Bot::Configuration.GameHour = data->Hour;

	// Remove darkness.
	if (Bot::Configuration.NoDark) {
		data->Night = 4;
	}

	if (Bot::State.IsRunning) {
		if (Bot::Configuration.mode == Mode::LEVEL) {
			if (Bot::Zone.Current() == Map::VOD) {
				if (data->Hour == 6) {
					// Sound a notification that Devils may be nearby
				}
			}
		}
		else if (Bot::Configuration.mode == Mode::RESTOCK) {
			if (Bot::State.IsWaiting) {
				if (data->Hour < 22 AND data->Hour > 0) {
					BotUser::FindNpc(RestockBot::Vendor.Name);
				} else {
					Bot::Print(Debug::CRAFTING, "Shop is closed.");
				}
			}
		}
	}
}
void HandleItemFieldInfo(PBYTE bypData) {

	Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::ITEM_FIELD_INFO");

	if (Bot::State.IsRunning) {

		if (!Bot::Configuration.NoLoot) {

			short DroppedItemCount = ParseData(bypData, short);

			for (int i = 0; i < DroppedItemCount; i++) {

				ItemFieldType Type = ParseEnum(bypData, ItemFieldType);
				CPoint Point = ParseData(bypData, int);

				switch (Type) {

					case ItemFieldType::NEW: {

						ITEMFIELD_DATA data;

						data.Point = Point;
						data.PictureId = ParseData(bypData, short);
						data.Type = ParseData(bypData, byte);	// This is the items rarity.
						data.Quantity = ParseData(bypData, int);
						byte ItemNameLength = ParseData(bypData, byte);
						data.Name = ParseString(bypData, ItemNameLength);
						data.RFM = UserManager::GetCellRange(BotUser::Me->Data->CellPos, data.Point);
						data.RareItem = Bot::Loot.RareItemList.Contains(data.Name);

						//Bot::Print(Debug::LOOT, "Adding item to the FieldList...\nName: %s\nCell: %d %d\nType: %d\nQuantity: %d\n", data.Name, Point.x, Point.y, data.Type, data.Quantity);

						if (!Bot::Loot.Filter.Contains(data.Name)) {
							Bot::Loot.FieldList.Add(data);
						}

					} break;

					case ItemFieldType::REMOVE: {
						//Bot::Print(Debug::LOOT, "Removing item [X: %d Y: %d] from the FieldList.", Point.x, Point.y);
						Bot::Loot.FieldList.Remove(Point);
					} break;
				}
			}
		}
	}	
}

BOOL WINAPI PacketParserHook::MY::PacketProcess(PBYTE bypData, int size) {

	OriginalProcess = true;

	switch ((PacketType)bypData[0]) {

		// Movement
	case PacketType::MOVEFIRST:
	case PacketType::MOVEMIDDLE:
	case PacketType::MOVEEND:
	case PacketType::RUN_MOVEFIRST:
	case PacketType::RUN_MOVEMIDDLE:
	case PacketType::RUN_MOVEEND:
		HandleMove(bypData[0], bypData + 1);
		break;

		// Attack
	case PacketType::ATTACK:
		HandleAttack(bypData + 1);
		break;
	case PacketType::SPECIAL_ATTACK_CIRCLE:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SPECIAL_ATTACK_CIRCLE");
	case PacketType::SPECIAL_ATTACK_RAIL:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SPECIAL_ATTACK_RAIL");
	case PacketType::SPECIAL_ATTACK:
		HandleSpecialAttack(bypData + 1);
		break;

		// Magic
	case PacketType::MAGIC_ALL:
		HandleMagicAll(bypData + 1);
		break;
	case PacketType::MAGIC_ATTACK:
		HandleMagicAttack(bypData + 1);
		break;
	case PacketType::MAGIC_READY:
		HandleMagicReady(bypData + 1);
		break;
	case PacketType::MAGIC_CIRCLE:
		HandleMagicCircle(bypData + 1);
		break;
	case PacketType::SHOW_MAGIC:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SHOW_MAGIC");
		break;
	case PacketType::EFFECT:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::EFFECT");
		break;
	case PacketType::STATUS_INFO:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::STATUS_INFO");
		break;
	case PacketType::POISON:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::POISON");
		break;

		// Crafting
	case PacketType::CRAFTTABLE_OPEN: {
		if (Bot::State.IsRunning) {
			if (Bot::Configuration.mode == Mode::CRAFT) {
				Bot::Print(Debug::CRAFTING, "Crafting...");
				CraftBot::Table.IsOpen = true;
				Bot::State.IsOpeningNpc = false;
			}
		}	
	} break;
	case PacketType::CRAFTITEM:
		HandleCraftItem(bypData + 1);
		break;


		// Npc Dialogue
	case PacketType::NPC_DIALOGUE_SELECTION:
		HandleNpcDialogue(bypData + 1);
		break;


		// Shop
	case PacketType::SHOP_OPEN:
		HandleShopOpen(bypData + 1);
		break;
	case PacketType::GETITEM:
		HandleGetItem(bypData + 1);
		break;


		// Repair
	case PacketType::REPAIR_OPEN:
		HandleRepairOpen();
		break;
	case PacketType::REPAIR_REQ:
		// This is the box that appears after you have clicked repair.
		OriginalProcess = false;
		break;


		// Warehouse
	case PacketType::WAREHOUSE_OPEN:
		HandleWarehouseOpen(bypData + 1);
		break;
	case PacketType::WITHDRAW_ITEM:
		break;


		// TP
	case PacketType::TOWNPORTALREQ:
	case PacketType::TOWNPORTALEND:
		HandleTownPortal(bypData);
		break;


		// Items
	case PacketType::ITEM_FIELD_INFO:
		HandleItemFieldInfo(bypData + 1);
		break;
	case PacketType::ITEM_INFO:
		// Received when switching armours and weps etc
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::ITEM_INFO");
		break;
	case PacketType::ITEM_CHANGE_INFO:
		HandleChangeItemInfo(bypData + 1);
		break;

		// Character
	case PacketType::ZONECHANGE:
		HandleZoneChange(bypData + 1);
		break;
	case PacketType::AREASTATE:
		HandleAreaState(bypData + 1);
		break;
	case PacketType::USERMODIFY:
		// Delete a user before the original PacketProcess so that the user's data is still available to the game client.
		HandleUserDelete(bypData + 1);
		break;
	case PacketType::CHARTER_DATA:
		HandleCharterData(bypData + 1);
		break;
	case PacketType::SETGRAY:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::SETGRAY");
		break;
	case PacketType::ACOUNT_LOGIN:
		HandleAccountLogin(bypData + 1);
		break;
	case PacketType::LOGIN:
		HandleLogin();
		break;
	case PacketType::PUTITEM_BELT:
		HandleRecItemToBelt(bypData + 1);
		break;
	case PacketType::ITEM_USE:
		HandleItemUse(bypData + 1);
		break;

		// Misc
	case PacketType::BATTLE:
		break;
	case PacketType::PARTY_INVITE:
		HandlePartyInvite(bypData + 1);
		break;
	case PacketType::CHAT:
		HandleChat(bypData + 1);
		break;
	case PacketType::LIVEBROADCAST:
		Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::LIVEBROADCAST");
		break;

	case PacketType::DEAD:
		HandleDeadUser(bypData + 1);
		break;
	case PacketType::MOBHP:
		Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::MOBHP");
		break;
	case PacketType::PARTY_LOCATION:
		Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::PARTY_LOCATION");
		break;
	case PacketType::SET_TIME:
		HandleSetTime(bypData + 1);
		break;
	case PacketType::SPEEDHACK:
		Bot::Print(Debug::PACKETPROCESS, "ReceivePacketType::SPEEDHACK");
		break;
	case PacketType::SETRUNMODE:
		break;
	case PacketType::HPMP_RECOVERY:
		break;
	case PacketType::DUMMY_ATTACK:
		break;
	case PacketType::ENCRYPTION_START_RESULT:
		break;
	case PacketType::INV_ALL:
		break; 
	case PacketType::CONTRIBUTION_STATUS:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::CONTRIBUTION_STATUS");
		break;
	case PacketType::ALL_CHAT_STATUS:
		Bot::Print(Debug::PACKETPROCESS, "RecievePacketType::ALL_CHAT_STATUS");
		break;

		// Unknown packet
	default:
		Bot::Print(Debug::PACKETPROCESS, "Received UNKNOWN packet\n\tSize: %d\tIndex: %X\n", size, bypData[0]);
		break;
	}

	if (OriginalProcess) {
		// Call the original function. 
		PREPARE_PACKETPARSER_CALL();
		((tPacketProcess)CC::PacketProcess.OriginalFunction())(bypData, size);
	}

	switch ((PacketType)bypData[0]) {

		case PacketType::USERMODIFY: {
			// Add or modify a user after the original PacketProcess so that the game data is already available from within the client.
			HandleUserAddModify(bypData + 1);
		} break;

		//case PacketType::MAGIC_ATTACK: {

		//	PARAM_MAGICATTACK* data = (PARAM_MAGICATTACK *)(bypData + 1);
		//	printf_s("[%d] [%d]\n", data->Src.Uid, data->Dst.Uid);
		//	CUser *pTarget = nullptr;
		//	if (UserManager::UserList.Get(data->Dst.Uid, pTarget)) {

		//		// Draw damage overhead.
		//		if (Bot::Configuration.DamageOverHead) {

		//			// Draw magic damage done by me.
		//			if (data->Src.Uid == BotUser::Me->Data->Uid) {
		//				//// Damage.
		//				//if (pTarget->Data->Health.Current != 0 AND pTarget->Data->Health.Current != 1) 
		//				//{
		//				//	//if (pTarget->Data->Health.Current == pTarget->Data->Health.Maximum) 
		//				//	{
		//				//		//pTarget->PreviousHp = pTarget->Data->Health.Current;
		//				//	}
		//				//	//else 
		//				//	if (pTarget->Data->Health.Current < pTarget->PreviousHp) 
		//				//	{
		//				//		pTarget->PreviousHp = pTarget->Data->Health.Current;
		//				//	}
		//				//}

		//				/*if ((pTarget->Data->Health.Maximum - pTarget->Data->Health.Current) > 0) {
		//				DDrawHook::BotUserDmg.FloatingDmgAdd(DmgInfo((pTarget->Data->Health.Maximum - pTarget->Data->Health.Current), GetTickCount(), DmgType::ATTACK, false));
		//				}*/

		//			}
		//		}
		//	}
		//} break;
	}

	return OriginalProcess;
}
BOOL WINAPI PacketParserHook::MY::RawPacketProcess(PBYTE bypData, int size) {

	// Client doesn't like this
	//PacketType PktType = (PacketType)ParseData(bypData, WORD);

	//WORD PktType = *(WORD *)bypData;
	//printf_s("%X %d\n", PktType, PktType);

	switch (bypData[0]) {
		case SM_GAMEINFO_ACK:
			Bot::Print(Debug::PACKETPROCESS, "RecieveRawPacketType::SM_GAMEINFO_ACK");
			break;
		case SM_LOGIN_ACK:
			Bot::Print(Debug::PACKETPROCESS, "RecieveRawPacketType::SM_LOGIN_ACK");
			Bot::Print(Debug::PACKETPROCESS, "Result [%d] [%s]", bypData[4], (bypData[4] == 0 || bypData[4] == 1) ? "LOGIN SUCCEEDED" : "LOGIN FAILED");
			if (bypData[4] == 0 OR bypData[4] == 1) {
				bypData[4] = 0; 
			}
			break;
		case SM_GAME_ACK:
			Bot::Print(Debug::PACKETPROCESS, "RecieveRawPacketType::SM_GAME_ACK");
			break;
		case SM_CONNECTINFO_ACK:
			Bot::Print(Debug::PACKETPROCESS, "RecieveRawPacketType::SM_CONNECTINFO_ACK");
			break;
		default:
			Bot::Print(Debug::SOCKETMNG, "RecieveRawPacketType [%X]", bypData[0]);
			break;
	}

	// Call the original function. 
	PREPARE_PACKETPARSER_CALL();
	BOOL result = ((tRawPacketProcess)CC::RawPacketProcess.OriginalFunction())(bypData, size);
	return result;
}