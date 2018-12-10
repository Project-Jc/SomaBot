// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__180690AB_F9A1_11D2_8421_0020AF9F40BD__INCLUDED_)
#define AFX_CHILDVIEW_H__180690AB_F9A1_11D2_8421_0020AF9F40BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//

#include "SocketMng.h"
#include "UserManager.h"
#include "PacketParser.h"
//#include "MapManager/SM_MapManager.h"
#include "MapManager.h"
#include "LSPResManager.h"
#include "Weather.h"

//#include "IGObject\\DDEngine.h"
#include "IGObject\\DDEngineLSP.h"
#include "IGObject\\MapObj.h"
#include "IGObject\\AniObjLsp.h"
//#include "TestEngine.h"
#include "TileMapTool.h"
#include "TileSetTool.h"

#include "Dialog_AccountLogin.h"
#include "Dialog_Login.h"
#include "SLoadingView.h"

#include "Rain.h"
#include "LightSource.h"
#include "house.h"
#include "TransObj.h"

#define MAX_RAIN 150
//#include "FogEffect.h"

#define WM_LOGOUT_CANCEL	(WM_USER+200)

//((�紁E)
//#include "Item.h"
//#include "Item_Arm.h"
//#include "Item_sArm.h"

class BBDX_Editor;
class BBDX_ChatEditor;
class BBDX_ChatOuter;
//��E�.
class CDialog_Login;

//��E�
typedef struct  _tgCharData 
{
	BYTE   IdSize;
	char   *userID;
	short  Class;
	short  Level;
	short  Str;
	short  Dex;
	short  Inteli;
	short  Cha;
	short  Wis;
	short  Con;
	short  Hp;
	short  MaxHp;
	short  Mp;
	short  MaxMp;
	short  Stm;
	short  MaxStm;
	short  Age;
	short  Moral;
	short  Gender;
	short  Hair;
	short  Skin;
	CString  Group;
	short  Fame;
	int    SwordExp;
	int    SpearExp;
	int    BowExp;
	int    AxeExp;
	int    KnuckleExp;
	int    StaffExp;
	int    WeaponMakeExp;
	int    ArmorMakeExp;
	int    AccMakeExp;
	int    PotionMakeExp;
	int    CookingExp;
	short  ItemCheck[10];

public:
	_tgCharData()
	{
		userID = NULL;
	}
	~_tgCharData()
	{
		if(userID) { delete [] userID; userID = NULL; }
	}
}CharData;
//��E�.

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();
	virtual ~CChildView();

// Attributes
public:
	static bool g_bWOTW;
	static bool	g_bMyCharDevil;
	static CUserManager		UserManager;		//User ��E��� (Server�� ���������� ����E��-������ ����ȭ)
	static DirectXMng		DirectXMng;			//DirctX ��E���
	static SocketMng		SocketMng;			//Socket ��E���
	static MapManager		MapManager;			//Map ��E���
	static CPacketParser	PacketParser;		//������ ���� ���� ��Ŷ �м���E
	static CWeather			Weather;

	static CDDEngineNew     g_pTEng;            // ����E����E.
//	static CDDEngineLSP     g_pTEng;            // ����E����E.
/*
	static CAniObjLSP		MapAniObjs[MAX_MAP_OBJ_REG];
	static CAniObjLSP		MapShadowAniObjs[MAX_MAP_OBJ_REG];
	static CAniObjLSP		MapAniHouseIn[MAX_MAP_OBJ_REG];
	static CAniObjLSP		MapAniHouseOut[MAX_MAP_OBJ_REG];*/

	CRain					m_Rain[MAX_RAIN];
//	CTransObj				m_TrnasObj;
//	static CLightSource		m_LightSource;
//	static CArray<LIGHT_INFO,LIGHT_INFO> m_ptLightPos;
	static CFont			*pAppFont;
	static int				bDlgIn;
//	static CFogEffect       FogEffect;          //�Ȱ� ȿ��E..

//	static CItem            Item;
//	static CItem_Arm        Item_Arm;

//public:
//	static CItem_Arm        Item_Arm;
//	static CItem_sArm        Item_sArm;

// Operations
	int m_nQtObjNeededRender; // �������� �Ǿ��� ��(ȭ�鳻�� �־��� ��) ����E�Ʈ ����E: ����E9.7
	struct _OBJECT_DATA // ĳ���� �����ϰ�E�׻��̿� ����E�Ʈ�� �������ҷ���E����E����ü : ����E9.7
	{
		POINT pt; // ����Ʈ ���� ����ǥ
		int nSet; // ��Ʈ ��ȣ
		int nSubSet; // ����E� ��ȣ
	};
	_OBJECT_DATA m_ObjStructs[256]; // ��ȭ�鿡 256 �� ���Ϸ� ����E
