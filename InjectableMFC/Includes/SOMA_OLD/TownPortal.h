#ifndef TOWNPORTAL_H
#define TOWNPORTAL_H
#include "LSPResMagic.h"

#define TOWNPORTAL_SOUND	 14

class CDDSurface;

class TP_INFO
{
public:
	TP_INFO();
	TP_INFO(int nIndex);
	~TP_INFO();
public:
	int		m_nUid;
	int		m_nTPIndex;	// Town Portal Index Number
	int		m_nCurFrame;	// The Current frame in the animation
	int		m_nFrame;
	DWORD	m_dwLastTick;
	DWORD	m_dwLastSoundTick;
	BYTE	m_bActionFlag;
};

class TownPortal
{
public:
	TownPortal();
	~TownPortal();
	void ReleaseUnusedSpr();
	void RemoveAllData();
	bool LoadTPRes(int nIndex);
	CLSPResMagic* GetIndexedTP(int index);
	bool AddTPRef(int nIndex, int uid, BYTE bActionFlag);
	bool AddRefInOrder(TP_INFO* pRef);
	void DrawUserTP(CDDSurface *pSurface, int iUid);
	void DrawAllTP(CDDSurface *pSurface);
	bool DrawTP(CDDSurface *pSurface, int x, int y, TP_INFO* pTPInfoRef, int nCurIndex, int Alpha = 1);
	void SendCompletedTPAck(BYTE bActionFlag, int uid);
protected:
	CArray<TP_INFO* ,TP_INFO*> m_arrTPInfo;
};

#endif