// AlphaProcess.h: interface for the CAlphaProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHAPROCESS_H__B36D8028_1829_11D3_8422_00105A6B97E2__INCLUDED_)
#define AFX_ALPHAPROCESS_H__B36D8028_1829_11D3_8422_00105A6B97E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>


#define COLORDOGE		0
#define SCREEN			1
#define DOGEBURN		2
#define DIFFERENT		3
#define LIGHTEN			4
#define DOGESCREEN		5


class CDirectDraw;
class CDDSurface;





// 디더링 매트릭스 
#define MATX 2
#define MATY 2

class CAlphaProcess  
{
public:
	static short DitherMatrix[MATY][MATX];

	static DWORD m_dwRMask;
	static DWORD m_dwGMask;
	static DWORD m_dwBMask;
	static DWORD m_dwRBMask;
	static DWORD m_dwHalf16Mask;
	static DWORD m_dwHalf32Mask;

	static DWORD m_dwAlphaMask1;
	static DWORD m_dwAlphaMask2;

	static DWORD m_dwR32Mask;
	static DWORD m_dwG32Mask;
	static DWORD m_dwB32Mask;

	static int	m_nRMaskOffset;
	static int	m_nGMaskOffset;
	static int	m_nBMaskOffset;

	static WORD	m_table[6][32][32];

	static int	m_nPixelFormat;

	typedef struct HSV
	{
		float h;
		float s;
		float v;
	}HSV;

	typedef struct RGB
	{
		int r;
		int g;
		int b;
	}RGB;

	static void	InitBitMask(int nPF);

	//static BOOL BltNormal(CDDSurface *pDestSurf, RECT *pDestRect, CDDSurface *pSrcSurf, RECT *pSrcRect);
	//Dest Surface의 DestRect에 color를 byAmount에 지정한 만큼 추가시킴
	static	BOOL	ToTheColor(CDDSurface* pDestSurface, RECT *pDestRect, COLORREF color, BYTE byAmount);

	//마스크와 Surface의 공통 영역구해옴
	static	RECT	GetAdjustedRect(CDDSurface *pSurface, RECT *pRect, RECT &alphaRect);


	//중심점을 이용한 Blt (지정한 위치에 Mask의 중심이 오도록 찍는다)
	static	RECT	GetAdjustedRect(CDDSurface *pSurface, POINT pos, RECT &alphaRect);

	// berserk(1999.11.10)
	static void		FadeRect(CDDSurface *pDestSurface, int alphaDepth, WORD wColor, RECT rcDest, int pixelFormat);

//////////////////////////////////////////////////
//	Coded By : Dino
//	Coded On 2000-03-02 오후 2:08:39
//
	static void LightBlit(CDDSurface *pDestSurface, BYTE *pAlphaMask, WORD wColor, RECT rcDest, int pixelFormat);
	static void LightBlit2(CDDSurface *pDestSurface, BYTE *pAlphaMask, WORD wColor, RECT rcDest, int pixelFormat);

//	End Of Code (By Dino On 2000-03-02 오후 2:08:39 )
//////////////////////////////////////////////////	
	static void LightBlit3(CDDSurface *pDestSurface, RECT r, 
		CDDSurface *pSrcSurface, RECT sr, int pixelFormat, int nMinAlpha=32);



