// TileSetTool.h: interface for the CTileSetTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESETTOOL_H__C0E02734_3F05_4163_8E9A_FDAE5D9BA470__INCLUDED_)
#define AFX_TILESETTOOL_H__C0E02734_3F05_4163_8E9A_FDAE5D9BA470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "IGObject\\DDEngine.h"
#include <ddraw.h>
#include "_TILE_SUB_SET_DATA_TOOL.h"

#include <string>
#include "ObjLSP.h"

#include "DManage\\BB_Direct.h"
class CTileSetTool
{
//protected:
public:
	// _TILE_SUB_SET_DATA_TOOL m_Datas[MAX_TILE_SET][MAX_TILE_SUB_SET];

	_TILE_SUB_SET_DATA_TOOL* m_Datas[MAX_TILE_SET];
//	_TILE_SUB_SET_DATA_TOOL* m_arrAniDatas[100];
	int m_nAniCount;

public:
	//_TILE_SET_HEADER m_HTSs[MAX_TILE_SET];
	_TILE_SET_HEADER_SIMPLE m_HTSs[MAX_TILE_SET];

	//LPDIRECTDRAWSURFACE7 m_lpDDSs[MAX_TILE_SET];

	//CDirectDraw*	m_pDD;
	//CDDSurface *m_lpDDSs[MAX_TILE_SET];

	CObjLSP *m_pTileLSP[MAX_TILE_SET];
public:
	void ReleaseUnusedSpr();
	void Load(int nIndex);
	POINT *DataGetPosCur(int nTS, int nTSS, int nIndex);
	int *DataGetFlagCur(int nTS, int nTSS, int nIndex);
	POINT *DataGetPos(int nTS, int nTSS, int nIndex, int nAniNum = 0);
	_TILE_SUB_SET_DATA_TOOL* DataGetSafe(int nTS, int nTSS);
	void DataSetSafe(int nTS, int nTSS, _TILE_SUB_SET_DATA_TOOL* pTileSet);
	
	void Alloc(int nTS, int nTSS, int width, int height);
	void Release();
	void ReleaseAll();

	_TILE_SUB_SET_DATA_TOOL* DataGet(int nTS, int nTSS);

	void Load(const char* szFileName);
	void ReAlloc(int nTS, int nTSS, int width, int height);

	void ResetAni();
	void Increase();
	void Decrease();

	CTileSetTool();
	virtual ~CTileSetTool();
};

#endif // !defined(AFX_TILESETTOOL_H__C0E02734_3F05_4163_8E9A_FDAE5D9BA470__INCLUDED_)

