// SLoadingView.h: interface for the SLoadingView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLOADINGVIEW_H__D0678B81_ABEE_11D4_826D_00D0B769A081__INCLUDED_)
#define AFX_SLOADINGVIEW_H__D0678B81_ABEE_11D4_826D_00D0B769A081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIRes.h"
#include "./DManage/BB_Direct.h"

class SLoadingView : public CUIRes
{
public:
	SLoadingView();
	virtual ~SLoadingView();

	BOOL  Create(CDirectDraw *pDD, CString szLibName, int nType, BOOL bMoveable = FALSE, BOOL bTrans = FALSE);
	BOOL  Draw( CDDSurface* pDestSurf );
	void SetPercentage(int const& nPercentage)
	{
		m_nPercentage = nPercentage;
	}

private:
	int m_nPercentage; // out of 100 for loading bar
};

#endif // !defined(AFX_SLOADINGVIEW_H__D0678B81_ABEE_11D4_826D_00D0B769A081__INCLUDED_)
