#if _MSC_VER > 1000
#pragma once
#endif

#include "User.h"
#include "UIRes.h"
#include "ChildView.h"
#include "soma.h"

typedef struct _srBar{
		CRect InnerBar;
		CRect OuterBar;
	}srBar;


class CDialog_PartySidebar : public CUIRes  
{
public:
	CDialog_PartySidebar();
	virtual ~CDialog_PartySidebar();

public:
	BOOL Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bModal = FALSE, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL Draw(CDDSurface *pDest, int nCurHp, int nMaxHp, int nCurMp ,int nMaxMp,CString Name,int nUid);
	srBar CalcBar(int Cur,int Max, int i);
	void ShowWindow();
	void HideWindow();

	DDSURFACEDESC2 sd;
	CDDSurface* pSurface;	
};

