#pragma once
#include "stdafx.h"

struct WAREHOUSE_DATA {
	short Index;
	short Quantity;
	CString Name;
};

#pragma pack(1)
struct ITEM_DATA {

	char Unknown1[0x4];	// This is some address which is at the start of every ITEM_DATA

	short Index; // The position of the item in inventory

	ItemType Type;

	Armor ArmPos;

	short Unknown2; // Shows zero? Unused?
	
	short PictureNumber;

	Gender Gender;

	byte Unknown3; // Seems to always be zero. Unused?

	short MinDamageOrDefence;
	short MaxDamageOrDefence;

	short Weight;
	short Speed;

#pragma pack(1)
	struct Required {
		short Str;
		short Int;
		short Dex;
		short Skill;
	} Required;

	short HP;
	short MP;

#pragma pack(1)
	struct Dura {
		short Current;
		short Maximum;
		short OriginalMaximum;
	} Dura;

	short ShopPrice; // How much the item would cost if bought new from a shop.

	short Unknown4; // Seems to always be zero. Unused?

	short SpecialNumber;

#pragma pack(1)
	struct MagicDmgOrDef {
		short Minimum;
		short Maximum;
	} MagicDmgOrDef;

	enum class Class : short {
		HUMAN = 0,
		DEVIL = 10
	} Class;

	char Unknown5[0x10];

	CString Name;

	int UpgradeLvl; // This shows 1 if an item is upgraded. Not sure if it's actually an int though.
};

#pragma pack(1)
struct MAGICBELT_DATA {
	char Unknown0[0x4];
	short Number; 
	char Unknown1[0x12];
	CString Name;
	char Unknown2[0x4];
	short Range;
	short MpCost;
	short HpCost;
	char Unknown3[0xA];
	short Power;
	char Unknown4[0x2]; 
	short SkillReq;
};

#pragma pack(1)
struct USER {
	char Unknown0[0x38];

	BOOL Dead;

	char Unknown1[0xC];

	enum class Motion : int {
		STOPPED = 0,
		WALKING,
		// This includes aggressive magic spells.
		ATTACKING,
		// True if being attacked.
		ATTACKED,
		// This is only true for defensive spells like heal.
		CASTING,
		DIE,
		RUNNING,
	} AnimationIndex;

	// char Unknown2[0xC];
	char Unknown2[0x8];

	Direction Direction;

	char Unknown3[0x3];

	CString Name;

	char Unknown4[0x4];

	int UserWarping;

	char Unknown5[0x78];

	byte DebuffCount;

	char Unknown6[0xFF];

	CPoint CellPos;

	char Unknown7[0x28];

	int Uid;

	char Unknown8[0x14];

	// Displays 1 if the current user.
	int IsMe;

	// Number of the house the player is in (If any).
	int HouseNumber;

	int Level;

	char Unknown9[0x4];

#pragma pack(1)
	struct Experience {
		int Maximum;
		int Current;
	} Experience;

	char Unknown10[0x4];

#pragma pack(1)
	struct Health {
		int Current;
		int Maximum;

	} Health;

#pragma pack(1)
	struct Mana {
		int Current;
		int Maximum;
	} Mana;

#pragma pack(1)
	struct Stamina {
		int Maximum;
		int Current;
		int Previous;
	} Stamina;

#pragma pack(1)
	struct Weight {
		int Maximum;
		int Current;
	} Weight;

	// No idea what this is?
	int At; 

	int Barr;

	int Style;

	//enum class Gender : int {
	//	UNISEX, WOMAN, MAN
	//} Gender;

	// 0 is FEMALE. 1 is MALE.
	int Gender;

	// Previously DeadEnable. Previously BYTE.
	bool CanDie; 

	char Unknown12[0x2F];

	int Skin;
	int Hair;

	int Unknown123;

	int CurrentWeapon;

	int Unknown122;

	int Stance;

	char Unknown13[0x30];

	ITEM_DATA Inventory[50];

	ITEM_DATA Belt[4];

	char Unknown14[0xE4];

	CString Designation;

	CString Guild;

	int Pos;

	short Age;

	short Deposition;

#pragma pack(1)
	struct Stats {
		short Strength;
		short Intelligence;
		short Dexterity;
		short Wisdom;
		short Charisma;
		short Constitution;
	} Stats;

#pragma pack(1)
	struct WeaponSkills {
		int Sword;
		int Spear;
		int Axe;
		int Knuck;
		int Bow;
		int Staff;
	} WeaponSkills;

#pragma pack (1)
	struct CraftingSkills {
		int Weapon;
		int Armour;
		int Accessory;
		int Potion;
		int Cooking;
	} CraftingSkills;

#pragma pack (1)
	struct MagicSkills {
		int Black;
		int White;
		int Blue;
	} MagicSkills;

	char Unknown15[0x8];

	short BloodType;

	//char Unknown16[0x8];

	short Class;
};

struct MAGIC_DATA {
	short Id;

	CString Name;

	byte TargetMethod;

	// 1 = blue, 2 = white, 3 = black
	byte Class;

	enum class School : BYTE {
		BLUE = 1,
		WHITE,
		BLACK
	} School;

	short Power;

	short CastTime;

