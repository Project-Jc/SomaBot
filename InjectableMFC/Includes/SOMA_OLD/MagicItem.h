// MagicItem.h: interface for the CMagicItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MagicItem_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_)
#define AFX_MagicItem_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"

class CMagicItem  : public CItem
{
public:
	LPDIRECTDRAWSURFACE7	m_lpSurf2;
	CDDSurface*  m_lpSurface2;
	_SPR_NORMAL  *m_SprData2;

	int m_UserItem[3];
	void ViewSelectBox(CPoint pos, int collar );

	void LoadSpr2(LPCSTR szFileName);
	void LoadObm2(LPCTSTR szFileName);

	virtual BOOL MouseProcess(BBDX_MouseInformation* pMouseInfo, int Dlg_Pos = 0, int Select_Box = 0, int MoveFlag = 1);
	void InitItem(CDirectDraw *pDirectDraw, CString strFile);
	CMagicItem();
	virtual ~CMagicItem();


};

#endif // !defined(AFX_MagicItem_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_)
