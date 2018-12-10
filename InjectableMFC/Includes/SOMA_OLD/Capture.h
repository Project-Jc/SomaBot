// Capture1.h: interface for the CCapture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPTURE1_H__D4514E34_2D47_401E_9621_56952D1D0A64__INCLUDED_)
#define AFX_CAPTURE1_H__D4514E34_2D47_401E_9621_56952D1D0A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define BFT_BITMAP 0x4d42
#define HDIB HANDLE
#define PALVERSION   0x300
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define MAKE_ZERO(exp) memset(&(exp), 0, sizeof(exp))

class CCapture
{

public:
	CCapture();
	//RECT wndRect;
	static void CaptureScreen(HWND hWnd, char* szFileName);
	static WORD FAR DibSave(HANDLE hDib, LPSTR lpFileName);
	static HPALETTE FAR GetSystemPalette();
	static WORD FAR PaletteSize(LPSTR lpDIB);
	static WORD FAR DIBNumColors(LPSTR lpDIB);
	static HDIB FAR CopyScreenToDIB(LPRECT lpRect);
	static HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	static HDIB FAR BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal);
	static int FAR PalEntriesOnDevice(HDC hDC);


	enum {
      ERR_MIN = 0,                     // All error #s >= this value
      ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!
      ERR_MEMORY,                      // Not enough memory!
      ERR_READ,                        // Error reading file!
      ERR_LOCK,                        // Error on a GlobalLock()!
      ERR_OPEN,                        // Error opening a file!
      ERR_CREATEPAL,                   // Error creating palette.
      ERR_GETDC,                       // Couldn't get a DC.
      ERR_CREATEDDB,                   // Error create a DDB.
      ERR_STRETCHBLT,                  // StretchBlt() returned failure.
      ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.
      ERR_STARTDOC,                    // Error calling StartDoc().
      ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.
      ERR_SETABORTPROC,                // Error calling SetAbortProc().
      ERR_STARTPAGE,                   // Error calling StartPage().
      ERR_NEWFRAME,                    // Error calling NEWFRAME escape.
      ERR_ENDPAGE,                     // Error calling EndPage().
      ERR_ENDDOC,                      // Error calling EndDoc().
      ERR_SETDIBITS,                   // Error calling SetDIBits().
      ERR_FILENOTFOUND,                // Error opening file in GetDib()
      ERR_INVALIDHANDLE,               // Invalid Handle
      ERR_DIBFUNCTION,                 // Error on call to DIB function
      ERR_MAX                          // All error #s < this value
     };

};

#endif // !defined(AFX_CAPTURE1_H__D4514E34_2D47_401E_9621_56952D1D0A64__INCLUDED_)
