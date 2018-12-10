// UserManager.h: interface for the CUserManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMANAGER_H__A8E67521_39D1_11D3_8422_00105A6B97E2__INCLUDED_)
#define AFX_USERMANAGER_H__A8E67521_39D1_11D3_8422_00105A6B97E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TownPortal.h"

class NODE;
class CPathFind;
class CDDSurface;
class MapManager;
class CUser;
class CMagic;
class CMagicComet;
class CBlood;
class CArrowManager;

typedef CMap<int, int, CUser *, CUser *> MAP_USER;
typedef CTypedPtrArray<CPtrArray, CUser*> ARR_USER;
typedef CTypedPtrList<CPtrList, CMagic*> LST_MAGIC;

#define	UM_UID_INVALID		-1

#define BEATEDMOTION         1
#define BEATEDSOUND          2
#define BEATEDEFFECT         3
#define ATTACKMOTION         4
#define ATTACKSOUND          5
#define ATTACKEFFECT         6
#define ATTACKFAIL           7

#define NORMAL_ATTACK_SPEED 100
#define SLOW_ATTACK_SPEED	 200
#define FAST_ATTACK_SPEED	 40

#define DEAD_USER_TIME 5000

//////////////////////////////////////////////////////////////////////////////////////////////////
// 마법 테이블로 데이터를 읽을 구조체
typedef struct _TAG_SPECIAL_TABLE
{
	short m_nTableNum; // Special Attack no
	BYTE m_byDisplayType; // 1 = Show attack animation (like the double attack for sword, based on 
	//files in armature: A7GXXYY; G = Gender, X = Number in special Table, y = Weapon(bow, axe etc)
	//... although this aint quite the same as normal .. i.e. Spear = 10 and bow = 11 ..
	// 2 = Dont show an extra attack animation
	BYTE m_byAttackType; // 1 = No attack, passive skill. 2 = Single Attack,
	// 3 = Multiple attack fast, 4 = Speed up normal attacks, 6 = Concussion / Paralyze... stops user moving and turns him grey
	BYTE m_byTarget; // 3 = other player or mob, 4 = other player and yourself, 1 = yourself
	BYTE m_byType; // 1 = normal, 2 = line attack, 3 = mass attack
	short m_nStPreSpecial; // Animation that plays when aura is cast (in front of char)
	short m_nStPostSpecial; // Animation that plays when aura is cast (behind char)
	short m_nFlyArrow; // Arrow Number to play when aura is used...
	short m_nEndPreSpecial; // Animation that plays for collision (in front of char)
	short m_nEndPostSpecial; // Animation that plays for collision (behind char)
	short m_nEndPostHit; // Animation that plays when a player is hit while aura is active
	short m_vNoAttacks; // Amount of times to repeat attack aura(like double shooting
	short m_vAttackAni; // Attack animation File number in armature
} _SPECIAL_TABLE;

typedef struct _TAG_SET_SPECIAL_INFO
{
	_SPECIAL_TABLE		m_stSpecialTable;
	int					m_nSorUid	  ;
	CArray<int , int>	m_arrDestUid  ;
	DWORD				m_dwNowTime   ;
	int 				m_nSIndex	  ; // Sound index
	int					m_nDir		  ;
	int					m_nAttacks	  ; // Amount of times to attack
	int					m_nStTime	  ; // Time until Precast Skill ends
	int					m_nConTime; // Time Until Aura Ends
} _SET_SPECIAL_INFO;

typedef struct _TAG_MAGIC_TABLE
{
	int		m_nTableNum		; //테이블 정보 순서
//	CString m_strMagicName	; //마법이름
	int		m_nAttackType	; //공격유형
							  //1)캐릭터에 효과 2)발사되어 목표물로 날아가는 마법 3)캐릭터에 효과 후 지속되는 마법

	short     m_vMethod		; //패킷종류
							  //1)PKT_ATTACK_ARROW 2)PKT_ATTACK_MAGIC_RAIL 3)PKT_MAGIC_CIRCLE	
	int		m_nDes			; //마법을 줄 대상 유형
							  //1)자신 2)자신/타인 3)타인
	int		m_nStPreMagic	; //캐릭터 뒤쪽에 그려질 그림
	int		m_nStPostMagic	; //캐릭터 앞쪽에 그려질 그림. 0이면 파일이 없음..
	int		m_nStTime		; //날아가는 매직이 생길 시간....

	int		m_nFlyMagic		; //날아가는 매직...
	int		m_nEffecKind	; 
							  
	int		m_nFireNum		; //날라가는 갯수

	int		m_nEndPreMagic	; //대상 캐릭터 뒤쪽에 그려질 그림
	int		m_nEndPostMagic	; //대상 캐릭터 앞쪽에 그려질 그림
//	int		m_nEndTime		;

//	int		m_nConPreMagic	;
//	int		m_nConPostMagic	;
	int		m_nConType		; //지속되는 매직의 타입...
	int		m_nConTime		;
} _MAGIC_TABLE;

