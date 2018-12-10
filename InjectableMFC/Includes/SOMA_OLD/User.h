// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__5A5901E5_3853_11D3_8422_00105A6B97E2__INCLUDED_)
#define AFX_USER_H__5A5901E5_3853_11D3_8422_00105A6B97E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "illstruct.h"

#include "MyMagicData.h"
#include "MySpecialAttackData.h"
#include "MyAbilityData.h"
#include "ItemData.h"
#include "ObjAniLSP.h"

class DirectXMng;
class CCharManager;
class CCharRes;
class CLSPResChar;
class CDDSurface;
class CUserManager;
class CMagic;
class CArmatureManager;
class CArmature;

//class CItemData;
const int DEFAULT_GREY_TIME = 30000; // 30 seconds before name starts flashing
const int FLASH_TIME = 200; // Time delay between flashes when grey

#define MOVEPOS_KIND_NEXT	0
#define MOVEPOS_KIND_END	1

#define AM_KIND_NUM		8

//0:�Ǽ�,��Ŭ, ��E��� 1:�Ѽհ� 2:��հ� 3: â 4:Ȱ 5:����,�ظ�, ��E� 6:������ 7: ���� 8:��Į(����)
#define NOTHINGARM      0           // Punch / Knuckle
#define SINGLESWORD     1           // 1 handed swords
#define BIGSWORD        2 // ��հ� // 2 handed swords
#define SPEAR           3 // Ȱ     // SPEAR
#define BOW             4            // BOW
#define BIGAXE          5           // 2 Handed Axe
#define WAND            6 //������E������ // Staffs
#define CROSSBOW        7           // XBOX
//#define DOUBLESWORD     8 //��Į Finito - DOESN'T EXIST 
#define SINGLEAXE	9	    // 1 Handed Axe

typedef struct {
	int kind;
	CPoint ptPos;
} MOVEPOS;		//Move�� ��û�� Routing Data����E��

typedef struct _TAG_FLY_END_MAGIC {
	int		m_nMagicIndex;//���� �ε���
	BOOL	m_bType;//���� Ÿ��
	int		m_nPoisonKind;//������ ����E
	int		m_nTableNum;//���� ���̺�E��ȣ
} FLY_END_MAGIC;

typedef struct _TAG_APPLIED_CON_MAGIC {
	DWORD		m_dwContime;//���ӽð�
	short		m_nTableNum;//���� ���̺�E��ȣ
} APPLIED_CON_MAGIC;

typedef struct _TAG_APPLIED_CON_SPECIAL {
	DWORD		m_dwContime;//���ӽð�
	short		m_nTableNum;//���� ���̺�E��ȣ
	int			m_iSorUid;
	bool		m_bActive;
} APPLIED_CON_SPECIAL;

#define		SPECIAL_MAGIC		0
#define		BLUE_MAGIC			1
#define		WHITE_MAGIC			2
#define		BLACK_MAGIC			3

typedef CTypedPtrArray<CPtrArray, CMyMagicData *> CMyMagicDataArray;
typedef CMap<int, int, CMyMagicData *, CMyMagicData *> CMyMagicDataMap;

typedef CTypedPtrArray<CPtrArray, CMySpecialAttackData *> CMySpecialAttackDataArray;
typedef CMap<int, int, CMySpecialAttackData *, CMySpecialAttackData *> CMySpecialAttackDataMap;

typedef CTypedPtrArray<CPtrArray, CMyAbilityData *> CMyAbilityDataArray;
typedef CMap<int, int, CMyAbilityData *, CMyAbilityData *> CMyAbilityDataMap;


#define	INV_COUNT				50
#define BELT_COUNT				4


class CUser {
public:
	short GetInv_ItemUsage(int index);
	int GetInv_ItemIndex(int index);
	int GetInv_ItemType(int index);
	int GetInv_ItemPic(int index);
	int GetInv_ItemArm(int index);
	CString GetInv_ItemName(int index);

	int GetJangchk_Item(int index);
	void Change_View_item(short index, short picnum);
	void Chang_Item(short fromindex, short toindex);
	void SetInvAll(PBYTE bypData);
	void			SetWearItem(int nType, int nIndex, int btype = -1, int barm = -1);
	void ChangeItemInfo(BYTE bBelt, BYTE nSpec, CItemData& inv_data);

	CString Attack(CUser *ptmp);
	CString Defense(CUser *ptmp);
	CString MAttack(CUser *ptmp);
	CString MDefense(CUser *ptmp);

	bool			HealthAbovePercent(int iPercent);
	void			SetSpecial(int nIndex, int nMagicType, int height = 0, bool bForceNew = false);

	void			SetMagic(int nIndex, int nMagicType, int height = 0);

	void			SendNextMoveReq(CPoint ptPosAllowed);
	void			SetMoveFirst(CPoint ptDst, CPoint ptFirst);
	void			SetMoveMiddle(CPoint ptDst);					//������ ó��
	void			SetMoveEnd(CPoint ptDst);
	void			SetMove(CPoint ptDst);

	void			SendNextRunMoveReq(CPoint ptPosAllowed, CPoint nextpt, CPoint m_ptDestFinalPos);
	void			SetRunMoveFirst(CPoint ptDst, CPoint ptFirstv, CPoint ptNext);
	void			SetRunMoveMiddle(CPoint ptDst, CPoint ptNext);					//������ ó��
	void			SetRunMoveEnd(CPoint ptDst, CPoint ptNext);
	void			SetRunMove(CPoint ptDst, CPoint ptNext);

	void			SetMoveList(CList<CPoint, CPoint>* pRoute);
	static CList<CPoint, CPoint>* CalcMoveList(CPoint ptStart, CPoint ptEnd, int Direction = 0);
	CList<CPoint, CPoint>* CalcMoveList2(CPoint ptStart, CPoint ptEnd, int FindStep);
	int				GetUserId() { return m_iUserId; }
	void			SetUserId(int id) { m_iUserId = id; }
	// Custom
	bool			m_bSlowed;
	int				m_TargetID;
	CString			m_TargetStr;
	bool			IsAggressive();
	bool			BlackListed;
	bool			IsAttacking();
	bool			HasFreeInvSpace();
	int				NearestEmptyInvSlot();
	bool			IsMagicMob();
	// End

