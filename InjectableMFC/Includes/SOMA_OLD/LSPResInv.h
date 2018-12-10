// LSPResInv.h: interface for the CLSPResInv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESINV_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESINV_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAniLsp.h"
//#include "ObjAni.h"
//#include "ObjLSP.h"
#include "illstruct.h"

//class CObjLSP;
//struct _ANI_DATA;

class CLSPResInv //: public CObjLSP
{
public:
	BOOL ReleaseUnusedSpr();
	void RemoveAllData();
	
//	CObjAni* GetAniData(int ani_index);

	virtual bool LoadInv(CString szSprFileName);

	CLSPResInv();
	virtual ~CLSPResInv();

//protected:
public:
	CObjLSP m_ResInv;

//	CObjAni				m_objAni[MAX_RES_COUNT];
public:
	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPRESINV_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