	// 엄청난 크기의 인라인 함수.. 속도에 매우 민감하다. - BltAlphaAdd.
	static inline void PutPixelNormal(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelAdd(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDest, int nAlphaSrc); // nAlphaSrc 와 nAlphaDest 의 합은 반드시 32 이어야 한다..
	static inline void PutPixelDark(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc); // nAlphaSrc - 0 이 젤 시커멓다.. 32 가 제일 밝다.
	static inline void PutPixelShadow(const WORD* pDest, const WORD* pSrc, int nCopyCount); // 0 시커먼 그림자.. 32 그림자 없음..
	static inline void PutPixelShadow2(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, WORD wColor); // nAlphaSrc - 스프라이트의 밝기 값. dwDestColor - 컬러의 32비트 값. (반드시 "32-nAlphaSrc"의 비율과 일치하여야 한다.
	static inline void PutPixelColor2(const WORD* pDest, const WORD* pSrc, int nCopyCount, WORD wColor);

	static inline void PutPixelHalf(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelHalf2(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelMix(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc); // nAlphaSrc - 0 이 젤 시커멓다.. 32 가 제일 밝다.
	static inline void PutPixelMix2(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc); // nAlphaSrc - 0 이 젤 시커멓다.. 32 가 제일 밝다.

	static inline void PutPixelBlack(const WORD* pDest, const WORD* pSrc, int nCopyCount);

	// pSrc의 Blue색의 Level값을 wColor의 Level값으로 취하여 pDest에 그린다. 
	static inline void PutPixelColorLevel(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD wColor); 
	// pSrc의 값과 wColor의 값의 중간값을 pDest에 그린다. (ColorLevel에 비하여 속도는 빠르다)
	static inline void PutPixelColorSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD wColor);
	static inline void PutPixelSetColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwDestColor);
	static inline void PutPixelLight(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc);


	static inline void PutPixelColorDoge(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelScreen(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelDogeBurn(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelDifferent(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelLighten(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static inline void PutPixelDogeScreen(const WORD* pDest, const WORD* pSrc, int nCopyCount);


	static inline void PutPixelGray(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc);


	static inline void PutPixelChannel(const WORD* pDest, const WORD* pSrc, int nCopyCount, BYTE* pChannel);

	
	//static inline void SHIFT_LEFT(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	//static inline void SHIFT_RIGHT(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	//static inline void SKIP_RED(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	//static inline void SKIP_GREEN(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	//static inline void SKIP_BLUE(const WORD* pDest, const WORD* pSrc, int nCopyCount);

	CAlphaProcess();
	virtual ~CAlphaProcess();

	//void (CAlphaProcess::*m_ptrFunc[10])(const WORD* pDest, const WORD* pSrc, int nCopyCount); 

protected:
public:

	static inline void RGB_TO_HSV(RGB rgb, HSV *hsv) {
		float min, max, delta, rc, gc, bc;
		
		rc = (float)(rgb.r / 255.0f);
		gc = (float)(rgb.g / 255.0f);
		bc = (float)(rgb.b / 255.0f);
		
		max = max(rc, max(gc, bc));
		min = min(rc, min(gc, bc));
		delta = max - min;
		hsv->v= max;
		
		if (max != 0.0f)
			hsv->s= delta / max;
		else
			hsv->s = 0.0f;
		
		if (hsv->s== 0.0f) {
			hsv->h = 0.0f;
		}
		else {
			if (rc == max)
				hsv->h = (gc - bc) / delta;
			else if (gc == max)
				hsv->h = 2 + (bc - rc) / delta;
			else if (bc == max)
				hsv->h = 4 + (rc - gc) / delta;
			
			hsv->h *= 60.0f;
			if (hsv->h < 0.0f)
				hsv->h += 360.0f;
		}
	};

	static inline void HSV2RGB(HSV hsv,RGB *rgb) {
		float f, x, y, z;
		int i;
		
		hsv.v *= 255.0f;
		
		if (hsv.s == 0.0f) {
			rgb->r = rgb->g= rgb->b= (int)hsv.v;
		}
		else {
			while (hsv.h < 0)
				hsv.h += 360;
			
			hsv.h = (float)(fmod(hsv.h, 360) / 60.0f);
			i = (int)hsv.h;
			f = (float)(hsv.h - i);
			x = (float)(hsv.v * (1.0f - hsv.s));
			y = (float)(hsv.v * (1.0f - (hsv.s * f)));
			z = (float)(hsv.v * (1.0f - (hsv.s * (1.0f - f))));
			
			switch (i) {
			case 0: 
				rgb->r = (WORD)hsv.v; 
				rgb->g = (WORD)z;
				rgb->b = (WORD)x;
				break;
			case 1: 
				rgb->r = (WORD)y;
				rgb->g = (WORD)hsv.v;
				rgb->b = (WORD)x;
				break;
			case 2: 
				rgb->r = (WORD)x;
				rgb->g = (WORD)hsv.v;
				rgb->b = (WORD)z;
				break;
			case 3: 
				rgb->r = (WORD)x;
				rgb->g = (WORD)y;
				rgb->b = (WORD)hsv.v;
				break;
			case 4: 
				rgb->r = (WORD)z;
				rgb->g = (WORD)x;
				rgb->b = (WORD)hsv.v;
				break;
			case 5: 
				rgb->r = (WORD)hsv.v;
				rgb->g = (WORD)x;
				rgb->b = (WORD)y;
				break;
			}
		}
	};
};

#define AP_MIXBLT(f, s, alpha, mask) \
	((((((f&mask)-(s&mask))*(alpha)) >> 5) + (s&mask)) & (mask))

#define AP_MIXHALF(f, s, mask) \
	((((((f&mask)-(s&mask))) >> 1) + (s&mask)) & (mask))




//////////////////@@@@@
/* #define AP_MIXADD(f, s, mask) \
	(( (f&mask) + (s&mask)  ) & (mask)) */

// 엄청난 크기의 인라인 함수.. 속도에 매우 민감하다. - BltAlphaAdd.
inline void CAlphaProcess::PutPixelNormal(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
		mov ebx, ecx;
		and ebx, 1;
		jz DWORD_COPY;
		
		//COPY_LOOP:		// 16-bit copy
		movsw;
		dec ecx;
		jz END_LOOP;
		
DWORD_COPY:			
		shr ecx, 1;			// 32-bit copy	
		rep movsd;
END_LOOP:
	}
};


inline void CAlphaProcess::PutPixelLight(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc)
{
	ASSERT(nAlphaSrc>=0);
	//ASSERT(nAlphaSrc<32);

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..


		//mov edx, [esi]; // Spr Pixel
		mov dx, WORD PTR [esi]; // Spr Pixel
		push edx;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		mov eax, edx;
		and eax, dwRMask;
		mov ecx, nRMaskOffset;
		shr eax, cl;
		mul nAlphaSrc;
		shr eax, 5;

		cmp eax, 31;
		jle NEXT_1;
		mov eax, 31;
NEXT_1:
		shl eax, cl
		mov ebx, eax;

		pop edx;
		push edx;
		mov eax, edx;
		and eax, dwGMask;
		mov ecx, nGMaskOffset;
		shr eax, cl;
		mul nAlphaSrc;
		shr eax, 5;
		cmp eax, 31;
		jle NEXT_2;
		mov eax, 31;
NEXT_2:
		shl eax, cl
		add ebx, eax;

		pop edx;
		mov eax, edx;
		and eax, dwBMask;
		mul nAlphaSrc;
		shr eax, 5;
		cmp eax, 31;
		jle NEXT_3;
		mov eax, 31;
NEXT_3:
		add ebx, eax;

		mov WORD PTR [edi], bx;

		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
};




inline void CAlphaProcess::PutPixelSetColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwDestColor)
{
	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		//mov ecx, [esi]; // Spr Pixel
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		mov ebx, dwDestColor;
		mov WORD PTR [edi], bx;

//DONT_COPY:
		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
};



inline void CAlphaProcess::PutPixelBlack(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	__asm
	{
		mov edi, pDest;
		//mov esi, pSrc;
		mov ecx, nCopyCount;
		mov ebx, ecx;
		and ebx, 1;
		jz DWORD_COPY;
		
		//COPY_LOOP:		// 16-bit copy

		mov eax, 0;
		stosw;			// ax의 값을 edi위치로..

		dec ecx;
		jz END_LOOP;
		
DWORD_COPY:			
		shr ecx, 1;			// 32-bit copy	
		//shl eax, 16;
		rep movsd;
END_LOOP:
	}
}



inline void CAlphaProcess::PutPixelAdd(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	__asm
	{
		mov edi, pDest;	// 백그라운드 주소 저장.
		mov esi, pSrc;	// 스프라이트 주소 저장.

		mov ecx, nCopyCount; // 가로 길이.

COPY_LOOP:
		push ecx;
		mov cx, WORD PTR [esi]; // Spr

		jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		//mov ebx, 0; // 최종 픽셀

		mov eax, ecx; // Spr - R
		mov dx, WORD PTR [edi]; // Bkg - R
		and eax, dwRMask;
		and edx, dwRMask;
		add edx, eax;
		cmp edx, dwRMask;
		ja COPY_R1;
		jmp ADD_R1;
COPY_R1:
		mov edx, dwRMask;
ADD_R1:
		//or ebx, edx;
		mov ebx, edx;

		mov eax, ecx; // Spr - G
		mov dx, WORD PTR [edi]; // Bkg - G
		and eax, dwGMask;
		and edx, dwGMask;
		add edx, eax;
		cmp edx, dwGMask;
		ja COPY_G1;
		jmp ADD_G1;
COPY_G1:
		mov edx, dwGMask;
ADD_G1:
		or ebx, edx;

		mov eax, ecx; // Spr - B
		mov dx, WORD PTR [edi]; // Bkg - B
		and eax, dwBMask;
		and edx, dwBMask;
		add edx, eax;
		cmp edx, dwBMask;
		ja COPY_B1;
		jmp ADD_B1;
COPY_B1:
		mov edx, dwBMask;
ADD_B1:
		or ebx, edx;

		mov WORD PTR [edi], bx; // 백버퍼에 씀
		
DONT_COPY:
		add edi, 2;
		add esi, 2;

		pop ecx;
		dec ecx;
		jnz COPY_LOOP;
	}
};


inline void CAlphaProcess::PutPixelSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	
	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	__asm
	{
		mov edi, pDest;	// 백그라운드 주소 저장.
		mov esi, pSrc;	// 스프라이트 주소 저장.

		mov ecx, nCopyCount; // 가로 길이.

COPY_LOOP:
		push ecx; // 카운트 기억

		mov cx, WORD PTR [esi]; // 스프라이트 픽셀

		jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..
		
		//mov ebx, 0; // 최종 픽셀 값..

		//mov ecx, [esi]; // Spr 픽셀
		
		mov dx, WORD PTR [edi]; // Bkg - R
		push edx;
		mov eax, ecx // Spr - R
		and edx, dwRMask;
		and eax, dwRMask;
		cmp eax, edx;
		jb COPY_R1_BKG;
		mov edx, eax
COPY_R1_BKG:
		//or ebx, edx;
		mov ebx, edx;

		pop edx;
		push edx;
		//mov dx, WORD PTR [edi]; // Bkg - R
		mov eax, ecx // Spr - R
		and edx, dwGMask;
		and eax, dwGMask;
		cmp eax, edx;
		jb COPY_G1_BKG;
		mov edx, eax
COPY_G1_BKG:
		or ebx, edx;


		pop edx;
		//mov dx, WORD PTR [edi]; // Bkg - R
		mov eax, ecx // Spr - R
		and edx, dwBMask;
		and eax, dwBMask;
		cmp eax, edx;
		jb COPY_B1_BKG;
		mov edx, eax
COPY_B1_BKG:
		or ebx, edx;

		mov WORD PTR [edi], bx; // 백버퍼에 씀
	
DONT_COPY:
		add edi, 2;
		add esi, 2;

		pop ecx; // 카운트 복구
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CAlphaProcess::PutPixelTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDest, int nAlphaSrc) // nAlphaSrc 와 nAlphaDest 의 합은 반드시 32 이어야 한다..
{
/*		__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..

		mov ebx, 0; // 최종 색깔 초기화

		mov ecx, [esi]; // Spr Pixel

		jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		mov eax, ecx; // Spr - R B
		and eax, dwRBMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax;

		mov eax, ecx; // Spr - G
		and eax, dwGMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax;
		
		mov ecx, [edi]; // Bkg Pixel

		mov eax, ecx; // Bkg - R B
		and eax, dwRBMask;
		mul nAlphaDest;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax;
		
		mov eax, ecx; // Bkg - G
		and eax, dwGMask;
		mul nAlphaDest;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax;
		
		mov [edi], bx;
DONT_COPY:
		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
*/	};

inline void CAlphaProcess::PutPixelDark(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc) // nAlphaSrc - 0 이 젤 시커멓다.. 32 가 제일 밝다.
{
	ASSERT(nAlphaSrc>=0);
	ASSERT(nAlphaSrc<32);


	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..

		mov ebx, 0; // 최종 색깔 초기화

		mov ecx, [esi]; // Spr Pixel

		jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		mov eax, ecx; // Spr - R B
		and eax, dwRBMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax;
		
		mov eax, ecx; // Spr - G
		and eax, dwGMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax;
		
		mov [edi], bx;

DONT_COPY:

		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CAlphaProcess::PutPixelShadow(const WORD* pDest, const WORD* pSrc, int nCopyCount) // 0 시커먼 그림자.. 32 그림자 없음..
{

	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;


	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;

		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..

		mov cx, WORD PTR [esi]; // 알파값 계산
		jcxz DONT_COPY; // 소스가 검은 색이면 넘어간다.
		and ecx, 0x001f; // 
		mov ebx, 0x001f;
		sub ebx, ecx;
		mov ecx, ebx;

		mov ebx, 0; // 최종 색깔 초기화

		mov ax, WORD PTR [edi]; // Bkg - R B
		push eax;

		and eax, dwRBMask;
		mul ecx;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax;
		
		//mov ax, WORD PTR [edi]; // Bkg - G
		pop eax;
		and eax, dwGMask;
		mul ecx;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax;
		
		mov WORD PTR [edi], bx;

DONT_COPY:
		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	} 
}; 

// nAlphaSrc - 스프라이트의 밝기 값. dwDestColor - 컬러의 32비트 값. (반드시 "32-nAlphaSrc"의 비율과 일치하여야 한다.
inline void CAlphaProcess::PutPixelColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, WORD wColor) 
{
	ASSERT(nAlphaSrc>=0);
	ASSERT(nAlphaSrc<32);

	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;

	DWORD dwAlphaMask1 = m_dwAlphaMask1;
	DWORD dwAlphaMask2 = m_dwAlphaMask2;

	int nAlphaDst = 31 - nAlphaSrc;
	DWORD dwColor = (wColor << 16) | wColor;

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

//COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov ax, WORD PTR [esi]; // Spr Pixel
		//cmp eax, 0;
		//jz DONT_COPY;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov cx, wColor;//WORD PTR [edi]; // Bkg - R B

		and eax, dwRBMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBMask;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwRBMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax; 
	
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwGMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGMask;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwGMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax; 
	
		mov WORD PTR [edi], bx;
//DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx;
		dec ecx;
		jz  END_LOOP;

		// 여기서부터는 32bit(DWORD)계산.
DWORD_HALF_MIX: 
		shr ecx, 1;
		

COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov eax, [esi]; // Spr Pixel
		//cmp eax, 0;
		//jz DONT_COPY;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov ecx, dwColor; //[edi]; // Bkg - R B

		and eax, dwAlphaMask1;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwAlphaMask1;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask1;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwAlphaMask1;
		add ebx, eax; 
	

		// dwAlphaMask2는 Mul 연산을 먼저할 경우 OverFlow가 있다.
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaSrc;
		and eax, dwAlphaMask2;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaDst;
		and eax, dwAlphaMask2;
		add ebx, eax; 
	
		mov [edi], ebx;
//DONT_COPY:

		add edi, 4;
		add esi, 4;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	} 
};



// dwDestColor - 컬러의 16비트 확장 값.
inline void CAlphaProcess::PutPixelColor2(const WORD* pDest, const WORD* pSrc, int nCopyCount, WORD wColor) 
{

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;


	int colorR = (wColor & dwRMask ) >> nRMaskOffset;
	int colorG = (wColor & dwGMask ) >> nGMaskOffset;
	int colorB = (wColor & dwBMask );

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
		//mov edx, nAlpha;
COPY_LOOP:
		push ecx; // 카운트 저장..

		// mov ebx, 0; // 최종 색깔 초기화

		mov ax, WORD PTR [esi]; // Spr Pixel
		push eax;

		and eax, dwRMask;

		mov dx, wColor;
		and edx, dwRMask;
		mov ecx, nRMaskOffset;
		shr edx, cl;
		mul edx;

		shr eax, 5;
		shr eax, cl;

		cmp eax, 31;
		jle NEXT_1;
		mov eax, 31;
NEXT_1:
		shl eax, cl;
		mov ebx, eax;


		//mov eax, [esi]; // Spr Pixel
		pop eax;
		push eax;
		and eax, dwGMask;

		mov dx, wColor;
		and edx, dwGMask;
		mov ecx, nGMaskOffset;
		shr edx, cl;
		mul edx;

		shr eax, 5;
		shr eax, cl;

		cmp eax, 31;
		jle NEXT_2;
		mov eax, 31;
NEXT_2:
		shl eax, cl;
		add ebx, eax;

		

		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwBMask;

		mov dx, wColor;
		and edx, dwBMask;
		//mov ecx, nBOffset;
		//shr edx, cl;
		mul edx;

		shr eax, 5;
		//shr eax, cl;

		cmp eax, 31;
		jle NEXT_3;
		mov eax, 31;
NEXT_3:
		//shl eax, cl;
		add ebx, eax;
		
		
		
		mov WORD PTR [edi], bx;
// DONT_COPY:

		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
};


inline void CAlphaProcess::PutPixelShadow2(const WORD* pDest, const WORD* pSrc, int nCopyCount) 
{
	DWORD dwHalf16Mask = m_dwHalf16Mask;
	DWORD dwHalf32Mask = m_dwHalf32Mask;

	__asm
	{
		mov edi, pDest;
		mov ecx, nCopyCount;

		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

		mov dx, WORD PTR [edi]; // Dest Spr Pixel
		and edx, dwHalf16Mask;
		shr edx, 1;
		mov WORD PTR [edi], dx;		// ax는 16bit Register.

	
		add edi, 2;
		//pop ecx; // 카운트 복구..
		dec ecx;
		jz  END_LOOP;

		// 여기서부터는 32bit(DWORD)계산.
DWORD_HALF_MIX: 
		shr ecx, 1;
		
COPY_LOOP:
		mov edx, [edi]; // 
		and edx, dwHalf32Mask;
		shr edx, 1;
		mov [edi], edx;

		 
//DONT_COPY:
		add edi, 4;
		
		//pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	} 
}; 

inline void CAlphaProcess::PutPixelHalf(const WORD* pDest, const WORD* pSrc, int nCopyCount) // nAlphaSrc - 0 이 젤 시커멓다.. 32 가 제일 밝다.
{
	DWORD dwHalf16Mask = m_dwHalf16Mask;
	DWORD dwHalf32Mask = m_dwHalf32Mask;

	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

		//push ecx; // 카운트 저장..
	
		mov ax, WORD PTR [esi]; // Spr Pixel
		mov dx, WORD PTR [edi]; // Dest Spr Pixel
		and eax, dwHalf16Mask;
		and edx, dwHalf16Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, wMask;
		mov WORD PTR [edi], ax;		// ax는 16bit Register.
		
		add edi, 2;
		add esi, 2;
		//pop ecx; // 카운트 복구..
		dec ecx;
		jz  END_LOOP;

		// 여기서부터는 32bit(DWORD)계산.
DWORD_HALF_MIX: 
		shr ecx, 1;
		
COPY_LOOP:
		//push ecx; // 카운트 저장..
		//mov ebx, 0; // 최종 색깔 초기화
		
		mov eax, [esi]; // Spr Pixel
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..
		mov edx, [edi]; // 
		and eax, dwHalf32Mask;
		and edx, dwHalf32Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, dwMask;
		mov [edi], eax;

//DONT_COPY:
		add edi, 4;
		add esi, 4;
		
		//pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	}
};

inline void CAlphaProcess::PutPixelHalf2(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	DWORD dwHalf16Mask = m_dwHalf16Mask;

	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;


COPY_LOOP:
		//push ecx; // 카운트 저장..
		//mov ebx, 0; // 최종 색깔 초기화
		
		mov ax, WORD PTR [esi]; // Spr Pixel
		jcxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..
		mov dx, WORD PTR [edi]; // 
		and eax, dwHalf16Mask;
		and edx, dwHalf16Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, dwMask;
		mov WORD PTR [edi], ax;

DONT_COPY:
		add edi, 2;
		add esi, 2;
		
		//pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
//END_LOOP:
	}
};


inline void CAlphaProcess::PutPixelMix(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc)
{
	ASSERT(nAlphaSrc>=0);
	ASSERT(nAlphaSrc<32);

	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;

	DWORD dwAlphaMask1 = m_dwAlphaMask1;
	DWORD dwAlphaMask2 = m_dwAlphaMask2;

	int nAlphaDst = 31 - nAlphaSrc;

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

//COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov ax, WORD PTR [esi]; // Spr Pixel
		//cmp eax, 0;
		//jz DONT_COPY;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov cx, WORD PTR [edi]; // Bkg - R B

		and eax, dwRBMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBMask;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwRBMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax; 
	
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwGMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGMask;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwGMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax; 
	
		mov WORD PTR [edi], bx;
//DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx;
		dec ecx;
		jz  END_LOOP;

		// 여기서부터는 32bit(DWORD)계산.
DWORD_HALF_MIX: 
		shr ecx, 1;
		

COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov eax, [esi]; // Spr Pixel
		//cmp eax, 0;
		//jz DONT_COPY;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov ecx, [edi]; // Bkg - R B

		and eax, dwAlphaMask1;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwAlphaMask1;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask1;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwAlphaMask1;
		add ebx, eax; 
	

		// dwAlphaMask2는 Mul 연산을 먼저할 경우 OverFlow가 있다.
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaSrc;
		and eax, dwAlphaMask2;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaDst;
		and eax, dwAlphaMask2;
		add ebx, eax; 
	
		mov [edi], ebx;
//DONT_COPY:

		add edi, 4;
		add esi, 4;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	}
};


inline void CAlphaProcess::PutPixelMix2(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc)
{
	ASSERT(nAlphaSrc>=0);
	ASSERT(nAlphaSrc<32);

	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;

	DWORD dwAlphaMask1 = m_dwAlphaMask1;
	DWORD dwAlphaMask2 = m_dwAlphaMask2;

	int nAlphaDst = 31 - nAlphaSrc;

	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

//COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov ax, WORD PTR [esi]; // Spr Pixel
		cmp ax, 0;
		jz DONT_COPY16;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov cx, WORD PTR [edi]; // Bkg - R B

		and eax, dwRBMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBMask;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwRBMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwRBMask;
		add ebx, eax; 
	
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwGMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGMask;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwGMask;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax; 
	
		mov WORD PTR [edi], bx;
DONT_COPY16:

		add edi, 2;
		add esi, 2;

		pop ecx;
		dec ecx;
		jz  END_LOOP;

		// 여기서부터는 32bit(DWORD)계산.
DWORD_HALF_MIX: 
		shr ecx, 1;
		

COPY_LOOP:
		push ecx; // 카운트 저장..

		//mov ebx, 0; // 최종 색깔 초기화

		mov eax, [esi]; // Spr Pixel
		cmp eax, 0;
		jz DONT_COPY32;
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..

		push eax;
		mov ecx, [edi]; // Bkg - R B

		and eax, dwAlphaMask1;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwAlphaMask1;
		//add ebx, eax;
		mov ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask1;
		mul nAlphaDst;
		shr eax, 5;
		and eax, dwAlphaMask1;
		add ebx, eax; 
	

		// dwAlphaMask2는 Mul 연산을 먼저할 경우 OverFlow가 있다.
		//mov eax, [esi]; // Spr Pixel
		pop eax;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaSrc;
		and eax, dwAlphaMask2;
		//add ebx, eax;
		or	ebx, eax;

		mov eax, ecx;
		and eax, dwAlphaMask2;
		shr eax, 5;
		mul nAlphaDst;
		and eax, dwAlphaMask2;
		add ebx, eax; 
	
		mov [edi], ebx;
DONT_COPY32:

		add edi, 4;
		add esi, 4;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	}
};


