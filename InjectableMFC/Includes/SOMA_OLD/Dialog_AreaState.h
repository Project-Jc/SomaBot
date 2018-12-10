// Dialog_Crest.h: interface for the CDialog_Crest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_AREA_STATE_H__9349F7C1_94A5_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_AREA_STATE_H__9349F7C1_94A5_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"

class CDialog_AreaState : public CUIRes  
{
public:
	CDialog_AreaState();
	virtual ~CDialog_AreaState();

public:
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect, CRect standard_rect);
	void ViewCaption(CDDSurface *pDest, CRect standard_rect, CString str);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);

private:
	CFont		*m_pFont;
	int			m_nViewCaption;
	CString		m_vStrCaption;
};

#endif // !defined(AFX_DIALOG_AREA_STATE_H__9349F7C1_94A5_11D4_98B2_00C02615866B__INCLUDED_)
