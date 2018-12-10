// Dialog_GuildJoin.h: interface for the CDialog_GuildJoin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_GUILDJOIN_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_GUILDJOIN_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

class CDialog_GuildJoin : public CUIRes  
{
public:
	CDialog_GuildJoin();
	virtual ~CDialog_GuildJoin();
	void RcvApplicationList(PBYTE bypData);
	void DialogOpen(PBYTE bypData, int iViewMode = 1);
	void SwitchViewMode(int iViewMode);
	void SendAskUserInfo();
	void RcvAskUserInfo(PBYTE bypData);
	void SendJoinPerson();
	void SendRejectPerson();
	void RcvJoinPerson(PBYTE bypData);
	void RcvRejectPerson(PBYTE bypData);
	void AddPerson(CString m_name);
public:
	BOOL CheckChangeButtonState(MAreaCtrl* area, MBtnCtrl* btn_ctrl, CPoint pt, int state);
	BOOL CheckAreaButtons(int state, BBDX_MouseInformation *pMouseInfo);
	void ReCalculateButtons();
	void ChangeToButtonEx(int btn_index, int area_index);
	void RemoveRectManager(int vSpec, int vIndex);

	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);

	void ShowWindow(int Id, CUIRes *pParent);
	void HideWindow();
	void HideAllWindow();
	void DeleteContents();
	void SetScrollData();
	void DrawTipRect(CDDSurface *pDest, CRect& t_rect);
	void ViewCaption(CDDSurface *pDest, CPoint pos, CString str);

	int m_vSelIndex, m_vProcessIndex;
	CFont			*m_pFont;
	CStringArray m_arrPerson;
	CStringArray m_arrInfo;
	CByteArray m_arrInfoFlag;
	BOOL	m_bProcessing;
	int		m_vId;
	int		m_iViewMode; // 1 = Application List, 2 = Guild Ally, 3 = Current Allies
};

#endif // !defined(AFX_DIALOG_GUILDJOIN_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_)
