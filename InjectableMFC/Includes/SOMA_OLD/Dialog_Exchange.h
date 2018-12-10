// Dialog_Exchange.h: interface for the CDialog_Exchange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_EXCHANGE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_EXCHANGE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_

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

class CDialog_Exchange : public CUIRes  
{
public:
	CDialog_Exchange();
	virtual ~CDialog_Exchange();

	void RcvTradeOk();
	void RcvTradeCancel();
	void RcvTradeResult(PBYTE bypData);
	void RcvTradeMoney(PBYTE bypData);
	void RcvTradeItem(PBYTE bypData);
	void RcvAckTrade(PBYTE bypData);
	void RcvReqTrade(PBYTE bypData);
	void CallBack_MessageBox(int nId, int nValue);
	void SendTradeOther();
	void SendTradeCancel();
	void SendTradeAck(BYTE result);
	void SendTradeAddItem(short item_number, short count = 1);
	void SendTradeSetMoney(DWORD money);
	void SendTradeOk();
public:
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);
	CString MakeCaption(CDDSurface *pDest, CItemData *item_detail);

	void SetScrollMeData();
	void SetScrollOtherData();

	void AskHowMuch();
	void AskTrade(CString other_user, int UId);
	void WaitTrade();
	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void RefreshData();
	void DeleteContents();
	void InitData();
	void ConfirmItemAdd();

	CUIScrollBar	*m_pMeExgScrollBar, *m_pOtherExgScrollBar, *m_pMeScrollBar;
//	BBDX_TextOuter	*m_pMeExgMoney, *m_pOtherExgMoney, *m_pMeMoney;
//	CTWFont			*m_pTWFont;
	CFont			*m_pFont;
	CString			m_strMeExgMoney, m_strOtherExgMoney;
	CItemDataArray	m_arrItemMe;
	CItemDataArray	m_arrItemMeExg;
	CItemDataArray	m_arrItemOtherExg;
	int				m_vCurMeItem;
	int				m_vDragItem;
	CPoint			m_vDragOffset;
	CString			m_vOtherUser;
	int				m_vUId;
	BOOL			m_bTrading;
	BOOL			m_bOtherOk;
	BOOL			m_bMeOk;
	int				m_vSelSpec;
	int				m_vTipIndex;
	CString			m_strCaption;
	BOOL			m_bOtherExgCanScroll;
	BOOL			m_bMeExgCanScroll;
	BOOL			m_bMeCanScroll;

	BOOL			m_bProcessing;
	int				m_iTradeCount;
};

#endif // !defined(AFX_DIALOG_EXCHANGE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
