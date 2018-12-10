// MyMagicData.h: interface for the CMyMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MySpecialAttackData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_)
#define AFX_MySpecialAttackData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySpecialAttackData  
{
public:
	CMySpecialAttackData();
	virtual ~CMySpecialAttackData();

public:
	void Init();
	short m_vNumber;
	BYTE m_vTgtMethod;
	CString m_vName;
	CString m_vExplain;	
	short m_vDistance;
	short m_vUseHp;
	short m_vUseMp;
	short m_vUseStam;
	int m_iSkillDura;
	int m_iTerm;
	short m_vDamage;
	short m_iMinExp;
	BYTE m_byClass;
	int m_iDecTerm;
	short m_sTHP;
	short m_sTMP;
};

#endif // !defined(AFX_MyMagicData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_)
