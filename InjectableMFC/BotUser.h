#pragma once

struct Subscriber {

	vector<string> Users;

	struct Serials {

		DWORD cpuId,
			  hddId,
		      uniqueId;

		Serials(DWORD cpu, DWORD hdd, DWORD unique) :
			cpuId(cpu), hddId(hdd), uniqueId(unique) {}

	} Serials;

	Subscriber(vector<string> users, DWORD cpu, DWORD hdd, DWORD unique) :
		Users(users), Serials(cpu, hdd, unique) {}
};

class Subscribers {
private:
	vector<Subscriber> List;
public:

	vector<string> PermittedUsers;

	vector<Subscriber>::const_iterator Begin(void) {
		return List.begin();
	}
	vector<Subscriber>::const_iterator End(void) {
		return List.end();
	}

	void Add(Subscriber Subscriber) {
		List.push_back(Subscriber);
	}

	int Count() {
		return List.size();
	}

	bool VerifyId(DWORD* Id) {
		for (auto xt = Begin(); xt != End(); xt++) {
			//printf_s("%x\t%x\n", xt->Serials.cpuId, Id[0]);
			if (xt->Serials.cpuId == Id[0]) {
				if (xt->Serials.hddId == Id[1]) {
					if (xt->Serials.uniqueId == Id[2]) {
						PermittedUsers = xt->Users; return true;
					}
				}
			}
		} return false;
	}
};

class WareHouse {
private:
	vector<WAREHOUSE_DATA> List;
public:

	vector<WAREHOUSE_DATA>::const_iterator Begin(void) {
		return List.begin();
	}
	vector<WAREHOUSE_DATA>::const_iterator End(void) {
		return List.end();
	}

	void Add(WAREHOUSE_DATA Item) {
		List.push_back(Item);
	}

	int Count(void) {
		return List.size();
	}

	bool Contains(CString Item) {
		for (vector<WAREHOUSE_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Item) != -1) {
				return true;
			}
		} return false;
	}

	void Flush(void) {
		List.clear();
	}

	WAREHOUSE_DATA* GetByName(CString Item) {
		for (auto xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Item) != -1) {
				return (WAREHOUSE_DATA *)&*xt;
			}
		} return NULL;
	}

	bool GetByName(CString Item, WAREHOUSE_DATA* &Data) {
		for (auto xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Item) != -1) {
				Data = (WAREHOUSE_DATA *)&*xt;
				return true;
			}
		} return false;
	}

	WAREHOUSE_DATA* GetByIndex(int Index) {
		for (auto xt = Begin(); xt != End(); xt++) {
			if (xt->Index == Index) {
				return (WAREHOUSE_DATA*)&*xt;
			}
		}
	}

};

class SpecialAttackList {
private:
	vector<SPECIALATTACK_DATA> List;
public:

	vector<SPECIALATTACK_DATA>::const_iterator Begin(void) {
		return List.begin();
	}
	vector<SPECIALATTACK_DATA>::const_iterator End(void) {
		return List.end();
	}

	void Add(SPECIALATTACK_DATA Skill) {
		List.push_back(Skill);
	}

	int Count(void) {
		return List.size();
	}

	bool Contains(CString Skill) {
		for (vector<SPECIALATTACK_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name == Skill) {
				return true;
			}
		} return false;
	}

	SPECIALATTACK_DATA* GetByName(CString Skill) {
		for (vector<SPECIALATTACK_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Skill) != -1) {
				return (SPECIALATTACK_DATA *)&*xt;
			}
		} return NULL;
	}
	SPECIALATTACK_DATA* GetByClassNdName(CString Skill, ItemType Class) {
		for (vector<SPECIALATTACK_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Skill) != -1) {
				if (xt->Class == Class) {
					return (SPECIALATTACK_DATA *)&*xt;
				}
			}
		} return NULL;
	}
	SPECIALATTACK_DATA* GetById(short SkillId) {
		for (vector<SPECIALATTACK_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Id == SkillId) {
				return (SPECIALATTACK_DATA *)&*xt;
			}
		} return NULL;
	}
};

class MagicList {
private:
	vector<MAGIC_DATA> List;
public:
	MAGIC_DATA* AtkSpell;

	vector<MAGIC_DATA>::const_iterator Begin(void) {
		return List.begin();
	}
	vector<MAGIC_DATA>::const_iterator End(void) {
		return List.end();
	}

	void Add(MAGIC_DATA Skill) {
		List.push_back(Skill);
	}

	int Count(void) {
		return List.size();
	}

