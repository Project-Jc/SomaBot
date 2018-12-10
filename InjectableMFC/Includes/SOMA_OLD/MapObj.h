// MapObj.h: interface for the CMapObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJ_H__421F291A_2F93_4654_8E53_CA5B035E4EA0__INCLUDED_)
#define AFX_MAPOBJ_H__421F291A_2F93_4654_8E53_CA5B035E4EA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IGOBJECT\\myconstanddef.h"
#include "ObjAniLSP.h"


#include <list>

class MapManager;

class CMapObj  
{
private:
	typedef struct SAVE_BUF
	{
		int index;
		short value;
	} SAVE_BUF;
public:
	//_MAP_OBJ_HEADER m_Header;
	_MAP_OBJ_HEADER_EX m_Header;

	MapManager	*m_bpMapManager;

protected:
	HGLOBAL m_hData; // 맵 데이터 메모리 핸들
	short int* m_pnData; // 실제 맵 데이터.. 상위 4비트는 플래그.. 0-없음 1-충돌 2-투명찍기 4-안찍기. 하위 12비트는 인덱스
	std::list<int>	m_listHalfAlphaObj;
	std::list<int> m_listAddAlphaObj;

	CObjAniLSP *m_pObjs[MAX_MAP_OBJ_REG_EX]; // 에니메이션, 스프라이트 객체

public:
	void SetAlphaObj(_MAP_OBJ_HEADER* MOH);
	void SetAlphaObj(_MAP_OBJ_HEADER_EX* MOH);
	BOOL GetHalfAlpha(int nSet);
	BOOL GetAddAlpha(int nSet);
	void DeleteAlphaObj();

	void ReleaseUnusedSpr();
	void Alloc(int nWidth, int nHeight);

	short int DataGet(int x, int y) 
	{
		if(x < 0 || y < 0 || x >= m_Header.nWidth || y >= m_Header.nHeight) return -1;
		else return m_pnData[y*m_Header.nWidth + x];
	}
	void DataSet(int x, int y, short int nData)
	{
		if(x < 0 || y < 0 || x >= m_Header.nWidth || y >= m_Header.nHeight) return;
		m_pnData[y*m_Header.nWidth + x] = nData;
	}
	char CMapObj::GetSetData(int x, int y)
	{
		if(x < 0 || y < 0 || x >= m_Header.nWidth || y >= m_Header.nHeight) return -1;
		return ((m_pnData[y*m_Header.nWidth + x])&0xff00)>>8;
	}
	char CMapObj::GetSubSetData(int x, int y)
	{
		if(x < 0 || y < 0 || x >= m_Header.nWidth || y >= m_Header.nHeight) return -1;
		return (m_pnData[y*m_Header.nWidth + x])&0xff;
	}

	void Load(HANDLE& hFile);
	void Load(const char* szFileName);
	void Release();
	
public:
	CObjAniLSP * GetAniObject(int nIndex);
	CMapObj();
	virtual ~CMapObj();
};

#endif // !defined(AFX_MAPOBJ_H__421F291A_2F93_4654_8E53_CA5B035E4EA0__INCLUDED_)