inline void CAlphaProcess::PutPixelColorLevel(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD wColor) 
{
	//DWORD dwMask = 0x03e07c1f;		//0000 0011 1110 0000 0111 1100 0001 1111

	DWORD dwRBMask = m_dwRBMask;
	DWORD dwGMask = m_dwGMask;


	__asm // RGB 값마다 강도 계산..
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
COPY_LOOP:
		push ecx; // 카운트 저장..
		
		//mov ebx, 0; // 최종 색깔 초기화
		
		mov cx, WORD PTR [esi]; // Spr Pixel
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..
		//mov edx, [edi]; // Bkg - R B
		
		
		//mov edx, ecx; // Spr - R B
		and ecx, 0x00000000001f;	// Src의 Blue Color-Level..
		
		mov eax, wColor;
		and eax, dwRBMask;
		mul ecx;			// mul 연산때문에 edx를 사용할 수 없음. 
		shr eax, 5;
		and eax, dwRBMask;
		mov ebx, eax;
		
		mov eax, wColor;
		and eax, dwGMask;
		mul ecx;
		shr eax, 5;
		and eax, dwGMask;
		add ebx, eax;
		
		mov WORD PTR [edi], bx;
		//DONT_COPY:
		
		add edi, 2;
		add esi, 2;
		
		pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
	}
};