//////////////////////////////////////////////////////////////////////////////////////////////////
// 마법 설정에 대한 정보를 리스트로 관리할 구조체
typedef struct _TAG_SET_MAGIC_INFO
{
	_MAGIC_TABLE		m_stMagicTable;
	int					m_nSorUid	  ;
	CArray<int , int>	m_arrDestUid  ;
	DWORD				m_dwNowTime   ;
	BOOL				m_bReady	  ; //공격준비가 필요한 마법인가? 
	int 				m_nSIndex	  ; //패킷을 보냈는가?
	int					m_nDir		  ; //마법적용방향 100이면 방향에 적용되지 않음
	CMagicComet*		m_Comet		  ; //꼬리가 다린 
	bool				m_bPreCast	  ;
	bool				m_bAttAniShown; // Whether or not the attack animation has been shown yet or not...
} _SET_MAGIC_INFO;

typedef struct _TAG_ACTION_INFO
{
	int  m_ActionId;
	int  m_ActionType;
	DWORD m_ActionTickTime;
	bool m_bSound;
} ACTION_INFO;

typedef struct _TAG_DEAD_INFO
{
	DWORD	m_dwDeadTime;	//죽은 시간
	CPoint	m_ptDeadPos;	//죽은 위치
	int		m_nDeadUserId;	//아이디
} DEAD_INFO;

typedef CTypedPtrArray<CPtrArray, ACTION_INFO *> ARR_ACTION_INFO;

class CUserManager  //유저 관리자
{
public:
	void ChangItem(int uid, short fromindex, short toindex);// 따른 캐릭터가 장착을 바꾸었을때..물론 본인도 가능..
	void ChangItem(short fromindex, short toindex);// 장착 인벤토리에서 가방 인벤토리로..또는 반대..
	BOOL SendChang_Item(short fromindex, short toindex);
	void Set_MyInv_Data_All(int uid, PBYTE bypData);

	BOOL SpecialChar( _SET_SPECIAL_INFO* stSetSpecialInfo );
	BOOL SingleSpecialChar( _SET_SPECIAL_INFO* stSetSpecialInfo );
	BOOL MultiSpecialChar( _SET_SPECIAL_INFO* stSetSpecialInfo );
	void ApplySpecialEx();

	BOOL MagicChar( _SET_MAGIC_INFO* stSetMagicInfo );
	void ApplyMagicEx();
	void ApplyConMagic();
	void ApplyConMagic( int t_index, CUser* pUser );
	void ApplyConSpecial();
	void ApplyConSpecial( int t_index, CUser* pUser, bool bActive );
	void UserAppliedMagic( int uid, BYTE num, short* magic_index, DWORD* dwConTime );
	void ReadMagicTable(int bSuccess, int iSorUid, short Hp, short Mp, DWORD exp, int *iDestUid,short *pHP,int nDstUidNum, int magicindex, DWORD dwConTime, int Dir = 100);
	void ReadMagicTable(int bSuccess, int iSorUid, int nDestUid, int magicindex, DWORD dwStartTime, int Dir = 100, bool bPreCast = false);
	void MagicTableInit();

	void ReadSpecialTable(int bSuccess, int iSorUid, int *iDestUid,short *pHP,int nDstUidNum, int specialindex, DWORD dwConTime, int Dir = 100, int traceX = -1, int traceY = -1);
	void SpecialTableInit();
	void DeleteSpecial(int iDestUid);

	void DeleteMagic(int iDestUid);
	void PoisonAffect(int nDesUid,short HP, short sMagicNo, DWORD dwRemainTime);
	void CharStateChange( int nDestUid, PBYTE bypData);
	void MagicAttackKind( _SET_MAGIC_INFO* stSetMagicInfo, int nSorUid, int* pDesUid, int nDestNum );
	void SetMagicComet(  _SET_MAGIC_INFO* stSetMagicInfo, int nSorUid, int* pDesUid, int nDestNum );

	BOOL SkyMagicChar( _SET_MAGIC_INFO* stSetMagicInfo , bool bMultiMagic);
	BOOL FlyMagicChar( _SET_MAGIC_INFO* stSetMagicInfo );
	BOOL ConMagicChar( _SET_MAGIC_INFO* stSetMagicInfo );
	BOOL FlyMagicComet( _SET_MAGIC_INFO* stSetMagicInfo );
	
