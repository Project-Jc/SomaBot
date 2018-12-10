#pragma once

enum class Debug : BYTE {
	GENERAL,
	LOOT,
	USERLIST,
	CRAFTING,
	MOVEMENT,
	CHAT,
	TARGET,
	WAIT,
	NPC,
	STAMINA,
	PACKETPROCESS,
	GMDETECTION,
	OUTPUT,
	POINTGENERATION,
	INPUT,
	USER,
	SOCKETMNG,
	HEAL
};

enum class FileType : BYTE {
	LVL_PATH = 0,
	PRELVL_PATH,
	VENDOR_PATH,
	BLKSMITH_PATH,
	CRAFT_PATH,
	RESTOCK_PATH,
	TEMPORARY_PATH
};

enum class Behaviour : BYTE {
	INSANE,
	AGGRESSIVE,
	CASUAL,
	PENSIONER
};

enum class Mode : BYTE {
	LEVEL,
	HEAL,
	RESTOCK,
	CRAFT
};

enum class Map : BYTE {
	TYT = 1,
	MERC = 4,
	ABIAS,
	PAND = 9,
	PAND_CHANDALIR,
	IC,
	VOD,
	CASTLE = 14,
	CASTLE_THRONE,
	CASTLE_CRAFT,
	CONTI,
	HWAN,
	HWAN_CASTLE_LV1,
	HWAN_CASTLE_LV2,
	DEVDUNG
};

enum class Response : BYTE {
	NORESPONSE,
	DISCONNECT,
	NOTIFY
};

enum class Stat : BYTE {
	STR,
	DEX,
	INT,
	CON
};

struct StatInfo {
	Stat Type;
	short Value;
	DWORD Time;
	int Exp;

	StatInfo(Stat type, short value, DWORD time, int exp) : 
		Type(type), Value(value), Time(time), Exp(exp) {}

	CString ValueAsCString(void) {
		CString Stat;
		Stat.Format("%d", Value);
		Stat.Insert(Stat.GetLength() - 1, '.');
		return Stat;
	}
};

class CraftBot {
private:
	static CString CProfession;
	static CString CRecipe;
public:

	static struct CraftBotState {
		bool RestockRequired;
		bool RestockPending;
		bool NpcRequired;

		void CancelProduction(bool RestockPending) {

			Table.IsOpen = false;
			Table.IsOpening = false;
	
			State.RestockPending = RestockPending;

			if (CProfession == "Accessory" || CProfession == "Armour" || CProfession == "Weapon") {
				State.NpcRequired = true;
			} else {
				State.RestockRequired = true;
			}
		}
		void BeginProduction(void) {
			State.NpcRequired = false;
			Table.IsOpening = true;
		}

		void FlagRestockReq(void) {
			State.NpcRequired = false;
			State.RestockPending = false;
			State.RestockRequired = true;
		}

	} State;
                      
	static struct ConfigurationData {
		// The inventory index of the newly crafted item (Used for depositing Beef jerkey, Pots etc...).
		byte CraftedItemIndex;

		// The inventory position of the crafting material (Updated on a successful craft packet).
		byte MaterialIndex;

		// The minimum possible material needed for a single craft.
		byte MinMaterialCount;

	} Configuration;

	static struct TableData {
		bool IsOpen;
		bool IsOpening;

		int Uid;

		CString Name;

	} Table;

	static struct CraftBotSet {

		void Recipe(CString NewRecipe) {
			CRecipe = NewRecipe;
		}

		void Profession(CString NewProfession) {
			CProfession = NewProfession; 

			// Accessory.
			if (CProfession == "Accessory") {
				Table.Name = "ANVIL";
				Configuration.MinMaterialCount = 3;
			}
			// Potion & Cooking.
			else if (CProfession == "Potion") {
				Table.Name = "REAGENT TABLE";
				Configuration.MinMaterialCount = 2;
			}
			else if (CProfession == "Cooking") {
				Table.Name = "OVEN";
				Configuration.MinMaterialCount = 2;
			}
			// Weapon & Armour
			else if (CProfession == "Armour") {
				Table.Name = "FORGE";
				Configuration.MinMaterialCount = 1;
			}
			else if (CProfession == "Weapon") {
				Table.Name = "FORGE";
				Configuration.MinMaterialCount = 1;
			}
		}

	} Set;

	static struct CraftBotGet {

		CString Type(void) {
			return CProfession;
		}
		CString Recipe(void) {
			return CRecipe;
		}

	} Get;
	
};

class RestockBot {
private:
	static short InvItemIndex;
	static vector<CString> MaterialList;
public:

	static struct RestockVendor {
		CString Name;
		short ID;
		short PurchaseQuantity;
		SHOP_ITEM Item;
	} Vendor;

	static struct RestockBotState {
		bool IsRestocking = true;
		bool IsWaitingToBuy;
		bool JustBought;
	} State;

	static struct RestockBotSet {

		void Materials(CString List) {
			MaterialList.clear();
			if (List.GetLength() > 0) {
				CString Name;
				for (int i = 0; i <= List.GetLength(); i++) {
					if (List[i] == ',' || (i == List.GetLength())) {
						printf_s("\tNew Restock Item: %s\n", Name);
						MaterialList.push_back(Name);
						Name.Empty();
					} else {
						Name += List[i];
					}
				} if (MaterialList[0].GetLength() > 0) {

					CString NewMaterial = MaterialList[0], Name;

					if (NewMaterial == "Basic Leather" || NewMaterial == "Tiger Leather" || NewMaterial == "Wooden Board" || NewMaterial == "Linen") {
						Name = "JINMOO";
					} 
					else if (NewMaterial == "Wolf Meat" || NewMaterial == "Tiger Meat") {
						Name = "MOMO";
					} 
					else if (NewMaterial == "Leocrot Horn" || NewMaterial == "Wolf Liver") {
						Name = "ASEOL";
					}
					else if (NewMaterial == "Copper Metal" || NewMaterial == "Orcish Metal") {
						Name = "POLD";
					}
					if (Name.GetLength() > 0) {
						printf_s("\tSet Vendor: %s\n", Name);
						Vendor.Name = Name;
					}
				}
			}
		}

		void ShopEntry(SHOP_ITEM Item, short Id) {
			Vendor.ID = Id;
			Vendor.Item = Item;
			State.IsWaitingToBuy = true;
		}
		void ItemIndex(short NewIndex) {
			InvItemIndex = NewIndex;
		}

	} Set;

	static struct RestockBotGet {

		vector<CString>::const_iterator MaterialListBegin() {
			return MaterialList.begin();
		}
		vector<CString>::const_iterator MaterialListEnd() {
			return MaterialList.end();
		}

		short ItemIndex(void) {
			return InvItemIndex;
		}

	} Get;
};

class Bot {
private:
public:
	static CString Version;

	static CString ExePath;

	static struct BotState {

		bool IsRunning;
		bool IsTrainingInt;	

		bool HasTarget;
		bool IsRegainingStamina;
		bool AwaitingRevival;
		bool IsLooting;
		bool IsBeingPkd;

		bool PotsAndRepairs;
		bool IsRepairing;
		bool IsRestocking;
		bool IsPathRunning;

		bool IsEnteringPortal;
		bool HasEnteredPortal;