	short Range;

	int Duration;

	short MpUsage;
};

struct SPECIALATTACK_DATA {
	short Id;

	CString Name;

	byte TargetMethod;

	struct Usage {
		short Health;
		short Mana;
		short Stamina;
	} Usage;

	short Power;

	short Range;

	int Duration;

	int CoolDown;

	ItemType Class;
};

struct ITEMFIELD_DATA {
	CPoint Point;
	short PictureId;
	byte Type;
	int Quantity;
	CString Name;
	int RFM;
	bool RareItem;
};

typedef USER *PUSER;

class CUser {
public:
	// USER struct to pull data from.
	PUSER Data;

	// Target details.
	CUser *Target;
	int TargetUid;

	// This currently only works for BotUser::Me.
	int *Race;
	byte *Stamina;

	// Debuff states.
	bool Weakened;
	bool Slowed;

	// This is used to determine if a target has moved as we're approaching it (It is updated and evaluated on ManageAttack()).
	CPoint PrevCellPos;

	// When a mob is aggressive... This will hold the range in which that mob will detect us and begin moving towards us.
	int AggroRange;

	int PreviousHp;

	CUser(PUSER data) {
		// Tie this CUser to a USER struct.
		Data = data;

		Target = nullptr;
		TargetUid = 0;

		PreviousHp = Data->Health.Current;

		SetAggroRange();
	}

	// Methods.
	void ResetTarget(void) {
		Target = nullptr;
		TargetUid = 0;
	}
	bool HasTarget(void) {
		return (TargetUid != 0);
	}

	void SetAggroRange(void) {
		if (!IsPlayer() && IsAggressive()) {
			// Set a default AggroRange. (Most common value).
			AggroRange = 8;
			if (Data->Name == "DEVILFISH") {
				AggroRange = 12;
			} else if (Data->Name == "SAND VALKIRYE" || Data->Name == "WAR BEAST") {
				AggroRange = 10;
			} 
		}
	}

	BOOL IsCharStop(void) {
		return (Data->AnimationIndex == USER::Motion::STOPPED);
	}
	BOOL IsCharAttack(void) {
		return ((Data->AnimationIndex == USER::Motion::ATTACKING) || HasTarget());
	}
	BOOL IsMoving(void) {
		return ((Data->AnimationIndex == USER::Motion::WALKING) || (Data->AnimationIndex == USER::Motion::RUNNING));
	}

	BOOL IsWeakened(void) {
		return Weakened;
	}
	BOOL IsSlowed(void) {
		return Slowed;
	}

	BOOL IsPlayer(void) {
		return (Data->Uid < 10000);
	}
	BOOL IsMagicMob(void) {
		CString MagicMobNames[] = { 
			"GAZER", "ELDER GAZER", "EVIL BRIC", "DRUID", "WRAITH", "LICH LORD", "NIGHTMARE", "LICH", "PRISMATIC ENTITY", 
			"PRISMATIC SHADOW", "NECRONIS", "ELITE GARGOYLE FIGHTER", "DARIAN SHAMAN", "ELF" 
		};

		for (int i = 0; i < 14; i++) {
			if (Data->Name == MagicMobNames[i]) {
				return true;
			}
		} return false;
	}
	BOOL IsAggressive(void) {

		const CString PassiveMobs[] = { 
			"DIRE WOLF", "SAVANNAH TIGER", "MUMMY", "GHOUL", "WHITE TIGER", "DRUID", "LEOCROT", "GIANT SPIDER WHELP", "YOUNG GIANT SPIDER", "DRUID", "PSYCHO TROLL", 
			"SCORPION WHELP", "PALUS", "BRIC", "EVIL BOAR", "GRIZZLY BEAR", "SENTIENT BOAR", "FRAGAST", "OWL BEAR", "KING DIRE WOLF", "FESTIVE ELF", "INGUIT", "SEDLAK", 
			"TILLEK", "INFERNAL MANTIS", "FAMILIAR", "ANCIENT BIRD", "FLESH GOLEM", "DREAD GHOUL", "WAR BEAST COMMANDER", "EVIL BRIC",

			"ELF", "DRIFTING VENDOR", "GUARD", "WAREHOUSE KEEPER", "DAEJUNGHYUNG", "SANTA", "YOUNGSI", "GUKSEO", "REAGENT TABLE", "REAGENT VENDOR", "MAGIC TRAINER", "FREEBULLETINBOARD",
			"GUILD MASTER", "GEM TRADER", "TANNER", "YEONGEOP", "BLACKSMITH", "KAJI", "TOWN CRYER", "ADVISOR HALL KEEPER", "OVEN"

		};

		// Have to use the actual number of CStrings because ->GetLength() isn't accurate?
		for (int i = 0; i < 52; i++) {
			if (PassiveMobs[i] == Data->Name) {
				return false;
			}
		} return true;
	}

	BOOL IsDevil(void) {
		return (*(int*)Race > 0);
	}
	BOOL StaminaOn(void) {
		return (*(bool*)Stamina);
	}

	int RangeFromCell(CPoint Cell) {
		return (abs(Cell.x - Data->CellPos.x) + abs(Cell.y - Data->CellPos.y));
	}

};
