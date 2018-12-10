// MoveCell.h: interface for the MoveCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVECELL_H__73D8C5A1_7733_11D3_98AF_00C02615866B__INCLUDED_)
#define AFX_MOVECELL_H__73D8C5A1_7733_11D3_98AF_00C02615866B__INCLUDED_

class MOVEINFO
{
public:
	MOVEINFO();
	~MOVEINFO() {};

	BOOL m_bMovable;
	short m_nEventNumber;
	short m_nHouseNumber;
	BYTE m_nAreaNumber;
};

class MoveCell  
{
public:
	MoveCell();
	MoveCell(int cx, int cy);
	virtual ~MoveCell();

	void RemoveData();
	void Initialize(int cx, int cy);

	BOOL Load(CString szFileName);

	void SetMovable(int x, int y, BOOL movable);
	void SetEvent(int x, int y, short event);
	void SetHouse(int x, int y, short house);
	void SetArea(int x, int y, short Area);

	BOOL GetMovable(int x, int y);
	short GetEvent(int x, int y);
	short GetHouse(int x, int y);
	short GetArea(int x, int y);

	BOOL IsInRange(int x, int y);
	BOOL IsInRange(POINT pos);
public:
	int m_vMoveInfoVersion;		// 버전
	SIZE m_vDim;				// Cell들의 크기
	MOVEINFO **m_pMoveInfo;		// 각 Cell들의 정보
};

inline BOOL MoveCell::IsInRange(int x, int y)
{
	if (x<0) return FALSE;
	if (x>=m_vDim.cx) return FALSE;
	if (y<0) return FALSE;
	if (y>=m_vDim.cy) return FALSE;
	return TRUE;
}

inline BOOL MoveCell::IsInRange(POINT pos)
{
	return IsInRange(pos.x, pos.y);
}

#endif // !defined(AFX_MOVECELL_H__73D8C5A1_7733_11D3_98AF_00C02615866B__INCLUDED_)
