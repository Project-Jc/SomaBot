// Dialog_RepairTrade.h: interface for the CDialog_RepairTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_REPAIRTRADE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_REPAIRTRADE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_

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

//8 - Repair button , 7 - Trade Button, 6 - Recv Repair Item,  5 - RcvTrade Item, 9 - Repair mode change, 4 - Money, 3 - Open Repair, 1 - Repair req, 

class CDialog_RepairTrade : public CUIRes  
{
public:
	CDialog_RepairTrade();
	virtual ~CDialog_RepairTrade();

	void RcvTradeOk(PBYTE bypData);
	void RcvTradeCancel();
	void RcvTradeResult(PBYTE bypData);
	void RcvTradeMoney(PBYTE bypData);
	void RcvTradeItem(PBYTE bypData);
	void RcvRepairItem(PBYTE bypData);
	CItemData* RcvItem(PBYTE bypData);

	void RcvAckTrade(PBYTE bypData);
	void RcvReqTrade(PBYTE bypData);
	void CallBack_MessageBox(int nId, int nValue);
	void SendTradeOther();
	void SendTradeCancel();
	void SendTradeAck(BYTE result);
	void SendTradeAddItem(short item_number, short count = 1, BYTE bType = 5);
	void SendTradeSetMoney(DWORD money);
	void SendTradeOk();
public:
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void RefreshData();

	void SetScrollMeData();
	void SetScrollOtherData();

	void AskHowMuch();
	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void DeleteContents();
	void InitData();

	void RcvRepairTradeReq(PBYTE bypData);
	void RcvRepairOpen(PBYTE bypData);
	void SendChangeRepair(BYTE mode);
	void SendRepair();
	void SendTrade();
	void RcvTradeButton();
	void RcvRepairButton(PBYTE bypData);
	void RcvChangeRepair(PBYTE bypData);
	void ConfirmItemAdd();

	CUIScrollBar	*m_pOtherExgScrollBar, *m_pMeScrollBar;
	CFont			*m_pFont;
	CString			m_strOtherExgMoney;
	CItemDataArray	m_arrItemOtherExg, m_arrItemMe;
	int				m_vCurMeItem;
	int				m_vDragItem;
	CPoint			m_vDragOffset;
	BOOL			m_bTrading;
	BOOL			m_bTradeOk;
	BOOL			m_bRepairOk;
	int				m_vSelSpec;
	int				m_vTipIndex;
	BOOL			m_bOtherExgCanScroll;
	BOOL			m_bMeCanScroll;

	CString			m_strOtherUser;
	int				m_iReqUid;
	int				m_iRepairUid;
	bool			m_bRepairing;
	short			m_nDurAfterNormalRep;
	short			m_nDurAfterSpecialRep;
	BYTE			m_bNormalRepairChance;
	CItemData*		m_pRepairItem;

	BOOL			m_bProcessing;
	int				m_iTradeCount;
	bool			m_bTradeRepair;
};

#endif // !defined(AFX_DIALOG_EXCHANGE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
