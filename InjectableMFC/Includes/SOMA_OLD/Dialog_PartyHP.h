// Dialog_Message3.h: interface for the CDialog_Message3 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DIALOG_PARTYHP222
#define DIALOG_PARTYHP222

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "User.h"
#include "UIRes.h"
//class BBDX_TextOuter;
//class CTWFont;

struct PartyMemberInfo
{
	int uid;
	int x;
	int y;

	PartyMemberInfo()
	{
		uid = -1;
		x = -1;
		y = -1;
	}
};

class CDialog_PartyHP : public CUIRes  
{
public:
	CDialog_PartyHP();
	virtual ~CDialog_PartyHP();
	void RemoveAll();

public:
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest, int nCurHp, int nMaxHp, CPoint ptDrawPos);
	bool IsPartyMember(int iUid);
	bool AddPartyMember(int iUid);
	bool DelPartyMember(int iUid);
	bool m_bNew;
	void DeleteParty();
	void RcvReqParty(PBYTE bypData);
	void RcvPartyResult(PBYTE bypData);
	void CallBack_MessageBox(int nId, int nValue);
	void SendPartyAccept();
	void SendPartyRefuse();
	void SendPartyDeny();

	void ShowWindow();
	void HideWindow();

	CArray<PartyMemberInfo ,PartyMemberInfo> m_arrPartyMembers;
	int				m_iMemCount;
	BOOL			m_bParty;
	CString			m_strPartyLdr;
};

#endif // !defined(AFX_DIALOG_MESSAGE3_H__721EE1A1_7781_11D4_98B0_00C02615866B__INCLUDED_)
