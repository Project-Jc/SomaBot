#ifndef BOTUSE_H
#define BOTUSE_H
#include "User.h"
#include <vector>
#include "iBotMessageHandle.h"

class iBotUser
{
public:
	CUser* m_cBotUser;
	CUser* m_cBotTarget;

	int m_vBotTargetUid;
	CPoint m_cBotTargetPosition;

	bool m_bBotRunning,
		 m_bBotAttacking,
		 m_bBotMovingToAttack,
		 m_bIsBeingAttacked,
		 m_bIsBeingPkd,
		 m_bReEquipWeapon;

	int m_vAttackFailure,
		m_vIsBeingPkdCount;

	// Waypoint variables
	int m_vCurrentWP,
		m_vStuckCount;
	bool m_vMoving;
	std::vector<CPoint> HuntingWaypoints,
						PathWaypoints,
						DropPoints;

	CString	HuntingWaypointFile,
			PathWaypointFile;

	// Monster hunting list
	std::vector<CString> TargetList;
	std::vector<int> BlackList;

	// Looting
	int m_vDropCount,
		m_vCurrentDrop;
	bool m_bLooting,
		 m_bLootingEnabled;

	// Repair and restock etc
	bool m_bPotsnRepairs,
		 m_bRepairDone,
		 m_bRepairing,
		 m_bRestockDone,
		 m_bOpeningNPC;

	int m_vCrItem;

	// Int variables
	int m_nUserAttackSpell,
		m_nUserWeakenSpellIndex,
		m_nUserSlowSpellIndex,
		m_nDebuffSentCount,
		m_nDebuffFailedCount;

	bool m_bIntMode,
		 m_bCastingDebuff,
		 m_bCastingWeaken,
		 m_bCastingSlow;

	// Healing
	int m_nHealPercent,
		m_nUserHealSpellIndex,
		m_bWeaponIndex;

	int m_nHealSentCount,
		m_nHealSentTimeOut,
		m_nFailedHealCount;

	bool m_bHealDone,
		 m_bHealRunning,
		 m_bHealEnabled,
		 m_bSwitchingWeapon;

	// Range stuff
	int m_nTargetMaxRange,
		m_nAttackDistance;

	// Bot stopping conditions
	CString StopStat;
	short StopValue;
	bool StopTriggered;

	// MP & HP potting 
	int m_vMPThreshold,
		m_vHPThreshold,
		m_vPotionIndex;
	bool m_vPottingHP,
		 m_vPottingMP;

	int m_vItemIndex;

	bool m_bWarpRequired,
		 m_bWarpEnabled,
		 m_bIsWarping;

	// Zone
	int m_vHuntingMap,
		m_vPreviousMap;
	bool m_bPathWaypoints,
		 m_bEnterPortal;

	// Timing
	DWORD m_dwTickDelay,
		  m_dwWaitTick,
		  m_dwSendMessageTick;

	int m_vRandomCount,
		m_vRandomNumber;
	bool m_bTriggerWait;

	// Death
	int m_vDeathCount;
	bool m_bReviveSent;

	// Message displays
	bool ShowLooting,
		 ShowHasTarget,
		 ShowAttack,
		 ShowMoving,
		 ShowWaiting,
		 ShowTargetDied,
		 ShowLowHealth,
		 ShowLowMana,
		 ShowPotting,
		 ShowHealing,
		 ShowPathMovement,
		 ShowWarp,
		 ShowDebuff;

	// Chat stuff
	std::vector<iBotMessageHandle> MessageHandler;

	bool m_bPushingRested;
	int m_vRestedPktSentCount;
	CString RestedPlayerName;
	
public:
	iBotUser();
	~iBotUser();
	void Initialize(CUser* BotUsr);
	void MainLoop();
	bool PlayerHasTarget();
	bool Delay(int iDelay);
	bool TargetFound();
	int GetCellRange(CPoint iCellPos);
	void ShowMsg();
	void MoveToPoint(CPoint iPoint);
	void GrabDrop(CPoint DropPoint);
	void Test();
	CPoint GenerateNewPoint(CPoint cpDestination, int iSubsequentRange);
	int GenerateRandomNumber(int Min, int Max);
	bool PlayerHealthLow();
	bool PlayerManaLow();	
	bool PlayerHasPotion(int sPotion);
	void SendItem(int ItemIndex);
	void CastAura();
	void SetAttackDist();
	bool TargetDead();
	bool HasItem(CString iName, int iType);
	bool ItemIsRepairable(int ItemIndex);
	void RepairAll();
	bool MonitorMovement();
	bool DropsFound();
	void ResetTarget();
	void ShopOpen();
	void DuraCheck();
	void Revive(BYTE WarpOrRevive);
	void BlackSmithOpen();
	bool PlayerHasAura(CString Aura);
	void BeginAttack();
	void DetectUserSpells();
	void SwitchGear(short FromIndex, short ToIndex);
	void FindNearestWP();
	bool Wait(int Milliseconds);
	bool WaitDelay(int iDelay);
	void SellGear();
	bool NpcUidSet(CString NpcType);
	void DepositGear();
	bool UserHasHeal();
	bool UserHasWeaken();
	bool UserHasSlow();
	CString HuntingZone();
	void GiveRested(CString CharacterName);
	void SendHeal();
	bool HealReady();
	void MessageCheck();
	bool NextMessageSend(int iDelay);
	bool PlayerRequiresHealing();
	bool SafeToHeal();
	void ShowMsg(CString Message, CString Colour);
};

#endif