// LSPCharRes.h: interface for the CLSPResChar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPRESCHAR_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPRESCHAR_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAniLsp.h"
//#include "ObjAni.h"
//#include "ObjLSP.h"
#include "illstruct.h"

//class CObjLSP;
struct _ANI_DATA;

class CLSPResChar //: public CObjLSP
{
public:
	BOOL ReleaseUnusedSpr();
	void RemoveAllData();
	
//	CObjAni* GetAniData(int ani_index);

	void	SetIndex(int index) {m_vIndex = index;};
	BOOL	Load(LPCSTR	szFileName, int pf, int index);
	virtual CObjAniLSP* LoadRes(int ani_index);

	CObjAniLSP* GetRes(int ani_index);
	CLSPResChar();
	virtual ~CLSPResChar();

//protected:
private:
	CObjAniLSP* m_ResChar[MAX_RES_COUNT];

//	CObjAni				m_objAni[MAX_RES_COUNT];
//private:
	int					m_vIndex;
	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
