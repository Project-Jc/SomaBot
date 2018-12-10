// LSPCharRes.h: interface for the CLSPResArmature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSPARMRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_LSPARMRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ObjAni.h"
#include "ObjLSP.h"


class CObjLSP;

//#define MAX_EQUIP_MANAGE_NUM	0xA0

#define AM_KIND_ARMOR	0
#define AM_KIND_HELMET	1
#define AM_KIND_BOOT	2
#define AM_KIND_JEAN	3
#define AM_KIND_SHIELD	4
#define AM_KIND_ARMS	5
#define AM_KIND_SH      6 //그림자.
#define	AM_KIND_SPECIAL 7

#define AM_KIND_NUM		8

/*
#define EQUIP_KIND_MOTION	0
#define EQUIP_KIND_JACKET	1
#define EQUIP_KIND_SHIRT	2
#define EQUIP_KIND_PANTS	4
#define EQUIP_KIND_SHOES	5
#define EQUIP_KIND_SHADOW	6
#define EQUIP_KIND_WEAPON	7
#define EQUIP_KIND_HAIR		8
#define EQUIP_KIND_HELMET	9
#define	EQUIP_KIND_GLOVES	10
#define	EQUIP_KIND_HITMARK	11

#define EQUIP_KIND_NUM		12*/


struct _ANI_DATA;

// CLSPRes에서 상속받아 Body를 관리하며..
// Member로서 CLSPRes Array를 가지며 그외의 장신구들을 관리하고 있다.
class CLSPResArmature //: public CObjLSP
{
public:
	BOOL ReleaseUnusedSpr();
	void RemoveAllData();
	
	CObjAni* GetAniData();

	virtual BOOL InitResChar(int nClass, int nGender, int nAniIndex);
	virtual BOOL LoadRes(int nType, int nIndex);

	CObjLSP * GetTypeIndexedRes(int nType, int nIndex);
	CLSPResArmature();
	virtual ~CLSPResArmature();

//protected:
private:
	CMap<int, int, CObjLSP*, CObjLSP*>  m_mapEquip;

	//_ANI_HEADER			m_HAni;
	//_ANI_DATA *			m_pAniData;				//특정 부위 방어구에 대한 종류들. (Array로 관리)


	CObjAni				m_objAni;
	

	CString				m_szPath;//장비 아이템 리소스 path
	CString				m_szArmPath;

	int					m_nGender;
	int					m_nAniIndex;
	int					m_nClass;


	DWORD				m_dwLastTick;
};

#endif // !defined(AFX_LSPCHARRES_H__E1A4DD02_4C5B_11D4_946F_0090272340A6__INCLUDED_)
