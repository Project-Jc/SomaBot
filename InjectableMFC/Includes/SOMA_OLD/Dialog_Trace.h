// Dialog_Trace.h: interface for the CDialog_Trace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_Trace_H__152D4C24_947D_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_Trace_H__152D4C24_947D_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
class BBDX_Editor;

#define TRACE_HUMAN 0
#define TRACE_MONSTER 1

class CDialog_Trace : public CUIRes  
{
public:
	CDialog_Trace();
	virtual ~CDialog_Trace();

	double calcAngle(float x1, float y1, float x2, float y2);
    void DrawTrace(CPoint srcPos, CPoint charPos, bool bText = false);
	void SetTrace(int x, int y, int iUseSkillID, DWORD dwConTime, DWORD	dwStartTime);
public:
	void SetWnd(CWnd *wnd) {m_pWnd = wnd;};
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void ShowWindow(int iTarget, short nSkill, short nDir);
	void HideWindow();
	void HideAllWindow();

	CString		m_vName;
	BOOL		m_bProcessing;
	int			m_iTargetID;
	short		m_nSkillID;
	short		m_nDir;
	int			m_iTraceX;
	int			m_iTraceY;
	DWORD		m_dwConTime;
	DWORD		m_dwStartTime;
	int			m_iUseSkillID;
};

#endif // !defined(AFX_DIALOG_Trace_H__152D4C24_947D_11D4_98B2_00C02615866B__INCLUDED_)
