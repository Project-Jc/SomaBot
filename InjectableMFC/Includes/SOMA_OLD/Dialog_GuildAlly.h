// Dialog_GuildAlly.h: interface for the CDialog_GuildAlly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_GuildAlly_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_GuildAlly_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
//class BBDX_TextOuter;
class BBDX_MouseInformation;
//class CTWFont;

#define MAX_ALLIED_GUILDS 3

typedef struct GUILD_ALLY
{
	CString strName;
	short nGuildNum;
}GUILD_ALLY;

class CDialog_GuildAlly : public CUIRes  
{
public:
	CDialog_GuildAlly();
	virtual ~CDialog_GuildAlly();

	void RcvGuildAllyList(PBYTE bypData);
	void DialogOpen(PBYTE bypData, int iViewMode = 1);
	void SwitchViewMode(int iViewMode);
	void SendAllyGuild();
	void SendRemoveAlly();
	void RcvAllyGuild(PBYTE bypData);
	void RcvAllyGuildResult(PBYTE bypData);
	void RcvRemoveAllyResult(PBYTE bypData);
	void CallBack_MessageBox(int nId, int nValue);
	void SendAllyResult(int result);
	void AddGuild(CString guild_name, int nGuildNum);
	void ChangeGuildAllied(int nGuildNum, bool bAllied);
	void RefreshAllyList(int iList[]);
public:
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

	int m_vSelIndex, m_vProcessIndex;
	CFont			*m_pFont;

	CArray<GUILD_ALLY,GUILD_ALLY> m_arrGuild;
	CArray<GUILD_ALLY,GUILD_ALLY> m_arrAlliedGuilds;
	BOOL	m_bProcessing;
	int		m_vId;
	int		m_iViewMode; // 1 = Application List, 2 = Guild Ally, 3 = Current Allies
	short	m_nGuildNum;
	short	m_nGuildReqNum;
};

#endif // !defined(AFX_DIALOG_GuildAlly_H__152D4C21_947D_11D4_98B2_00C02615866B__INCLUDED_)
