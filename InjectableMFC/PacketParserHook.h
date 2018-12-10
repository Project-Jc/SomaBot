#pragma once

#define PREPARE_PACKETPARSER_CALL() __asm mov ecx, 0x006D8370;

#define Parse(address, type, size) *(type *) address; address += (sizeof(type) + size);
#define ParseData(address, type) *(type *) address; address += sizeof(type);
#define ParseEnum(address, enumName) *(enumName *) address; address += sizeof(std::underlying_type<enumName>::type);
#define ParseString(address, length) CString((LPCSTR) address, length); address += length;
#define ParseCPoint(address, length) *(int *) address; address += sizeof(int);

typedef BOOL(WINAPI *tPacketProcess)(PBYTE bypData, int size);
typedef BOOL(WINAPI *tRawPacketProcess)(PBYTE bypData, int size);

class PacketParserHook {                       
private:
public:
	static bool Init(void);
	static void Exit(void);

	struct CC {
		static CodeCave PacketProcess;
		static CodeCave RawPacketProcess;
	};

	struct MY {
		static BOOL WINAPI PacketProcess(PBYTE bypData, int size);
		static BOOL WINAPI RawPacketProcess(PBYTE bypData, int size);
	};
};

enum class ChatType : BYTE {
	NORMAL = 1,
	PRIVATE_INCOMING,
	ZONE,
	PRIVATE_OUTGOING,
	SYSTEM,
	GM_NOTICE,
	GUILD,
	SHOUT,
	YELLOW_STAT,
	BLUE_STAT,
	PARTY
};

enum class ItemFieldType : BYTE {
	NEW = 1,
	REMOVE
};

#pragma pack(1)
struct ColorRGB {
	BYTE Red;
	BYTE Green;
	BYTE Blue;
};

#pragma pack(1)
struct PARAM_CRAFTITEM {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

	short TotalMaterialCount;
	short FirstMaterialIndex;
	short FirstMaterialCountNew;
	short SecondMaterialIndex;
	short SecondMaterialCountNew;
	short CraftedItemIndex;

};

#pragma pack(1)
struct PARAM_MOVE {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

	int Uid;

	short Unknown;

	short ptDest_X, ptDest_Y;

	short ptFirst_X, ptFirst_Y;

	short ptNext_X, ptNext_Y;

	short Stamina;

	short Direction;
};

#pragma pack(1)
struct PARAM_USERMODIFY {
	enum class Type : BYTE {
		MODIFY = 1,
		REMOVE
	} Type;

	int Uid;

	int Class;

#pragma pack(1)
	struct Pos {
		short X;
		short Y;
	} Pos;
};

#pragma pack(1)
struct PARAM_ZONECHANGE {
	int Uid;

#pragma pack(1)
	struct Pos {
		short X;
		short Y;
	} Pos;

	short MapNum;
};

#pragma pack(1)
struct PARAM_ATTACK {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

#pragma pack(1)
	struct Uid {
		int Src;
		int Dst;
	} Uid;

	byte AttackArrow;

	short Direction;

	//int Unused[4];
	int Unused;
	int Unused1;
	int Unused2;
	int Unused3;

#pragma pack(1) 
	struct Health {
		short NewCurrent;
		short NewMax;
	} Health;

	DWORD ExpGiven;

};

#pragma pack(1)
struct PARAM_MAGICREADY {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

#pragma pack(1)
	struct Uid {
		int Src;
		int Dst;
	} Uid;

	short Direction;

	short MagicId;

	BYTE Distance;

	DWORD StartTime;
};

#pragma pack(1)
struct PARAM_MAGICATTACK {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

	short Direction;

#pragma pack(1)
	struct Src {
		int Uid;
		short NewHealth;
		short NewMana;
	} Src;

	DWORD ExpGiven;

	struct Dst {
		int Uid;
		short MagicId;
		short NewHealth;
		short NewHealthMax;
	} Dst;
};

#pragma pack(1)
struct PARAM_MAGICCIRCLE {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

	short Direction;

	int SrcUid;
};

#pragma pack(1)
struct PARAM_SPECIALATTACK {
	enum class Result : BYTE {
		SUCCESS = 1,
		FAILURE,
		UNKNOWN
	} Result;

	struct Src {
		int Uid;
		short NewHealth;
		short NewMana;
		short NewStamina;
		short NewDirection;
	} Src;

	short AuraIndex;

	DWORD Duration;

	struct Dst {
		int Uid;
		short NewHealth;
		short NewMana;
	} Dst;

	short xCell, yCell;
};

#pragma pack(1)
struct PARAM_ITEMDATA {
	short Index;
	byte Type;
	byte ArmPos;
	short Class;
	short PicNum;

	byte Gender;
	short MinDamage;
	short MaxDamage;
	short Weight;
	short Speed;

	short Str;
	short Int;
	short Dex;
	short Skill;

	short HP;
	short MP;

	short Dura;

	byte NameLength;
	CString Name;

	byte UpgradeLvl;

	char Unknown[0x18];
};

#pragma pack(1)
struct PARAM_CHARTERDATA {
	int Uid;

	byte Value;

	short Level;

	struct Exp {
		DWORD Maximum;
		DWORD Current;
	} Exp;

	struct Health {
		short Maximum;
		short Current;
	} Health;

	struct Mana {
		short Maximum;
		short Current;
	} Mana;
};

#pragma pack(1)
struct PARAM_ITEMFIELD {
	short PicNumer;
	ItemType Type;
	int Quantity;
	byte ItemNameLength;
	CString Name;
};

#pragma pack(1)
struct PARAM_SETTIME {
	short Year;
	short Month;
	short Day;
	short Hour;
	short Minute;

	byte Rain;
	byte Unknown;
	byte Night;
};

#pragma pack(1) 
struct PARAM_SHOPOPEN {
	short ShopNumber;
	short ShopBehaviour;

	short BuyRate;
	short SellRate;

	short Unknown;

	short ItemCount;
};

#pragma pack(1)
struct PARAM_MAGICALL {
	short Id;

	byte MethodOne;
	byte MethodTwo;

	short CastTime;

	byte Type;

	byte TargetMethod;

	byte NameLength;
	CString *Name;

	byte ExplanationLength;
	CString Explanation;

	short Distance;

	short MpUsage;

	int Duration;

	short Damage;

	short Unknown;

	short MinIntReq;

	int Unknown1;
};

#pragma pack(1)
struct PARAM_CHANGE_ITEM_INFO {

	enum class Location : BYTE {
		BLT,
		INV
	} Location;

	short Index;

	enum class Type : BYTE {
		ALL = 0xFF,
		TYPE = 0x1,
		ARM,
		PICNUM = 0x4,
		DURA = 0x8
	} Type;
};

#pragma pack(1)
struct PARAM_AREASTATE {
	int Uid;
	enum class Type : BYTE {
		REMOVE,
		STAMINA,
		HEALTH
	} Type;
};

