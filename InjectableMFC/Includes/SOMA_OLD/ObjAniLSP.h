// ObjAniLSP.h: interface for the CObjAniLSP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIOBJLSP_H__65C6CB21_84CF_11D3_9925_00C026EE03FC__INCLUDED_)
#define AFX_ANIOBJLSP_H__65C6CB21_84CF_11D3_9925_00C026EE03FC__INCLUDED_

#include "stdafx.h"
#include "ObjLSP.h"
#include "ObjAni.h"

class CObjAniLSP 
: public CObjLSP, // 기본적으로 8 방향을 지원하는 에니메이션 오브젝트
public CObjAni
{
public:
	CObjAniLSP();
	virtual ~CObjAniLSP();
};

#endif // !defined(AFX_ANIOBJ_H__65C6CB21_84CF_11D3_9925_00C026EE03FC__INCLUDED_)
