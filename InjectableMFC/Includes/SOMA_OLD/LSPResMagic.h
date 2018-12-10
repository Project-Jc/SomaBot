// LSPCharRes.h: interface for the CLSPResMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESMAGIC_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESMAGIC_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAniLsp.h"
//#include "ObjAni.h"
//#include "ObjLSP.h"
const int MAGIC_SPEED = 1200;

class CLSPResMagic //: public CObjLSP
{
public:
	void RemoveAllData();
	
	void	DrawMagic(CDDSurface *pSurface, int x, int y, int nFrame, int Alpha = 1);
	virtual BOOL LoadMagic(CString &path);
	int		GetMagicDirFrame()
	{
		return m_ResMagic.AniDataGet(0)->nMaxFrm;
	}
	int		GetMagicFrameNum()
	{
		return m_ResMagic.m_HSpr.nQt;
	}
	int		GetMagicSpeed()
	{
		return MAGIC_SPEED/m_ResMagic.AniDataGet(0)->fSpd;
	}
	int		GetTotalSpellLength()
	{
		return GetMagicDirFrame()*GetMagicSpeed();
	}
	void	ChangeFrame(int &iSpriteIndex);
	int		GetFrame(int &iSpriteIndex, int iDir = 0);
	BOOL	ReleaseUnusedSpr();

	CLSPResMagic();
	virtual ~CLSPResMagic();
//protected:
//public:
private:
	CObjAniLSP m_ResMagic;

//	CObjAni				m_objAni[MAX_RES_COUNT];
//public:
	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