inline void CAlphaProcess::PutPixelColorSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD wColor) 
{
	//DWORD dwMask = 0x7bde7bde;
	//DWORD  wMask = 0x00007bde;		//  0111 1011 1101 1110

	DWORD dwHalf16Mask = m_dwHalf16Mask;
	DWORD dwHalf32Mask = m_dwHalf32Mask;

	DWORD dwColor = (wColor<<16) | wColor;
	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
		
		// nCopyCount가 홀수인지 짝수인지를 판단. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_PROCESS;
		
		//push ecx; // 카운트 저장..
		
		mov ax, WORD PTR [esi]; // Spr Pixel
		mov edx, wColor; // Dest Spr Pixel
		and eax, dwHalf16Mask;
		and edx, dwHalf16Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, wMask;
		mov WORD PTR [edi], ax;		// ax는 16bit Register.
		
		add edi, 2;
		add esi, 2;
		//pop ecx; // 카운트 복구..
		dec ecx;
		jz  END_LOOP;
		
		// 여기서부터는 32bit(DWORD)계산.
DWORD_PROCESS: 
		shr ecx, 1;
		
COPY_LOOP:
		//push ecx; // 카운트 저장..
		//mov ebx, 0; // 최종 색깔 초기화
		
		mov eax, [esi]; // Spr Pixel
		//jecxz DONT_COPY; // 스프라이트 색깔이 0 이면... 처리 안함..
		mov edx, dwColor; // 
		
		and eax, dwHalf32Mask;
		and edx, dwHalf32Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, dwMask;
		mov [edi], eax;
		
		//DONT_COPY:
		add edi, 4;
		add esi, 4;
		
		//pop ecx; // 카운트 복구..
		dec ecx;
		jnz COPY_LOOP;
END_LOOP:
	}
};



