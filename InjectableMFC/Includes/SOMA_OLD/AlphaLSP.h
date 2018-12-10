// AlphaLSP.h: interface for the CAlphaLSP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHALSP_H__97B9521E_7A89_11D4_A572_000021EC6C24__INCLUDED_)
#define AFX_ALPHALSP_H__97B9521E_7A89_11D4_A572_000021EC6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "AlphaProcess.h"
#include "IGOBJECT//myconstanddef.h"
//



class CDDSurface;
class CAlphaLSP 
{
public:
	static	void		Decode2_555(_SPR_LSP* pSpr); // 565 LSP 를 555 LSP 로 바꾸어준다.. 
	static	BOOL		IsIntersect(_SPR_LSP *pSpr, RECT *pRect);
	//void SetPixelFormat(int nPixelFormat);

	CAlphaLSP();
	virtual ~CAlphaLSP();

	static BOOL BltNormal(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	static BOOL BltAdd(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	static BOOL BltSoft(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	static BOOL BltColor(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaSrc, WORD wColor);
	static BOOL BltDark(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaSrc);
	static BOOL BltShadow(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	static BOOL BltTrans(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaDest, int nAlphaSrc);

	static BOOL BltShadowRoundRect(CDDSurface *pDestSurf, RECT *pDestRect);
	static BOOL BltShadowRect(CDDSurface *pDestSurf, RECT *pDestRect);

	static BOOL BltHalf(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	static BOOL BltMix(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaSrc);
	static BOOL BltMix2(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaSrc);

	static BOOL BltGray(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect, int nAlphaSrc);


/////////////////////

	static	BOOL	BltLSPShadow(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPTrans(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, int nAlphaSrc, RECT* pClipRect=NULL);

	static	BOOL	BltLSPAlphaAdd(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPAlphaSoft(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPNormal(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPMix(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, int nAlphaSrc, RECT* pClipRect=NULL);
	static	BOOL	BltLSPLight(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, int nAlphaSrc, RECT* pClipRect=NULL);


	static  BOOL	BltLSPScanHalf(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);

	static	BOOL	BltLSPHalf(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPDark(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, int nAlphaSrc, RECT* pClipRect=NULL);

	static	BOOL	BltLSPColorLevel(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, WORD wColor, RECT* pClipRect=NULL);
	static	BOOL	BltLSPColorSoft(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, WORD wColor, RECT* pClipRect=NULL);
	static	BOOL	BltLSPSetColor(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, DWORD dwColor, RECT* pClipRect = NULL);
	static	BOOL	BltLSPColor(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, WORD wColor, int nAlphaSrc, RECT* pClipRect=NULL);
	static	BOOL	BltLSPColor2(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, WORD wColor, RECT* pClipRect=NULL);


	static	BOOL	BltLSPColorDoge(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPScreen(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPDogeBurn(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPDifferent(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPLighten(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);
	static	BOOL	BltLSPDogeScreen(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, RECT* pClipRect=NULL);


	static	BOOL	BltLSPChannel(CDDSurface *pSurface, POINT ptPos, _SPR_LSP* pSpr, int nChannelOffset, RECT* pClipRect=NULL);



	static	BOOL	GetClippedRect(RECT* pRectClip, RECT *pRectLSP, RECT* pRectRegion);

	static	void	HandleFocus(WORD *pDest, int nCopyCount, int nY);
	
};


inline BOOL CAlphaLSP::GetClippedRect(RECT* pRectClip, RECT *pRectLSP, RECT *pRectRegion)
{
	ASSERT(pRectRegion);

	BOOL bUseXClip = FALSE;
	*pRectClip = *pRectLSP;


	if(pRectLSP->left < pRectRegion->left)
	{ 
		pRectClip->left = pRectRegion->left;
		bUseXClip = TRUE;
	}
	if(pRectLSP->right > pRectRegion->right)
	{ 
		pRectClip->right = pRectRegion->right;
		bUseXClip = TRUE;
	}

	if(pRectLSP->top < pRectRegion->top)
	{
		pRectClip->top = pRectRegion->top;
	}
	if(pRectLSP->bottom > pRectRegion->bottom)
	{
		pRectClip->bottom = pRectRegion->bottom;
	}
	return bUseXClip; 
}

inline void	CAlphaLSP::HandleFocus(WORD *pDest, int nCopyCount, int nY)
{
	//0x07e0;
	//1111 1000 0001 1111
 	*(pDest-1) = (WORD)(0xF81F);	// 0x001f 
	*(pDest+nCopyCount) = (WORD)(0xF81F);
	if (nY==0)
	{
		int count = nCopyCount;
		while (count)
		{
			*(pDest+count) = (WORD)(0xF81F);
			count--;
		}
		//continue;
	}  
} 


#endif // !defined(AFX_ALPHALSP_H__97B9521E_7A89_11D4_A572_000021EC6C24__INCLUDED_)
