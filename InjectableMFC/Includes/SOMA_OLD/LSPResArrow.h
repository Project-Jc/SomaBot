// LSPCharRes.h: interface for the CLSPResArrow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESARROW_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESARROW_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAniLSP.h"
//#include "ObjAni.h"
//#include "ObjLSP.h"

class CLSPResArrow //: public CObjLSP
{
public:
	void RemoveAllData();

	BOOL Draw(CDDSurface* pSurface,CPoint ptPos,int nCurFrame);
	BOOL LoadRes(int nArrowKind);

	BOOL	ReleaseUnusedSpr();

	CLSPResArrow();
	virtual ~CLSPResArrow();
//protected:
public:
	CObjAniLSP m_ResArrow;

//	CObjAni				m_objAni[MAX_RES_COUNT];
private:
	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
