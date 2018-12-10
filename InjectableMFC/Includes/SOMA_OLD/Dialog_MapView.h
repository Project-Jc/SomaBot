// Dialog_MapView.h: interface for the CDialog_MapView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_MAPVIEW_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_MAPVIEW_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"

class CDialog_MapView : public CUIRes  
{
public:
	CDialog_MapView();
	virtual ~CDialog_MapView();

public:
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();

	LPDIRECTDRAWSURFACE7 m_pPointSurface;
	CDDSurface		*m_pDDPointSurface;
	LPDIRECTDRAWSURFACE7 m_pPartyPointSurface;
	CDDSurface		*m_pDDPartyPointSurface;
};

#endif // !defined(AFX_DIALOG_QUIT_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_)
