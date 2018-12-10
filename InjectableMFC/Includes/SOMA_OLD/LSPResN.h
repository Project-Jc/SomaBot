// LSPCharRes.h: interface for the CLSPResN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPResN_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPResN_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjLsp.h"
//#include "ObjAni.h"
//#include "ObjLSP.h"

struct NRES
{
	CString				m_szFileName;
	CObjLSP*			m_nRes;
};

typedef CArray<NRES,NRES> NRES_ARRAY;

#define DRAW_TYPE_NORMAL 1
#define DRAW_TYPE_ALPHA 2

class CLSPResN //: public CObjLSP
{
public:
	BOOL ReleaseUnusedSpr();
	void RemoveAllData();
	
public:
	CLSPResN();
	virtual ~CLSPResN();
public:
	BOOL Draw(CDDSurface *pDest, CPoint ptPos, char *szName, int iFrame, int iDrawType = DRAW_TYPE_NORMAL);
	BOOL Load(char *szFileName);
	void BltLSPNormal(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data);
private:
    NRES_ARRAY m_arrNRes;

	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
