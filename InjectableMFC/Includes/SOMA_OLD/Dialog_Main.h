// Dialog_Main.h: interface for the CDialog_Main class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOG_MAIN_H__53DA6201_6AB6_11D4_98B0_00C02615866B__INCLUDED_)
#define AFX_DIALOG_MAIN_H__53DA6201_6AB6_11D4_98B0_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "MySpecialAttackData.h"
#include "MyAbilityData.h"
class BBDX_MouseInformation;
class CMyMagicData;

#define NORMAL_MODE		0
#define BATTLE_MODE		1
#define PK_MODE			2

#define MAX_MAGIC_BELT_ITEMS 12
#define MAGIC_BELT_SIZE 4

class CDialog_Main : public CUIRes  
{
public:
	CDialog_Main();
	virtual ~CDialog_Main();
	void CallBack_MessageBox(int nId, int nValue);
	void SendUseItem(short index);
	void SendThrowItem(short index, short count);
	void SendItemToInv(short fromindex, short toindex);
	void RcvItemToInv(PBYTE bypData);
	void SendChangeBeltIndex(int nFrom, int nTo);
	void ChangeMagicBelt(int nFrom, int magicBeltFromIndex, int nTo, int magicBeltToIndex);
public:
	BOOL OnLButtonDblClk(BBDX_MouseInformation *pMouseInfo);
	BOOL OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	BOOL OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	BOOL OnRButtonDown(BBDX_MouseInformation *pMouseInfo);
	int MouseProcess(BBDX_MouseInformation *pMouseInfo);

	BOOL DragDraw(CDDSurface *pDest);
	BOOL Draw(CDDSurface *pDest);
	BOOL OnButtonAction(int nSpec, int nIndex);
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = TRUE, BOOL bMoveable = TRUE, BOOL bTrans = TRUE);

	void HideWindow();
	void InitData();
	void ResetStatus();

	void SetMagicData(int index, CMyMagicData *magic_data);
	void LoadMagicData(CMyMagicData *magic_item[]);

	void SetSpecialData(int index, CMySpecialAttackData *special_data);
	void LoadSpecialData(CMySpecialAttackData *special_data[]);

	void SetAbilityData(int index, CMyAbilityData *ability_data);
	void LoadAbilityData(CMyAbilityData *ability_data[]);

	void SetSelMagic(int index, int magicBeltIndex);
	void SetBattleMode(int b_mode);

	void DrawTipRect(CDDSurface *pDest, CRect& t_rect, CRect standard_rect);
	void ViewCaption(CDDSurface *pDest, CRect standard_rect, CString str);

	void ChangeBeltIndex(int index);

	void SendPreCast(int index);
	void SetPreCast(int magic_id);

	// True if the currently selected ability eqauls this type
	bool IsSelAbility(int iType);

	CMyMagicData *m_pMagicData[12];
	CMySpecialAttackData *m_pSpecialData[12];
	CMyAbilityData * m_pAbilityData[12];

	CArray<CString, CString> m_arrClassNames;
	CString m_strHP, m_strStam;

	int m_vSelMagicIndex;
	int				m_vDragSpec; // 1 : 아이템 2 : 마법
	int				m_vDragItem;
	CPoint			m_vDragOffset;
	short			m_vFromItem, m_vToItem;
	BOOL			m_bChanging;
	int				m_bViewCaption;
	int				m_bViewMagicExCaption;
	int				m_bViewBtnCaption;
	int				m_bViewBtnExCaption;
	int				m_bViewItem;
	CString			m_vStrCaption, m_vMoral;
	int				m_vLBtnDown;
	CFont			*m_pFont;
	double			m_vShowHP, m_vHPFactor;
	double			m_vOldHP, m_vNewHP, m_vCurHP;
	DWORD			m_vOldHPTickCount;
	double			m_vShowMP, m_vMPFactor;
	double			m_vOldMP, m_vNewMP, m_vCurMP;
	DWORD			m_vOldMPTickCount;
	BOOL			m_bMenuDownOver;
	BOOL			m_bIsOverMoral;

	int				m_iMagicBeltIndex;
	int				m_iMagicBeltExIndex;
	int				m_iMagicBeltEx2Index;
	int				m_iDragMagicBeltIndex;
	int				m_vSelMagicBeltIndex;
	int				m_iPreCast;
};

#endif // !defined(AFX_DIALOG_MAIN_H__53DA6201_6AB6_11D4_98B0_00C02615866B__INCLUDED_)
