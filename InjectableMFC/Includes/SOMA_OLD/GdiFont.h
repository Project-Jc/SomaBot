// GdiFont.h: interface for the CGdiFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIFONT_H__B1608CC5_92CE_11D4_98B2_00C02615866B__INCLUDED_)
#define AFX_GDIFONT_H__B1608CC5_92CE_11D4_98B2_00C02615866B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#ifdef	KOR_VER
	#define DEFAULT_EDITOR_FONT	_T("±¼¸²Ã¼")
#else if CHN_VER
	#define DEFAULT_EDITOR_FONT	_T("SIMSUN")  
#endif

class CGdiFont // : public CFont
{
public:
	CGdiFont();
	virtual ~CGdiFont();

	BOOL CreatePointFont(int nPointSize, LPCSTR lpszFaceName);
	BOOL CreateFont(LPCSTR strFontName, int nHeight, int nWeight = FW_NORMAL);
	void DeleteFont();
	int DrawText(HDC hDC, CString str, LPRECT lpRect, UINT nFormat );
	void DrawText(HDC hDC, CString strData, CRect rtDraw, UINT nFormat,
				COLORREF color = RGB(255,255,255), COLORREF colorBk = RGB(0,0,255), int sel_start=0, int sel_end=0, int caretPos=-1);

	void TextOut(HDC hDC, CString strData, CRect rtDraw, UINT nFormat,
				COLORREF color = RGB(255,255,255), COLORREF colorBk = RGB(0,0,255), int sel_start=0, int sel_end=0, int caretPos=-1);
	void TextOut(HDC hDC, CPoint pt, LPCTSTR lpszText, COLORREF bColor=RGB(255,255,255), UINT uFlag=TA_LEFT);
	int TextOutClip(HDC hDC, int nWidth, CString lpszText, COLORREF bColor=RGB(255,255,255), 
		int caretPos=-1, int blockStart=0, int blockEnd=0, BOOL fWordBreak=TRUE);

	int	GetHeight(){return m_nHeight;}

	BOOL IsHangul(CString str, int nIndex);

public:
	//CFont	*m_pFont;
	int lpnFit[100], alpDX[100];
	SIZE lpSize[100];

	int m_nHeight;

	HFONT m_hFont;

};

#endif // !defined(AFX_GDIFONT_H__B1608CC5_92CE_11D4_98B2_00C02615866B__INCLUDED_)