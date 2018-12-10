

#if !defined(AFX_SENCRYPTION_H__24E85DFA_7A9E_4707_8A6A_BDEB26C34932__INCLUDED_)
#define AFX_SENCRYPTION_H__24E85DFA_7A9E_4707_8A6A_BDEB26C34932__INCLUDED_

#include <windows.h>

class SEncryption
{
public:
	SEncryption();
	void			SetKey();
	void			Encode_Decode(LPBYTE lpTarget, LPBYTE lpSource, long nLen);

	BYTE			m_Key[8];
};

#endif