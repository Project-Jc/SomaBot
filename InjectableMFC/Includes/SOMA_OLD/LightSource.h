// LightSource.h: interface for the CLightSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTSOURCE_H__FF6D1C21_AA85_11D4_956A_00D0B77259E8__INCLUDED_)
#define AFX_LIGHTSOURCE_H__FF6D1C21_AA85_11D4_956A_00D0B77259E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef LIGHTINFO
#define LIGHTINFO
typedef struct TAG_LIGHT_INFO
{
	int m_kind; // 0 = low ambience, mid range; 1 = high ambience, low range; 2 = high ambience, high range; 3 = low ambience low range
	CPoint m_pt;
}LIGHT_INFO;
#endif

typedef struct _LIGHT_SOURCE_HEADER
{
	char szID[4]; // "LSDF"
	char szRemark[64];
	int nQt;
} LIGHT_SOURCE_HEADER;

typedef struct _LIGHT_SOURCE_DATA
{
	COLORREF crColor;
	RECT rc;
} LIGHT_SOURCE_DATA;

class CLightSource  
{
public:
	_LIGHT_SOURCE_HEADER m_Header;
	_LIGHT_SOURCE_DATA* m_pDatas;

public:
	void Load(const char* szFileName);
	void Alloc(int nQt);
	_LIGHT_SOURCE_DATA* DataGet(int index);
	void Release();
	CLightSource();
	virtual ~CLightSource();

};


#endif // !defined(AFX_LIGHTSOURCE_H__FF6D1C21_AA85_11D4_956A_00D0B77259E8__INCLUDED_)