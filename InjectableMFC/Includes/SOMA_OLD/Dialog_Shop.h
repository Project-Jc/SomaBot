// Dialog_Shop.h: interface for the CDialog_Shop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_SHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_SHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
#include "DarkDef.h"
#include "ItemData.h"
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

class CDialog_Shop : public CUIRes  
{
public:
	CDialog_Shop();
	virtual ~CDialog_Shop();

public:
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);

	void InitData();
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	void CallBack_MessageBox(int nId, int nValue);

	void SetScrollData();
	void ShopSell(int how_many, int index);
	void ShopSendSell(int how_many);
	void ShopSendBuy(int how_many);
	void ShopBuy(int how_many, int Index);
	void purplepots(int how_many);
	void hppots(int how_many);
	void hhppots(int how_many);
	void mppots(int how_many);
	void leather(int how_many);
	void linen(int how_many);
	void copper(int how_many);
	void wheat(int how_many);
	void blue(int how_many);
	void liver(int how_many);
	void horn(int how_many);
	void wood(int how_many);
	void Shoesell12(int how_many);
	void Shoesell13(int how_many);
	void Shoesell14(int how_many);
	void Shoesell15(int how_many);
	void Shoesellnew(int how_many);
	void Shoesell(int how_many);


	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void RefreshData();
	void ShopOpen(PBYTE bypData);
	void DeleteContents();

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);

	void UpdateItemAmount(PBYTE bypData);

	CUIScrollBar *m_pHandScrollBar, *m_pShopScrollBar;
//	BBDX_TextOuter *m_pMoneyOuter, *m_pCostOuter;
	short m_vShopMode;
	CItemDataArray	m_arrAuctionItem;
	CItemDataArray m_arrItemMe;
	CItemDataArray m_arrItemShop;
	short m_vShopNumber;
	int m_vCurMeItem, m_vCurShopItem;
	int					m_vCurItem;
//	CTWFont *m_pTWFont;
	CFont			*m_pFont;
	int				m_vTipIndex;
	DWORD			m_vSellRate;
	DWORD			m_vBuyRate;
	BOOL			m_bProcessing;
	BOOL			m_bHandCanScroll;
	BOOL			m_bShopCanScroll;
};

#endif // !defined(AFX_DIALOG_SHOP_H__66BD2D01_6924_11D4_98B0_00C02615866B__INCLUDED_)
