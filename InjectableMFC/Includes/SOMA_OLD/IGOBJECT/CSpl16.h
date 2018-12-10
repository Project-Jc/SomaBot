#ifndef CSPL16_H
#define CSPL16_H

//#include <afxwin.h>
#include <stdio.h>
//#include <fstream.h>
//#include <windows.h>

const DWORD IMG_FLIP =  0x1;
const DWORD CREATE_24_BIT = 0x2;
const DWORD CREATE_16_BIT = 0x4;

typedef struct _SOMA_IMAGE_HEADER
{
	char szRemark[68]; // Info
	char szBMPFN[64]; // BMP FILE NAME
	int nWidth; // Width of image
	int nHeight; // Height of image
	COLORREF crCK;
	int nFrames; // No of frames in the image
} SOMA_IMAGE_HEADER, *LPSOMA_IMAGE_HEADER;

typedef struct _FRAME_POS
{
	RECT rcFrame;
	POINT pos;
} FRAME_POS, *LPFRAME_POS;
// Total size of structure: 28 bytes
typedef struct _FRAME_INFO
{
	int nXMoveTrans; // Repositioning of sprite by x
	int nYMoveTrans; // Repositioning of sprite by y
	int nXDiff;  // Difference between total width and nXMoveTrans
	int nYDiff;	 // Difference between total height and nYMoveTrans
	int nHeight;  // Height of Frame
	int nFrameSize;  // Size of frame based on the m_bCompressed size in the spl file
	int nUnknown;  // I have no idea what this is for!!!
}FRAME_INFO, *LPFRAME_INFO;


class CSpl16
{
	private:
		bool m_bCompressed;
		WORD m_wTransColRef;
		int m_intTransColRef;
		DWORD m_dwFlags;
		// The whole spl file in this exact format and structure
		SOMA_IMAGE_HEADER m_SImgHead;
		LPFRAME_POS m_pFramePos;
		WORD m_wPalleteSize;
		WORD *m_wpPallete;
		LPFRAME_INFO m_pFrameInfo;
		void *m_bpImg;
		void loadData(HANDLE hIMGFile);
		void SetUpImgPos(SOMA_IMAGE_HEADER header, LPFRAME_POS pFramePos, LPFRAME_INFO pFrameInfo);
		bool InitImgBuf(int nWidth, int nHeight, void *&w_pImg);
		void setPixelRGB565(WORD *w_pImg, int nPixel, WORD color);
		void setPixelRGB24(BYTE *bpImg, int pixel, COLORREF crC);
		WORD colorLookup(WORD *wpPallete , int nColRef);
		WORD RGB16(COLORREF crColor);
		int RGB24(WORD wColor);
		void SafeRelease(void* p);
	public:
		CSpl16();
		CSpl16(COLORREF crCK, DWORD dwFlags);
		~CSpl16();
		bool LoadSpl16(const char *szFileName);
		bool loadDlg(const char* szFileName);
		WORD* GetImg16();
		BYTE* GetImg24();
		void GetImgDim(int &nWidth, int &nHeight);
		void CleanUp();
};

#endif