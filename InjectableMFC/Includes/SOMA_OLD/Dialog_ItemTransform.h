// Dialog_ItemTransform.h: interface for the CDialog_ItemTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_ITEMTRANSFORM_H__8850F5A6_9462_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_DIALOG_ITEMTRANSFORM_H__8850F5A6_9462_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "UIScrollBar.h"
#include "ItemData.h"
class BBDX_MouseInformation;

class CDialog_ItemTransform : public CUIRes  
{
public:
	CDialog_ItemTransform();
	virtual ~CDialog_ItemTransform();

	void DialogOpen(PBYTE bypData);
	void RecvItemTransform(PBYTE bypData);
	void SendItemTransform();
	void SendItemTransform1(int how_many);

public:
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);

	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
void CallBack_MessageBox(int nId, int nValue);
	void InitData();
	void ShowWindow(int Id = 0, CUIRes *pParent = NULL);
	void HideWindow();
	void HideAllWindow();
	void DeleteContents();

	CItemDataArray	m_arrAuctionItem;
	CItemDataArray m_arrItemMe;
	CItemDataArray m_arrItemShop;
	short m_vShopNumber;
	int m_vCurMeItem, m_vCurShopItem;
	int					m_vCurItem;

//	CUIScrollBar *m_pScrollBar;
	CStringArray	m_ItemNames;
	int				*m_ItemIndexes;
	CFont			*m_pFont;
	int				m_nItemCount;
	int				m_vSelIndex, m_vProcessingIndex;
	BOOL			m_bProcessing;
	int				m_vId;
};

#endif // !defined(AFX_DIALOG_ITEMTRANSFORM_H__8850F5A6_9462_11D4_98B2_00C02615866B__INCLUDED_)
