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

class CObjAni //: public CObjLSP // �⺻������ 8 ������ �����ϴ� ���ϸ��̼� ������Ʈ
{
public:
	//_ANI_HEADER			m_HAni;
	_ANI_HEADER_SIMPLE		m_HAni;


protected:
	_ANI_DATA*			m_pAnis[MAX_ANI];

	int		m_nCtrlQt; // ���ϸ��̼� ���� ��
	int*	m_nCurAnis; // ���� ������ ���ϸ��̼�
	int*	m_nDefaultAnis; // �⺻ ���ϸ��̼�.. �������� ������ ���� ���ϸ��̼��� �ȴ�..
	DWORD*	m_dwTickFrms; // ���ϸ��̼�..
	int*	m_nCurFrms; // ���� ������ ���ϸ��̼��� ������.
	int*	m_nCurDirs; // ���� ������ ����
	int*	m_nFlags; // ���ϸ��̼� ��Ʈ�� �÷���   FL_LOOP, FL_ONCE, FL_ONCE_FIX;

	float	m_fASpeed; // �׼� ���ǵ�.. */

public:
	int		FrmGetAnimated(int nCtrl = 0); // ���� ���ϸ��̼� �����Ϳ� ���� ���ϴ� ������ ��ȣ�� ����
	float	ActionSpeedSet(float fSpeed); // ���ϸ��̼� ��� �ӵ��� ����

	void	AniAlloc(int nQt); // ���ϸ��̼� ������ ���� ��ŭ �Ҵ�
	void	AniCtrlAlloc(int nQt); // ���ϸ��̼� ���� �� ����
	void	AniRelease(); // ���ϸ��̼� ������ ������
	void	AniCtrlRelease(); // ���ϸ��̼� ���� ������ ������.

	int	__fastcall GetMaxDir() { return m_HAni.nDirQt;}
	int __fastcall GetMaxFrame(int nAni) {	return m_pAnis[nAni]->nMaxFrm; }
	void __fastcall SetMaxFrame(const int nAni, const int nMaxFrm) { m_pAnis[nAni]->nMaxFrm = nMaxFrm; }

	int		AniGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurAnis[nCtrl]; } // ������ ���ϸ��̼� ��ȣ ��������
	void	AniSet(int nAni, int nFlag = FL_ANI_LOOP, int nCtrl = 0); // ���ϸ��̼� ��ȣ ����
	void	DirSet(int nDir, int nCtrl = 0); // ���� ����
	int		DirGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurDirs[nCtrl]; } // ���� ���� ��������

	int		FrmGet(int nCtrl = 0) { if(nCtrl < 0 || nCtrl >= m_nCtrlQt) return 0; return m_nCurFrms[nCtrl]; } // 
	void	FrmSet(int nFrm, int nCtrl = 0); // 
	

	int	__fastcall FrmGetByAniAndDirAndSeq(int nAni, int nDir, int nFrm); // ���� ���ϸ��̼� ����� ��ȣ�� ���� ������ ��ȣ�� �Ѱ� �ش�.
	//int	__fastcall FrmGetMax(int nCtrl = 0);
	_ANI_DATA* __fastcall AniDataGet(int index); // ���ϸ��̼� ������ ������ ��������

	BOOL		AniLoad(const char* szFileName); // ���ϸ��̼� ���� �ҷ�����
	
	CObjAni();
	virtual ~CObjAni();
};


#endif // !defined(AFX_OBJANI_H__97B9521F_7A89_11D4_A572_000021EC6C24__INCLUDED_)