inline void CAlphaProcess::PutPixelChannel(const WORD* pDest, const WORD* pSrc, int nCopyCount, BYTE* pChannel) 
{
	ASSERT(nCopyCount>0);

	//WORD MaskR=8,MaskB=8,MaskG=8,MaskRShift=10,MaskGShift=5;
	//if (nPixelFormat == 565) {MaskG=4;MaskRShift=11;}
	//int red,green,blue;

	BYTE alpha;
	RGB rgb, backrgb;

	/* WORD dat,MaskR=8,MaskB=8,MaskG=8,MaskRShift=10,MaskGShift=5;
	if (nPixelFormat == 565) {MaskG=4;MaskRShift=11;}
	int redC,greenC,blueC;
	*/

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;


	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;



	WORD *pDst = (WORD*)pDest;

	while (nCopyCount--) {
		rgb.r = ((*pSrc & dwRMask) >> nRMaskOffset)<<3;
		rgb.g = ((*pSrc & dwGMask) >> nGMaskOffset)<<3;
		rgb.b = ((*pSrc & dwBMask))<<3;

		alpha = *pChannel;
		ASSERT(alpha>0);


		if (alpha!=255) {
			backrgb.r = ((*pDst & dwRMask) >> nRMaskOffset)<<3;
			backrgb.g = ((*pDst & dwGMask) >> nGMaskOffset)<<3;
			backrgb.b = ((*pDst & dwBMask))<<3;



			// 255 bit right-shift(8) & div 2(1);
			rgb.r = (rgb.r * alpha + backrgb.r * (255-alpha)) >> 9;
			rgb.g = (rgb.g * alpha + backrgb.g * (255-alpha)) >> 9;
			rgb.b = (rgb.b * alpha + backrgb.b * (255-alpha)) >> 9;



			/* HSV hsv, backhsv;
			RGB_TO_HSV(rgb,&hsv);
			RGB_TO_HSV(backrgb,&backhsv);
			if (backhsv.v!=1) {
				hsv.h=backhsv.h;
				hsv.v=backhsv.v-(float)((1.0-hsv.v)/2);
				hsv.s=backhsv.s;
				if (hsv.v<0) {
					hsv.v=0;
					hsv.h=0;
					hsv.s=0;
				}

				HSV2RGB(hsv,&rgb);
			} */
		}
		*pDst = (WORD)(
					((rgb.r>>3)<<nRMaskOffset)&dwRMask | 
					((rgb.g>>3)<<nGMaskOffset)&dwGMask | 
					((rgb.b>>3))&dwBMask
					);

		pSrc++;
		pDst++;
		pChannel++;
	}
};



