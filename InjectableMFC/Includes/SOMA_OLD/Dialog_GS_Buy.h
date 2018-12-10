// Dialog_GS_Buy.h: interface for the CDialog_GS_Buy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_GS_Buy_H__9FCA6741_78DB_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_GS_Buy_H__9FCA6741_78DB_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "DarkDef.h"
#include "ItemData.h"
class BBDX_MouseInformation;
//class BBDX_TextOuter;
//class CTWFont;

class CDialog_GS_Buy : public CUIRes  
{
public:
	CDialog_GS_Buy();
	virtual ~CDialog_GS_Buy();

public:
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void CallBack_MessageBox(int nId, int nValue);
	void ShowWindow(int Id, CUIRes *pParent, CItemData *item_guild_store, int iViewMode = 1);
	void HideWindow();
	void HideAllWindow();

//	BBDX_TextOuter		*m_pDetailOuter, *m_pUserOuter, *m_pCostOuter;
//	CTWFont				*m_pTWFont;
	CString				m_strDetail, m_strUser, m_strCost;
	CFont				*m_pFont;
	CItemData			*m_pGuildStoreItem;
	CString				m_vRegistrant;
	int					m_vGuildStoreCount;
	int					m_vId;
	DWORD				m_vValue;
	BYTE				m_bViewMode; // 1 = GS BUY, 2 = Trader Exchange
};

#endif // !defined(AFX_DIALOG_AUCTIONDETAIL_H__9FCA6741_78DB_11D4_98B0_00C02615866B__INCLUDED_)
