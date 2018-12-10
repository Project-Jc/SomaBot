// Dialog_Ability.h: interface for the CDialog_Ability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_ABILITY_H__99698444_6F6B_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_ABILITY_H__99698444_6F6B_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
#include "DarkDef.h"
#include "ItemData.h"

/*
#define MAKE_ALL				0
#define MAKE_1HANDED_SWORD		1
#define MAKE_2HANDED_SWORD		2
#define MAKE_CLUB				3
#define MAKE_AXE				4
#define MAKE_BOW				5
#define MAKE_SPEAR				6
#define MAKE_KNUCKLE			7
#define MAKE_STAFF				8
#define MAKE_ARMORTOP			9
#define MAKE_ARMORBOT			10
#define MAKE_ARMORFEET			11
#define MAKE_ROBE				12
#define MAKE_GARMENTTOP			13
#define MAKE_GARMENTBOT			14
#define MAKE_SHOES				15
#define MAKE_RING				16
#define MAKE_BRACELET			17
#define MAKE_NECKLACE			18
#define MAKE_EARING				19
#define MAKE_POTION				20
#define MAKE_FOOD				21
*/
#define TYPE_DISASSEMBLE 2
#define TYPE_SYNTHESIS 3
#define TYPE_SMELT 4
#define TYPE_PREPARE 5
#define TYPE_UPGRADE 8

class BBDX_Editor;
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

class CDialog_Ability : public CUIRes  
{
public:
	CDialog_Ability();
	virtual ~CDialog_Ability();

	void Open(PBYTE bypData);
	void ReceiveProduct(PBYTE bypData);

	void SendProduce();
    
	void SendProduceSynthesis();
		void SendProduceSynthesi();
	void SendProduceDisassemble();
	void SendProduceOther();
	void SendProduceOthe();
public:
	void CallBack_MessageBox(int nId, int nValue);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnRButtonDown(BBDX_MouseInformation *pMouseInfo);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	void SetWnd(CWnd *wnd) {m_pWnd = wnd;};
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void InitData();
	void SetScrollData();

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void RefreshData();
	void DeleteContents();

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);

	void SwapItems(CItemData* item_src, CItemData* item_dst, int amount, bool bAskHowMuch);
	BYTE GetSwapPlace(CItemData* item_src);

//	CUIScrollBar	*m_pScrollBar;
//	BBDX_TextOuter	*m_pOuter, *m_pItemOuter, *m_pNameOuter;
	CString			m_strItemOuter;
	CItemDataArray  m_arrItemMake;
	CItemData		m_vSelItemMake[5];
	int				m_vDragItem;
	CPoint			m_vDragOffset;
	int				m_vMakeSpec[6];
	int				m_vCurMakeSpec;
//	CTWFont			*m_pTWFont;
	int				m_vItemPic;
	CString			m_vItemName;
	CFont			*m_pFont;
	int				m_vTipIndex;
	int				m_vSelSpec;
	BYTE			m_vGender, m_vOldGender;

    CItemData		*m_vSrcItem, *m_vDstItem;
	BYTE			m_bAbilityType;
	BYTE			m_bDrawFocus;
	CString			m_strAbilityType;

	CItemData		*m_MakeItem;

	bool			m_bAwaitingProduct; // Stop produce being sent multiple times (Wait for server result)
};

#endif // !defined(AFX_DIALOG_MAKEITEM_H__99698444_6F6B_11D4_98B0_00C02615866B__INCLUDED_)
