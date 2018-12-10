// Dialog_HelpList.h: interface for the CDialog_HelpList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_HELPRead_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_)
#define AFX_DIALOG_HELPRead_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"

class LIST_DATA;
class BBDX_MouseInformation;
class CUIRes;
class CDirectDraw;
class CChildView;
class CDDSurface;
class BBDX_ChatOuter;

class CDialog_HelpRead : public CUIRes  
{
public:
	CDialog_HelpRead(CWnd *pWnd = NULL);
	virtual ~CDialog_HelpRead();

	void	ShowWindow();
	void	HideWindow();
	void	HideAllWindow();
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnLButtonDblClk(BBDX_MouseInformation *pMouseInfo);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);
	BOOL OnButtonAction(int nSpec, int nIndex);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	void InitData(CDirectDraw *pDD, CChildView *pWnd);
	
	CFont			*m_pFont;
	BBDX_ChatOuter *m_pChatOuter;
	
	void LoadStrings();	// load help file
	void Set(int ntopic); //what topic number do we have atm?
	void SetScrollData();

	int m_nStartIndex;	//page index
	int	m_nVListCount;	//total index's
	int m_nTopic;		//current topic
	bool m_bLoading;		//stops next/prev button from working while loading
	CString Topic;		//topic to display in header


};

#endif // !defined(AFX_DIALOG_HELPRead_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_)