inline void CAlphaProcess::PutPixelColorDoge(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;


	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;


	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[COLORDOGE][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[COLORDOGE][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[COLORDOGE][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};

inline void CAlphaProcess::PutPixelScreen(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	
	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[SCREEN][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[SCREEN][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[SCREEN][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};

inline void CAlphaProcess::PutPixelDogeBurn(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[DOGEBURN][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[DOGEBURN][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[DOGEBURN][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};

inline void CAlphaProcess::PutPixelDifferent(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[DIFFERENT][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[DIFFERENT][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[DIFFERENT][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};

inline void CAlphaProcess::PutPixelLighten(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;

	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[LIGHTEN][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[LIGHTEN][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[LIGHTEN][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};

inline void CAlphaProcess::PutPixelDogeScreen(const WORD* pDest, const WORD* pSrc, int nCopyCount)
{
	WORD r, g, b, e, d;
	WORD *pDst = (WORD*)pDest;


	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	for (int i=0; i<nCopyCount; i++) {
		e = pSrc[i];
		d = pDst[i];

		r = m_table[DOGESCREEN][(d&dwRMask)>>nRMaskOffset][(e&dwRMask)>>nRMaskOffset];
		g = m_table[DOGESCREEN][(d&dwGMask)>>nGMaskOffset][(e&dwGMask)>>nGMaskOffset];
		b = m_table[DOGESCREEN][(d&dwBMask)         ][(e&dwBMask)         ];

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};


inline void CAlphaProcess::PutPixelGray(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc)
{

	WORD r, g, b, mid; 
	WORD *pDst = (WORD*)pDest;


	DWORD dwRMask = m_dwRMask;
	DWORD dwGMask = m_dwGMask;
	DWORD dwBMask = m_dwBMask;

	int	nRMaskOffset = m_nRMaskOffset;
	int	nGMaskOffset = m_nGMaskOffset;

	for (int i=0; i<nCopyCount; i++) {
		r = (WORD)(pSrc[i]&dwRMask)>>nRMaskOffset;
		g = (WORD)(pSrc[i]&dwGMask)>>nGMaskOffset;
		b = (WORD)(pSrc[i]&dwBMask);

		
		mid = (r+g+b)/3;

		if (r>mid)	r = max(r-nAlphaSrc, mid);
		else 		r = min(r+nAlphaSrc, mid);

		if (g>mid)	g = max(g-nAlphaSrc, mid);
		else 		g = min(g+nAlphaSrc, mid);
		
		if (b>mid)	b = max(b-nAlphaSrc, mid);
		else 		b = min(b+nAlphaSrc, mid);

		pDst[i] = (WORD)( (r << nRMaskOffset) | (g << nGMaskOffset) | (b) );
	}
};


#endif // !defined(AFX_ALPHAPROCESS_H__B36D8028_1829_11D3_8422_00105A6B97E2__INCLUDED_)