	//마법관련 처리함수 (uid를 가진 유저에게 nIndex의 마법을 걸어줌)
	void				SetSkyMagic(int nSorUid, int nDstUid, int nMagicIndex, int nSeries = 1, bool bMultiMagic = false);
	void				SetSkyMagic(CPoint ptSor, CPoint ptDst, int nDir, int nMagicIndex, int nMagicType , int nDesUid = -1,int nSeries = 1);
	int					SetAimedMagic(int nSorUid, int nDstUid, int nMagicIndex, int nMagicType);
	int					SetAimedMagic(CPoint ptSor, CPoint ptnDstUid, int nMagicIndex, int nMagicType, int nDesUid = -1,int nSeries = 1);
	void				DrawDownMagics(CDDSurface *pSurface);	//바닥에 깔리는 마법 그리기
	void				DrawUpMagics(CDDSurface *pSurface);		//날으는 마법 그리기

	void				SetSpecial(int uid, int nIndex, int nMagicType, int height = 0, bool bForceNew = false);

	void				SetMagic(int uid, int nIndex, int nMagicType, int height = 0);	//캐릭터에 단위 마법 검
	void				RailAttackMagic(int bSuccess, int iSorUid, int bDir, int bLen, int nMagicType);
	int					SetViewMagic(int uid, int nMagicIndex, int nMagicType, int iDir, int iLen);
	//공격관련 처리함수
	BOOL				NextAttack(int nAttackDelay);
	DWORD AttackTick;
	DWORD m_dwAttack;
	BOOL				SendAttack(int type = 0);							//공격을 함 (Focus가 있는 User를 대상으로함)
	BOOL				SendReadyMagic(int type);
	BOOL				SendReadyMagic1(int type);
	BOOL				SendReadMagic(int type);
	BOOL				SendMagicAttack(int type, short method, int nDestUid = -1, BYTE dir = 100);
	BOOL				SendMagicAttack1(int type, short method, int nDestUid = -1, BYTE dir = 100);
	
	BOOL				SendSpecialAttack(short type, int nDestUid = -1, short dir = 100, CString name = "", BYTE byTraceMode = 0);
	
	void				AttackChar(BYTE bSuccess, int iSorUid, CPoint ptSorPos, int iDestUid, CPoint ptDestPos, short nAttackArrow, bool bAttackSound = true);	//화면표시(Source가 Destination공격)
	void                AttackMagic(int suid, int euid, int magic_type,int nSeries = 1);// 날라 가는 매닉을 버여준다..음냐..위함수에 포함..
	void				Attack3DMagic(int suid, int euid, TCHAR* strName);

	void				SetMoveFirst(CUser *pUser, CPoint ptDst, CPoint ptFirst);	//서버에서 온 응답처리(Move First)
	void				SetMoveMid(CUser *pUser, CPoint ptDst);					//			"		  (Move Middle)
	void				SetMoveEnd(CUser *pUser, CPoint ptDst);					//			"		  (Move End)

	void				SetRunMoveFirst(CUser *pUser, CPoint ptDst, CPoint ptFirst, CPoint ptNext);	//서버에서 온 응답처리(Move First)
	void				SetRunMoveMid(CUser *pUser, CPoint ptDst, CPoint ptNext);					//			"		  (Move Middle)
	void				SetRunMoveEnd(CUser *pUser, CPoint ptDst, CPoint ptNext);					//			"		  (Move End)

	//이동관련 패킷처리 함수
	void				ModifyUser(int uid, BYTE bDeadEnable, CString t_name, CString t_guild, CString t_partyleader, int picnum, short moral, BYTE live, BYTE battle_mode, int x, int y, short item[10], short sGuildVW, short sGuildPicId, short sUserDir, int iGuild, short nClass, BYTE bWOTW, BYTE byGuildWar, short sGuildWarAgainst, BYTE byBlackName, BOOL bIsI=FALSE, short hair = 101 , short skin = 0, short hpMax = 1, short hpCur = 1);	//사용자 정보갱신 (있으면 갱신, 없으면 추가)
	void				DeleteUser(int uid, int x, int y);						//사용자 정보 없애기

	BOOL				IsMoveable(CPoint pt);									//이동가능 위치인가?(맵상에서)
	void				SetMoveableTable(MapManager* pMapManager);			//PathFinder에서 사용할 Map Table 생성및 설정 (PathFinder사용하지 않고 있음)
	BOOL				IsInRange(CRect r, CUser *pUser, CPoint ptPos);			//Rect에 유저가 포함되는가(화면에 유저?) ptPos는 맵의 그리기 Offset
	CPoint				GetMyPos();												//나의 위치 가져오기(Pixel)
	void				Move();													//유저들 이동처리
	static	int			compare(const void *arg1, const void *arg2);			//화면 그리기 위해 User Quick Sort 비교함수
	void				DrawUserSortY(int count);								//그릴 User를 Sorting함(Y축에 대해서)
	void				RenderUsers(CDDSurface *pSurface, CPoint ptMapOffset);	//유저들 그리기
	void				InHouseRenderUsers(CDDSurface *pSurface, CPoint ptMapOffset);
	void				RenderBlindUser(CDDSurface *pSurface, CPoint ptMapOffset);
	//void				OutHouseDrawObj(CDDSurface *pSurface, CPoint ptMapOffset);

