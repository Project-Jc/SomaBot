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
	void		SprRelease(); // ��������Ʈ ������ ������
	//COLORREF	SrcCKGet() { return m_HSpr.SrcCK; } // �ҽ� �÷�Ű ��������
	_SPR_LSP* __fastcall GetSpr(int nIndex, int nType=0);

	BOOL SprLoad(const char* szFileName); // ��������Ʈ ���� �ҷ�����

private:
	void	CreatePalette(RGBQUAD *rgbColor);

	BOOL	SprLoad(int nIndex, int nType);
	void	Decode2_555(_SPR_LSP* pSpr); // 565 LSP �� 555 LSP �� �ٲپ��ش�..
	void	Decode2_565(_SPR_LSP* pSpr); // 555 LSP �� 565 LSP �� �ٲپ��ش�..


	void	Swap_R_G(_SPR_LSP* pSpr); 
	void	Swap_R_B(_SPR_LSP* pSpr); 
	void	Swap_G_B(_SPR_LSP* pSpr);

	void	Rote_Clk(_SPR_LSP* pSpr);
	void	Rote_CClk(_SPR_LSP* pSpr);

//public:
	void	SprAlloc(int nQt);
	BOOL	Convert8PixelTo16Pixel(BYTE* p8Pixels, WORD *p16Pixels, RGBQUAD* pRGBTbls, int nQt); // 8��Ʈ �ε��� �ȼ��� 16��Ʈ �ȼ��� �ٲ۴�.
	WORD	RGB16(WORD r, WORD g, WORD b) { return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3); } // 565 �����϶��� �ζ��� �Լ��� ����



public:
	CObjLSP();
	virtual ~CObjLSP();

	_SPR_HEADER_SIMPLE		m_HSpr;
	WORD			m_wPalleteSize;
	WORD*			m_pwPalette;
	int*			m_pnDecodeSize;


	char			m_nLoadNear;	// ���� ������ Load�� �Բ� Load�� ������ ������ ��.
	char			m_nStyle;		// �ϴ� �ӽ÷� ���. 


protected:
	DWORD			m_dwLastTick;		// ���� �ֱٿ� Access�� �ð�.
	HANDLE			m_hFile;

public:
	_SPR_LSP*		m_pSprs[MEM_SPR];	

//private:
public:
	void Invert(_SPR_LSP *pSpr);
	CString			m_szFileName;
};

#endif // !defined(AFX_ANIOBJLSP_H_INCLUDED_)







