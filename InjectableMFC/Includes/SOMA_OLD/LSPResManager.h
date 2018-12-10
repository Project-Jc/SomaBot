// CResManager.h: interface for the CResManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESMANAGER_H__B64F5296_4B47_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESMANAGER_H__B64F5296_4B47_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LSPResArmature.h"
#include "LSPResChar.h"
#include "LSPResMagic.h"
#include "LSPResN.h"

#include "LSPResBlood.h"
#include "LSPResArrow.h"

#include "LSPResInv.h"

/*
class CLSPRes;

class CLSPResArmature;
class CLSPResArmatureEx;
class CLSPResArmatureStatus;*/


class CDDSurface;

#define MAX_ACTION_MANAGE_NUM	0xFF
#define MAX_GENE 256

UINT ThreadLoadRes(LPVOID lParam);

class CResManager  
{
	friend UINT ThreadLoadRes(LPVOID lParma);
public:
	void RemoveAllData();
	void ReleaseUnusedSpr();
	
	int	GetPixelFormat() {return m_PixelFormat;}
	void SetPixelFormat(CDDSurface*	pSurface);
	void SetPixelFormat(int pf) {m_PixelFormat = pf;}

	CLSPResArmature* GetArmature(int nClass, int nGender, int nAniIndex);
	CLSPResChar* GetChar(int nIndex);
	CLSPResMagic* GetMagic(int nIndex);
	CLSPResMagic* GetSpecial(int nIndex);
	CLSPResBlood* GetBlood(int nIndex);
	CLSPResArrow* GetArrow(int nIndex); 
	CLSPResMagic* GetTownPortal(int nIndex);
	CLSPResInv* GetInv(int nType, int nIndex, bool bDevil);

	CLSPResN* GetNRes(){return &m_ResN;}

	CResManager();
	virtual ~CResManager();
protected:
	CLSPResArmature* LoadArmatureRes(int nClass, int nGender, int nAniIndex);
	CLSPResChar* LoadCharRes(int nIndex);
	CLSPResMagic* LoadMagicRes(int nIndex);
	CLSPResMagic* LoadSpecialRes(int nIndex);
	CLSPResBlood* LoadBloodRes(int nIndex);
	CLSPResArrow* LoadArrowRes(int nIndex);
	CLSPResMagic* LoadTownPortalRes(int nIndex);
	CLSPResInv* LoadInvRes(int nType, int nIndex, bool bDevil);

	CMap<int, int, CLSPResArmature*, CLSPResArmature*> m_mapArmature;
	CMap<int, int, CLSPResChar*, CLSPResChar*> m_mapChar;
	CMap<int, int, CLSPResMagic*, CLSPResMagic*> m_mapMagic;
	CMap<int, int, CLSPResMagic*, CLSPResMagic*> m_mapSpecial;
	CMap<int, int, CLSPResBlood*, CLSPResBlood*> m_mapBlood;
	CMap<int, int, CLSPResArrow*, CLSPResArrow*> m_mapArrow;
	CMap<int, int, CLSPResMagic*, CLSPResMagic*> m_mapTownPortal;
	CMap<int, int, CLSPResInv*,	CLSPResInv*> m_mapInv;

	CLSPResN m_ResN;

	int					m_PixelFormat;
};

#endif // !defined(AFX_LSPRESMANAGER_H__B64F5296_4B47_11D4_946F_0090272340A6__INCLUDED_)