		bool IsOpeningNpc;
		bool IsWaiting;

		bool HealthLow;
		bool ManaLow;

		void Terminate(Debug Type, CString Reason) {	
			if (Reason.GetLength() > 0) {
				Print(Type, Reason);
				Bot::OutPut.SoundPlay(105);
			}
			Print(Type, "Terminated.");
			IsRunning = false;	
			Reset();
		}

		void Reset(void) {
			OutPut.ShowIsAttacking = true;

			Loot.Reset();
			Attack.Reset();
			Warp.Reset();

			IsPathRunning = false;
			IsEnteringPortal = false;
			IsOpeningNpc = false;
		}

		void SetWait(bool Wait) {
			if (Wait) {
				Bot::Print(Debug::GENERAL, "Bot is now waiting.");
				IsWaiting = true;
			} else {
				Bot::Print(Debug::GENERAL, "Bot is no longer waiting.");
				IsWaiting = false;
			}
		}

		void HpOut(void) {
			Bot::Print(Debug::GENERAL, "Consumed all of the HP potions at index [%d]", Items.HpPotionIndex);
			Items.HpPotionIndex = -1;
			HealthLow = false;
		}

		void MpOut(void) {
			Bot::Print(Debug::GENERAL, "Consumed all of the MP potions at index [%d]", Items.MpPotionIndex);
			Items.MpPotionIndex = -1;
			ManaLow = false;
		}

		void PkDetected(void) {
			Print(Debug::GENERAL, "A PK has been detected.");

			OutPut.SoundPlay(107);
			State.IsBeingPkd = true;

			if (Configuration.IgnorePk) {
				Print(Debug::GENERAL, "However IGNORE_PK is true.");
			} else {
				Warp.FlagReq();
			}
		}

		struct HealState {

			// This delay is set when the bots behaviour is evaluated.
			int Delay;

			bool Enabled;

			bool Required;
			bool SafeHealRequired;
			bool WepChangeRequired;
			bool ReEquipWepRequired;

			void FlagReq(bool ChangeWeapon) {	
				if (ChangeWeapon) {
					Bot::Print(Debug::GENERAL, "But a weapon change is required beforehand.");
				} else if (SafeHealRequired) {
					Bot::Print(Debug::GENERAL, "Will wait for a time to safely heal.");
				} else  {
					Bot::Print(Debug::GENERAL, "Running.");
				}  WepChangeRequired = ChangeWeapon; Required = true;
			}

		} Heal;

		struct WarpState {

			// This delay is set when the bots behaviour is evaluated.
			int Delay;

			bool Done;
			bool Required;	
			bool InProgress;

			void FlagReq(void) {

				Bot::Print(Debug::GENERAL, "A warp is now required.");

				if (Bot::Configuration.NoWarp) {
					Bot::Print(Debug::GENERAL, "However NO_WARP is true so we won't warp.");
				} else {

					ITEM_DATA* TpData = nullptr;
					if (BotUser::Inventory.GetByName("Town Portal Scroll", TpData)) {

						// Reset the attack state here because if the botuser's target is not cleared here it will not be removed from the UserList when we warp to town. This causes a crash.
						Bot::State.Attack.Reset();

						// Resetting loot prevents the bot from looting items that were added to the list while we were hunting (LootAfterwards).
						Loot.Reset();

						Bot::State.HasEnteredPortal = false;

						NearbyUserDetection.DetectedUsers.Clear();

						Bot::Print(Debug::GENERAL, "Found [%s] at index [%d]", TpData->Name, TpData->Index);
						Bot::Items.TpIndex = TpData->Index;
						Required = true;

					} else {
						State.Terminate(Debug::GENERAL, "No TP could be found.");
						SocketMng::SendLogOut();
					}
				}
			}

			void Reset(void) {
				Done = false;
				Required = false;
				InProgress = false;		
			}

		} Warp;

		struct AttackState {

			bool TrackingFailures;

			int FailureCount;

			bool IsAttacking;

			bool MagicReadySuccess;

			DWORD ApproachTick;
			DWORD WeakenCastTick;

			void Reset(void) {

				BotUser::Me->Target = nullptr;
				BotUser::Me->TargetUid = 0;

				Waypoints.Attack = NULL;

				State.HasTarget = false;

				FailureCount = 0;
				TrackingFailures = true;

				MagicReadySuccess = false;
				IsAttacking = false;

				OutPut.ShowIsAttacking = false;
			}

			void NoteFailure(void) {
				Bot::Print(Debug::TARGET, "Attack failure noted.");
				if (++FailureCount >= Configuration.AttackTimeOut) {
					Configuration.BlackList.Add(BotUser::Me->TargetUid, 0);
					Reset();
				}
			}
		
		} Attack;

	} State;

	static struct WaypointData {

		vector<CPoint> CraftPath, LvlPath, PreLvlPath, RestockPath, VendorPath, BlksmithPath, TmpPath;

		unordered_map<FileType, CString> File;

		CPoint Attack;

		int Index;

		CString Portal;

		CPoint GetCurrent(void) {
			if (Bot::Configuration.mode == Mode::RESTOCK)
				return RestockPath[Index];
			else if (Bot::Configuration.mode == Mode::CRAFT)
				return CraftPath[Index];
			else if (Bot::State.IsPathRunning)
				return PreLvlPath[Index];
			else if (Bot::State.IsRepairing)
				return BlksmithPath[Index];
			else if (Bot::State.IsRestocking)
				return VendorPath[Index];
			else
				return LvlPath[Index];
		}

