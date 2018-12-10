// ObjLSP.h: interface for the CObjLSP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJANILSP_H_INCLUDED_)
#define AFX_OBJANILSP_H_INCLUDED_

//#include "AniObj.h"
#include "IGOBJECT//myconstandDef.h"
#include "AlphaLSP.h"


#define MEM_SPR 12


class CObjLSP 
{
public:
	int			GetChannelOffset(int nFrame);
	BOOL		ReleaseUnusedSpr();
	void		SprRelease(); // 스프라이트 데이터 날리기
	//COLORREF	SrcCKGet() { return m_HSpr.SrcCK; } // 소스 컬러키 가져오기
	_SPR_LSP* __fastcall GetSpr(int nIndex, int nType=0);

	BOOL SprLoad(const char* szFileName); // 스프라이트 파일 불러오기

private:
	void	CreatePalette(RGBQUAD *rgbColor);

	BOOL	SprLoad(int nIndex, int nType);
	void	Decode2_555(_SPR_LSP* pSpr); // 565 LSP 를 555 LSP 로 바꾸어준다..
	void	Decode2_565(_SPR_LSP* pSpr); // 555 LSP 를 565 LSP 로 바꾸어준다..


	void	Swap_R_G(_SPR_LSP* pSpr); 
	void	Swap_R_B(_SPR_LSP* pSpr); 
	void	Swap_G_B(_SPR_LSP* pSpr);

	void	Rote_Clk(_SPR_LSP* pSpr);
	void	Rote_CClk(_SPR_LSP* pSpr);

//public:
	void	SprAlloc(int nQt);
	BOOL	Convert8PixelTo16Pixel(BYTE* p8Pixels, WORD *p16Pixels, RGBQUAD* pRGBTbls, int nQt); // 8비트 인덱스 픽셀을 16비트 픽셀로 바꾼다.
	WORD	RGB16(WORD r, WORD g, WORD b) { return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3); } // 565 포맷일때의 인라인 함수로 선언



public:
	CObjLSP();
	virtual ~CObjLSP();

	_SPR_HEADER_SIMPLE		m_HSpr;
	WORD			m_wPalleteSize;
	WORD*			m_pwPalette;
	int*			m_pnDecodeSize;


	char			m_nLoadNear;	// 단위 프레임 Load시 함께 Load할 주위의 프레임 수.
	char			m_nStyle;		// 일단 임시로 사용. 


protected:
	DWORD			m_dwLastTick;		// 가장 최근에 Access한 시간.
	HANDLE			m_hFile;

public:
	_SPR_LSP*		m_pSprs[MEM_SPR];	

//private:
public:
	void Invert(_SPR_LSP *pSpr);
	CString			m_szFileName;
};

#endif // !defined(AFX_ANIOBJLSP_H_INCLUDED_)







