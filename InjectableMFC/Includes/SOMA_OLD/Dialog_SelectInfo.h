// Dialog_Message3.h: interface for the CDialog_Message3 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DIALOG_SELECTINFO222
#define DIALOG_SELECTINFO222

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "User.h"
#include "UIRes.h"
//class BBDX_TextOuter;
//class CTWFont;

class CDialog_SelectInfo : public CUIRes  
{
public:
	CDialog_SelectInfo();
	virtual ~CDialog_SelectInfo();

public:
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);

	void ShowWindow();
	void HideWindow();
	void SetUser(int uid){m_SelUid = uid;}
	int GetCurSelUserId(){return m_SelUid;}

//	BBDX_TextOuter	*m_pOuter;
//	CTWFont			*m_pTWFont;
	int				m_SelUid;
	CFont			*m_pFont;
	bool			m_bDontShow;
};

#endif // !defined(AFX_DIALOG_MESSAGE3_H__721EE1A1_7781_11D4_98B0_00C02615866B__INCLUDED_)
