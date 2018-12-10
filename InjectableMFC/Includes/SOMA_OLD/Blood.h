// Blood.h: interface for the CBlood class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOOD_H__CD4EDB81_B6FF_11D4_956A_00D0B77259E8__INCLUDED_)
#define AFX_BLOOD_H__CD4EDB81_B6FF_11D4_956A_00D0B77259E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDDSurface;
class CUser;
class CLSPResBlood;
typedef struct _TAG_BLOOD_INFO
{
	CPoint	m_ptBlood;		//피가 그려지는 위치
	int		m_nUid;			//죽은 캐릭터 아이탛E
	int		m_nBloodKind;
	int		m_nMaxFrame;	//피의 최큱E프레임
	int		m_nCurFrame;	//피의 현픸E프레임 
	DWORD	m_TickTime;
	BOOL	m_bStained;
	BOOL	m_bAttackBlood;
}BLOOD_INFO;

class CBlood  
{
private:
	CArray<BLOOD_INFO*,BLOOD_INFO*> m_arrBlood;
public:
	void Add( int uid, int type, CPoint ptBlood ,BOOL bAttackBlood = FALSE);
	BOOL Load( int nBloodKind );
	BOOL Draw(CDDSurface *pSurface,CPoint ptMapPos,int nUid);
	BOOL DrawBloodStain(CDDSurface *pSurface,CPoint ptMapPos);
//	CBlood(CDDSurface *pSurface);
	CBlood();
	virtual ~CBlood();

};

#endif // !defined(AFX_BLOOD_H__CD4EDB81_B6FF_11D4_956A_00D0B77259E8__INCLUDED_)