public:
    int m_S_Connection;// ���ǿ� ����Ǿ�E�ִ°�?

	// newAccount 
	CString m_New_Account_ID;
	CString m_New_Account_Passwd, m_New_Account_Passwd2;
	CString m_New_Account_Name;
	CString m_New_Account_SocNo;
	CString m_New_Account_Address;
	CString m_New_Account_TellNo;
	CString m_New_Account_Email;

	bool    m_bAccountNewOk;
	bool    m_bLoginOk;

	// ��E�
	SLoadingView	 *m_LoadinView;
	CharData		*m_CharData[3];
	bool			 m_bMakeChar;
	bool			 m_bMakeFail;
	bool			 m_bDelFail;
	bool			 m_bMakeProcess;
	int				 m_CharNum; //�����݁Eĳ���� ����E

	CString			 m_conIP; //������ ip...
	int				 m_Port;
	void  InitChar();
	// ��E�.

/*
	BBDX_Editor*	pBBDX_Editor_id;//1
	BBDX_Editor*	pBBDX_Editor_password;//2
	BBDX_Editor*	pBBDX_Editor_password2;//3 �н�����EȮ��
	BBDX_Editor*	pBBDX_Editor_name;//4
	BBDX_Editor*	pBBDX_Editor_email;//5
	BBDX_Editor*	pBBDX_Editor_number;//6 �ֹι�ȣ
	BBDX_Editor*	pBBDX_Editor_address;//7 �ּ�
	BBDX_Editor*	pBBDX_Editor_phonenumber;//8�� ��E���ȣ
	BBDX_Editor*	pBBDX_Editor_phonenumber2;//9 ����Ǵ� �ڵ�ƁE..
*/
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	static void SetSelectInfoUser(CUser *pUser, bool bFocus);
//	int m_limit_old_y;
//	void SetOld_limit_y( int limit_y ){ m_limit_old_y = limit_y; }

	/*
	void ArrangeTileResource();

	void RemoveUnUseLSP();

	void DrawShawdowObj();
	void DrawShawdowObj(CDDSurface *pSurface);

	void DrawShawdowObjEx();

	void DrawObj(int limit_y);
	void DrawObjEx(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);
/*
	void DrawHouseIn(int x, int y, CPoint org_pos, CPoint offset_pos, BOOL bAbove = FALSE);//���ȳ��� �׷��ֱ�E
	CPoint DrawMyInHouseAndInCheck(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//�� �׷��ֱ�E�� �ȿ� ������
	void DrawCommonObj(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//�Ϲ� ����E�Ʈ �׸���E
	CPoint DrawMyInObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���ȿ� ���̬���� �׷��ִ� �Լ�E
	void DrawMyOutObj(int limit_y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���ۿ� ������ �׷��ִ� �Լ�E
	void DrawMyOutHouse(int y, CPoint org_pos, CPoint offset_pos, CPoint x_range, BOOL bAbove = FALSE);//���׷��ֱ�E���� ���ۿ� ������ 
	void DrawTransOutHouse(int x, int y, CPoint org_pos, CPoint offset_pos, int nAlphaSrc, int nAlphaDst);
*/

	BOOL FieldCheck(CPoint pt, int x, int y, RECT* pRC, WORD* data);
	BOOL FieldCheck(CPoint pt, int x, int y, RECT* pRC, BYTE* data);
	BOOL LineCheck(CPoint pt, int x, int y, RECT* pRC, WORD* data);
	BOOL LineCheck(CPoint pt, int x, int y, RECT* pRC, BYTE* data);

//	CHouse m_house;
	int m_Mouse_In;
	void DrawObject();
	void DrawHousePlace();
//	void DrawTile(int type =0);
    int GetItemArmTypeIndex(int i); //��E�.. 10��E16��..
	int GetItemDrawOrderIndex(int i);
	int GetItemPicIndex(int i);
	void ReadItemIndex();
	void ViewSelectBox(CRect rBox, int color=0);
	void Magic_Draw();
	void ChatMode(int type);
	void DrawObjcet();
	void DrawRains();
	void DrawRainDrops();
