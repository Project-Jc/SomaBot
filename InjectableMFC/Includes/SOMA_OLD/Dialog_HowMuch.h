// Dialog_HowMuch.h: interface for the CDialog_HowMuch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_HOWMUCH_H__B2B3A2A1_7790_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_HOWMUCH_H__B2B3A2A1_7790_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
class BBDX_Editor;

// Area Defines

#define TEXT_AREA 0
#define HOW_MUCH_IMAGE 1
#define IMAGE_PLACEMENT 2
#define TRACE_IMAGE 3
#define RECALL_IMAGE 4

class CDialog_HowMuch : public CUIRes  
{
public:
	CDialog_HowMuch();
	virtual ~CDialog_HowMuch();

	void SetWnd(CWnd *wnd) {m_pWnd = wnd;};
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

//	void ShowWindow(int Id, CUIRes *pParent);
	void ShowWindow(int Id, int Quantity, CUIRes *pParent, BYTE bEditMode = 1);
	void HideWindow();
	void HideAllWindow();

	int			m_vId;
	DWORD		m_vValue;
	CString		m_strName;
	BYTE		m_bEditMode; // 1 = How Much(default), 3 = trace, 4 = Recall
};

#endif // !defined(AFX_DIALOG_HOWMUCH_H__B2B3A2A1_7790_11D4_98B0_00C02615866B__INCLUDED_)
