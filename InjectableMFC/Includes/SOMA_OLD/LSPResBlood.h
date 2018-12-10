// LSPCharRes.h: interface for the CLSPResBlood class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESBLOOD_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESBLOOD_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ObjAni.h"
#include "ObjLSP.h"

class CLSPResBlood //: public CObjLSP
{
public:
	void RemoveAllData();

	BOOL Draw(CDDSurface* pSurface,CPoint ptPos,int nCurFrame, BOOL bNext);
	BOOL LoadRes(int nBloodKind);
	BOOL LoadRes(CString szFileName);

	BOOL	ReleaseUnusedSpr();

	CLSPResBlood();
	virtual ~CLSPResBlood();
//protected:
public:
	CObjLSP m_ResBlood;

//	CObjAni				m_objAni[MAX_RES_COUNT];
private:
	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
