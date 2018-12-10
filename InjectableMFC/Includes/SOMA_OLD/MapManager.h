// MapManager.h: interface for the MapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__04CD82ED_790D_11D4_A572_000021EC6C24__INCLUDED_)
#define AFX_MAPMANAGER_H__04CD82ED_790D_11D4_A572_000021EC6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DManage\\BB_Direct.h"
#include "DirectXMng.h"
#include "TileMapTool.h"
#include "TileSetTool.h"
#include "MoveCell.h"
#include "LightSource.h"
#include "TransObj.h"
#include "MapObj.h"
#include "House.h"

class MapManager  
{
	friend class CBuildHouseManager; // might need to change the implementation of this..
public:
	MOVEINFO** GetMoveableTable(int &dimx, int &dimy);
	int  IsHouse(CPoint PosCell);
	short GetAreaNumber(CPoint posCell);

public:
	void SetView(int nFlag) { m_View_Flag = nFlag; }
	void ChangeAni();

	SIZE GetMapDimension()
	{
		SIZE MapSize;
		MapSize.cx = m_dimMap.cx*2 -1;
		MapSize.cy = m_dimMap.cy*2 -1;
		return MapSize;
	//	return m_dimMap;
	};

	SIZE GetMapTileDimension()
	{
		SIZE size;
		size.cx = m_pTileMapTool->m_HTMap.nWidth;
		size.cy = m_pTileMapTool->m_HTMap.nHeight;
		return size;
	};

	void SetMapDimension(CSize dimMap)
	{
		m_dimMap.cx = dimMap.cx/2;
		if(dimMap.cx%2) m_dimMap.cx++;
		m_dimMap.cy = dimMap.cy/2;
		if(dimMap.cy%2) m_dimMap.cy++;
	}

	CPoint GetMapStartPos(); //화면상에서 LeftTop(0,0)이 실제 맵에서 Pixel로 몇X몇 에 위치하는가.
	CPoint SetCenterPos(CPoint pos);

	BOOL IsChangePos(CPoint cell_offset);
	void DrawTile(CPoint cell_offset);
	void DrawChangeTile(CPoint cell_offset);
	void DrawTile(POINT ptDstPos, POINT ptSrcCellPos);
	void DrawTopSurface(CDDSurface *pDestSurface);
	void DrawBaseSurface(CDDSurface *pDestSurface);
	void DrawShadow(CPoint cell_offset);
	void DrawPartShadow(CPoint cell_offset);

	BOOL GetClippedRect(RECT *pRC, RECT* pRCClip);
		

	void LoadMap(int nNum);
	void InitManager(CDirectDraw *pDD, HWND hWnd, int nPixelFormat);

	CPoint PixelToCell(CPoint ptPixel);
	CPoint CellToPixel(CPoint ptCell);

	MoveCell* GetMoveCell(){ return m_pTMove;};
	int	 GetMapNum() { return m_nMapNum;};

	MapManager();
	virtual ~MapManager();

/*
	list<POINT>		m_listDrawObject;
	list<POINT>		m_listDrawInnerObject;

	list<POINT>		m_listDrawObject_Bak;
	list<POINT>		m_listDrawInnerObject_Bak;*/

	CPoint			m_ptMyPos;
	CHouse			m_house;
protected:

	CPoint			m_ptCenterPos;//Pixel 단위					//그리기 중심이 될 위치(캐릭터의 Center위치)
	CPoint			m_MapStartIndex;							//m_ptCenterPos를 기준으로 했을때 LeftTop에 그려질 Cell의 Index
	CPoint			m_offsetMap;								//도트 상으로 MapStartIndex를 얼마나 움직여서 찍어야 하는가? (맵은 cell단위로 찍히는 것이 아니라 Pixel단위로 찍힘: m_ptCenterPos기준)

	CPoint			m_ptMapStartPixel;							// 맵의 현재 View Pixel


	int				m_View_Flag;// 풀화면인지 작은 화면인지 플레그 세팅을 하게한다. 0 - 풀화면 1 - 왼쪽 작은화면 2 - 오른쪽 작은화면


	CDirectDraw*	m_pDD;
	HWND			m_hWnd;


	CTileMapTool*	m_pTileMapTool;
	CTileSetTool*	m_pTileSetTool;
	CLightSource*	m_pLightSource;

	CMapObj*        m_pMapObj;
	CMapObj*        m_pMapShadowObj;
	CMapObj*		m_pMapHouseIn;
	CMapObj*		m_pMapHouseOut;

	MoveCell*		m_pTMove;

	CTransObj		m_TransObj;

	int			m_nMapNum;
	int m_limit_old_y;

	CArray<LIGHT_INFO,LIGHT_INFO> m_ptLightPos;
public:
	int			m_cell_width;
	int			m_cell_height;

	SIZE			m_dimMap;		

	void SetOld_limit_y( int limit_y ){ m_limit_old_y = limit_y; }
    void AddLight(int x, int y, int iKind)
	{
		AddLight(CPoint(x,y), iKind);

	}
	void AddLight(CPoint pt, int iKind)
	{
        LIGHT_INFO li;
		li.m_kind = iKind;
		li.m_pt = pt;
		m_ptLightPos.Add(li);
	}
	void RemoveData();

	void Release();
	void ReleaseUnusedSpr();
	void DrawHouseIn(int x, int y, CPoint org_pos, CPoint offset_pos, BOOL bAbove = FALSE);//집안내부 그려주기
	CPoint DrawMyInHouseAndInCheck(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//집 그려주기 집 안에 있을때
	CPoint DrawMyInObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//집안에 들어갔을때 그려주는 함수
	void DrawMyOutObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//집밖에 있을때 그려주는 함수
	void DrawMyOutHouse(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//집그려주기 내가 집밖에 있을때 
	void DrawCommonObj(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//일반 오브젝트 그리기 
	void DrawTransOutHouse(int x, int y, CPoint org_pos, CPoint offset_pos, int nAlphaSrc, int nAlphaDst);

	BOOL FieldCheck(CPoint pt, int x, int y, RECT* pRC, WORD* data);
	BOOL LineCheck(CPoint pt, int x, int y, RECT* pRC, WORD* data);

	void DrawNight();
	void DrawNight1(BOOL bRight);

	void BltLSPTile(CDDSurface *pSurface, int x, int y, RECT* pRC, RECT* pRCOffset, WORD* data, BOOL bUseColorKey, bool safeZone ,int nRenderType = 0, int nAlpha = 0, COLORREF crColor = 0);
	void BltLSPShadow(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	void BltLSPPartShadow(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);


private:
	CRect m_rcBaseCenter;
	int m_vWidth, m_vHeight;
	CPoint m_vMapPos, m_vOrgPos;			// Cell 단위(32X32)
	CPoint m_vCurPos;
    CDDSurface *m_pBaseSurface;
	CDDSurface *m_pTopSurface;
};


inline BOOL MapManager::IsChangePos(CPoint cell_offset)
{
	if(cell_offset.x != m_vMapPos.x || cell_offset.y != m_vMapPos.y) return TRUE;
	else return FALSE;
};
#endif // !defined(AFX_MAPMANAGER_H__04CD82ED_790D_11D4_A572_000021EC6C24__INCLUDED_)