	bool Contains(CString Magic) {
		for (vector<MAGIC_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Magic) != -1) {
				return true;
			}
		} return false;
	}

	MAGIC_DATA* GetByName(CString Magic) {
		for (vector<MAGIC_DATA>::const_iterator xt = Begin(); xt != End(); xt++) {
			if (xt->Name.Find(Magic) != -1) {
				return (MAGIC_DATA *)&*xt;
			}
		} return NULL;
	}

	struct BeltData {
		MAGICBELT_DATA** Belt;

		int* BeltSelIndex;

		int SelectedIndex(void) {
			return *(int *)BeltSelIndex;
		}

		CString SpellName(void) {
			return Belt[SelectedIndex()]->Name.GetString();
		}

		void Id(void) {
			printf_s("%d %X\n", Belt[SelectedIndex()]->Number, &Belt[SelectedIndex()]->Number);
		}

	} Belt;
};

class BotUser {
private:
public:
	static CUser *Me;

	static DWORD WINAPI Process(LPVOID lpParameter);

	static Subscribers Subscribers;

	static WareHouse WareHouse;

	static SpecialAttackList SpecialAttackList;

	static MagicList MagicList;

	static struct InventoryData {

		bool Contains(CString Name) {
			for (int i = 10; i < 50; i++) {
				if (Item.Name(i).Find(Name) != -1) {
					return true;
				}
			} 
			for (int i = 0; i < 4; i++) {
				if (Item.Name(i).Find(Name) != -1) {
					return true;
				}
			} 
			return false;
		}

		int NearestEmpty(int Exclusion) {
			for (int i = 10; i < 50; i++) {
				if (BotUser::Me->Data->Inventory[i].PictureNumber == -1) {
					if (Exclusion == NULL) {
						return i;
					} else {
						if (Exclusion != i) {
							return i;
						}
					}
				}
			} return -1;
		}

		int SpaceAvailable(void) {
			int i = 10, FreeSlots = 0;
			for (; i < 50; i++) {
				if (BotUser::Me->Data->Inventory[i].PictureNumber == -1) {
					FreeSlots++;
				}
			} return FreeSlots;
		}

		int BwAvailable(void) {
			if (BotUser::Me->Data->Weight.Maximum == BotUser::Me->Data->Weight.Current) {
				return BotUser::Me->Data->Weight.Maximum;
			} return (BotUser::Me->Data->Weight.Maximum - BotUser::Me->Data->Weight.Current);
		}

		// Single item evaluations.
		struct ItemData {

			// Does the item exist?
			bool IsValid(int Index) {
				return (BotUser::Me->Data->Inventory[Index].PictureNumber != -1);
			}

			// Name.
			CString Name(int Index) {
				if (Index >= 50) {
					return BotUser::Me->Data->Belt[(Index - 50)].Name.GetString();
				} return BotUser::Me->Data->Inventory[Index].Name.GetString();
			}

			// Quantity.
			int Count(int Index) {
				if (Index >= 50) {
					return BotUser::Me->Data->Belt[(Index - 50)].Dura.Current;
				} return BotUser::Me->Data->Inventory[Index].Dura.Current;
			}

			// Type.
			ItemType Type(int Index) {
				if (Index >= 50) {
					return Me->Data->Belt[(Index - 50)].Type;
				} return Me->Data->Inventory[Index].Type;		
			}

			// The items position when equipped.
			Armor ArmPos(int Index) {
				return BotUser::Me->Data->Inventory[Index].ArmPos;
			}

			// Gender.
			Gender Gender(int Index) {
				return BotUser::Me->Data->Inventory[Index].Gender;
			}

			// Dura.
			int DuraOrigMax(int Index) {
				return BotUser::Me->Data->Inventory[Index].Dura.OriginalMaximum;
			}
			int DuraMax(int Index) {
				return BotUser::Me->Data->Inventory[Index].Dura.Maximum;
			}
			int Dura(int Index) {
				return BotUser::Me->Data->Inventory[Index].Dura.Current;
			}

			// Potions.
			int Hp(int Index) {
				if (Index >= 50) {
					return Me->Data->Belt[(Index - 50)].HP;
				} return Me->Data->Inventory[Index].HP;
			}
			int Mp(int Index) {
				if (Index >= 50) {
					return Me->Data->Belt[(Index - 50)].MP;
				} return Me->Data->Inventory[Index].MP;
			}

			// Requirements.
			struct Requirements {

				int Strength(int Index) {
					return (BotUser::Me->Data->Inventory[Index].Required.Str * 10);
				}
				int Intelligence(int Index) {
					return (BotUser::Me->Data->Inventory[Index].Required.Int * 10);
				}
				int Dexterity(int Index) {
					return (BotUser::Me->Data->Inventory[Index].Required.Dex * 10);
				}
				int Skill(int Index) {
					return (BotUser::Me->Data->Inventory[Index].Required.Skill * 10);
				}

			} Requirement;

			// Misc.
			bool HasMagicAvailable(int Index) {
				return (BotUser::Me->Data->Inventory[Index].SpecialNumber == 46);
			}

			bool IsWepOrArmour(int Index) {
				return ((Type(Index) == ItemType::ARMOR) || (Type(Index) == ItemType::ACC) || (ArmPos(Index) == Armor::HANDS));
			}

