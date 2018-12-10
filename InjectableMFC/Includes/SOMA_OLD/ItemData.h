
#if !defined(AFX_ITEM_DATA_H__9FCA6741_78DB_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_ITEM_DATA_H__9FCA6741_78DB_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DarkDef.h"
class BBDX_MouseInformation;
class CDDSurface;

#define NORMAL_ITEM 0
#define REPAIR_ITEM 1
#define SHOP_ITEM 2

#define GET_VALUE 0x01
#define GET_EXTRA_USAGE 0x02
#define NORM_LAYOUT 0x04
#define TRADE_LAYOUT 0x08

#define ITEM_TYPE_STORAGE NORM_LAYOUT
#define ITEM_TYPE_ME GET_VALUE | GET_EXTRA_USAGE | NORM_LAYOUT
#define ITEM_TYPE_EXCHANGE TRADE_LAYOUT

#define SPECIAL_NUMBER 3
#define SPECIAL_OPT_NUMBER 3

typedef struct _SPECIAL_DATA
{
	short		m_sSpecialNo;		// Special no e.g. pos gem / shout scroll / compass
	short		m_sOpt[SPECIAL_OPT_NUMBER];
} SPECIAL_DATA;

class CItemData
{
public:
	CItemData();
	~CItemData();
	void ViewCaption(CDDSurface *pDest, CPoint caption_pos, BYTE bType = NORMAL_ITEM, CString strType = "");
	void ViewStaticCaption(CDDSurface *pDest, CRect caption_rect);
	void Init();
	void RcvItemData(PBYTE bypData, int &offset, BYTE bFlags);
	void RcvItemData(PBYTE bypData, int &offset, int index, BYTE bFlags);
	static	int	compare(const void *arg1, const void *arg2); // For qsort on the item array
	void SetItemRarity();
	CItemData& operator=(CItemData& other);
    bool IsSpecial(short sSpecialNo)
    {
		for(int i = 0; i < SPECIAL_NUMBER; i++)
        {
			if (m_SpecialData[i].m_sSpecialNo == sSpecialNo)
			{
				return true;
			}
        }
        return false;
    }

public:
	int			m_vIndex;			// Item index... relative to the amount of items you have etc.
	BYTE		m_vType;			// Á¾·ù
	BYTE		m_vGender;			// Item Gender, 0 = GenderLess, 1 = woman, 2 = man
	int			m_vPicNum;			// Pic Number, relative to the files in the item folder
	short		m_vMinDamage;
	short		m_vMaxDamage;
	short		m_vWeight;			// Item Weight
	short		m_vLimitStr;		// Required Str
	short		m_vLimitInt;		// Required  Int
	short		m_vLimitDex;		// Dex needed to wear item
	short		m_vLimitSkill;		// Required Skill
	DWORD		m_vValue; // Value of the item, Only used in shops and auction/Guild shop
	CString		m_vName; // Name of the item	
	short		m_vUsage; // Current Duration / Amount of an item such as pots, dependant on whether it is stackable
	short		m_vMaxUsage; // Current Duration (MAX)
	short		m_vOrgMaxUsage; // Original Duration (MAX)
	short		m_vSpeed; // Speed of the weapon
	short		m_vClass; // Class of the item... i.e. 0 = human, 10 = devil
	short		m_nHp;
	short		m_nMp;
	SPECIAL_DATA m_SpecialData[SPECIAL_NUMBER]; // There can be a maximum of 3 different specials per item
	CString Speed;
	CString GetSpeed()
	{
	if (m_vSpeed>1200){Speed="F";}
	if (m_vSpeed<=1200){Speed="E";}
	if (m_vSpeed<=1100){Speed="D";}
	if (m_vSpeed<=1000){Speed="C";}
	if (m_vSpeed<=900){Speed="B";}
	if (m_vSpeed<=800){Speed="A";}
	if (m_vSpeed<=720){Speed="A+";}
	if (m_vSpeed<=640){Speed="A++";}
	if (m_vSpeed<=560){Speed="A+++";}
	return Speed;
	}
	short			m_vQuantity;  // Used in shops, represents the amount of items left in stock

	BYTE		m_vArmPos;			// Used for my items, represents where the item should be placed
	// on the body

	CString		m_vRegistrant; // Used in auction and guild shop, Displays the name of the person
	// who registered the item
	CString		m_vHighBidder; // Highest bidder in auction

	CString		m_strItemsReq[5]; // Items required for exchange
	short		m_nAmount[5]; // Amount of items required for exchange
	BYTE		m_bUpgradeLvl; // Upgrade lvl of item... has + X next to name

	int			m_vRareType; // Used to define the rarity of an item
};


class CItemDisplay
{
public:
	CItemDisplay();
	~CItemDisplay();
	static CString CreateNameStr(CItemData* item_data);
	static CString CreateStatsStr(CItemData* item_data);
	static CString CreateReqStr(CItemData* item_data);
	static CString CreateSpecialStr(CItemData* item_data);
	static void ViewCaption(CItemData *item_data, CDDSurface *pDest, CPoint caption_pos, BYTE bType, CString strType);
	static void ViewStaticCaption(CItemData *item_data, CDDSurface *pDest, CRect caption_rect);
private:
	static void DrawTipRect(CDDSurface *pDest, CRect& t_rect);
	static void DrawCaption(CDDSurface *pDest, CPoint pos, CString t_str, int stats_Length, int req_Length, int special_Length, BYTE bType);
	static void DrawCaption(CDC *pDC, CRect &t_rect, CString t_str, COLORREF crCK, int settings);
	static void DrawStaticCaption(CDDSurface *pDest, CRect rect, CString t_str, int stats_Length, int req_Length, int special_Length);
};

typedef CMap< int, int, CItemData *, CItemData*&> CItemDataMap;
typedef CTypedPtrArray<CPtrArray, CItemData *> CItemDataArray;

#endif
