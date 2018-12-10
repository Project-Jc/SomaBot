// Item_Small.h: interface for the CItem_Small class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Item_Small_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_)
#define AFX_Item_Small_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"

class CItem_Small  : public CItem
{
public:
	void ViewSelectBox(CPoint pos, int collar );

	ITEM_SPRITE m_ItemSprite[50]; // Hsoma Item image array
	ITEM_SPRITE m_DItemSprite[50]; // Dsoma Item image array

	void LoadSpr();
	void LoadObm();

	virtual BOOL MouseProcess(BBDX_MouseInformation* pMouseInfo, int Dlg_Pos = 0, int Select_Box = 0, int MoveFlag = 1);
	virtual void Draw(CDDSurface* pDestSurf, int count=0, int tmp_y=0, int index=0);
	void InitItem(CDirectDraw *pDirectDraw);
	CItem_Small();
	virtual ~CItem_Small();


};

#endif // !defined(AFX_Item_Small_H__870BC321_DF02_11D3_B175_00105A735718__INCLUDED_)
