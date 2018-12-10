// Dialog_HelpList.h: interface for the CDialog_HelpList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_HELPLIST_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_)
#define AFX_DIALOG_HELPList_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "Dialog_HelpRead.h"

class LIST_DATA;
class BBDX_MouseInformation;
class CUIRes;
class CDirectDraw;
class CChildView;
class CDDSurface;

class CDialog_HelpList : public CUIRes  
{
public:
	CDialog_HelpList(CWnd *pWnd = NULL);
	virtual ~CDialog_HelpList();

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
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	void InitData(CDirectDraw *pDD, CChildView *pWnd);
	
	CFont			*m_pFont;
	CString			m_ListString[10];
	
	CDialog_HelpRead			*m_pDialog_HelpRead;

	void ListNext();	// next button
	void ListBack();	// back button
	void LoadStrings();	// load help file
	void Read();		// read a specific index
	void StopReading(); // return to main window
	void Reset();

	int m_nSelRow;		//selected index
	int m_nStartIndex;	//page index
	int	m_nVListCount;	//total index's
	bool m_bLoading;	//stops next/prev button from working while loading
	bool m_bReading;	//tells us if we're reading a topic

};

#endif // !defined(AFX_DIALOG_HELPList_H__6566C203_6153_11D4_A1F5_00D0B769A081__INCLUDED_)
