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
// ���� ���̺�� �����͸� ���� ����ü
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
	int		m_nTableNum		; //���̺� ���� ����
//	CString m_strMagicName	; //�����̸�
	int		m_nAttackType	; //��������
							  //1)ĳ���Ϳ� ȿ�� 2)�߻�Ǿ� ��ǥ���� ���ư��� ���� 3)ĳ���Ϳ� ȿ�� �� ���ӵǴ� ����

	short     m_vMethod		; //��Ŷ����
							  //1)PKT_ATTACK_ARROW 2)PKT_ATTACK_MAGIC_RAIL 3)PKT_MAGIC_CIRCLE	
	int		m_nDes			; //������ �� ��� ����
							  //1)�ڽ� 2)�ڽ�/Ÿ�� 3)Ÿ��
	int		m_nStPreMagic	; //ĳ���� ���ʿ� �׷��� �׸�
	int		m_nStPostMagic	; //ĳ���� ���ʿ� �׷��� �׸�. 0�̸� ������ ����..
	int		m_nStTime		; //���ư��� ������ ���� �ð�....

	int		m_nFlyMagic		; //���ư��� ����...
	int		m_nEffecKind	; 
							  
	int		m_nFireNum		; //���󰡴� ����

	int		m_nEndPreMagic	; //��� ĳ���� ���ʿ� �׷��� �׸�
	int		m_nEndPostMagic	; //��� ĳ���� ���ʿ� �׷��� �׸�
//	int		m_nEndTime		;

//	int		m_nConPreMagic	;
//	int		m_nConPostMagic	;
	int		m_nConType		; //���ӵǴ� ������ Ÿ��...
	int		m_nConTime		;
} _MAGIC_TABLE;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ������ ���� ������ ����Ʈ�� ������ ����ü
typedef struct _TAG_SET_MAGIC_INFO
{
	_MAGIC_TABLE		m_stMagicTable;
	int					m_nSorUid	  ;
	CArray<int , int>	m_arrDestUid  ;
	DWORD				m_dwNowTime   ;
	BOOL				m_bReady	  ; //�����غ� �ʿ��� �����ΰ�? 
	int 				m_nSIndex	  ; //��Ŷ�� ���´°�?
	int					m_nDir		  ; //����������� 100�̸� ���⿡ ������� ����
	CMagicComet*		m_Comet		  ; //������ �ٸ� 
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
	DWORD	m_dwDeadTime;	//���� �ð�
	CPoint	m_ptDeadPos;	//���� ��ġ
	int		m_nDeadUserId;	//���̵�
} DEAD_INFO;

typedef CTypedPtrArray<CPtrArray, ACTION_INFO *> ARR_ACTION_INFO;

class CUserManager  //���� ������
{
public:
	void ChangItem(int uid, short fromindex, short toindex);// ���� ĳ���Ͱ� ������ �ٲپ�����..���� ���ε� ����..
	void ChangItem(short fromindex, short toindex);// ���� �κ��丮���� ���� �κ��丮��..�Ǵ� �ݴ�..
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
	
	//�������� ó���Լ� (uid�� ���� �������� nIndex�� ������ �ɾ���)
	void				SetSkyMagic(int nSorUid, int nDstUid, int nMagicIndex, int nSeries = 1, bool bMultiMagic = false);
	void				SetSkyMagic(CPoint ptSor, CPoint ptDst, int nDir, int nMagicIndex, int nMagicType , int nDesUid = -1,int nSeries = 1);
	int					SetAimedMagic(int nSorUid, int nDstUid, int nMagicIndex, int nMagicType);
	int					SetAimedMagic(CPoint ptSor, CPoint ptnDstUid, int nMagicIndex, int nMagicType, int nDesUid = -1,int nSeries = 1);
	void				DrawDownMagics(CDDSurface *pSurface);	//�ٴڿ� �򸮴� ���� �׸���
	void				DrawUpMagics(CDDSurface *pSurface);		//������ ���� �׸���

	void				SetSpecial(int uid, int nIndex, int nMagicType, int height = 0, bool bForceNew = false);

	void				SetMagic(int uid, int nIndex, int nMagicType, int height = 0);	//ĳ���Ϳ� ���� ���� ��
	void				RailAttackMagic(int bSuccess, int iSorUid, int bDir, int bLen, int nMagicType);
	int					SetViewMagic(int uid, int nMagicIndex, int nMagicType, int iDir, int iLen);
	//���ݰ��� ó���Լ�
	BOOL				NextAttack(int nAttackDelay);
	DWORD AttackTick;
	DWORD m_dwAttack;
	BOOL				SendAttack(int type = 0);							//������ �� (Focus�� �ִ� User�� ���������)
	BOOL				SendReadyMagic(int type);
	BOOL				SendReadyMagic1(int type);
	BOOL				SendReadMagic(int type);
	BOOL				SendMagicAttack(int type, short method, int nDestUid = -1, BYTE dir = 100);
	BOOL				SendMagicAttack1(int type, short method, int nDestUid = -1, BYTE dir = 100);
	
