#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN							// Exclude rarely-used stuff from Windows headers
#endif

//#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS		// some CString constructors will be explicit

#include <afxwin.h>								// MFC core and standard components
#include <afxext.h>								// MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>								// MFC OLE classes
#include <afxodlgs.h>							// MFC OLE dialog classes
#include <afxdisp.h>							// MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>								// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>								// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>							// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>								// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>

// Windows 
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <cstdint>

// Time
#include <atltime.h>

// Screenshots
#include <atlimage.h>

// File I/O
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::remove;
using std::reverse;
using std::find;
using std::copy;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

// DirectX
#include <ddraw.h>

// BeaEngine
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL 
#include "BeaEngine.h"
#pragma comment(lib, "BeaEngine")

// SOMA
#include "PacketDef.h"
#include "DarkDef.h"
#include "CUser.h"
#include "SocketMng.h"
#include "UserManager.h"
#include "BotUser.h"
#include "Bot.h"
#include "Gui.h"

// Hooking
#include "CodeCave.h"

// Sounds
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "resource.h"

// If defined, the bot will print messages to an allocated console.
#define DBG

// Soma defines
#define INVALID_UID -1

#define WM_SOCKET_NOTIFY	0x0373
#define BELT				0
#define INVENTORY			1

#define INV_COUNT 50
#define BELT_COUNT 4

#define REVIVE 1
#define WARP 2

#define MAX_MAGIC_BELT_ITEMS 12

#define OR ||
#define AND &&
#define IS ==

#define WM_MOUSEHWHEELSCROLL 0x020A

#define szDLLName "Dabura.dll"

#define SMPROTOCOL				(WORD)(0x8000)
#define SM_LOGIN_REQ			(SMPROTOCOL + 0)	
#define SM_LOGIN_ACK			(SMPROTOCOL + 1)	

#define SM_REGSN_REQ			(SMPROTOCOL + 8)
#define SM_REGSN_ACK			(SMPROTOCOL + 9)

#define SM_DOWN_REQ				(SMPROTOCOL + 10)
#define SM_DOWN_ACK				(SMPROTOCOL + 11)	

#define SM_GAME_REQ				(SMPROTOCOL + 12)
#define SM_GAME_ACK				(SMPROTOCOL + 13)	

#define SM_GAMEINFO_REQ			(SMPROTOCOL + 14)
#define SM_GAMEINFO_ACK			(SMPROTOCOL + 15)

#define SM_DIR_REQ				(SMPROTOCOL + 16)
#define SM_DIR_ACK				(SMPROTOCOL + 17)

#define SM_CONNECTINFO_ACK      (SMPROTOCOL + 100)
#define SM_ALIVE_ACK			(SMPROTOCOL + 999)

#define SM_AFK					(SMPROTOCOL + 0x3E7)

#define PARTYREFUSE				0x00
#define	PARTYACCEPT				0x01