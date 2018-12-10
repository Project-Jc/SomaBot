// Dialog_HairShop.h: interface for the CDialog_HairShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_HAIRSHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_HAIRSHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
#include "DarkDef.h"
#include "ItemData.h"
#include "Item_Inv.h"
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

class CDialog_HairShop : public CUIRes  
{
public:
	CDialog_HairShop();
	virtual ~CDialog_HairShop();

public:
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void SendNewHair();
	void UpdateHair();
	void CalcHairCost();

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void ShopOpen(PBYTE bypData, CItem_Inv* item_inv);

	CFont			*m_pFont;
	int				m_vMarketPrice;
	int				m_vChangeHairPrice;
	int				m_vHairModePrice;
	BYTE			m_bHairStyle;
	BYTE			m_bHairColor;
	BYTE			m_bHairMode;
	BOOL			m_bProcessing;
	CItem_Inv*		m_pHairChar;
	int				m_vHairCost;
	int				m_iPrevHair;
};

#endif // !defined(AFX_DIALOG_SHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_)
