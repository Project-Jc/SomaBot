// DxCursor.h: interface for the CDxCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXCURSOR_H__62502769_3A93_11D3_8422_00105A6B97E2__INCLUDED_)
#define AFX_DXCURSOR_H__62502769_3A93_11D3_8422_00105A6B97E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjAniLSP;

#ifndef _SPR_DATA
#define _SPR_DATA
class SPR_DATA
{
public:
	_ANI_HEADER		m_HAni;
	_ANI_DATA		m_AniData;
	_SPR_HEADER		m_HSpr;
	_SPR_LSP		*m_arrSprData;
	WORD			**m_arrVData;
	BYTE			**m_arrVData2;
	WORD			*m_arrPalleteData;
	BOOL			m_bCompressed;

public:
	SPR_DATA();
	~SPR_DATA();
};
#endif

class CDDSurface;

#define CURSOR_COUNT		7

#define NORMAL_CURSOR		0
#define ATTACK_CURSOR		1
#define PICKUP_CURSOR		2
#define PK_ATTACK_CURSOR	3
#define CHASE_CURSOR		4
#define COMPASS_CURSOR		5
#define	HAMMER_CURSOR		6

class CDxCursor  
{
public:
	void				InitCursor();
	void				Draw(CDDSurface *pSurface, CPoint ptPos);			//Ŀ�� �׸���
	void				SetCursorIndex(int index);	//Ŀ�� ��ü
	void				RemoveAllData();
	void				BltLSPNormal(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data);

	CDxCursor();
	virtual ~CDxCursor();
protected:
	CObjAniLSP			*m_arrCursor[CURSOR_COUNT];	//Cursor�׸��� �������(Surface�� ����)
	int					m_iCursorIndex;		//����ϴ� Cursor�� Index
	int					m_iSpriteIndex;
	DWORD				m_dwTick;
	CPoint				m_ptHotSpot;		//32X32ũ���� Cursor�׸������� HotSpot
};

#endif // !defined(AFX_DXCURSOR_H__62502769_3A93_11D3_8422_00105A6B97E2__INCLUDED_)