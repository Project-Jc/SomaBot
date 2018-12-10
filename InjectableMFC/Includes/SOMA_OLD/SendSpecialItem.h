// Dialog_Message3.h: interface for the CDialog_Message3 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SEND_SPECIAL_ITEM_H
#define SEND_SPECIAL_ITEM_H

#include "UIRes.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSendSpecialItem
{
public:
	bool IfSpecial(short nSpecial);
	void HandleSpecialItem(CUIRes* pParent, byte byBelt, short nIndex);
	void HandleMessageBox(int nId, int nValue);
	static void CallBack_MessageBox(int nId, int nValue);

private:
	short m_nSpecial;
	byte m_byBelt;
	short m_nIndex;
};

#endif // SEND_SPECIAL_ITEM_H