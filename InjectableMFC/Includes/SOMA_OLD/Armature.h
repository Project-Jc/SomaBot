// Armature.h: interface for the CArmature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMATURE_H__28B7FFC1_6507_11D3_8424_00105A6B96E7__INCLUDED_)
#define AFX_ARMATURE_H__28B7FFC1_6507_11D3_8424_00105A6B96E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "illstruct.h"
#include "User.h"

class CArmatureRes;

class CArmature  
{
public:
	CArmature();
	virtual ~CArmature();

	void	ReadyData(int nAniIndex, int nFrameNum, enum CUser::UserDirection eDir);

	friend		class	CUser;
protected:	//DataMember
//	CArmatureRes*	m_pArmatureRes[AM_KIND_NUM];	//AM_KIND_NUM -> 7
	_SPR_LSP*		m_pSprData[AM_KIND_NUM];
	CObjLSP*		m_pObjLsp[AM_KIND_NUM];
	CObjAni*		m_pObjAni[AM_KIND_NUM];

	int				m_nGender;
	bool			m_bReLoad;

	int				m_nClass[AM_KIND_NUM];
	int				m_nIndex[AM_KIND_NUM];
};

#endif // !defined(AFX_ARMATURE_H__28B7FFC1_6507_11D3_8424_00105A6B96E7__INCLUDED_)