		bool IsNearNext(void) {		
			if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent()) <= 4) {
				Bot::Print(Debug::MOVEMENT, "Arrived near point [X: %d Y: %d]", Bot::Waypoints.GetCurrent().x, Bot::Waypoints.GetCurrent().y);		
				return true;
			} return false;
		}

		bool IsAtEnd(void) {
			if (Index >= (Count() - 1)) {
				Bot::Print(Debug::MOVEMENT, "Reached the end of the waypoint list.");
				return true;
			} return false;
		}

		void Increment(void) {
			Index++;
			Print(Debug::MOVEMENT, "Next point [X: %d Y: %d]\n", GetCurrent().x, GetCurrent().y);

			// Determine if the current waypoint requires entry into a portal.
			if (GetCurrent().x == 31337) {
				Print(Debug::MOVEMENT, "Will now attempt to enter a portal.");
				State.IsEnteringPortal = true;
				Index++;

				// Force the bot to heal to full HP if we're about the enter VOD.
				if (Portal == "VALLEY OF DEATH") {
					if (BotUser::Me->Data->Stats.Strength <= 800) {
						//BotUser::EvaluateNewValues(BotUser::Me->Data->Health.Current, 0);
					}
				}
			}
		}

		void Reset(void) {
			Index = 0;
			if (Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
				Bot::Configuration.BlackList.Clear();
			}
			if (Bot::Configuration.mode == Mode::RESTOCK) {
				reverse(RestockPath.begin(), RestockPath.end());
				RestockBot::State.IsRestocking = !RestockBot::State.IsRestocking;
			}
			else if (Bot::Configuration.mode == Mode::CRAFT) {
				BotUser::Me->StaminaOn() ? SocketMng::ToggleStamina(false) : SocketMng::ToggleStamina(true);
				reverse(CraftPath.begin(), CraftPath.end());			
			}
			else if (Bot::State.IsPathRunning) {
				Bot::State.IsPathRunning = false;
			}
			else if (Bot::State.IsRepairing) {
				Bot::State.IsRestocking = true;
				Bot::State.IsRepairing = false;

				// Equip any accessories that may have been removed during leveling.
				Bot::Print(Debug::GENERAL, "Re-equipping accessories.");
				vector<BYTE> EquippedAcc;
				for (int i = 10; i < 50; i++) {
					if (BotUser::Inventory.Item.Type(i) == ItemType::ACC) {
						if (find(EquippedAcc.begin(), EquippedAcc.end(), (BYTE)BotUser::Inventory.Item.ArmPos(i)) == EquippedAcc.end()) {
							// Equip item.
							SocketMng::SwitchItem(i, (short)BotUser::Inventory.Item.ArmPos(i));
							// Add this item to the list of previously equipped accessories so we do not attempt to equip it again.
							EquippedAcc.push_back((BYTE)BotUser::Inventory.Item.ArmPos(i));
						}
						else { // Equip the second ring in the left hand wrist slot.
							if (BotUser::Inventory.Item.ArmPos(i) == Armor::WRIST) {
								// Equip item.
								SocketMng::SwitchItem(i, (short)8);
							}
						}
					}
				}
			}
			else if (Bot::State.IsRestocking) {
				Bot::State.IsRestocking = false;
				Bot::State.PotsAndRepairs = false;

				if (Bot::Waypoints.PreLvlPath.size() > 0) {
					Print(Debug::MOVEMENT, "Running the PRELVL_PATH.");
					State.IsPathRunning = true;
				} else {
					Bot::State.Terminate(Debug::MOVEMENT, "No PRELVL_PATH loaded.");
				}
			}
		}

		bool VendorAndBlkSmithLoaded(void) {
			return (VendorPath.size() > 0 && BlksmithPath.size() > 0);
		}

		void RunPepairAndRestock(void) {
			Reset();
			Print(Debug::GENERAL, "Repairing and restocking.");
			State.PotsAndRepairs = true;
			State.IsRepairing = true;
		}

		void FindNearest(void) {

			// If we need to run a prelvl path then first determine if we have entered the portal (if there is one) on the prelvl path. This prevents issues with waypoints that are detected as being of a close range inside the portal.
			int i = 0;
			if (Bot::State.IsPathRunning) {
				if (Bot::Waypoints.Portal != "") {
					if (Bot::Zone.CurrentAsCString() == Bot::Waypoints.Portal) {	
						for (int x = 0; x < Count(); x++) {
							if (PreLvlPath.at(x).x = 31337) {
								i = (x + 1); break;
							}
						}
					}				
				}
			}

			for (; i < Count(); i++) {

				switch (Bot::Configuration.mode) {

					case Mode::RESTOCK: {
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, RestockPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
							Index = i;
						}
					} break;

					case Mode::CRAFT: {
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, CraftPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
							Index = i;
						}
					} break;

					default: {
						if (Bot::State.IsPathRunning) {	
							// If we're running a prelvl path and have not entered the desired portal... Then do not check waypoints after the portal name. 
							if (Bot::Waypoints.Portal != "") {
								if (Bot::Zone.CurrentAsCString() != Bot::Waypoints.Portal) {
									if (PreLvlPath.at(i).x == 31337) {
										return;
									}
								}
							}
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, PreLvlPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
								Index = i;
							}		
						} else if (Bot::State.IsRepairing) {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BlksmithPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
								Index = i;
							}
						} else if (Bot::State.IsRestocking) {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, VendorPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
								Index = i;
							}
						} else {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, LvlPath.at(i)) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent())) {
								Index = i;
							}
						}
					} break;
				}
				//Bot::Print("Nearest waypoint: %d %d Range: %d", GetCurrent().x, GetCurrent().y, UserManager::GetCellRange(BotUser::Me->Data->CellPos, GetCurrent()));
			}
		}

		int Count(void) {
			if (Bot::Configuration.mode == Mode::RESTOCK)
				return RestockPath.size();
			else if (Bot::Configuration.mode == Mode::CRAFT)
				return CraftPath.size();
			else if (Bot::State.IsPathRunning)
				return PreLvlPath.size();
			else if (Bot::State.IsRepairing)
				return BlksmithPath.size();
			else if (Bot::State.IsRestocking)
				return VendorPath.size();
			else 
				return LvlPath.size();
		}

		void Add(void) {
			Bot::Print(Debug::MOVEMENT, "Added temporary waypoint: %d %d", BotUser::Me->Data->CellPos.x, BotUser::Me->Data->CellPos.y);
			TmpPath.push_back(BotUser::Me->Data->CellPos);
		}

		void Clear(FileType Type) {

			switch (Type) {

				// Leveling.
				case FileType::LVL_PATH: {
					LvlPath.clear();
				} break;
				case FileType::PRELVL_PATH: {
					PreLvlPath.clear();
				} break;
				case FileType::VENDOR_PATH: {
					VendorPath.clear();
				} break;
				case FileType::BLKSMITH_PATH: {
					BlksmithPath.clear();
				} break;

				// Restocking.
				case FileType::RESTOCK_PATH: {
					RestockPath.clear();
				} break;

				// Crafting.
				case FileType::CRAFT_PATH: {
					CraftPath.clear();
				} break;

					// Temporary path.
				case FileType::TEMPORARY_PATH: {
					TmpPath.clear();
				} break;

				// Clear all.
				default: {
					LvlPath.clear();
					PreLvlPath.clear();			
					VendorPath.clear();
					BlksmithPath.clear();
					RestockPath.clear();
					CraftPath.clear();
				} break;
			}
		}

		void Load(CString Filename, FileType Type) {

			if (Filename.GetLength() > 0) {

				ifstream wpFile;
				wpFile.open("Waypoints\\" + Filename);

				if (!wpFile.is_open())	{
					printf_s("\tCouldn't open file: %s [%s]\n\tCreating a new waypoint file: %s.\n", Filename, Waypoints.TypeAsCString(Type), Filename);
					ofstream Cfg("Waypoints\\" + Filename);
				}
				else {
					Clear(Type);
					int Count = 0;
					char Line[255], Value[255];

					File.emplace(Type, Filename);

					if (Type == FileType::PRELVL_PATH) {		
						while (wpFile.getline(Line, 255))	{			
							int LineLength = string(Line).length();
							if (isalpha(Line[0])) {
								Portal = CString(Line);
								PreLvlPath.push_back(CPoint(31337));
							}
							else {
								CPoint NewPoint; CString Value;
								for (int i = 0; i <= LineLength; i++) {
									Value += Line[i];
									if (i == string(Line).find(' ')) {
										NewPoint.x = atoi(Value);
										Value.Empty();
									}
								}
								NewPoint.y = atoi(Value);
								PreLvlPath.push_back(NewPoint);
							} Count++;
						}
					}
					else {
						int vOne, vTwo;
						while (wpFile >> vOne && wpFile >> vTwo) {
							switch (Type) {
								case FileType::LVL_PATH: {
									LvlPath.push_back(CPoint(vOne, vTwo));
								} break;
								case FileType::VENDOR_PATH: {
									VendorPath.push_back(CPoint(vOne, vTwo));
								} break;
								case FileType::BLKSMITH_PATH: {
									BlksmithPath.push_back(CPoint(vOne, vTwo));
								} break;
								case FileType::RESTOCK_PATH: {
									RestockPath.push_back(CPoint(vOne, vTwo));
								} break;
								case FileType::CRAFT_PATH: {
									CraftPath.push_back(CPoint(vOne, vTwo));
								} break;
							} Count++;
						}
					}
					printf_s("\tLoaded [%d] waypoints from [%s] [%s]\n", Count, Filename, Waypoints.TypeAsCString(Type));
					wpFile.close();
				}
			}
		}

		void Save(CString File) {
			// Need to use ExePath here as we're injected...
			ofstream wpFile(ExePath + "Waypoints\\" + File);

			for (auto xt = TmpPath.begin(); xt != TmpPath.end(); xt++) {
				wpFile
					<< xt->x
					<< " " 
					<< xt->y 
					<< endl;
			} Bot::Print(Debug::MOVEMENT, "Saved %d waypoints to file: %s", TmpPath.size(), "Waypoints\\" + File);
		}

		CString GetFile(FileType Type) {
			auto xt = File.find(Type);
			if (xt != File.end()) {
				return xt->second;
			} return "";
		}
		
		CString TypeAsCString(FileType Type) {
			switch (Type) {
				case FileType::LVL_PATH: {
					return "LVL_PATH";
				} break;
				case FileType::PRELVL_PATH: {
					return "PRELVL_PATH";
				} break;
				case FileType::VENDOR_PATH: {
					return "VENDOR_PATH";
				} break;
				case FileType::BLKSMITH_PATH: {
					return "BLKSMITH_PATH";
				} break;
				case FileType::RESTOCK_PATH: {
					return "RESTOCK_PATH";
				} break;
				case FileType::CRAFT_PATH: {
					return "CRAFT_PATH";
				} break;
			} 
		}

	} Waypoints;

	static struct ConfigurationData {

		// Testing;
		int tmpDelay = 1000;

		Mode mode;

		Behaviour behaviour;

		CString Cfg;

		SPECIALATTACK_DATA *AuraData;

		CString Aura;

		// Bot::Wait uses this.
		int WaiTtick;

		int GameHour;

		int SearchRange;

		int AttackDistance;

		int ApproachTimeOut;
		int AttackTimeOut;
		int LootTimeOut;

		int HpThreshold;
		int MpThreshold;

		int HealThreshold;
		int HealUntilThreshold = 83;

		int DuraThreshold;

		int AuraReuseTime;
		int AuraLastUseTime;

		bool ServingBabidi;

		bool DrawMinimap;

		bool DrawInfo;
		bool DamageOverHead;
		bool TrackTarget;
		bool AutoSerum;
		bool NoDark;
		bool CastReduction;
		bool UseSlow;

		bool ChatNotify;
		bool ChatRespond;

		bool LootAfterwards;

		bool NoLoot;
		bool NoWarp;
		bool IgnorePk;
		bool RegainStam;
		bool Randomization;

		int Test;
		bool CastFast;

		void ModifyBehaviour(Behaviour New) {
			switch (New) {
				case Behaviour::INSANE: {
					// Warp delay is the amount of time the bot will pause after it has warped.
					Bot::State.Warp.Delay = 3000;
					// Heal delay is the amount of time the bot will pause before and after it has healed.
					Bot::State.Heal.Delay = 1000;
					// Loot delay between each item looted.
					Bot::Loot.Delay = 100;
				} break;
				case Behaviour::AGGRESSIVE: {
					Bot::State.Warp.Delay = 3000;
					Bot::State.Heal.Delay = 2000;
					Bot::Loot.Delay = 300;
				} break;
				case Behaviour::CASUAL: {
					Bot::State.Warp.Delay = 10000;
					Bot::State.Heal.Delay = 3000;
					Bot::Loot.Delay = 600;
				} break;
				case Behaviour::PENSIONER: {
					Bot::State.Warp.Delay = 20000;
					Bot::State.Heal.Delay = 5000;
					Bot::Loot.Delay = 800;
				} break;
			} Bot::Configuration.behaviour = New;
		}

		CString ModeAsCString(void) {
			switch (mode) {
				case Mode::LEVEL:  {
					return "Level";
				}
				case Mode::RESTOCK: {
					return "Restock";
				}
				case Mode::CRAFT: {
					return "Craft";
				}
			}
		}

		CString BehaviourAsCString(void) {
			switch (behaviour) {
				case Behaviour::INSANE: {
					return "Insane";
				}
				case Behaviour::AGGRESSIVE: {
					return "Aggressive";
				}
				case Behaviour::CASUAL: {
					return "Casual";
				}
				case Behaviour::PENSIONER: {
					return "Pensioner";
				}
			}
		}

		void Create(CString File) {

			printf_s("Creating a new default configuration file: %s.\n", File);

			ofstream Cfg("Configs\\" + File);

				switch (mode) {

					// Leveling.
					case Mode::LEVEL: {
						Cfg 
							<< "## Hp & Mp potting value percentages. " << endl
							<< "HP_PERCENT=40" << endl
							<< "MP_PERCENT=20" << endl << endl

							<< "HEAL_ENABLED=0" << endl
							<< "HEAL_PERCENT=60" << endl << endl

							<< "## Minimum dura percent before a replacement is required. Anything below 10% has a chance to break." << endl
							<< "DURA_PERCENT=15" << endl << endl

							<< "## The range in which to look for mobs (Each cell is 2 not 1). " << endl
							<< "SEARCH_RANGE=32" << endl << endl

							<< "## The total amount of time before the bot will time out when approaching a mob (Counted in seconds)." << endl
							<< "APPROACH_TIMEOUT=10" << endl << endl

							<< "## The total amount of failed attacks before timing out (Logged in actual attack failures)." << endl
							<< "ATTACK_TIMEOUT=10" << endl << endl

							<< "## The amount of failures allocated before looting times out (Counted in failures when moving to a loot point)." << endl
							<< "LOOT_TIMEOUT=2" << endl << endl

							<< "## The aura to use (Aura names are case sensitive)." << endl
							<< "AURA=" << endl << endl

							<< "## The names of the mobs to hunt (All names must be upper case)." << endl
							<< "TARGET_LIST=" << endl << endl

							<< "## The names of mobs to ignore (Even if they're attacking you)." << endl
							<< "IGNORE_LIST=" << endl << endl

							<< "## The list of items which are to be excluded from looting (Item names are case sensitive)." << endl
							<< "LOOTFILTER_LIST=" << endl << endl

							<< "## The list of items to loot immediately if LootAfterwards is true." << endl
							<< "RARE_ITEM_LIST=[Intense],[Epic],[Rare]" << endl << endl

							<< "## The various waypoint files the bot uses (Filenames are NOT case sensitive)." << endl
							<< "LVL_PATH=" << endl
							<< "PRELVL_PATH=" << endl
							<< "VENDOR_PATH=" << endl
							<< "BLKSMITH_PATH=" << endl;
					} break;

					// Restocking.
					case Mode::RESTOCK: {
						Cfg 
							<< "## The material to gather (Enter material names exactly as they are represented in-game. Example: Copper Metal)." << endl
							<< "## For the collection of more than one material, seperate each item with a comma (Copper Metal,Orcish Metal)" << endl
							<< "MATERIAL=Copper Metal" << endl << endl

							<< "## The waypoint file from the Warehouse to the Vendor." << endl
							<< "RESTOCK_PATH=Abias-WH2VENDOR.wpf" << endl << endl;
					} break;

					// Crafting.
					case Mode::CRAFT: {
						Cfg 
							<< "## The profession in which to skill (Professions: Armour, Weapon, Accessory, Cooking and Potion)." << endl
							<< "PROFESSION=Armour" << endl << endl

							<< "## The recipe to be used (Recipes: Cheap Shoes, Dagger, Ring, Beef Jerkey and 10HP)." << endl
							<< "RECIPE=Cheap Shoes" << endl << endl

							<< "## The waypoint file from the Warehouse to the Crafting Table." << endl
							<< "CRAFT_PATH=Tyt-WH2FORGE.wpf" << endl << endl;
					} break;
						
					// Healing Wind bot.
					case Mode::HEAL: {
						Cfg
							<< "## Health percentage in which the bot will begin healing the heal target." << endl
							<< "HEAL_PERCENT=50" << endl << endl

							<< "## Enable or disable the casting of Slow." << endl
							<< "USE_SLOW=1" << endl << endl;
					} break;
				}
		}

		void Load(CString File) {

			if (File.GetLength() < 1) {
				return;
			}

			if (!PathFileExists("Configs\\" + File)) {
				printf_s("Could not open file: %s.\n", File);
				Create(File);
			}

			ifstream wpFile("Configs\\" + File);

			printf_s("Loading bot configuration from file: %s\n\n", File);

			Bot::Configuration.Cfg = File;

			char Line[1024], Value[1024];

			while (wpFile.getline(Line, sizeof(Line)))	{

				if (Line[0] != '#' && Line[0] != ' ') {

					int Seperator = string(Line).find('=');

					copy((Line + (Seperator + 1)), (Line + (Seperator + 1)) + string(Line).length(), Value);

					switch (Bot::Configuration.mode) {

						case Mode::LEVEL: {
							// Hp & Mp percent values.
							if (CString(Line).Find("HP_PERCENT") != -1) {
								int value = atoi(Value); (value > 99 || value < 1) ? HpThreshold = 40 : HpThreshold = value; printf_s("\tHpThreshold: %d\n", HpThreshold);
							}
							else if (CString(Line).Find("MP_PERCENT") != -1) {
								int value = atoi(Value); (value > 99 || value < 1) ? MpThreshold = 20 : MpThreshold = value; printf_s("\tMpThreshold: %d\n\n", MpThreshold);
							}

							// Healing values.
							else if (CString(Line).Find("HEAL_ENABLED") != -1) {
								State.Heal.Enabled = atoi(Value); printf_s("\tHealEnabled: %d\n", State.Heal.Enabled);
							}
							else if (CString(Line).Find("HEAL_PERCENT") != -1) {
								int value = atoi(Value); (value > 99 || value < 1) ? HealThreshold = 60 : HealThreshold = value; printf_s("\tHealThreshold: %d\n\n", HealThreshold);
							}

							// Dura.
							else if (CString(Line).Find("DURA_PERCENT") != -1) {
								int value = atoi(Value); (value > 99 || value < 1) ? DuraThreshold = 15 : DuraThreshold = value; printf_s("\tDuraThreshold: %d\n\n", DuraThreshold);
							}

							// Aura.
							else if (CString(Line).Find("AURA") != -1) {
								if (CString(Value).GetLength() > 0) {
									Aura = Value; printf_s("\tAura: %s [L: %d]\n\n", Aura, Aura.GetLength());
								}
							}

							// Search range.
							else if (CString(Line).Find("SEARCH_RANGE") != -1) {
								int value = atoi(Value); (value > 32 || value < 1) ? SearchRange = 32 : SearchRange = value; printf_s("\tSearchRange: %d\n\n", SearchRange);
							}

							// Time outs.
							else if (string(Line).find("APPROACH_TIMEOUT") != -1) {
								int value = atoi(Value); (value > 20 || value < 1) ? ApproachTimeOut = 10 : ApproachTimeOut = value; printf_s("\tApproachTimeOut: %d\n", ApproachTimeOut);
							}
							else if (CString(Line).Find("ATTACK_TIMEOUT") != -1) {
								int value = atoi(Value); (value > 20 || value < 1) ? AttackTimeOut = 10 : AttackTimeOut = value; printf_s("\tAttackTimeOut: %d\n", AttackTimeOut);
							}
							else if (CString(Line).Find("LOOT_TIMEOUT") != -1) {
								int value = atoi(Value); (value > 10 || value < 1) ? LootTimeOut = 2 : LootTimeOut = value; printf_s("\tLootTimeOut: %d\n", LootTimeOut);
							}

							// Target, Ignore & Loot filter lists.
							else if (CString(Line).Find("TARGET_LIST") != -1) {
								TargetList.ParseList(Value);
							}
							else if (CString(Line).Find("IGNORE_LIST") != -1) {
								IgnoreList.ParseList(Value);
							}
							else if (CString(Line).Find("LOOTFILTER_LIST") != -1) {
								Loot.Filter.ParseList(Value);
							}
							else if (CString(Line).Find("RARE_ITEM_LIST") != -1) {
								Loot.RareItemList.ParseList(Value);
							}

							// Path files
							else if (CString(Line).Find("PRELVL_PATH") != -1) {
								Waypoints.Load(Value, FileType::PRELVL_PATH);
							}
							else if (CString(Line).Find("LVL_PATH") != -1) {
								Waypoints.Load(Value, FileType::LVL_PATH);
							}
							else if (CString(Line).Find("VENDOR_PATH") != -1) {
								Waypoints.Load(Value, FileType::VENDOR_PATH);
							}
							else if (CString(Line).Find("BLKSMITH_PATH") != -1) {
								Waypoints.Load(Value, FileType::BLKSMITH_PATH); printf_s("\n");
							}
						} break;

						case Mode::RESTOCK: {
							if (CString(Line).Find("MATERIAL") != -1) {
								//RestockBot::Set.MaterialType(Value); printf_s("\tMaterial: %s\n", RestockBot::Get.MaterialName());
								RestockBot::Set.Materials(Value);
							} 
							else if (CString(Line).Find("RESTOCK_PATH") != -1) {
								Waypoints.Load(Value, FileType::RESTOCK_PATH); printf_s("\n");
							}
						} break;

						case Mode::CRAFT: {
							if (CString(Line).Find("PROFESSION") != -1) {
								CraftBot::Set.Profession(Value); printf_s("\tProfession: %s\n", CraftBot::Get.Type());
							}
							else if (CString(Line).Find("RECIPE") != -1) {
								CraftBot::Set.Recipe(Value); printf_s("\tRecipe: %s\n", CraftBot::Get.Recipe());
							}
							else if (CString(Line).Find("CRAFT_PATH") != -1) {
								Waypoints.Load(Value, FileType::CRAFT_PATH); printf_s("\n");
							}
						} break;

						case Mode::HEAL: {	
							if (CString(Line).Find("HEAL_PERCENT") != -1) {
								int value = atoi(Value); (value > 99 || value < 1) ? HealThreshold = 60 : HealThreshold = value; printf_s("\tHealThreshold: %d\n\n", HealThreshold);
							}
							else if (CString(Line).Find("USE_SLOW") != -1) {
								Bot::Configuration.UseSlow = atoi(Value); printf_s("\tUseSlow: %s\n\n", Bot::Configuration.UseSlow ? "TRUE" : "FALSE");
							}
						} break;
					}
				}
			}
		}

		void Save(CString File) {

			ofstream Cfg("Configs\\" + File);

			switch (mode) {
				// Leveling.
				case Mode::LEVEL: {
					Cfg
						<< "## Hp & Mp potting value percentages. " << endl
						<< "HP_PERCENT=" << HpThreshold << endl
						<< "MP_PERCENT=" << MpThreshold << endl << endl

						<< "HEAL_ENABLED=" << State.Heal.Enabled << endl
						<< "HEAL_PERCENT=" << HealThreshold << endl << endl

						<< "## Minimum dura percent before a replacement is required. Anything below 10% has a chance to break." << endl
						<< "DURA_PERCENT=" << DuraThreshold << endl << endl

						<< "## The range in which to look for mobs (Each cell is 2 not 1). " << endl
						<< "SEARCH_RANGE=" << SearchRange << endl << endl

						<< "## The total amount of time before the bot will time out when approaching a mob (Counted in seconds)." << endl
						<< "APPROACH_TIMEOUT=" << ApproachTimeOut << endl << endl

						<< "## The total amount of failed attacks before timing out (Logged in actual attack failures)." << endl
						<< "ATTACK_TIMEOUT=" << AttackTimeOut << endl << endl

						<< "## The amount of failures allocated before looting times out (Counted in failures when moving to a loot point)." << endl
						<< "LOOT_TIMEOUT=" << LootTimeOut << endl << endl

						<< "## The aura to use (Aura names are case sensitive)." << endl
						<< "AURA=" << Aura << endl << endl

						<< "## Behaviour controls the bot behaviour." << endl
						<< "## Current Behaviours are: INSANE, AGGRESSIVE, CASUAL and PENSIONER." << endl
						<< "BEHAVIOUR=" << BehaviourAsCString().MakeUpper() << endl << endl

						<< "## The names of the mobs to hunt (All names must be upper case)." << endl
						<< "TARGET_LIST=" << TargetList.GetList() << endl << endl

						<< "## The names of mobs to ignore (Even if they're attacking you)." << endl
						<< "IGNORE_LIST=" << IgnoreList.GetList() << endl << endl

						<< "## The list of items which are to be excluded from looting (Item names are case sensitive)." << endl
						<< "LOOTFILTER_LIST=" << Loot.Filter.GetList() << endl << endl

						<< "## The various waypoint files the bot uses (Filenames are NOT case sensitive)." << endl
						<< "LVL_PATH=" << Waypoints.GetFile(FileType::LVL_PATH) << endl
						<< "PRELVL_PATH=" << Waypoints.GetFile(FileType::PRELVL_PATH) << endl
						<< "VENDOR_PATH=" << Waypoints.GetFile(FileType::VENDOR_PATH) << endl
						<< "BLKSMITH_PATH=" << Waypoints.GetFile(FileType::BLKSMITH_PATH) << endl;
				} break;

				// Restocking.
				case Mode::RESTOCK: {
					Cfg
						<< "## The material to gather (Enter material names exactly as they are represented in-game. Example: Copper Metal)." << endl
						<< "MATERIAL=" << endl << endl

						<< "## The waypoint file from the Warehouse to the Vendor." << endl
						<< "RESTOCK_PATH=" << endl << endl;
				} break;

				// Crafting.
				case Mode::CRAFT: {
					Cfg
						<< "## The profession in which to skill (Professions: Armour, Weapon, Cooking, Potion & Accessory)." << endl
						<< "PROFESSION=" << endl << endl

						<< "## The recipe to be used (Recipes: Cheap Shoes, Beef Jerkey, Dagger etc)." << endl
						<< "RECIPE=" << endl << endl

						<< "## The waypoint file from the Warehouse to the Crafting Table." << endl
						<< "CRAFT_PATH=" << endl << endl;
				} break;
			}
		}
	
		struct TargetListData {

			vector<CString> Targets;

			void Add(CString Name) {
				Targets.push_back(Name);
			}

			void ParseList(CString List) {
				Clear();
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tNew Target: %s\n", Name);
							Add(Name);
							Name.Empty();
						} else {
							Name += List[i];
						}
					} printf_s("\n");
				}
			}

			CString GetList(void) {
				CString List;
				for (int i = 0; i < Count(); i++) {
					if (i == 0) {
						List += Targets[i];
					} else {
						List += ("," + Targets[i]);
					}
				} return List;
			}

			bool Contains(CString Name) {
				return find(Targets.begin(), Targets.end(), Name) != Targets.end();
			}

			void Clear(void) {
				Targets.clear();
			}

			int Count(void) {
				return Targets.size();
			}

		} TargetList;

		struct IgnoreListData {

			vector<CString> AvoidList;

			void Add(CString Name) {
				AvoidList.push_back(Name);
			}

			void ParseList(CString List) {
				Clear();
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tWill Ignore: %s\n", Name);
							Add(Name);
							Name.Empty();
						} else {
							Name += List[i];
						}
					} printf_s("\n");
				}
			}

			CString GetList(void) {
				CString List;
				for (int i = 0; i < Count(); i++) {
					if (i == 0) {
						List += AvoidList[i];
					} else {
						List += ("," + AvoidList[i]);
					}
				} return List;
			}

			bool Contains(CString Name) {
				return find(AvoidList.begin(), AvoidList.end(), Name) != AvoidList.end();
			}

			void Clear(void) {
				AvoidList.clear();
			}

			int Count(void) {
				return AvoidList.size();
			}

		} IgnoreList;

		struct BlackListData {

			unordered_map<int, BYTE> Blacklisted;

			// Zero is a bugged mob. One is a mob that can't be reached.
			void Add(int Uid, BYTE Type) {
				Bot::Print(Debug::TARGET, "Blacklisting [%s] [Uid: %d] [Type: %d]", BotUser::Me->Target->Data->Name, Uid, Type);
				Blacklisted.emplace(Uid, Type);
			}

			bool Contains(int Uid, BYTE Type) {
				auto xt = Blacklisted.find(Uid);
				if (xt != Blacklisted.end() && xt->second == Type)
					return true;
				return false;
			}

			void Clear(void) {
				Blacklisted.clear();
			}

			void Remove(int Uid) {	
				auto xt = Blacklisted.find(Uid);
				if (xt != Blacklisted.end()) {
					Bot::Print(Debug::TARGET, "De-Blacklisting [Uid: %d] [Type: %d]", xt->first, xt->second);
					Blacklisted.erase(xt);
				}
			}

			int Count(void) {
				return Blacklisted.size();
			}

		} BlackList;

		struct DeathData {

			int Count;
			int Limit = 2;
			int Delay;

			void Increment(void) {			
				if (++Count >= Limit) {
					Print(Debug::GENERAL, "Death limit reached.");
					SocketMng::SendLogOut();
					Reset();							
				} Bot::State.AwaitingRevival = false;
			}

			void Reset(void) {
				Count = 0;
			}

		} Death;

		struct HealBotData {

			int CastCount;
			int CastLimit;

			DWORD Delay;
			DWORD LastTick;

		} HealBot;

		struct GmDetectionData {

			bool IsEnabled;

			Response LiveResponse;

			// This is used to determine when to look for the first GM again.
			int Count;

			vector<CString> GmList;
			vector<CString>::const_iterator CurrentGm;

			void ParseList(CString List) {
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tWill search for GM: %s\n", Name);
							GmList.push_back(Name);
							Name.Empty();
						}
						else {
							Name += List[i];
						}
					} printf_s("\n");
					CurrentGm = GmList.begin();
				}
			}

		} GmDetection;

	} Configuration;

	static struct ItemLocations {

		int HpPotionIndex = -1;
		int MpPotionIndex = -1;

		int WeaponIndex = -1;
		int ShieldIndex = -1;

		int InvEmptyIndex = -1;

		int TpIndex = -1;

	} Items;

	static struct LootData {

		vector<ITEMFIELD_DATA> LootList;

		// This delay is set when the bots behaviour is evaluated.
		int Delay;

		int Failed;

		int Index;

		void Add(ITEMFIELD_DATA data) {
			Print(Debug::LOOT, "Adding [%s] at [X: %d Y: %d] to the loot list.", data.Name, data.Point.x, data.Point.y);
			LootList.push_back(data);
		}

		int Count(void) {
			return LootList.size();
		}

		void Increment(void) {
			// Increment the LootList index and reset if we've looted all of the available items.
			if (++Index >= Count()) {
				Reset();
			}

			// Reset the failed count after every successful loot if LootAfterwards is on.
			else if (Configuration.LootAfterwards) {
				Failed = 0;
			}

			//Delay = BotUser::RandomNumberGenerator((BYTE)Configuration.behaviour * 250, (BYTE)Configuration.behaviour * 500);
		}

		void Reset(void) {
			State.IsLooting = false;	
			LootList.clear();
			Index = 0;
			Failed = 0;
		}

		bool Contains(ITEMFIELD_DATA data) {
			for (auto xt = LootList.begin(); xt != LootList.end(); xt++) {
				if (xt->Point == data.Point) {
					return true;
				}
			} return false;
		}

		void FindNearby(void) {

			if (!Configuration.NoLoot) {

				// Get the previous amount of drops so we can determine how many drops the current mob has dropped.
				int PreviousDropCount = Count();

				// Find new drops.
				for (auto xt = FieldList.ItemFieldList.begin(); xt != FieldList.ItemFieldList.end(); xt++) {
					// Perform a closer range check on barr drops to prevent the picking up of other peoples drops.
					if (UserManager::GetCellRange(BotUser::Me->Target->Data->CellPos, xt->Point) <= (xt->Name == "Money" ? 2 : 4)) {			
						if (Bot::Configuration.LootAfterwards) {
							// Continue processing if the item is not already in the LootPoint list.
							if (!Contains(*xt)) {
								// If a rare item is detected and LootAfterwards is true then begin looting immediately.
								if (xt->RareItem) {
									Print(Debug::LOOT, "Detected rare item [%s]. Will loot immediately.", xt->Name);
									State.IsLooting = true;
								}
							} else { // Skip adding this item if it is found to be in the LootPoint list already.
								continue;
							}
						} Add(*xt);				
					}
				}

				// Display how many items the mob has dropped.
				Print(Debug::LOOT, "Dropped [%d] total item(s).", (Count() - PreviousDropCount));

				// If LootAfterwards is on, determine if we need to loot now or continue killing a group of mobs first.
				if (Bot::Configuration.LootAfterwards) {
					// First check to see if looting has already been enabled by the detection of a rare item...
					if (!State.IsLooting) {
						// Continue the evaluation only if there is loot that has been found.
						if (Count() > 0) {
							for (auto xt = UserManager::UserList.Begin();; xt++) {
								CUser *pUser = xt->second;
								if (xt == UserManager::UserList.End()) {
									State.IsLooting = Count(); return;
								} else if (BotUser::IsValid(pUser)) {
									// Continue the evaluation if the target is on the TargetList or if it is attacking us (Do not begin looting if some mob is hitting us)
									if (Bot::Configuration.TargetList.Contains(pUser->Data->Name.GetString()) || (pUser->TargetUid == BotUser::Me->Data->Uid && !Bot::Configuration.IgnoreList.Contains(pUser->Data->Name.GetString()))) {
										if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) <= Bot::Configuration.AttackDistance) {
											if (!pUser->Data->Dead) {
												Print(Debug::LOOT, "Will not yet loot items because of potential target [%s] [Uid: %d] [RFM: %d]",
													pUser->Data->Name,
													pUser->Data->Uid,
													UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos)); break;
											}
										}
									}
								}
							}
						}
					}
				} // Enable looting immediately if loot is found and LootAfterwards is off.
				else {
					State.IsLooting = Count();
				}		
			}
		}

		ITEMFIELD_DATA GetCurrent(void) {
			return LootList[Index];
		}

		void NoteFailure(void) {
			// Do not display output on the first movement attempt.
			if (Failed >= 1) {
				Print(Debug::LOOT, "Loot failure noted [%d].", Failed);
			}
			// Plus one on the timeout here because we don't want to count the first movement.
			if (++Failed >= (Configuration.LootTimeOut + 1)) {
				Print(Debug::LOOT, "Looting timed out.");
				Reset();
			}
		}

		struct LootFilterData {

			vector<CString> FilterList;

			void Add(CString Name) {
				FilterList.push_back(Name);
			}

			void ParseList(CString List) {
				Clear();
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tNew LootFilter Item: %s\n", Name);
							Add(Name);
							Name.Empty();
						} else {
							Name += List[i];
						}
					} printf_s("\n");
				}
			}

			CString GetList(void) {
				CString List;
				for (int i = 0; i < FilterList.size(); i++) {
					if (i == 0) {
						List += FilterList[i];
					} else {
						List += ("," + FilterList[i]);
					}
				} return List;
			}

			bool Contains(CString Name) {
				return find(FilterList.begin(), FilterList.end(), Name) != FilterList.end();
			}

			void Clear(void) {
				FilterList.clear();
			}

		} Filter;

		struct FieldListData {

			vector<ITEMFIELD_DATA> ItemFieldList;

			void Add(ITEMFIELD_DATA data) {
				ItemFieldList.push_back(data);
			}

			void Remove(CPoint Point) {
				for (auto xt = ItemFieldList.begin(); xt != ItemFieldList.end();) {
					if (xt->Point == Point) {
						xt = ItemFieldList.erase(xt);
						// Remove the item from the LootList if it has been removed from the FieldList.
						if (!Bot::State.IsLooting) {
							if (Bot::Configuration.LootAfterwards) {
								for (auto zt = Loot.LootList.begin(); zt != Loot.LootList.end();) {
									if (zt->Point == xt->Point) {
										zt = Loot.LootList.erase(zt);
									} else {
										++zt;
									}
								}
							}
						}
					} else {
						++xt;
					}
				}
			}

		} FieldList;

		struct RareItemListData {

			vector<CString> List;

			void Add(CString Name) {
				List.push_back(Name);
			}

			void ParseList(CString List) {
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tNew RareItem: %s\n", Name);
							Add(Name);
							Name.Empty();
						} else {
							Name += List[i];
						}
					} printf_s("\n");
				}
			}

			bool Contains(CString Name) {
				for (auto xt = List.begin(); xt != List.end(); xt++) {
					if (Name.Find(*xt) != -1) {
						return true;
					}
				} return false;
			}

		} RareItemList;

	} Loot;

	static struct ZoneData {

		int* Number;

		Map Current(void) {
			return (*(Map *)Number);
		}

		CString CurrentAsCString(void) {
			switch (Current()) {
				case Map::TYT:
					return "THOUSAND YEAR TOWN";
				case Map::MERC:
					return "MERCENARY AREA";
				case Map::ABIAS:
					return "ABIAS";
				case Map::VOD:
					return "VALLEY OF DEATH";
				case Map::PAND:
					return "PANDIMONIA";
				case Map::CASTLE:
					return "CASTLE";
			}
		}

		CString GetVendor(CString Type) {

			CString BlkSmith, Vendor;

			switch (Current()) {
				case Map::TYT:
					BlkSmith = "BLACKSMITH "; Vendor = "REAGENT VENDOR ";
					break;

				case Map::MERC:
					BlkSmith = "HYUNRURAN"; Vendor = "HONGU";
					break;

				case Map::ABIAS:
					BlkSmith = "MOKPUNG"; Vendor = "MOMO";
					break;

				case Map::PAND:
					BlkSmith = "SAGONARI"; Vendor = "LAMATRA";
					break;

				case Map::CASTLE:
					BotUser::Me->IsDevil() ? (BlkSmith = "TAEMA", Vendor = "RAZARU") : false;
					break;
			}
			if (Type == "BLACKSMITH")
				return BlkSmith;
			return Vendor;
		}

	} Zone;

	static struct NearbyUserDetectionData {

		bool IsEnabled;

		bool IsNotifying;

		int NotificationTotalCount;
		int NotificationCount;
		int NotificationLimit;

		Response DetectedUsrResponse;

		struct ExclusionListData {

			vector<CString> List;

			void Add(CString Name) {
				List.push_back(Name);
			}

			void ParseList(CString List) {
				if (List.GetLength() > 0) {
					CString Name;
					for (int i = 0; i <= List.GetLength(); i++) {
						if (List[i] == ',' || (i == List.GetLength())) {
							printf_s("\tWill ignore NearbyUser: %s\n", Name);
							Add(Name);
							Name.Empty();
						} else {
							Name += List[i];
						}
					} printf_s("\n");
				}
			}

			bool Contains(CString Name) {
				return find(List.begin(), List.end(), Name) != List.end();
			}

			void Clear(void) {
				List.clear();
			}

			int Count(void) {
				return List.size();
			}

		} ExclusionList;

		struct DetectedUsersData {

			vector<CString> List;

			void Add(CString Name) {
				List.push_back(Name);
			}

			bool Contains(CString Name) {
				return find(List.begin(), List.end(), Name) != List.end();
			}

			int Count(void) {
				return List.size();
			}

			// This gets run when the user warps to town.
			void Clear(void) {
				List.clear();
			}

		} DetectedUsers;

	} NearbyUserDetection;

	static struct StatTrackerData {

		vector<StatInfo> Str;
		vector<StatInfo> Dex;
		vector<StatInfo> Int;
		vector<StatInfo> Con;

		bool IsEnabled;

		void Add(Stat Type) {

			// Play a sound notification.
			OutPut.SoundPlay(110);

			// Evaluate the new stat gain.
			switch (Type) {

				// Str.
				case Stat::STR: {
					Str.push_back(StatInfo(Stat::STR, (BotUser::Me->Data->Stats.Strength + 1), GetTickCount(), BotUser::Me->Data->Experience.Current));
					if (Str.size() > 1) {
						for (auto first = Str.begin(), second = Str.begin();;) {
							if (++second != Str.end()) {
								CTimeSpan Time = (second->Time - first->Time) / 1000;
								printf_s("\n\tGain: %s to %s   Time: %d:%d:%d   Exp: %.2f%%\n",
									first->ValueAsCString(), second->ValueAsCString(),
									Time.GetHours(), Time.GetMinutes(), Time.GetSeconds(),
									100.0 * (second->Exp - first->Exp) / BotUser::Me->Data->Experience.Maximum);
								first++;
							} else {
								printf_s("\n\n"); break;
							}
						}
					} else {
						printf_s("\n\tFirst stat gain detected... Will print report on next stat gain.\n\n\n");
					}
				} break;

				// Dex.
				case Stat::DEX: {
					Dex.push_back(StatInfo(Stat::DEX, (BotUser::Me->Data->Stats.Dexterity + 1), GetTickCount(), BotUser::Me->Data->Experience.Current));
					if (Dex.size() > 1) {
						for (auto first = Dex.begin(), second = Dex.begin();;) {
							if (++second != Dex.end()) {
								CTimeSpan Time = (second->Time - first->Time) / 1000;
								printf_s("\n\tGain: %s to %s   Time: %d:%d:%d   Exp: %.2f%%\n",
									first->ValueAsCString(), second->ValueAsCString(),
									Time.GetHours(), Time.GetMinutes(), Time.GetSeconds(),
									100.0 * (second->Exp - first->Exp) / BotUser::Me->Data->Experience.Maximum);
								first++;
							} else {
								printf_s("\n\n"); break;
							}
						}
					} else {
						printf_s("\n\tFirst stat gain detected... Will print report on next stat gain.\n\n\n");
					}
				} break;

				// Int.
				case Stat::INT: {
					Int.push_back(StatInfo(Stat::INT, (BotUser::Me->Data->Stats.Intelligence + 1), GetTickCount(), BotUser::Me->Data->Experience.Current));
					if (Int.size() > 1) {
						for (auto first = Int.begin(), second = Int.begin();;) {
							if (++second != Int.end()) {
								CTimeSpan Time = (second->Time - first->Time) / 1000;
								printf_s("\n\tGain: %s to %s   Time: %d:%d:%d   Exp: %.2f%%\n", 
									first->ValueAsCString(), second->ValueAsCString(), 
									Time.GetHours(), Time.GetMinutes(), Time.GetSeconds(), 
									100.0 * (second->Exp - first->Exp) / BotUser::Me->Data->Experience.Maximum);
								first++;
							} else {
								printf_s("\n\n"); break;
							}
						}
					} else {
						printf_s("\n\tFirst stat gain detected... Will print report on next stat gain.\n\n\n");
					}
				} break;

				// Con.
				case Stat::CON:
					break;
			}
		}

		void ClearAll(void) {
			Str.clear();
			Dex.clear();
			Int.clear();
			Con.clear();
		}

	} StatTracker;

	static struct OutPutData {

		bool ShowIsAttacking = true;

		int TextColour = 10;

		void SoundPlay(BYTE Type);

		void TextColourChange(void) {
			if (++TextColour >= 15) {
				TextColour = 10;
			}
		}
		void TextColourChange(int colour) {
			TextColour = colour;
		}

	} OutPut;

	static bool Wait(int Time);

	static void Print(Debug type, LPCSTR format, ...);
};