	int             m_MeDraw_Order[AM_KIND_NUM];

	int             m_CurAniSprNumber;
	int             m_AniSprNumber;
	BOOL			m_bDying;
	BOOL			m_bDeadFlag;
	int				m_bDead;
	int				m_vMoveStep;
	enum UserMotion {
		UM_Stop = 0,
		UM_Walk,
		UM_Attack,
		UM_Beated,
		UM_Magic,
		UM_Die,
		UM_RUN,


		/*		UM_StopInAttack,
				UM_WalkInAttack,
				UM_Run,
				UM_AttackSecond,

				UM_Push,
				UM_Dash,
				UM_Defence,
				UM_Ko,


				/*		UM_Stop = 0,
				UM_StopInAttack,
				UM_Walk,
				UM_WalkInAttack,
				UM_Run,
				UM_Attack,
				UM_AttackSecond,
				UM_Magic,
				UM_Push,
				UM_Dash,
				UM_Defence,
				UM_Ko,
				UM_Die,
				UM_Beated,
				*/
	} m_AnimationIndex, m_prevAnimation;
	BOOL	SetMotion(enum UserMotion um);	//Animation
	//	void	ActionMotion(enum UserMotion um, int delay_time);	//Animation ����: �̵�, ����, ����, ����E��сE

	enum UserDirection	//User
	{
		UD_Down = 0,
		UD_DownLeft,
		UD_Left,
		UD_UpLeft,
		UD_Up,
		UD_UpRight,
		UD_Right,
		UD_DownRight
	} m_Direction, m_PrevDir;
	void			SetDirection(enum UserDirection ud) { m_Direction = ud; }		//ĳ������ ���⼳��(8��ǁE

	//LSP ó����ƾ
	static  void    PutPixelNormal2(const WORD* pDest, const WORD* pSrc, int nCopyCount, const DWORD color, int alpha, DWORD dwRBBitMask, DWORD dwGBitMask);