//	void DrawNight();
//	void DrawNight1(BOOL bRight);
//	void BackLoading(CPoint pos);
//	void BackLoadingEx(CPoint pos);
void CallBack_MessageBox(int nId, int nValue);
	void SendLivePacket();
	void ClearScreen();
	int m_Old_Item_Type[9];
	static void FieldInfoItemProcess(PBYTE bypData);
	int m_Login_Char;
	void NewCharter();
	void LoginCharterView();
    static  void CharacterDataProcess(PBYTE bypData);
	//��E�.. static -> no static
	void DelHeroProcess(PBYTE bypData);
	void NewHeroProcess(PBYTE bypData);
	//��E�.
	static  void ThrowItemProcess(PBYTE bypData);
	static	void UseItemProcess(PBYTE bypData);
	static  void PickUpItemProcess(PBYTE bypData);
	static  void OutItemProcess(PBYTE bypData);
	static  void GetItemProcess(PBYTE bypData);
	static	void MoneyChangeProcess(PBYTE bypData);
	
	static  CPoint PixelToCellUnit(CPoint ptPixel);
	static  CPoint CellUnitToPixel(CPoint ptCell);

	int m_test_Flag;
	void SendDelHero(CString id);
	BOOL SendNewHero(CString id, CDialog_Login *Dialog_Login);
	
	BOOL LoginAcount(CString id, CString password);
	BOOL LoginAcountNew();
	BOOL InitLogin();
	void GameLogin();
	void LoginView();
	void Login_Window();
	BOOL LogIn();
	BOOL m_First_Menu;
	void Rail_MagicProcess();
	void AttackMagic(int suid, int euid, int magic_type);
	static BOOL ChangeItem(int index);
	int m_Arry_ItemIndex[50];
	int m_Arry_ItemType[50];
	void Search_Item();
	void Item_MouseProcess(BBDX_MouseInformation *pMouseInfo, int Dlg_Pos, int Select_Box, int MoveFlag = 1);
	void MagicItem_MouseProcess(BBDX_MouseInformation *pMouseInfo, int Dlg_Pos, int Select_Box, int MoveFlag = 1);
	void Item_Init();

	//DSOMA ITEMS
	int m_DItem_Pic_Index[1000];// �׸���ȣ�� ���߱�E���ؼ� ����E
	int m_DArm_type[1000]; //10��E16�� ��E�..
	int m_DDraw_Order[1000];

	// HSOMA ITEMS
	int m_Item_Pic_Index[1000];// �׸���ȣ�� ���߱�E���ؼ� ����E
	int m_Arm_type[1000]; //10��E16�� ��E�..
	int m_Draw_Order[1000];

	bool m_RestartGame;

	void					SetMainMode(int mode);

	//DirectInput ����E� ó���Լ�E
	int						MouseProcess();	
	void					AccountMouseProcess(int *id_flag);
	void					LoginMouseProcess(CDialog_Login *Dialog_Login);
	void					LBtnDown(CPoint &pos);
	void					LBtnPressing(CPoint &pos);
	void					LBtnUp(CPoint &pos);
	void					RBtnDown(CPoint &pos);
	void					RBtnPressing(CPoint &pos);
	void					RBtnUp(CPoint &pos);
	void					MouseMove(CPoint &pos, BOOL lClick = FALSE);

	void					InitStaticVariables();

	void					PacketProcess();
	BOOL					SocketInit();
	BOOL					SocketRelease();
	BOOL					IdleProcess(LONG turn);
	BOOL					NFocusIdleProcess(LONG turn);
	BOOL					DDRelease();	//DirectX ����E (Application �����)
	BOOL					DDInit();		//DirectX �ʱ�ȭ (Application ���۽�)
	void					SetMyUserId(int uid);			//�ڽ��� UserId (������ ����)
	int						GetMyUserId() {return m_iMyId;}

	void					CreateEditor();							//Editor����(��ũ���̹��� ����)
	BBDX_Editor*			GetEditor(){return m_pBBDX_Editor;}		//	"
	BBDX_ChatOuter*			GetChatOuter(){return m_pChatOuter;}	//	"
	BBDX_Editor*			GetChatOuterid(){return m_pBBDX_Editor_id;}	//	"
	BBDX_Editor*			GetChatOuterpassword(){return m_pBBDX_Editor_password;}	//	"
	int                     m_Chattype;// ä�� Ÿ��... 1: ��E� ä�� 2: ����E...
	CString                 m_CurMemoChatId;
	CString                 GetCurMemoChatId(){ return m_CurMemoChatId;}
	void	                SetCurMemoChatId(CString id_name){ m_CurMemoChatId = id_name;}
	void					DisplayDate();
	void					DisplayDate1();

CChildView*		m_bpConnectedView;

	void					ReStart();
	void					Notice(PBYTE bypData);

	void					OutputMapDebug();

	void					Mp3PlayStart(int nType = 0); // Plays an MP3
	void					CaptureScreenForJpg(); // Capture the screen for a jpg screenshot
	void					SetLogoutCount(bool bMode, int nType = 0); // Logout countdown
	void					AddLogMsg(CString msg, int size);

protected:
	BBDX_Editor*			m_pBBDX_Editor_id;
	BBDX_Editor*			m_pBBDX_Editor_password;
	BBDX_ChatOuter*			m_pChatOuter;
	BBDX_Editor*		m_pBBDX_Editor;
	CPoint					m_ptMapOffset;		//Pixel������ Offset(����׷�����E�ִ� ������ Offset
	int						m_iMyId;
	DWORD                   m_PathFindGap;
	DWORD					m_RightMouseGap;

	bool                    m_bCtrlUp;
	int						m_nLogoutCount; // Logout coutdown
	int						m_nLogoutType; // Restart or Full

	//Ű��E� ������E�������� �ƴϳ� �̰͵��� �ӽ÷� �翁Eɰ͵��̴�.....
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg	LRESULT OnPacketProcess(WPARAM, LPARAM);
	afx_msg	LRESULT	OnPacketPending(WPARAM, LPARAM);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void LBDB(CPoint &pos);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) ;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeEndComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeCompositionFull(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg	LRESULT OnLogoutCancel(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__180690AB_F9A1_11D2_8421_0020AF9F40BD__INCLUDED_)
