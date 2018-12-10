#include <vector>
#include "BBDX_ChatOuter.h"
#include "MainFrm.h"
#include "iBotUser.h"

extern int g_vMapNumber;

class iBotPath
{
public:
	std::vector<CPoint> PathBlacksmith, 
						PathPotVendor;

	bool PathSet;

	int BlkSmithUid,
		VendorUid;

public:
	iBotPath();
	~iBotPath();
	void LoadWaypoints(CString Filename, CString Type);
	void SetPaths();
	void ClearPaths();
	void LoadPathAbias();
	void LoadPathPandi();
	void LoadPathTyt();
	void LoadPathMerc();
};