			bool IsRepairable(int Index) {

				if (IsWepOrArmour(Index)) {

					if (BotUser::Me->IsDevil()) {
						return true;
					}
					else if (IsUpgraded(Index)) {
						return false;
					}
					else if (IsHero(Index)) {
						return false;
					}

					CString Excludes[] = { "Bone", "[Epic]", "[Intense]", "Medusa", "Elf" };

					for (int i = 0; i < 5; i++) {
						if (Name(Index).Find(Excludes[i]) != -1) {
							return false;
						}
					}
					//printf_s("\nitem [%s] [I: %d] [Dura: %d] [OrigMax: %d]", BotUser::Inventory.Item.Name(Index), Index, BotUser::Inventory.Item.Dura(Index), BotUser::Inventory.Item.DuraOrigMax(Index));
					if (Dura(Index) < (DuraOrigMax(Index) * 50 / 100)) {
						return true;
					}

				} return false;
			}

			bool IsUpgraded(int Index) {
				return (BotUser::Me->Data->Inventory[Index].UpgradeLvl > 0);
			}

			bool IsHero(int Index) {
				return (BotUser::Me->Data->Inventory[Index].Name.Find("Hero") != -1);
			}

		} Item;

		bool GetByType(ItemType Type, ITEM_DATA *&Data, bool FindHp) {
			for (int i = 10; i < 50; i++) {
				if (Item.Type(i) == Type) {
					if (Type == ItemType::POTION) {
						// Find HP potions.
						if (FindHp) {
							if (Item.Hp(i) > 0) {
								Me->Data->Inventory[i].Index = i;
								Data = &Me->Data->Inventory[i];
								return true;
							}
						} 
						// Find MP potions.
						else {
							if (Item.Mp(i) > 0) {
								Me->Data->Inventory[i].Index = i;
								Data = &Me->Data->Inventory[i];
								return true;
							}
						}
					} else {
						// The client doesn't properly update the index so we do it here.
						Me->Data->Inventory[i].Index = i;
						Data = &Me->Data->Inventory[i];
						return true;
					}
				}
			}
			for (int i = 0; i < 4; i++) {
				if (Me->Data->Belt[i].Type == Type) {
					if (Type == ItemType::POTION) {
						// Find HP potions.
						if (FindHp) {
							if (Me->Data->Belt[i].HP > 0) {
								Me->Data->Belt[i].Index = (i + 50);
								Data = &Me->Data->Belt[i];
								return true;
							}
						}
						// Find MP potions.
						else {
							if (Me->Data->Belt[i].MP > 0) {
								Me->Data->Belt[i].Index = (i + 50);
								Data = &Me->Data->Belt[i];
								return true;
							}
						}
					} else {
						// The client doesn't properly update the index so we do it here.
						Me->Data->Belt[i].Index = (i + 50);
						Data = &Me->Data->Belt[i];
						return true;
					}
				}
			} return false;
		}
		bool GetByName(CString Name, ITEM_DATA* &Data) {
			for (int i = 10; i < 50; i++) {
				if (BotUser::Me->Data->Inventory[i].Name.Find(Name) != -1) {
					// The client doesn't properly update the index so we do it here.
					BotUser::Me->Data->Inventory[i].Index = i;
					Data = &BotUser::Me->Data->Inventory[i]; 
					return true;
				}
			}
			for (int i = 0; i < 4; i++) {
				if (BotUser::Me->Data->Belt[i].Name.Find(Name) != -1) {
					// Add INV_COUNT to Index so we know later on if the item is located on the belt.
					BotUser::Me->Data->Belt[i].Index = (i + 50);
					Data = &BotUser::Me->Data->Belt[i]; 
					return true;
				}
			} return false;
		}
		ITEM_DATA* GetByName(CString Name) {
			for (int i = 10; i < 50; i++) {
				if (BotUser::Me->Data->Inventory[i].Name.Find(Name) != -1) {
					return &BotUser::Me->Data->Inventory[i];
				}
			} 
			for (int i = 0; i < 4; i++) {
				if (BotUser::Me->Data->Belt[i].Name.Find(Name) != -1) {
					return &BotUser::Me->Data->Belt[i];
				}
			} return NULL;
		}

	} Inventory;

	static void FindNpc(CString Name);

	static bool IsValid(CUser *pUser);

	static DWORD ResolvePointer(DWORD pAddress, int pLevels, ...);

	static BOOL CaptureScreen(CString Type);

	static void EvaluateNewValues(int NewHp, int NewMp);

	static int RandomNumberGenerator(int Min, int Max) {
		// Min: 0 Max: 5 will return an absolute range of 0-5
		return Min + (int)((double)rand() / (RAND_MAX + 1) * (Max - Min + 1));
	}

	static int EvenRandomNumberGenerator(int Min, int Max) {
		// Min: 0 Max: 6 will return an absolute range of 0-6
		int Num;
		do {
			Num = Min + (int)((double)rand() / (RAND_MAX + 1) * (Max - Min + 1));
		} while (Num % 2 != 0);
		return Num;
	}
};





