#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CInjectableMFCApp : public CWinApp {
public:
	CInjectableMFCApp();
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CInjectableMFCApp theApp;