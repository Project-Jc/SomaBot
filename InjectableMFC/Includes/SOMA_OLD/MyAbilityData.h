// MyMagicData.h: interface for the CMyMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyAbilityData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_)
#define AFX_MyAbilityData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyAbilityData  
{
public:
	CMyAbilityData();
	virtual ~CMyAbilityData();

public:
	void Init();
	short m_vNumber;
	BYTE m_bType;
	CString m_vName;
	CString m_vExplain;	
	short m_nMinExp;
	short m_nUseLeft; // Amount of times ability can be used left for today
	short m_nUseMax; // Maximum amount of times ability can be used in a day
};

#endif // !defined(AFX_MyMagicData_H__A0ECA1A1_E9F8_11D3_B175_00105A735718__INCLUDED_)
