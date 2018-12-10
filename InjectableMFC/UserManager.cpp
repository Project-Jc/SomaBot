#include "stdafx.h"
#include "PacketParserHook.h"

tGetMe UserManager::fGetMe = NULL;

tGetUserWithUid UserManager::fGetUserWithUid;

tMoveRequest UserManager::fMoveRequest;

tMoveRunRequest UserManager::fMoveRunRequest;

tIsMoveable UserManager::fIsMoveable;

UserList UserManager::UserList;

Direction UserManager::GetDirection(CPoint FromCell, CPoint ToCell) {

	if ((FromCell.y < ToCell.y) && (FromCell.x == ToCell.x))
		return Direction::DOWN;
	else if ((FromCell.y < ToCell.y) && (FromCell.x > ToCell.x))
		return Direction::DOWN_LEFT;
	else if ((FromCell.y == ToCell.y) && (FromCell.x > ToCell.x))
		return Direction::LEFT;
	else if ((FromCell.y > ToCell.y) && (FromCell.x > ToCell.x))
		return Direction::UP_LEFT;
	else if ((FromCell.y > ToCell.y) && (FromCell.x == ToCell.x))
		return Direction::UP;
	else if ((FromCell.y > ToCell.y) && (FromCell.x < ToCell.x))
		return Direction::UP_RIGHT;
	else if ((FromCell.y == ToCell.y) && (FromCell.x < ToCell.x))
		return Direction::RIGHT;
	else if ((FromCell.y < ToCell.y) && (FromCell.x < ToCell.x))
		return Direction::DOWN_RIGHT;
}

int UserManager::GetCellRange(CPoint StartCell, CPoint FinalCell) {
	return (int)(abs(StartCell.x - FinalCell.x) + abs(StartCell.y - FinalCell.y));
}

void UserManager::MoveRequest(CPoint Destination) {

	Bot::Print(Debug::MOVEMENT, "Walking to point [X: %d Y: %d]", Destination.x, Destination.y);

	PREPARE_USERMANAGER_CALL();
	CUser* Me = (CUser*)fGetMe();

	PREPARE_PACKETPARSER_CALL();
	fMoveRequest(Me, Destination, 20);
}

void UserManager::MoveRunRequest(CPoint Destination) {

	// Do not process movement if we are already moving or attacking (Performing the attack check prevents the multiple movement packets right after it's killed something)
	if (BotUser::Me->IsMoving() OR BotUser::Me->Data->AnimationIndex == USER::Motion::ATTACKING OR BotUser::Me->Data->AnimationIndex == USER::Motion::ATTACKED) {
		return;
	}
	
	// Do not continue if the bot is standing still to regain stamina.
	if (Bot::State.IsRegainingStamina) {
		if (BotUser::Me->Data->Stamina.Current > (BotUser::Me->Data->Stamina.Maximum * 60 / 100)) {
			Bot::State.IsRegainingStamina = false;
		} return;
	}

	// Toggle run mode off to regain stamina if the current stamina value is below 10%
	if (BotUser::Me->Data->Stamina.Current < (BotUser::Me->Data->Stamina.Maximum * 10 / 100)) {
		if (BotUser::Me->StaminaOn()) {
			SocketMng::ToggleStamina(false);
		}
		if (Bot::Configuration.RegainStam) {		
			// Check for aggressive nearby mobs before stopping to regain stamina.
			for (auto xt = UserManager::UserList.Begin();; xt++) {
				if (xt == UserManager::UserList.End()) {
					Bot::Print(Debug::MOVEMENT, "Regaining stamina.");
					Bot::State.IsRegainingStamina = true;
					return;
				} else {
					CUser *pUser = xt->second;
					if (BotUser::IsValid(pUser) AND !pUser->Data->IsMe AND pUser->IsAggressive()) {
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) < (pUser->AggroRange + 8)) {
							Bot::Print(Debug::GENERAL, "Not safe to stop to regain stam because of [%s] [Uid: %d] [RFM: %d]", pUser->Data->Name, pUser->Data->Uid, UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos));
							break;
						}
					}
				}
			}
		}
	} 

	// Toggle run mode on when we're walking and our stamina goes above 40%
	else if (BotUser::Me->Data->Stamina.Current > (BotUser::Me->Data->Stamina.Maximum * 30 / 100)) {
		if (!BotUser::Me->StaminaOn()) {
			SocketMng::ToggleStamina(true); 
			return;
		}
	}

	// Run if stamina is on. Walk if it isn't.
	switch (BotUser::Me->StaminaOn()) {
		case TRUE: {
			Bot::Print(Debug::MOVEMENT, "Running to point [X: %d Y: %d]", Destination.x, Destination.y);

			PREPARE_USERMANAGER_CALL();
			CUser* Me = (CUser*)fGetMe();

			PREPARE_PACKETPARSER_CALL();
			fMoveRunRequest(Me, Destination, 20);

		} break;

		case FALSE: {
			MoveRequest(Destination);		
		} break;
	}

	// Track time outs for looting.
	if (Bot::State.IsLooting) {
		Bot::Loot.NoteFailure();
	}
}