	static	void	PutPixelNormal(const WORD* pDest, const WORD* pSrc, int nCopyCount);
	static	void	PutPixelAlphaAdd(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwRBitMask, DWORD dwGBitMask, DWORD dwBBitMask);
	static	void	PutPixelAlphaSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwRBitMask, DWORD dwGBitMask, DWORD dwBBitMask);
	static	void	PutPixelTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDest, int nAlphaSrc, DWORD dwRBBitMask, DWORD dwGBitMask); // nAlphaSrc �� nAlphaDest �� ���� �ݵ�� 32 �̾��� �Ѵ�..
	static	void	PutPixelDark(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, DWORD dwRBBitMask, DWORD dwGBitMask); // nAlphaSrc - 0 �� �� ��Ŀ�ݴ�.. 32 �� ���� ���.
	static	void	PutPixelShadow(const WORD* pDest, int nCopyCount, int nAlphaDest, DWORD dwRBBitMask, DWORD dwGBitMask); // 0 ��Ŀ�� �׸���.. 32 �׸��� ����..
	static	void	PutPixelColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, DWORD dwDestColor, DWORD dwRBBitMask, DWORD dwGBitMask); // nAlphaSrc - ��������Ʈ�� �ుE��. dwDestColor - �÷��� 32��Ʈ ��. (�ݵ�� "32-nAlphaSrc"�� ������E��ġ�Ͽ��� �Ѵ�.
	static	void	PutPixelHalf(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static  void	PutPixelShadowEx(const WORD* pDest, int nCopyCount, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static  void CUser::PutPixelGray(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc);
	static	void	PutPixelBitStepTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDestBit, int nAlphaSrcBit, DWORD dwRBGBitMask, DWORD dwGRBBitMask);

	static	void	BltLSPShadow(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, int alpha);
	static	void	BltLSPShadowEx(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static	void	BltLSPShadowEx(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static	void	BltLSPTrans(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, int alpha, BOOL bFocus = FALSE);
	static	void	BltLSPAlphaAdd(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data);
	static	void	BltLSPAlphaAdd(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pallete);
	static	void	BltLSPNormal(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, BOOL bFocus = FALSE, int nPoisonKind = 0, int nFocusColor = 3, WORD wFlashCharColor = 0, bool bParalyzed = false);
	static	void	BltLSPNormal(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pPallete, BOOL bFocus = FALSE, int nPoisonKind = 0, int nFocusColor = 3, WORD wFlashCharColor = 0, bool bParalyzed = false);
	static	void	BltLSPDark(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, int strength = 1);
	static	void	BltLSPHalfAlpha(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static	void	BltLSPHalfAlpha(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pallete, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static	void	BltLSPHalfAlphaEx(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static	void	BltLSPHalfAlphaEx(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pallete, DWORD dwHalf16Mask, DWORD dwHalf32Mask);
	static  void	BltLSPDrawLine(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, int skip_line = 0);
	static  void	BltLSPDrawObj(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data);
	static  void	BltLSPDrawObj(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pallete);
	static  void	BltLSPBitStepTrans(CDDSurface *pSurface, int x, int y, RECT* pRC, WORD* data, int nAlphaSrc, int nAlphaDst);
	static  void	BltLSPBitStepTrans(CDDSurface *pSurface, int x, int y, RECT* pRC, BYTE* data, WORD* pallete, int nAlphaSrc, int nAlphaDst);

	static	BOOL	GetClippedRect(RECT *pRC, RECT* pRCClip);
	static  void	BltLSPChat(CDDSurface *pSurface);
	static  void	BltBlackTrans(CDDSurface *pSurface, CRect rc);


	static	CPoint	PixelToCellUnit(CPoint ptPixel);	//pixel��ǥ�� Cell��ǥ��(Cell��ǥ�� ������ �����)
	static	CPoint	CellUnitToPixel(CPoint ptCell);		//Cell��ǥ�� Pixel��ǥ��

	BOOL			IsCharStop() { return m_AnimationIndex == UM_Stop; }	//ĳ���Ͱ� ����E�E������ ����(�̵��� ���ݵ����� �ƴ�)
	BOOL			IsCharAttack() { return m_AnimationIndex == UM_Attack; }	//
	BOOL			IsCharDie() { return m_bDead == TRUE || m_bDeadFlag == TRUE; }	//
	void			Pause(BOOL tf) { m_bPauseChar = tf; }	//ĳ������ ���� �Ͻ�����EOR �Ͻ�����E����
	BOOL __fastcall	GetFocus() { return m_bFocus; }		//������ Focus����
	void __fastcall	SetFocus(BOOL tf) { m_bFocus = tf; }	//�׸��� �ڵ����� Ǯ�� (Ŀ���� ��E����� �ִ� �濁EFocus������)
	void			SetDirection(int direction);		//direction�� illstruct.h �� define��
	void			MoveNextStep_New2();					// Frame��ȭ�� �����̵���,

	void __fastcall	Move();								// �̵�ó�� (���� �Լ��翁EMoveXY, MoveX, MoveY)
	CRect&	__fastcall	GetCharRect() { return m_rCurCharRect; } //ȭ����� ǥ�ÿ��� (ĳ������ ǥ�ÿ���)
	BOOL __fastcall	GetSpr();
	void			RenderChar(CDDSurface* pSurface, CPoint PosMapOffset);	//ĳ���͸� �׸�
	void			DrawLine(CDDSurface* pSurface, CPoint PosMapOffsetint, int skip_line);

	//	void			RenderBodyAndArmature(CDDSurface *pSurface, CPoint ptScreen, RECT rcV1, RECT rcV2);
	void			RenderBodyAndArmature(CDDSurface *pSurface, CPoint ptScreen, RECT rcV1);

	void __fastcall	SetDstPos(CPoint ptDstPos);			//�̵��� ������E���� , Pixel���� ��ǥ
	void __fastcall	SetPosition(CPoint pos);			//�ʱ���ǥ ����(������ Warp����), Pixel���� ��ǥ

	void __fastcall	SetCellPosition(CPoint ptPosCell);	//������ Cell�� ĳ���� ��ġ��Ŵ
	void __fastcall	SetCellDstPos(CPoint ptPosDstCell);	//�̵��� ������ECell����

	CPoint&	__fastcall	GetPosition() { return m_Position; }	//������ ��ġ(Pixel����)
	CPoint	__fastcall	GetCellPosition() { return PixelToCellUnit(m_Position); }	//������ ��ġ(Cell ��ġ)
	void __fastcall	SetCharRes(int index, int nChar = 0);		//ĳ������ Resource����

	void			SetChatString(CString &str, bool bEmoteChatString = false);		//ä�� ���� (���߿� �Ӹ����� �翁E�)
	void			SetIdNameString(CString &str);		//ä�� ���� (���߿� �Ӹ����� �翁E�)

	CUser();
	virtual ~CUser();

	static	DirectXMng*			m_bpDirectXMng;			//DirectX ��E��� (CChildView::DirectXMng)
	static	CUserManager*		m_bpUserManager;		//����E��E��� (CChildView::UserManager)

	static	DWORD m_dwRBBitMask; // ��Ʈ ����ũ...	//LSPó����ƾ���� �翁E
	static	DWORD m_dwRBitMask;
	static	DWORD m_dwGBitMask;
	static	DWORD m_dwBBitMask;
	static	DWORD m_dwRBGBitMask;//r0b0g0
	static	DWORD m_dwGRBBitMask;//0g0r0b
	static	WORD  m_CheckKey;							 //�ȼ� ���� üũ��
	static	WORD  m_LineKey;


	//(�紁E
	CString m_strUserId;// �̰� �ۺ�ԯ���� ������ �ϴ°���..��..

	///////////////////////////////
	//protected:
public:
	void ChangeMoney(DWORD money);

	void AddMagic(short number, BYTE method1, BYTE method2, short start_time, BYTE type, BYTE target, CString name, CString explain, short distance, short use_mp, int use_time, short damage, short game_pic);
	void DelMagic(short number);

	void AddAbility(CMyAbilityData* data);
	void DelAbility(short number);

	void AddSpecialAttack(CMySpecialAttackData *data);
	void DelSpecialAttack(short number);

	void Add_Item(short index, BYTE type, BYTE arm, short picnum, BYTE gender, short min_damage, short max_damage, short weight, short limit_str, short limit_int, short limit_dex, short limit_skill, CString magic_name, CString s_num, short special[7], short sSpecialNo, DWORD value, short use_count, short max_use_count, short org_max_use_count, CString name);
	void Add_Item(PBYTE bypData, BYTE bBelt);
	void Add_Item(CItemData* item_data);
	void Del_Item(BYTE bBelt, short index);

	void ChangeBeltIndex(short from, short to);
	BOOL MoveItemToBelt(short from, short to);
	BOOL MoveItemToInv(short from, short to);
	void View_BubbleChat(CPoint bgpos);
	void View_Id(CPoint bgpos);
	void SendRevival(int type);
	void SetFlyEndMagic(int index, BOOL bType, int nTableNum, int nPoisonKind = 0);
	bool SetRunFlag(int nRunFlag);
	bool IsPixelAt(int iX, int iY);
	BOOL SetFrameChange();

	void MoveRequest(CPoint ptDstCellPos, bool bRunMode);

	int m_Run; // 1 = User Walking, 2 = User Running
	int m_Run_Flag; // Used to switch between walking and running animation smoothly.

	CArray<CMagic*, CMagic*>	m_arrMagic;	//������ ������ ����Ʈ//dspring_0720a
	CArray<CString, CString>	m_arrMagicName;//���������� ������ ��ĳ���Ϳ� ������ ���� �̸�dspring_0802_3dxx
	CArray<int, int>			m_nMagicIndex;//���������� ������ ��ĳ���Ϳ� ������ ���� �ε���dspring_0724
	CArray< FLY_END_MAGIC, FLY_END_MAGIC>		m_arrFlyEndMagic;
	CArray< APPLIED_CON_MAGIC, APPLIED_CON_MAGIC>	m_arrACM;
	CArray< APPLIED_CON_SPECIAL, APPLIED_CON_SPECIAL>	m_arrACS;

	//dummy damage
	typedef struct _DAMAGE_DISPLAY {
		short sDamage;
		DWORD dwTime;
		short sYMove;
		bool IsUser;
	} DAMAGE_DISPLAY;
	CArray<DAMAGE_DISPLAY, DAMAGE_DISPLAY> m_arrDmgDisplay;
	bool m_bDummyHasBeenHit;
	void AddDmgDisplay(short dmg, bool DrawAboveUser);
	void DrawDmgText(CPoint* pos);

	CArray<int, int>			m_arrHitMagic;

	CMagic*			m_pMagic;				//������ ����
	CString			m_szChatString;			//����E�ڽ��� ä��

	_SPR_LSP*		m_bpSprData;			//�׸��⿡ �翁E� Sprite Data (ILL Struct�� ���ǵ�)
	CObjAniLSP*		m_pObjAniLSP;

	CRect			m_rCurCharRect;			//����Eĳ������ ȭ��ǥ�ÿ���(ȭ����� ��ǥ)
	DWORD			m_dwTick;
	DWORD			m_dwLastTick;			// Used to control the frequency of updating frames	
	int				m_iSpriteIndex;			//SpriteIndex
	int				m_nFrameNum;			// The current frame number

	int				m_iCharResIndex;		//ĳ������ Resource�ε���
	int				m_nChar;		//CharRes���� Char Index (_CHR_HEADER �� nQt->ĳ���� ����)
	int				m_nCharRes;		//ĳ������ Resource��ȣ
	//	CCharRes*		m_bpCharRes;	//������ ��ȣ������ Setting�� Resource
	CLSPResChar*	m_pResChar;

	bool			m_bUserWarping; // True when a user is warping 
	bool			m_bUserInvisWarping; // True when a user is warping and needs to be invisable
	CPoint			m_Position;	// Current Position(Pixel Unit), Used for drawing the user
	CPoint          m_ptDestFinalPos; //Destination Position (Pixel Unit), Final Pos in Route

	CPoint			m_ptNextDstPosSent; // (Cell Unit)
	CPoint			m_ptLastDstPos; // (Cell Unit)

	CPoint			m_ptStartCellPos;   // Current cell where user is moving from
	CPoint			m_ptPostCellPos;	// Next Cell Pos, used to calculate user movement

	//	DWORD			m_dwMoveTick;
	DWORD           m_dwTickPrev;
	int				m_nFocusColor;


	CPoint			m_ptMoveStep;		//�̵��� ��ǥ����E

	BOOL			m_bFocus;			//Cursor������ Focus���� �ǹ�
	BOOL			m_bPauseChar;		//������ ���ۿ��� �Ͻ�����E
	int				m_nMotionStep;		//Walk�� Stop�� ������ �������� Step;
	int				m_nPoisonKind;		//������ ����E
	DWORD			m_dwTorchlight;		//ȶ�������ð�
	int				m_iUserId;			//�������� �Ҵ���� User Id

	CList<CPoint, CPoint>*	m_pRoute;	//�ڱ��ڽ��� ������ �����ؼ� ������E����
	int				m_nMagicType;		//������ ����E(�ٴ� Ÿ��? ���� Ÿ��?)

	CArmature*		m_pArmature;			//��E������ �ε���xxxxxxxxx

	//(�紁E
	// ���� ��ġ���� ����ü�� ������E������E �ƴϸ�E.������ ����Ÿ�� ������E������E����..
	//�ϴ��� ������ ��ġ��..
	//public:
	int m_nType;// ĳ������ ����E...0 ����Ȳ�� 1 ���� ���� 2 ���� ���� 3 ����Ȳ 4, 5.....11 npc....
	int m_Me;
	int m_InHouse;// -1�̸�E���̰�E..��Ÿ ��ȣ��E.����ȣ�̴�.

	int m_Level;

	DWORD m_Max_exp;
	DWORD m_Cur_exp;
	DWORD m_Cur_Pen;
	DWORD m_Max_exp_dec;
	DWORD m_Cur_Rest;

	int m_Cur_Hp;
	int m_Max_Hp;
	int m_Cur_Hp1;
	int m_Max_Hp1;

	int m_Cur_Mp;
	int m_Max_Mp;

	int m_Cur_mobHp;
	int m_Max_mobHp;

	int m_Max_Fa;
	int m_Cur_Fa;//�Ƿε�

	int m_Max_Weight;//����ִ� ���湫��.. 
	int m_Cur_Weight;

	int m_At;//���ݷ�
	DWORD m_Money;
	int m_Style;//��EǁE�.
	int m_Gender;
	BYTE m_bDeadEnable;

	//����E�庁E.
	int m_Armor;// ��ü ����
	int m_Helmet;// ���� �� �Ӹ�
	int m_Boot;//�Ź�
	int m_Jean;// ����E
	int m_Shield;// ����
	int m_Sword;// ����E
	int m_item_1;
	int m_item_2;
	int m_item_3;
	int m_item_4;
	int m_Skin;
	int m_Hair;

	int m_Arm_type;// ����E������E�ִ� ������ Ÿ�� 0:�Ǽ� 1:�Ѽհ� 2:��հ�, 3: â, 4: Ȱ, 5:����,6:��Į(����)

	int m_BattleMode;// 0 : ���⸦ ����E�ִ�. 1 : �Ϲ� ��E�E��E2 : PK ��E
	int m_PkRecive_Flag;
	int m_PkRecive_Attack_Flag;// ���� ��Ŷ�� ������..
	DWORD m_PkRecive_Attack_Time;// ������ �ð�
	DWORD m_dwBubbleTick;// ����ߤ�ÿ� �翁EҲ�..
	COLORREF m_vIdColor;

	DWORD m_dwMoveTick;
	DWORD m_dwnewMoveTick;


	DWORD m_dwAttackTick;
	DWORD m_dwBeatedTick;
	DWORD m_dwMagicTick;
	DWORD m_dwDieTick;

	DWORD m_FaTickCount;
	//�κ��丮....
	CItemData m_MyInvData[INV_COUNT];
	CItemData m_MyBeltData[BELT_COUNT];
	int		m_vInvCount;

	CMyAbilityDataArray m_arrAbility;
	CMyAbilityDataMap m_mapMyAbilityData;
	//������E�ִ� ���� ǥǁE
	CMySpecialAttackDataArray m_arrSpecialAttack;// 00 8.16�߰� �紁E..
	CMySpecialAttackDataMap m_mapMySpecialAttackData;

	CMyMagicDataArray m_arrBlueMagic;
	CMyMagicDataArray m_arrWhiteMagic;
	CMyMagicDataArray m_arrBlackMagic;
	CMyMagicDataMap m_mapMyMagicData;			// ��E� ����E

	CString	m_Designation;		// Īȣ
	CString	m_Guild;			// �Ҽ�
	int		m_Pos;				// Pos Value
	//	CString	m_Grade;			// ����
	short	m_Age;				// ����
	short	m_Deposition;		// ��ǁE
	short	m_Str;				// ȁE
	short	m_Int;				// ����
	short	m_Dex;				// ��ø
	short	m_Wis;				// ����
	short	m_Cha;				// �ŷ�
	short	m_Con;				// ü��E
	int		m_UsingSword;		// �˼�E
	int		m_UsingSpear;		// â��E
	int		m_UsingAxe;			// �б⼁E
	int		m_UsingKnuckle;		// �Ǽ�E
	int		m_UsingBow;			// �ü�E
	int		m_UsingCrook;		// ������
	int		m_MakingWeapon;		// ����E���ۼ�E
	int		m_MakingArmor;		// ���͸ ���ۼ�E
	int		m_MakingAccessory;	// ��ű� ���ۼ�E
	int		m_MakingPotion;		// �þ�E���ۼ�E
	int		m_Cooking;			// �丮
	int		m_BlackMagicExp;	// �渶�� ���ġ
	int		m_WhiteMagicExp;	// �鸶�� ���ġ
	int		m_BlueMagicExp;		// û���� ���ġ

	short	m_BloodType;

	void	SetDeposition(short deposition);

	bool    m_bFa;

	WORD	m_wFlashCharColor;
	DWORD	m_dwFlashCharColorTick;

	bool	m_bRunMode;

	// Class
	short	m_nClass; // 0 == Human, >=10 == Devil, 1000 = Human Monster, 2000 = Devil Monster
	BYTE	m_bWOTW;
	// Guild village war variables
	short	m_nGuildVW;
	int		m_iGuild;

	// Guild War Variables
	BYTE m_byGuildWar;
	short m_sGuildWarAgainst;
	BYTE m_byBlackName;

	// Guild Symbol pic var
	short	m_sGuildPicId;

	// Pk Grey Users
	BYTE	m_bGrey; // 1 = user grey, 2 = user flashing 0 = no grey
	DWORD	m_dwGreyTick; // Amount of time user has been grey for

	int		m_iRunSpeed;
	int		m_iWalkSpeed;

	void	SetGreyMode(BYTE byGreyMode);

	short m_nSpeedRate;	 // Speed rate of char in % of normal speed
	short m_nAttackRate; // Used only for attacking

	CString m_PartyLeader; // Party leader name used to check if player is in another players party or not

	bool	m_bParalyzed;
	bool	m_bBlinded;
	bool	m_bConfused;
	bool	m_bGiggled;
	bool	m_bWeakened;

	bool	m_bIsDoingMagic;
	bool	m_bIsDoingSpecial;

	DWORD	m_dwSpecialUseTime; // the time when special attack can be casted

	byte	m_byAreaState; // used to hold current area state (can't recover stam etc)

	void SetHitMagic(int iMid);
	void DelHitMagic(int iMid);
	void ApplyHitMagic();

	bool	m_bEmoteChatString; // Used to identify is a chat string is an emote and not normal chat

	//	int     m_HpRecoveryTime;
	//	DWORD   m_HpTickCount;	
	//	int     m_MpRecoveryTime;
	//	DWORD   m_MpTickCount;	

	//	void	HpRecovery();
	//	void	MpRecovery();
};

inline void CUser::SetDeposition(short deposition) {
	m_Deposition = deposition;

	if (m_Level >= 100) m_vIdColor = RGB(255, 128, 0);
	else if (m_Deposition <= -50) m_vIdColor = RGB(255, 0, 0);
	else if (m_Deposition >= -49 && m_Deposition <= -31) m_vIdColor = RGB(255, 70, 70);
	else if (m_Deposition >= -30 && m_Deposition <= -11) m_vIdColor = RGB(255, 140, 140);
	else if (m_Deposition >= -10 && m_Deposition <= -4) m_vIdColor = RGB(255, 200, 200);
	else if (m_Deposition >= -3 && m_Deposition <= 3) m_vIdColor = RGB(211, 237, 251);
	else if (m_Deposition >= 4 && m_Deposition <= 10) m_vIdColor = RGB(187, 230, 244);
	else if (m_Deposition >= 11 && m_Deposition <= 30) m_vIdColor = RGB(158, 208, 246);
	else if (m_Deposition >= 31 && m_Deposition <= 49) m_vIdColor = RGB(167, 190, 248);
	else if (m_Deposition >= 50) m_vIdColor = RGB(150, 150, 255);
}


inline void CUser::PutPixelNormal2(const WORD* pDest, const WORD* pSrc, int nCopyCount, const DWORD color, int alpha, DWORD dwRBBitMask, DWORD dwGBitMask) {
	int dstalpha = 32 - alpha;

	__asm // RGB ������ ���� �軁E.
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
	COPY_LOOP:
		push ecx; // ī��� ����E.

		mov ebx, 0; // ���� ����E�ʱ�ȭ

		mov ecx, color; // Spr Pixel

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov eax, ecx; // Spr - R B
		and eax, dwRBBitMask;
		shr eax, 5;
		mul alpha;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, color; // Spr - G
		and eax, dwGBitMask;
		mul alpha;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		mov cx, WORD PTR[esi]; // Bkg Pixel

		mov eax, ecx; // Bkg - R B
		and eax, dwRBBitMask;
		shr eax, 5;
		mul dstalpha;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov ax, WORD PTR[esi]; // Bkg - G
		and eax, dwGBitMask;
		mul dstalpha;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		mov WORD PTR[edi], bx;

	DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	}
}


// ��û�� ũ���� �ζ��� �Լ�E. �ӵ��� �ſ�E�ΰ��ϴ�. - BltAlphaAdd.
inline void CUser::PutPixelNormal(const WORD* pDest, const WORD* pSrc, int nCopyCount) {
	/*	__asm
		{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
		MOV_LOOP:
		movsw;
		dec ecx;
		jnz MOV_LOOP;
		}*/
	//__asm
	//{
	//	push edi;
	//	push esi;
	//	cld;
	//	mov edi, pDest;
	//	mov esi, pSrc;
	//	mov ebx, nCopyCount;
	//	mov eax, ebx;
	//	shr eax, 1;
	//	mov ecx, eax;
	//	rep movsd;
	//	mov eax, ebx;
	//	and eax, 1;
	//	mov ecx, eax;
	//	rep movsw;
	//	pop esi;
	//	pop edi;
	//}
	//	__asm
	//	{
	//		mov edi, pDest;
	//		mov esi, pSrc;
	//		mov ecx, nCopyCount;
	//		mov ebx, ecx;
	//		and ebx, 1;
	//		jz DWORD_COPY;
	//		
	//		//COPY_LOOP:		// 16-bit copy
	//		movsw;
	//		dec ecx;
	//		jz END_LOOP;
	//		
	//DWORD_COPY:			
	//		shr ecx, 1;			// 32-bit copy	
	//		rep movsd;
	//END_LOOP:
	//	}
	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
		mov ebx, ecx;
		and ebx, 1;
		jz DWORD_COPY;

		//COPY_LOOP:		// 16-bit copy
		movsw;
		dec ecx;
		jz END_LOOP;
	DWORD_COPY:
		shr ecx, 1;			// 32-bit copy	
		rep movsd;
	END_LOOP:
	}
};

inline void CUser::PutPixelAlphaAdd(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwRBitMask, DWORD dwGBitMask, DWORD dwBBitMask) {
	__asm
	{
		mov edi, pDest;	// ��׶��сE�ּ� ����E
		mov esi, pSrc;	// ��������Ʈ �ּ� ����E

		mov ecx, nCopyCount; // ���� ����.

	COPY_LOOP:
		push ecx;
		mov cx, WORD PTR[esi]; // Spr

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov ebx, 0; // ���� �ȼ�

		mov eax, ecx; // Spr - R
		mov dx, WORD PTR[edi]; // Bkg - R
		and eax, dwRBitMask;
		and edx, dwRBitMask;
		add edx, eax;
		cmp edx, dwRBitMask;
		ja COPY_R1;
		jmp ADD_R1;
	COPY_R1:
		mov edx, dwRBitMask;
	ADD_R1:
		or ebx, edx;

		mov eax, ecx; // Spr - G
		mov dx, WORD PTR[edi]; // Bkg - G
		and eax, dwGBitMask;
		and edx, dwGBitMask;
		add edx, eax;
		cmp edx, dwGBitMask;
		ja COPY_G1;
		jmp ADD_G1;
	COPY_G1:
		mov edx, dwGBitMask;
	ADD_G1:
		or ebx, edx;

		mov eax, ecx; // Spr - B
		mov dx, WORD PTR[edi]; // Bkg - B
		and eax, dwBBitMask;
		and edx, dwBBitMask;
		add edx, eax;
		cmp edx, dwBBitMask;
		ja COPY_B1;
		jmp ADD_B1;
	COPY_B1:
		mov edx, dwBBitMask;
	ADD_B1:
		or ebx, edx;

		mov WORD PTR[edi], bx; // ����ۿ� ��

	DONT_COPY:
		add edi, 2;
		add esi, 2;

		pop ecx;
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CUser::PutPixelAlphaSoft(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwRBitMask, DWORD dwGBitMask, DWORD dwBBitMask) {
	__asm
	{
		mov edi, pDest;	// ��׶��сE�ּ� ����E
		mov esi, pSrc;	// ��������Ʈ �ּ� ����E

		mov ecx, nCopyCount; // ���� ����.

	COPY_LOOP:
		push ecx; // ī��� �⾁E

		mov cx, WORD PTR[esi]; // ��������Ʈ �ȼ�

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov ebx, 0; // ���� �ȼ� ��..

		mov cx, WORD PTR[esi]; // Spr �ȼ�

		mov dx, WORD PTR[edi]; // Bkg - R
		mov eax, ecx // Spr - R
			and edx, dwRBitMask;
		and eax, dwRBitMask;
		cmp eax, edx;
		jb COPY_R1_BKG;
		mov edx, eax
		COPY_R1_BKG :
		or ebx, edx;

		mov dx, WORD PTR[edi]; // Bkg - R
		mov eax, ecx // Spr - R
			and edx, dwGBitMask;
		and eax, dwGBitMask;
		cmp eax, edx;
		jb COPY_G1_BKG;
		mov edx, eax
		COPY_G1_BKG :
		or ebx, edx;

		mov dx, WORD PTR[edi]; // Bkg - R
		mov eax, ecx // Spr - R
			and edx, dwBBitMask;
		and eax, dwBBitMask;
		cmp eax, edx;
		jb COPY_B1_BKG;
		mov edx, eax
		COPY_B1_BKG :
		or ebx, edx;

		mov WORD PTR[edi], bx; // ����ۿ� ��

	DONT_COPY:
		add edi, 2;
		add esi, 2;

		pop ecx; // ī��� ����
		dec ecx;
		jnz COPY_LOOP;
	}
};

// nAlphaSrc �� nAlphaDest �� ���� �ݵ�� 5�̾��� �Ѵ�..
inline void CUser::PutPixelBitStepTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDestBit, int nAlphaSrcBit, DWORD dwRBGBitMask, DWORD dwGRBBitMask) {
	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_ALPHA_MIX;
		/////////////////////////////////
		// ���ȼ� ó�����ִ� �κ�
		/////////////////////////////////
		add edi, 2;
		add esi, 2;
		dec ecx;
		jz  END_LOOP;

	DWORD_ALPHA_MIX:
		shr ecx, 1;

	COPY_LOOP:
		push ecx;
		//first

		mov eax, [esi];
		and eax, dwRBGBitMask;
		shr eax, 5;
		mul nAlphaSrcBit;

		and eax, dwRBGBitMask;
		mov ecx, eax;

		mov eax, [esi];
		and eax, dwGRBBitMask;
		mul nAlphaSrcBit;

		shr eax, 5;
		and eax, dwGRBBitMask;
		or  ecx, eax;
		//		or  eax, ecx;


		//second
		mov eax, [edi];
		and eax, dwRBGBitMask;
		shr eax, 5;
		mul nAlphaDestBit;

		and eax, dwRBGBitMask;
		mov ebx, eax;

		mov eax, [edi];
		and eax, dwGRBBitMask;

		mul nAlphaDestBit;
		shr eax, 5;
		and eax, dwGRBBitMask;
		or  eax, ebx;

		add  eax, ecx;//�λ����ϱ�E

		mov[edi], eax;

		add edi, 4;
		add esi, 4;

		pop ecx;
		dec ecx;
		jnz COPY_LOOP;

	END_LOOP:
	}
}

inline void CUser::PutPixelTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDest, int nAlphaSrc, DWORD dwRBBitMask, DWORD dwGBitMask) // nAlphaSrc �� nAlphaDest �� ���� �ݵ�� 32 �̾��� �Ѵ�..
{
	__asm // RGB ������ ���� �軁E.
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
	COPY_LOOP:
		push ecx; // ī��� ����E.

		mov ebx, 0; // ���� ����E�ʱ�ȭ

		// mov ecx, [esi]; // Spr Pixel
		mov cx, WORD PTR[esi]; // Spr Pixel

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov eax, ecx; // Spr - R B
		and eax, dwRBBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, ecx; // Spr - G
		and eax, dwGBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		mov ecx, [edi]; // Bkg Pixel

		mov eax, ecx; // Bkg - R B
		and eax, dwRBBitMask;
		mul nAlphaDest;
		shr eax, 5;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, ecx; // Bkg - G
		and eax, dwGBitMask;
		mul nAlphaDest;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		mov WORD PTR[edi], bx;

	DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CUser::PutPixelDark(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, DWORD dwRBBitMask, DWORD dwGBitMask) // nAlphaSrc - 0 �� �� ��Ŀ�ݴ�.. 32 �� ���� ���.
{
	__asm // RGB ������ ���� �軁E.
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
	COPY_LOOP:
		push ecx; // ī��� ����E.

		mov ebx, 0; // ���� ����E�ʱ�ȭ

		mov cx, WORD PTR[esi]; // Spr Pixel

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov eax, ecx; // Spr - R B
		and eax, dwRBBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, ecx; // Spr - G
		and eax, dwGBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		mov WORD PTR[edi], bx;

	DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CUser::PutPixelShadowEx(const WORD* pDest, int nCopyCount, DWORD dwHalf16Mask, DWORD dwHalf32Mask) // nAlphaSrc - 0 �� �� ��Ŀ�ݴ�.. 32 �� ���� ���.
{
	__asm
	{
		mov edi, pDest;
		mov ecx, nCopyCount;

		// nCopyCount�� Ȧ������E¦�������� �Ǵ�. 
		mov eax, ecx;
		and eax, 1;
		jz	DWORD_HALF_MIX;

		mov ax, [edi]; // Dest Spr Pixel
		and eax, dwHalf16Mask;
		shr eax, 1;
		mov[edi], ax;		// ax�� 16bit Register.

		add edi, 2;
		dec ecx;
		jz  END_LOOP;

		// ���⼭���ʹ� 32bit(DWORD)�軁E
	DWORD_HALF_MIX:
		shr ecx, 1;

	COPY_LOOP:
		mov eax, [edi]; // 
		and eax, dwHalf32Mask;
		shr eax, 1;
		mov[edi], eax;

		add edi, 4;

		dec ecx;
		jnz COPY_LOOP;
	END_LOOP:
	}
};

inline void CUser::PutPixelShadow(const WORD* pDest, int nCopyCount, int nAlphaDest, DWORD dwRBBitMask, DWORD dwGBitMask) // 0 ��Ŀ�� �׸���.. 32 �׸��� ����..
{
	__asm // RGB ������ ���� �軁E.
	{
		mov edi, pDest;
		//					mov esi, pSrc;
		mov ecx, nCopyCount;
	COPY_LOOP:
		push ecx; // ī��� ����E.

		mov ebx, 0; // ���� ����E�ʱ�ȭ

		mov cx, WORD PTR[edi]; // Bkg Pixel

		mov eax, ecx; // Bkg - R B
		and eax, dwRBBitMask;
		//		mul nAlphaDest;		
		//		shr eax, 5;
		shr eax, 1;		// nAlphaDest = 16�ϰ濁E(�׸����ϰ濁E
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, ecx; // Bkg - G
		and eax, dwGBitMask;
		//		mul nAlphaDest;
		//		shr eax, 5;
		shr eax, 1;		// nAlphaDest = 16�ϰ濁E(�׸����ϰ濁E
		and eax, dwGBitMask;
		add ebx, eax;

		mov WORD PTR[edi], bx;

		add edi, 2;
		//			add esi, 2;

		pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	}

};

inline void CUser::PutPixelColor(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc, DWORD dwDestColor, DWORD dwRBBitMask, DWORD dwGBitMask) // nAlphaSrc - ��������Ʈ�� �ుE��. dwDestColor - �÷��� 32��Ʈ ��. (�ݵ�� "32-nAlphaSrc"�� ������E��ġ�Ͽ��� �Ѵ�.
{
	__asm // RGB ������ ���� �軁E.
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;
	COPY_LOOP:
		push ecx; // ī��� ����E.

		mov ebx, 0; // ���� ����E�ʱ�ȭ

		mov cx, WORD PTR[esi]; // Spr Pixel

		jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..

		mov eax, ecx; // Spr - R B
		and eax, dwRBBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwRBBitMask;
		add ebx, eax;

		mov eax, ecx; // Spr - G
		and eax, dwGBitMask;
		mul nAlphaSrc;
		shr eax, 5;
		and eax, dwGBitMask;
		add ebx, eax;

		add ebx, dwDestColor; // �÷� ���� �����ش�.

		mov WORD PTR[edi], bx;

	DONT_COPY:

		add edi, 2;
		add esi, 2;

		pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	}
};

inline void CUser::PutPixelHalf(const WORD* pDest, const WORD* pSrc, int nCopyCount, DWORD dwHalf16Mask, DWORD dwHalf32Mask) // nAlphaSrc - 0 �� �� ��Ŀ�ݴ�.. 32 �� ���� ���.
{
	//ASSERT(dwRBMask == 0x7c1f);

	//DWORD dwMask = 0x7bde7bde;
	//DWORD  wMask = 0x00007bde;
	__asm
	{
		mov edi, pDest;
		mov esi, pSrc;
		mov ecx, nCopyCount;

		// nCopyCount�� Ȧ������E¦�������� �Ǵ�. 
		mov ebx, ecx;
		and ebx, 1;
		jz	DWORD_HALF_MIX;

		//push ecx; // ī��� ����E.

		mov ax, WORD PTR[esi]; // Spr Pixel
		mov dx, WORD PTR[edi]; // Dest Spr Pixel
		and eax, dwHalf16Mask;
		and edx, dwHalf16Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, wMask;
		mov WORD PTR[edi], ax;		// ax�� 16bit Register.

		add edi, 2;
		add esi, 2;
		//pop ecx; // ī��� ����..
		dec ecx;
		jz  END_LOOP;

		// ���⼭���ʹ� 32bit(DWORD)�軁E
	DWORD_HALF_MIX:
		shr ecx, 1;

	COPY_LOOP:
		//push ecx; // ī��� ����E.
		//mov ebx, 0; // ���� ����E�ʱ�ȭ

		mov eax, [esi]; // Spr Pixel
		//jecxz DONT_COPY; // ��������Ʈ ������ 0 �̸�E.. ó�� ����..
		mov edx, [edi]; // 
		and eax, dwHalf32Mask;
		and edx, dwHalf32Mask;
		shr eax, 1;
		shr edx, 1;
		add eax, edx;
		//and eax, dwMask;
		mov[edi], eax;

		//DONT_COPY:
		add edi, 4;
		add esi, 4;

		//pop ecx; // ī��� ����..
		dec ecx;
		jnz COPY_LOOP;
	END_LOOP:
	}
};

inline void CUser::PutPixelGray(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaSrc) {

	WORD r, g, b, mid;
	WORD *pDst = (WORD*) pDest;

	DWORD dwBMask = 0x1f;
	DWORD dwGMask = dwBMask << 6;
	DWORD dwRMask = dwBMask << 11;

	int	nRMaskOffset = 11;
	int	nGMaskOffset = 6;

	for (int i = 0; i<nCopyCount; i++) {
		r = (WORD) (pSrc[i] & dwRMask) >> nRMaskOffset;
		g = (WORD) (pSrc[i] & dwGMask) >> nGMaskOffset;
		b = (WORD) (pSrc[i] & dwBMask);


		mid = (r + g + b) / 3;

		if (r>mid)	r = max(r - nAlphaSrc, mid);
		else 		r = min(r + nAlphaSrc, mid);

		if (g > mid)	g = max(g - nAlphaSrc, mid);
		else 		g = min(g + nAlphaSrc, mid);

		if (b > mid)	b = max(b - nAlphaSrc, mid);
		else 		b = min(b + nAlphaSrc, mid);

		pDst[i] = (WORD) ((r << nRMaskOffset) | (g << nGMaskOffset) | (b));
	}
};

#endif // !defined(AFX_USER_H__5A5901E5_3853_11D3_8422_00105A6B97E2__INCLUDED_)
/*
// nAlphaSrc �� nAlphaDest �� ���� �ݵ�� 5�̾��� �Ѵ�..
inline void CUser::PutPixelBitStepTrans(const WORD* pDest, const WORD* pSrc, int nCopyCount, int nAlphaDestBit, int nAlphaSrcBit, DWORD dwRBGBitMask, DWORD dwGRBBitMask)
{
__asm
{
mov edi, pDest;
mov esi, pSrc;
mov ecx, nCopyCount;

mov ebx, ecx;
and ebx, 1;
jz	DWORD_ALPHA_MIX;
/////////////////////////////////
// ���ȼ� ó�����ִ� �κ�
/////////////////////////////////
add edi, 2;
add esi, 2;
dec ecx;
jz  END_LOOP;

DWORD_ALPHA_MIX:
shr ecx, 1;

COPY_LOOP:
push ecx;
//first
mov eax, [esi];
and eax, dwRBGBitMask;
shr eax, 5;
//		mov cl, byte ptr nAlphaSrcBit;
//		shl eax, cl;
mul nAlphaSrcBit;

and eax, dwRBGBitMask;
mov edx, eax;

mov eax, [esi];
and eax, dwGRBBitMask;
//		shl eax, cl;
mul nAlphaSrcBit;

shr eax, 5;
and eax, dwGRBBitMask;
or  edx, eax;
//second
mov eax, [edi];
and eax, dwRBGBitMask;
shr eax, 5;
//		mov cl, byte ptr nAlphaDestBit;
//		shl eax, cl;
mul nAlphaDestBit;

and eax, dwRBGBitMask;
mov ebx, eax;

mov eax, [edi];
and eax, dwGRBBitMask;

mul nAlphaDestBit;
//		shl eax, cl;
shr eax, 5;
and eax, dwGRBBitMask;
or  edx, eax;

or  edx,ebx;//�λ����ϱ�E
mov [edi],edx;

add edi, 4;
add esi, 4;

pop ecx; // ī��� ����..
dec ecx;
jnz COPY_LOOP;

END_LOOP:
}
}
*/