	BOOL				SendSpecialAttack(short type, int nDestUid = -1, short dir = 100, CString name = "", BYTE byTraceMode = 0);
	
	void				AttackChar(BYTE bSuccess, int iSorUid, CPoint ptSorPos, int iDestUid, CPoint ptDestPos, short nAttackArrow, bool bAttackSound = true);	//ȭ��ǥ��(Source�� Destination����)
	void                AttackMagic(int suid, int euid, int magic_type,int nSeries = 1);// ���� ���� �Ŵ��� �����ش�..����..���Լ��� ����..
	void				Attack3DMagic(int suid, int euid, TCHAR* strName);

	void				SetMoveFirst(CUser *pUser, CPoint ptDst, CPoint ptFirst);	//�������� �� ����ó��(Move First)
	void				SetMoveMid(CUser *pUser, CPoint ptDst);					//			"		  (Move Middle)
	void				SetMoveEnd(CUser *pUser, CPoint ptDst);					//			"		  (Move End)

	void				SetRunMoveFirst(CUser *pUser, CPoint ptDst, CPoint ptFirst, CPoint ptNext);	//�������� �� ����ó��(Move First)
	void				SetRunMoveMid(CUser *pUser, CPoint ptDst, CPoint ptNext);					//			"		  (Move Middle)
	void				SetRunMoveEnd(CUser *pUser, CPoint ptDst, CPoint ptNext);					//			"		  (Move End)

	//�̵����� ��Ŷó�� �Լ�
	void				ModifyUser(int uid, BYTE bDeadEnable, CString t_name, CString t_guild, CString t_partyleader, int picnum, short moral, BYTE live, BYTE battle_mode, int x, int y, short item[10], short sGuildVW, short sGuildPicId, short sUserDir, int iGuild, short nClass, BYTE bWOTW, BYTE byGuildWar, short sGuildWarAgainst, BYTE byBlackName, BOOL bIsI=FALSE, short hair = 101 , short skin = 0, short hpMax = 1, short hpCur = 1);	//����� �������� (������ ����, ������ �߰�)
	void				DeleteUser(int uid, int x, int y);						//����� ���� ���ֱ�

	BOOL				IsMoveable(CPoint pt);									//�̵����� ��ġ�ΰ�?(�ʻ󿡼�)
	void				SetMoveableTable(MapManager* pMapManager);			//PathFinder���� ����� Map Table ������ ���� (PathFinder������� �ʰ� ����)
	BOOL				IsInRange(CRect r, CUser *pUser, CPoint ptPos);			//Rect�� ������ ���ԵǴ°�(ȭ�鿡 ����?) ptPos�� ���� �׸��� Offset
	CPoint				GetMyPos();												//���� ��ġ ��������(Pixel)
	void				Move();													//������ �̵�ó��
	static	int			compare(const void *arg1, const void *arg2);			//ȭ�� �׸��� ���� User Quick Sort ���Լ�
	void				DrawUserSortY(int count);								//�׸� User�� Sorting��(Y�࿡ ���ؼ�)
	void				RenderUsers(CDDSurface *pSurface, CPoint ptMapOffset);	//������ �׸���
	void				InHouseRenderUsers(CDDSurface *pSurface, CPoint ptMapOffset);
	void				RenderBlindUser(CDDSurface *pSurface, CPoint ptMapOffset);
	//void				OutHouseDrawObj(CDDSurface *pSurface, CPoint ptMapOffset);

	CUser*				GetUserWithPos(CPoint ptPixelPos, CPoint ptMapOffset);	//������ ��ġ�� �ִ� ������ ����?(Pixel����)
	CUser*				GetUserWithCellPos(CPoint ptCellPos);					//				"				(Cell����)
	CUser*				GetUserWithUid(int uid);								//User Id�� �̿��� User��������
	int					GetUserNum() {return m_mapUser.GetCount();}				// ���?
	CUser*				GetMe();												//������ ��������
	int					GetFocusedUidInOthers() {return m_iCurFocusUidInOthers;}//Focus�� ������� ����? (�� ����)
	void				RemoveUser(int index);									//������ Index�� ���� ����
	void				SetMyUserId(int iUid){m_iMyUserId = iUid;}				//���� ���� Id����
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
