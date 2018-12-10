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

	CPoint GetMapStartPos(); //ȭ��󿡼� LeftTop(0,0)�� ���� �ʿ��� Pixel�� ��X�� �� ��ġ�ϴ°�.
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

	CPoint			m_ptCenterPos;//Pixel ����					//�׸��� �߽��� �� ��ġ(ĳ������ Center��ġ)
	CPoint			m_MapStartIndex;							//m_ptCenterPos�� �������� ������ LeftTop�� �׷��� Cell�� Index
	CPoint			m_offsetMap;								//��Ʈ ������ MapStartIndex�� �󸶳� �������� ���� �ϴ°�? (���� cell������ ������ ���� �ƴ϶� Pixel������ ����: m_ptCenterPos����)

	CPoint			m_ptMapStartPixel;							// ���� ���� View Pixel


	int				m_View_Flag;// Ǯȭ������ ���� ȭ������ �÷��� ������ �ϰ��Ѵ�. 0 - Ǯȭ�� 1 - ���� ����ȭ�� 2 - ������ ����ȭ��


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
	void DrawHouseIn(int x, int y, CPoint org_pos, CPoint offset_pos, BOOL bAbove = FALSE);//���ȳ��� �׷��ֱ�
	CPoint DrawMyInHouseAndInCheck(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//�� �׷��ֱ� �� �ȿ� ������
	CPoint DrawMyInObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���ȿ� ������ �׷��ִ� �Լ�
	void DrawMyOutObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���ۿ� ������ �׷��ִ� �Լ�
	void DrawMyOutHouse(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���׷��ֱ� ���� ���ۿ� ������ 
	void DrawCommonObj(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//�Ϲ� ������Ʈ �׸��� 
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
	CPoint m_vMapPos, m_vOrgPos;			// Cell ����(32X32)
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
