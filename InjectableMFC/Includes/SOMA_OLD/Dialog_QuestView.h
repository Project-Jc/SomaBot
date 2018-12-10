// Dialog_QuestView.h: interface for the CDialog_QuestView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_QUEST_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_QUEST_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"

class CUIScrollBar;
class BBDX_Editor;

struct QUEST
{
	short questId;
	BYTE inProgress;
	CString questTitle;
	CString questSrc;
};

class CDialog_QuestView : public CUIRes  
{
public:
	CDialog_QuestView();
	virtual ~CDialog_QuestView();

public:
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);

	void ShowWindow();
	void HideWindow();
	void HideAllWindow();
	void SetCurrentScrollData();
	void SetNewScrollData();
	void SetDescScrollData();

	void SendBriefDataReq();
	void SendDescReq(short questId);
	void ReadEvent(PBYTE event);

	void InitWnd(CWnd *pWnd);

	CFont *m_pFont;
	CWnd *m_pWnd;
	PBYTE m_eventBrief;
	CUIScrollBar *m_pCurrentScrollBar, *m_pNewScrollBar, *m_pDescScrollBar;
	CArray<QUEST,QUEST> m_ArrQuest, m_ArrProgQuest;
	BOOL m_bCurrentCanScroll, m_bNewCanScroll, m_bDescCanScroll, m_bDrawDesc;
	CString m_tQDesc;
	BBDX_Editor *m_pChatOuter;
	int m_vReadLineCount;
	int m_vLineGap;
	
};

#endif // !defined(AFX_DIALOG_QUEST_H__7408B4E1_96BD_11D4_98B2_00C02615866B__INCLUDED_)
