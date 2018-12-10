// Dialog_GuildStorage.h: interface for the CDialog_GuildStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_GuildStorage_H__128BEAE4_6DFD_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_GuildStorage_H__128BEAE4_6DFD_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
#include "DarkDef.h"
#include "ItemData.h"
class BBDX_Editor;
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

class CDialog_GuildStorage : public CUIRes  
{
public:
	CDialog_GuildStorage();
	virtual ~CDialog_GuildStorage();

public:
	void CallBack_MessageBox(int nId, int nValue);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void InitData();
	void SetWnd(CWnd *wnd) {m_pWnd = wnd;};
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	
	void SetScrollData();

	void SendToGuildStorage(short count = 1);
	void GetFromGuildStorage(short count = 1);

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void RefreshData();
	void DeleteContents();

	void GuildStorageOpen(PBYTE bypData);
	void SaveItemToGuildStorage(PBYTE bypData);
	void TakebackItemFromGuildStorage(PBYTE bypData);

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);
	void LimitChange(PBYTE bypData);

	void Send_CloseStorage();

	CUIScrollBar		*m_pHandScrollBar, *m_pGuildStorageScrollBar;
//	BBDX_TextOuter		*m_pOuter;
	CItemDataArray		m_arrItemMe;
	CItemDataArray		m_arrItemGuildStorage;
	int					m_vCurMeItem, m_vCurGuildStorageItem;
//	CTWFont				*m_pTWFont;
	CFont				*m_pFont;
	int					m_vTipIndex;
	BOOL				m_bHandCanScroll;
	BOOL				m_bGuildStorageCanScroll;
	BOOL				m_bLimit;
};

#endif // !defined(AFX_DIALOG_GuildStorage_H__128BEAE4_6DFD_11D4_98B0_00C02615866B__INCLUDED_)