	CUser*				GetUserWithPos(CPoint ptPixelPos, CPoint ptMapOffset);	//지정한 위치에 있는 유저는 누구?(Pixel단위)
	CUser*				GetUserWithCellPos(CPoint ptCellPos);					//				"				(Cell단위)
	CUser*				GetUserWithUid(int uid);								//User Id를 이용한 User가져오기
	int					GetUserNum() {return m_mapUser.GetCount();}				// 몇명?
	CUser*				GetMe();												//내정보 가져오기
	int					GetFocusedUidInOthers() {return m_iCurFocusUidInOthers;}//Focus를 가진사람 누구? (나 제외)
	void				RemoveUser(int index);									//지정한 Index의 유저 제거
	void				SetMyUserId(int iUid){m_iMyUserId = iUid;}				//나의 유저 Id설정
	BYTE				GetDirection(CPoint ptMyPos, CPoint ptDstPos, bool bConvToCell = true);

	void				RemoveAllData();
	void				RemoveAllDataWithoutMe();
	void                HpMpRecovery(PBYTE bypData);

	void				SendChangeDir(BYTE dir);								// Sends new player direction
	void				SendRunMode(bool bRunMode);								// tells server if player is walking / running
	bool				GetMovable(int x, int y);
	void				AddTPRef(int iUid, int nTPIndex, BYTE bActionFlag);
	void				SetGreyUser(int iUid, BYTE byGreyMode);
	
	void				RemoveAllConSpells(int iSorUid);
	void				RemoveAppliedConSpecials(int iSorUid);
	void				RemoveSpecialEffect(int nTableNum, int iSorUid);
	void				RemoveMagicEffect(int nTableNum, int iSorUid);
	void				ActivateSpecialCon(int nTableNum, int iDestUid);
	void				RemoveDeadUsers();

	CUserManager();
	virtual ~CUserManager();

//protected:
public:
	BOOL m_bMyCovered;
	int  m_nCoverDegree;
	int m_ReadyOK;
	void ActionProcess();
	void ActionMotion(int uid, int type, int delay_time, bool bSound = true);
	int m_ActionCount;
	int  m_ActionId[50];
	int  m_ActionType[50];
	DWORD m_ActionTickTime[50];
	DWORD m_NextSendItem;
	bool NextSendItem(int iDelay);


	



	CPoint GetMyCellPos();
	void				UserDead(int uid, int  x, int y);
	void				PreUserDead(int uid, int x, int y);
	void				SetUserDead();

	int					GetMapData(CPoint pt);
	int					GetHouseMapData(CPoint pt);
	int					m_iMyUserId;		// UserId
	int					m_iCurFocusUidInOthers; //Focus Index
	int					m_iCurAttackUid;

	CUser*				m_pMe;
	CUser*				m_arrDrawUser[500];	// 300(20*15))
	CUser*				m_arrDrawDeadUser[1000];	
	MAP_USER			m_mapUser;			
	BOOL				m_bCursorInRange;	
	CBlood*				m_Blood;
	CArrowManager*		m_Arrow;

	LST_MAGIC			m_lstMagic;
	CMagicComet*		m_Comet;	
	CPathFind*			m_pPathFind;
	NODE*				m_pNode;	
	CArray< _SET_MAGIC_INFO*, _SET_MAGIC_INFO* > m_arrSetMagicInfo;
	CArray< _MAGIC_TABLE*	, _MAGIC_TABLE*	   > m_arrMagicTableInfo;

	CArray< _SET_SPECIAL_INFO*, _SET_SPECIAL_INFO* > m_arrSetSpecialInfo;
	CArray< _SPECIAL_TABLE*, _SPECIAL_TABLE*   > m_arrSpecialTableInfo;

	CArray< DEAD_INFO       , DEAD_INFO        > m_arrDeadUserInfo;
	CArray< DEAD_INFO       , DEAD_INFO        > m_arrRemoveDeadUser; // Dead users waiting to be removed...
	ARR_ACTION_INFO		m_arrActionInfo;

	TownPortal			m_tpUsers;
};

#endif // !defined(AFX_USERMANAGER_H__A8E67521_39D1_11D3_8422_00105A6B97E2__INCLUDED_)
