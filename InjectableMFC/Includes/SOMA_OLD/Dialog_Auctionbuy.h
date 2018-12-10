// Dialog_AuctionBuy.h: interface for the CDialog_AuctionBuy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_AUCTIONBUY_H__FE821CE1_74F0_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_AUCTIONBUY_H__FE821CE1_74F0_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "DarkDef.h"
#include "ItemData.h"
class BBDX_MouseInformation;
class CUIScrollBar;
//class BBDX_TextOuter;
//class CTWFont;

class CDialog_AuctionBuy : public CUIRes  
{
public:
	CDialog_AuctionBuy();
	virtual ~CDialog_AuctionBuy();

	void RcvAuction(PBYTE bypData);
	void RcvGuildStoreResult(PBYTE bypData);
	void RcvOpen(PBYTE bypData, int iViewMode = 1);
	void RcvOpenAuction(PBYTE bypData);
	void RcvOpenGuildStore(PBYTE bypData);
	void RcvOpenExchange(PBYTE bypData);
	void SendAuction(DWORD money);
	void SendGuildStoreBuy(int number);
	void SendTraderBuy(int number);
public:
	void CallBack_MessageBox(int nId, int nValue);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	
    void UpdateItem(int item_num, int count);
	void SetScrollData();
	void InitData();
	void DeleteContents();
	void ShowWindow(int iViewMode = 1);
	void HideWindow();
	void HideAllWindow();

//	CUIScrollBar		*m_pScrollBar;
//	BBDX_TextOuter		*m_pNameOuter[10], *m_pMoneyOuter;
//	CTWFont				*m_pTWFont;
	CFont				*m_pFont;
	CString				*m_pExplain;
	CItemDataArray		m_arrAuctionItem;
	short				m_vAuctionNumber;				// 경매소 번호
	int					m_vCurItem;
	BYTE				m_bViewMode; // 1 = Auction, 2 = Guild Store, 3 = Exchange
};

#endif // !defined(AFX_DIALOG_AUCTIONBUY_H__FE821CE1_74F0_11D4_98B0_00C02615866B__INCLUDED_)
