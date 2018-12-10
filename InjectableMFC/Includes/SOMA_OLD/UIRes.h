// UIRes.h: interface for the CUIRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIRES_H__48952D6E_5195_11D4_946F_0090272340A6__INCLUDED_)
#define AFX_UIRES_H__48952D6E_5195_11D4_946F_0090272340A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>


#define		BUTTON_DISABLE	-1
#define		BUTTON_NORMAL	0
#define		BUTTON_FOCUS	1
#define		BUTTON_DOWN		2

#define		STYLE_PUSH		0
#define		STYLE_TOGGLE	1

#define		POS_NORMAL		0
#define		POS_LEFT		1
#define		POS_RIGHT		2
#define		POS_NOTEFFECT	3

class DLG_DATA
{
public:
	int				nWidth;
	int				nHeight;
	COLORREF		crCK;
	int				nUnknown;
	RECT			rcPos;
	int				nLine;
	int				nDataSize;
	BYTE			*m_arrVData;
	WORD			*m_arrPalleteData;
public:
	DLG_DATA();
	~DLG_DATA();
};

#include "ObjectClass.h"

class CDDSurface;
class CDirectDraw;
class CUIScrollBar;
class CUIHorzScrollBar;
class BBDX_MouseInformation;
class BBDX_Editor;

class CUIRes  
{
	friend CUIScrollBar;
	friend CUIHorzScrollBar;
public:
	virtual int GetPosMode();
	void Destroy();
	void SetWnd(CWnd *wnd) {m_pWnd = wnd;};

	BOOL SetToolTips(CString strMsg, CPoint pos);
	bool PtInUIRes(CPoint point);
	BOOL IsMoveable() { return m_bMoveable; };
	BOOL IsModal() { return m_bModal; };

	CSize	GetUISize() { return m_mainRect.Size();};
	int	 GetType() { return m_nType; };
	
	CPoint GetOffset() { return m_ptOffset; };
	void SetOffset(CPoint ptOffset) 
	{ 
		m_ptOffset = ptOffset; 
	};

	BOOL Draw3ButtonCtrl(CDDSurface *pDest, int nFrom, int nTo = -1);
	BOOL Draw3ButtonCtrl(CDDSurface *pDest, int nButton, CRect rtPos = CRect(0,0,0,0));
	BOOL Draw4ButtonCtrl(CDDSurface *pDest, int nFrom, int nTo = -1);
	int	CalcAreaPos(CPoint pos, int nSpec, int nFrom, int nTo);

	virtual BOOL	DragDraw(CDDSurface *pDest) {return TRUE;};
	virtual BOOL	Draw(CDDSurface *pDest);

	virtual	BOOL	OnLButtonDown(BBDX_MouseInformation *pMouseInfo);
	virtual	BOOL	OnLButtonUp(BBDX_MouseInformation *pMouseInfo);
	virtual	BOOL	OnMouseMove(BBDX_MouseInformation *pMouseInfo);
	virtual BOOL	OnLButtonDblClk(BBDX_MouseInformation *pMouseInfo) {return TRUE;};
	virtual BOOL	OnLButtonPressing(CPoint point);
	virtual BOOL	OnButtonAction(int nSpec, int nIndex);
	virtual BOOL	OnButtonActionContinue(int nID);
	virtual BOOL	OnMouseWheel(UINT nFlags, short zDelta, POINT pt);

	virtual int		MouseProcess(BBDX_MouseInformation *pMouseInfo);

	virtual BOOL	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {return FALSE;};
	virtual void	TapHopping() {};
	virtual void	CallBack_MessageBox(int nId, int nValue) {}

	virtual void	ShowWindow();
	virtual void	HideWindow();
	virtual void	HideAllWindow() {};
	virtual void	SetFocus();
	void			KillFocus();
	BOOL IsVisible() {return m_bShow;};
	BOOL IsLoading() {return m_bLoad;};

	void SetLoading(BOOL bLoad) { m_bLoad = bLoad; };
void SendProduce();
	void InitButton();

	BOOL			GetCaptureStatus(){return m_bCapture;}
	void			SetCapture();
	void			ReleaseCapture();

	virtual BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = TRUE, BOOL bMoveable = TRUE, BOOL bTrans = TRUE);


	CUIRes(CWnd* pParent = NULL);
	virtual ~CUIRes();

	enum TYPE { CHATIN = 0, CHATOUT };

	CPoint			m_ptOffset;		// TopLeft Pos
	BOOL			m_bMainUI;		// true only if the current dlg is the main ui
	BOOL			m_bMainMenuBar;	// used only if the current dlg is the main ui
	bool			m_bShowExMagic;
	CPoint			m_ptExBeltOffset;
	POINT			m_ptMenuOffset;	// Offset of the Menu bar in interface
	CRect			m_mainRect;		// Rect of This Dlg
	CUIRes			*m_pChild;
	DLG_DATA		m_newDlg;

	CAreaCtrlArray	m_arrArea;
	CBtnCtrlArray	m_arrBtn;
	CBtnCtrlExArray	m_arrBtnEx;
	CRectManagerArray m_arrRectManager;
	CUIScrollBar*		m_pScrollBar;
protected:
	BOOL LoadResourceNewDlg();
	BOOL LoadResource();

	CDDSurface		*m_pDDSurface;
	CDirectDraw		*m_pDD;

	int				m_vSpecCtrl;				
	int				m_vIndexCtrl;				
	int				m_vAreaIndex;			
	
	CString			m_szLibName;	// Library Name

	CPoint			m_ptDrag;
	BOOL			m_bDragging;
	BOOL			m_bItemDragging;

	int				m_nType;
	BOOL			m_bCapture;
	BOOL			m_bLoad;

	BOOL			m_bMoveable;
	BOOL			m_bTrans;
	BOOL			m_bEnableFocus;
	BOOL			m_bModal;

	CWnd			*m_pWnd;	// 게임 메인화면(Message와 Editor를 위해서.)
	BOOL			m_bShow;
	CUIRes			*m_pParent;
	int				m_vPosMode;

	BBDX_Editor		*m_pMainEditor;
	//CUIToolTips *		m_pToolTips;	
	bool			m_bCanScroll;
};

#endif // !defined(AFX_UIRES_H__48952D6E_5195_11D4_946F_0090272340A6__INCLUDED_)
