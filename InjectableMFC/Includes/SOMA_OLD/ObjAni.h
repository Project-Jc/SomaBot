// ObjAni.h: interface for the CObjAni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJANI_H__97B9521F_7A89_11D4_A572_000021EC6C24__INCLUDED_)
#define AFX_OBJANI_H__97B9521F_7A89_11D4_A572_000021EC6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ObjLSP.h"

class CObjAni //: public CObjLSP // 기본적으로 8 방향을 지원하는 에니메이션 오브젝트
{
public:
	//_ANI_HEADER			m_HAni;
	_ANI_HEADER_SIMPLE		m_HAni;


protected:
	_ANI_DATA*			m_pAnis[MAX_ANI];

	int		m_nCtrlQt; // 에니메이션 제어 수
	int*	m_nCurAnis; // 현재 지정된 에니메이션
	int*	m_nDefaultAnis; // 기본 에니메이션.. 마지막에 정해진 루프 에니메이션이 된다..
	DWORD*	m_dwTickFrms; // 에니메이션..
	int*	m_nCurFrms; // 현재 지정된 에니메이션의 프레임.
	int*	m_nCurDirs; // 현재 지정된 방향
	int*	m_nFlags; // 에니메이션 컨트롤 플래그   FL_LOOP, FL_ONCE, FL_ONCE_FIX;

	float	m_fASpeed; // 액션 스피드.. */

public:
	int		FrmGetAnimated(int nCtrl = 0); // 내부 에니메이션 데이터에 의해 변하는 프레임 번호를 리턴
	float	ActionSpeedSet(float fSpeed); // 에니메이션 평균 속도값 조정

	void	AniAlloc(int nQt); // 에니메이션 데이터 숫자 만큼 할당
	void	AniCtrlAlloc(int nQt); // 에니메이션 제어 수 설정
	void	AniRelease(); // 에니메이션 데이터 날리기
	void	AniCtrlRelease(); // 에니메이션 제어 데이터 날리시.

	int	__fastcall GetMaxDir() { return m_HAni.nDirQt;}
	int __fastcall GetMaxFrame(int nAni) {	return m_pAnis[nAni]->nMaxFrm; }
	void __fastcall SetMaxFrame(const int nAni, const int nMaxFrm) { m_pAnis[nAni]->nMaxFrm = nMaxFrm; }

	int		AniGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurAnis[nCtrl]; } // 지정된 에니메이션 번호 가져오기
	void	AniSet(int nAni, int nFlag = FL_ANI_LOOP, int nCtrl = 0); // 에니메이션 번호 지정
	void	DirSet(int nDir, int nCtrl = 0); // 방향 지정
	int		DirGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurDirs[nCtrl]; } // 현재 방향 가져오기

	int		FrmGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurFrms[nCtrl]; } // 
	void	FrmSet(int nFrm, int nCtrl = 0); // 
	

	int	__fastcall FrmGetByAniAndDirAndSeq(int nAni, int nDir, int nFrm); // 현재 에니메이션 방향과 번호에 따라 프레임 번호를 넘겨 준다.
	//int	__fastcall FrmGetMax(int nCtrl = 0);
	_ANI_DATA* __fastcall AniDataGet(int index); // 에니메이션 데이터 포인터 가져오기

	BOOL		AniLoad(const char* szFileName); // 에니메이션 파일 불러오기
	
	CObjAni();
	virtual ~CObjAni();
};


#endif // !defined(AFX_OBJANI_H__97B9521F_7A89_11D4_A572_000021EC6C24__INCLUDED_)
