// Mp3Player.h: interface for the CMp3Player class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEPLAYER_H__28382492_09BB_11D5_A572_000021EC6C24__INCLUDED_)
#define AFX_SAMPLEPLAYER_H__28382492_09BB_11D5_A572_000021EC6C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XaudioPlayer.h"

class CMp3Player : public XaudioPlayer
{
public:
	void SetOutputVolume(int nVolume);
	const int GetOutputVolume() const { return m_nVolume; }
    // member variables
    // BOOL m_Scrolling;
    // XA_PlayerState m_State;

    // methods
    CMp3Player(HINSTANCE instance);
	~CMp3Player();

    void OnNotifyReady();
	void OnNotifyOutputState(XA_OutputState state);
	void OnNotifyInputState(XA_InputState state);
    void OnNotifyNack(XA_NackInfo *info);
	void OnNotifyPlayerState(XA_PlayerState playState);

protected:
	BOOL	m_bEnable;
	int		m_nVolume;
};

#endif // !defined(AFX_SAMPLEPLAYER_H__28382492_09BB_11D5_A572_000021EC6C24__INCLUDED_)
