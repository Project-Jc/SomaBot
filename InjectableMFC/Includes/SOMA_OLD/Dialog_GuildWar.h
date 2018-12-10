// Dialog_GuildWar.h: interface for the CDialog_GuildWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_GUILDWAR_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_GUILDWAR_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_

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

class CDialog_GuildWar : public CUIRes  
{
public:
	CDialog_GuildWar();
	virtual ~CDialog_GuildWar();

	void RcvTradeOk(PBYTE bypData);
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

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void SetScrollMeData();
	void SetScrollOtherData();

	void AskHowMuch();
	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void DeleteContents();
	void InitData();

	void RcvGuildWarReq(PBYTE bypData);
	void RcvGuildWarExgOpen(PBYTE bypData);
	void ConfirmItemAdd();

	CUIScrollBar	*m_pMeExgScrollBar, *m_pOtherExgScrollBar, *m_pMeScrollBar;
//	BBDX_TextOuter	*m_pMeExgMoney, *m_pOtherExgMoney, *m_pMeMoney;
//	CTWFont			*m_pTWFont;
	CFont			*m_pFont;
	CString			m_strMeExgMoney, m_strOtherExgMoney;
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

    DWORD			m_vBarr;
	CItemDataArray	m_arrItemGuildStorage;	
	CItemDataArray	m_arrItemGuildStorageExg;	
	CString			m_strMyGuild; // Guild who sent the request
	CString			m_strOtherGuild; // Guild Recieving the request

	BOOL			m_bProcessing;
	int				m_iTradeCount;
};

#endif // !defined(AFX_DIALOG_EXCHANGE_H__D6602F24_774A_11D4_98B0_00C02615866B__INCLUDED_)
