// Dialog_AuctionSell.h: interface for the CDialog_AuctionSell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_AUCTIONSELL_H__FE821CE2_74F0_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_AUCTIONSELL_H__FE821CE2_74F0_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "DarkDef.h"
#include "ItemData.h"
class CUIScrollBar;
class BBDX_MouseInformation;
//class BBDX_TextOuter;
//class CTWFont;

class CDialog_AuctionSell : public CUIRes  
{
public:
	CDialog_AuctionSell();
	virtual ~CDialog_AuctionSell();

	void AuctionOpen(PBYTE bypData);
	void AskAuctionMoney();
	void SendAuctionItem(DWORD money = 0);
	void SendGetAuctionItem();
	void RcvAuctionItem(PBYTE bypData);
	void RcvGetAuctionItem(PBYTE bypData);
public:
	void ReCalculateButtons();
	void ChangeToButtonEx(int btn_index, int area_index);
	void RemoveRectManager(int vSpec, int vIndex);

	void GuildItemRegResult(PBYTE bypData);

	void CallBack_MessageBox(int nId, int nValue);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void SetScrollData();
	void SetAuctionData(CItemData *auction_item);
	void RefreshData();
	void InitData();
	void DeleteContents();
	void ShowWindow(int iViewMode = 1);
	void HideWindow();
	void HideAllWindow();

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);
	void ViewCaption(CDDSurface *pDest, CPoint pos, CString str);

//	CUIScrollBar		*m_pScrollBar;
//	BBDX_TextOuter		*m_pCostOuter, *m_pMoneyOuter;
//	CTWFont				*m_pTWFont;
	CString				m_strCost;
	CItemDataArray		m_arrItem;
	CItemData		*m_pAuctionItem;
	int					m_vCurItem;
	short				m_vAuctionNumber;
	int					m_vDelItem;
	CFont				*m_pFont;
	int					m_vTipIndex;
	int					m_vRegisterItem;
	int					m_vHowMany;
	int					m_vHowMuch;
	BOOL				m_bItemProcess;
	BYTE				m_bViewMode; // 1 = Auction, 2 = Guild Store
};

#endif // !defined(AFX_DIALOG_AUCTIONSELL_H__FE821CE2_74F0_11D4_98B0_00C02615866B__INCLUDED_)
