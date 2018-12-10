/*****************************************************************
|
|      XAudio Player Wrapper Class    
|
|      (c) 1996-1998 MpegTV, LLC
|      Author: Gilles Boccon-Gibod (gilles@mpegtv.com)
|
|
|      This class implements a thin C++ wrapper around
|      the Xaudio ASYNC API.
|      A typical player impementation would subclass this base 
|      class, and overload some of the stubbed notification 
|      funtions.
|      To send commands to the player, just use the functions
|      inherited from XA_Player.
|      To receive notification, overload the notification
|      functions in the subclass to implement your own 
|      notication handlers.
|
 ****************************************************************/

#ifndef __XAUDIO_PLAYER_H__
#define __XAUDIO_PLAYER_H__

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#ifndef STRICT
#define STRICT
#endif

#include <windows.h>
#include "include/player.h"

#pragma comment(lib, "lib\\xaudio.lib")


/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/
#define XA_REQUIRED_VERSION_MAJOR 3
#define XA_REQUIRED_VERSION_MINOR 8

/*----------------------------------------------------------------------
|       XaudioPlayer Class
+---------------------------------------------------------------------*/
class XaudioPlayer : public XA_Player
{
public:
            XaudioPlayer(HINSTANCE instance);
   virtual ~XaudioPlayer();

protected:
    HWND      m_HiddenWindow;
	HINSTANCE m_Instance;
    TCHAR    *m_ClassName;
};

#endif /* __XAUDIO_PLAYER_H__ */