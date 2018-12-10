#include "stdafx.h"
#include "DDrawHook.h"
#include "PacketParserHook.h"
#include <algorithm>

CUser* BotUser::Me = nullptr;

SpecialAttackList BotUser::SpecialAttackList;

Subscribers BotUser::Subscribers;

MagicList BotUser::MagicList;

WareHouse BotUser::WareHouse;

BotUser::InventoryData BotUser::Inventory;

HHOOK mouseHook;

int tmpDelay = 1000;

LRESULT CALLBACK MouseProc(UINT nCode, WPARAM wParam, LPARAM lParam) {
	//if (Gui::Visible()) {
	//	if (wParam == WM_MOUSEHWHEELSCROLL) {
	//		MSLLHOOKSTRUCT  *pMouseStruct = (MSLLHOOKSTRUCT  *)lParam;
	//		if (pMouseStruct != NULL) {
	//			POINT Mouse = { pMouseStruct->pt.x, pMouseStruct->pt.y };
	//			if (ScreenToClient(Bot::hWndParent, &Mouse)) {
	//				if (Gui::MouseIsOver(Mouse)) {
	//					Gui::ProcessInput(Mouse, (wParam == WM_MOUSEHWHEELSCROLL), (GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData) > 0));
	//				}
	//				//printf_s("X [%d] Y [%d] wParam [%X] Flags [%d] Data [%d]\n", pMouseStruct->pt.x, pMouseStruct->pt.y, wParam, pMouseStruct->flags, GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData));
	//			}
	//		}
	//	}
	//} 
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void Pause(void) {
	// These are the delays for when the player has arrived at an NPC and needs to wait so as to not look suspicious.

	// Default value is for INSANE.
	int Duration = 1000;
	switch (Bot::Configuration.behaviour) {
		case Behaviour::AGGRESSIVE:
			Duration = 3500;
			break;
		case Behaviour::CASUAL:
			Duration = 5500;
			break;
		case Behaviour::PENSIONER:
			Duration = 7500;
			break;
	}

	Bot::Print(Debug::GENERAL, "Sleeping for %dms", Duration);
	Sleep(Duration);
}

CPoint GenerateNewPoint(CPoint cpDestination, int Range)
{
	CPoint newPoint(NULL, NULL), iUsrPoint = BotUser::Me->Data->CellPos;

	Bot::Print(Debug::POINTGENERATION, "cpDestination is [X: %d Y: %d] [RFM: %d]", cpDestination.x, cpDestination.y, UserManager::GetCellRange(BotUser::Me->Data->CellPos, cpDestination));

	// Point is directly inline with the player on the X axis.
	if (cpDestination.x == iUsrPoint.x) {

		// Check whether it's above or below on the Y axis.
		if (cpDestination.y < iUsrPoint.y) {
			// Directly above player.
			Bot::Print(Debug::POINTGENERATION, "\tDIRECTLY ABOVE");
			newPoint.x = cpDestination.x;
			newPoint.y = cpDestination.y + Range;
		} else {
			// Directly below player.
			Bot::Print(Debug::POINTGENERATION, "\tDIRECTLY BELOW");
			newPoint.x = cpDestination.x;
			newPoint.y = cpDestination.y - Range;
		}
	}
	// Point is directly inline with the player on the Y axis.
	else if (cpDestination.y == iUsrPoint.y) {

		// Check whether it's above or below on the X axis.
		if (cpDestination.x < iUsrPoint.x) {
			// Directly to the left of player.
			Bot::Print(Debug::POINTGENERATION, "\tDIRECTLY LEFT");
			newPoint.y = cpDestination.y;
			newPoint.x = cpDestination.x + Range;
		} else {
			//	Directly to the right of player.
			Bot::Print(Debug::POINTGENERATION, "\tDIRECTLY RIGHT");
			newPoint.y = cpDestination.y;
			newPoint.x = cpDestination.x - Range;
		}
	}
	// Point is not directly inline with the player
	else {

		// If randomization is on then alter the attack range to produce a varying attack waypoint.
		if (Bot::Configuration.Randomization) {
			if (Bot::State.IsTrainingInt) {
				// Evaluate differently for Nightmare because the attack range is already reduced on Initialize().
				if (Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
					Range -= BotUser::EvenRandomNumberGenerator(0, 2);
				} else {
					Range -= BotUser::EvenRandomNumberGenerator(0, 4);
				}
			} else if (BotUser::Inventory.Item.Type(6) == ItemType::BOW) {
				if (Bot::Configuration.behaviour != Behaviour::INSANE) {
					Range -= BotUser::EvenRandomNumberGenerator(0, 4);
				}
			}
		}
	
		// Generate a list of potential new points.
		vector<CPoint> PotentialPoints;

		if (cpDestination.x < iUsrPoint.x && cpDestination.y < iUsrPoint.y)	{
			// Top left region.
			Bot::Print(Debug::POINTGENERATION, "\tTOP LEFT");
			for (int x = 1, y = 0; y <= Range;) {
				CPoint tmpPoint = CPoint((cpDestination.x + x), (cpDestination.y + y));
				if (UserManager::GetCellRange(cpDestination, tmpPoint) == Range) {
					PotentialPoints.push_back(tmpPoint);
					x = 0; y++;
				} else {
					x++;
				}
			}
		}
		else if (cpDestination.x < iUsrPoint.x && cpDestination.y > iUsrPoint.y)	{
			// Bottom left region.
			Bot::Print(Debug::POINTGENERATION, "\tBOTTOM LEFT");
			for (int x = 1, y = 0; y <= Range;) {
				CPoint tmpPoint = CPoint((cpDestination.x + x), (cpDestination.y - y));
				if (UserManager::GetCellRange(cpDestination, tmpPoint) == Range) {
					PotentialPoints.push_back(tmpPoint);
					x = 0; y++;
				} else {
					x++;
				}
			}
		}
		else if (cpDestination.x > iUsrPoint.x && cpDestination.y < iUsrPoint.y)	{
			// Top right region.
			Bot::Print(Debug::POINTGENERATION, "\tTOP RIGHT");
			for (int x = 1, y = 0; y <= Range;) {
				CPoint tmpPoint = CPoint((cpDestination.x - x), (cpDestination.y + y));
				if (UserManager::GetCellRange(cpDestination, tmpPoint) == Range) {
					PotentialPoints.push_back(tmpPoint);
					x = 0; y++;
				} else {
					x++;
				}
			}
		}
		else {
			// Bottom right region.
			Bot::Print(Debug::POINTGENERATION, "\tBOTTOM RIGHT");
			for (int x = 1, y = 0; y <= Range;) {
				CPoint tmpPoint = CPoint((cpDestination.x - x), (cpDestination.y - y));
				if (UserManager::GetCellRange(cpDestination, tmpPoint) == Range) {
					PotentialPoints.push_back(tmpPoint);
					x = 0; y++;
				} else {
					x++;
				}
			}
		}

		// Find the point nearest to the player.
		newPoint = PotentialPoints[0];
		for (int i = 0; i < PotentialPoints.size(); i++) {
			Bot::Print(Debug::POINTGENERATION, "PotentialPoint [X: %d Y: %d] [RFT: %d] [RFM: %d]", PotentialPoints[i].x, PotentialPoints[i].y, UserManager::GetCellRange(cpDestination, PotentialPoints[i]), UserManager::GetCellRange(BotUser::Me->Data->CellPos, PotentialPoints[i]));
			if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, PotentialPoints[i]) <= UserManager::GetCellRange(BotUser::Me->Data->CellPos, newPoint)) {
				newPoint = PotentialPoints[i];
			}
		}

		Bot::Print(Debug::POINTGENERATION, "Most suitable point [X: %d Y: %d] [RFT: %d] [RFM: %d]", newPoint.x, newPoint.y, UserManager::GetCellRange(cpDestination, newPoint), UserManager::GetCellRange(BotUser::Me->Data->CellPos, newPoint));
	
		// Use an alternate point to the newly generated point if RANDOMIZATION is on.
		if (Bot::Configuration.Randomization) {
			// Create an array of points that are equal to the range of the closest waypoint.
			vector<CPoint> RandomizedPoints;
			for (auto xt = PotentialPoints.begin(); xt != PotentialPoints.end(); xt++) {
				if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, *xt) == UserManager::GetCellRange(BotUser::Me->Data->CellPos, newPoint)) {
					RandomizedPoints.push_back(*xt);
				}
			}

			if (RandomizedPoints.size() > 0) {
				newPoint = RandomizedPoints.at(BotUser::RandomNumberGenerator(0, (RandomizedPoints.size() - 1)));
				Bot::Print(Debug::POINTGENERATION, "New random point [X: %d Y: %d] [RFT: %d] [RFM: %d]", newPoint.x, newPoint.y, UserManager::GetCellRange(cpDestination, newPoint), UserManager::GetCellRange(BotUser::Me->Data->CellPos, newPoint));
			}
		}
	}

	Bot::Print(Debug::MOVEMENT, "Generated point [X: %d Y: %d] [RFD: %d] [RFM: %d]",
		newPoint.x, newPoint.y,
		UserManager::GetCellRange(cpDestination, newPoint),
		UserManager::GetCellRange(BotUser::Me->Data->CellPos, newPoint));

	if (newPoint.x == 0 OR newPoint.y < 0) {
		Bot::State.Terminate(Debug::MOVEMENT, "Generated a false point.");
	}

	return newPoint;
}

void ManageAttack(void) {

	// Attack loop.
	if (Bot::State.Attack.IsAttacking) {

		// Target dead.
		if (BotUser::Me->Target->Data->Dead) {

			Bot::Print(Debug::TARGET, "Target died.");

			Bot::Loot.FindNearby();

			Bot::Print(Debug::USERLIST, "Deleting dead user [Uid: %d] from the UserList.", BotUser::Me->Target->Data->Uid);
			UserManager::UserList.Delete(BotUser::Me->Target->Data->Uid);

			// Recheck health after kililng a mob to see if we need healing etc.
			BotUser::EvaluateNewValues(BotUser::Me->Data->Health.Current, 0);

			Bot::State.Attack.Reset();

			Bot::Waypoints.FindNearest();
		}

		// Cancel attacking if the target has moved out of range.
		else if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos) > Bot::Configuration.AttackDistance) {
			Bot::State.Attack.IsAttacking = false;
		}

		// Attack the target.
		else {

			// Magic attack.
			if (Bot::State.IsTrainingInt) {

				// Custom condition for killing annoyance mobs when hunting Nightmare in Abias.
				if (Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
					if (BotUser::Me->Target->Data->Name == "STIGE" OR BotUser::Me->Target->Data->Name == "SWAMP BEAST") {
						SocketMng::SendMagicAttack(BotUser::Me->TargetUid, BotUser::MagicList.AtkSpell->Id); return;
					}
				}

				// Cast Weakening if we have it.
				if (BotUser::MagicList.Contains("Weakening")) {

					// Detect if the target no longer has the Weakening debuff and reset it here.
					if (BotUser::Me->Target->IsWeakened()) {		
						if ((GetTickCount() - Bot::State.Attack.WeakenCastTick) > 10000 AND BotUser::Me->Target->Data->DebuffCount == 0) {
							//Bot::Print(Debug::GENERAL, "Resetting weakening on target.");
							BotUser::Me->Target->Weakened = false; 
						}
					} 
					
					// Have this on an if statement instead of an else because if Weakening has been reset we come straight here to re-cast.
					if (!BotUser::Me->Target->IsWeakened()) {
						// Only Weaken if the target has not yet been hit (Hp 0/0) or if it is above 15% current health.
						if ((BotUser::Me->Target->Data->Health.Maximum <= 1 AND BotUser::Me->Target->Data->Health.Current <= 1) OR BotUser::Me->Target->Data->Health.Current > (BotUser::Me->Target->Data->Health.Maximum * 15 / 100)) {
							if (!Bot::State.Attack.MagicReadySuccess) {
								SocketMng::SendMagicAttack(BotUser::Me->TargetUid, BotUser::MagicList.GetByName("Weakening")->Id);
							} return;
						}
					}
				} 

				// Send the attack spell.
				SocketMng::SendMagicAttack(BotUser::Me->TargetUid, BotUser::MagicList.AtkSpell->Id);
			}

			// Melee attack.
			else {		
				SocketMng::SendAttack();
			}

			// Use an aura.
			if (Bot::Configuration.AuraData) {
				if ((GetTickCount() - Bot::Configuration.AuraLastUseTime) > Bot::Configuration.AuraReuseTime) {
					SocketMng::SendAura(Bot::Configuration.AuraData);
				}
			}

			// Enable stamina regeneration.
			if (BotUser::Me->StaminaOn()) {
				SocketMng::ToggleStamina(false);
			}
		}
	}

	// Detect the range from the target and begin attacking if it is in range.
	else if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos) <= Bot::Configuration.AttackDistance) {
		if (BotUser::Me->IsCharStop()) {
			if (Bot::State.IsTrainingInt) {
				Sleep(600);
			} else if (BotUser::Inventory.Item.Type(6) == ItemType::BOW) {
				Sleep(1200);
			} else {
				Sleep(250 + ((BYTE)Bot::Configuration.behaviour * BotUser::RandomNumberGenerator(150, 250)));
			} Bot::State.Attack.IsAttacking = true;
		}
	}

	// Move towards the target.
	else {

		// Generate a new point to move to if the target has moved or if the attack waypoint is NULL (Checking a NULL point solves issues where the bot moves out of range of a mob that was initially in range).
		if (BotUser::Me->Target->Data->CellPos != BotUser::Me->Target->PrevCellPos OR (Bot::Waypoints.Attack.x == 0 AND Bot::Waypoints.Attack.y == 0)) {
			Bot::Print(Debug::TARGET, "Target moved... Generating new point.");
			BotUser::Me->Target->PrevCellPos = BotUser::Me->Target->Data->CellPos;
			Bot::Waypoints.Attack = GenerateNewPoint(BotUser::Me->Target->PrevCellPos, Bot::Configuration.AttackDistance);

			// Reset MagicReadySuccess if the target has moved... Prevents the bot stalling on Weakening.
			if (Bot::State.Attack.MagicReadySuccess) {
				Bot::Print(Debug::GENERAL, "MagicReadySuccess is now FALSE.");
				Bot::State.Attack.MagicReadySuccess = false;
			}
		}

		// Track movement time out.
		if ((GetTickCount() - Bot::State.Attack.ApproachTick) >= (Bot::Configuration.ApproachTimeOut * 1000)) {
			Bot::Print(Debug::TARGET, "Failed to approach target.");
			Bot::Configuration.BlackList.Add(BotUser::Me->TargetUid, 1);
			Bot::State.Attack.Reset();
		} else {
			UserManager::MoveRunRequest(Bot::Waypoints.Attack);
		}
	}
}

void ManageMovement(void) {

	if (Bot::Waypoints.IsNearNext()) {
		if (Bot::Waypoints.IsAtEnd()) {

			// Repairing & restocking (Leveling bot).
			if (Bot::State.PotsAndRepairs) {
				if (Bot::State.IsRepairing) {
					BotUser::FindNpc(Bot::Zone.GetVendor("BLACKSMITH"));
				} else if (Bot::State.IsRestocking) {
					BotUser::FindNpc(Bot::Zone.GetVendor("VENDOR"));
				} Pause();
			}

			// Abias restocking (Restock bot).
			else if (Bot::Configuration.mode == Mode::RESTOCK) {
				// Enable stamina regeneration when we have arrived at an NPC.
				SocketMng::ToggleStamina(false);
				if (RestockBot::State.IsRestocking) {
					// This conditions prevents the bot attempting to restock when the restock bot has been started between time 22 and 0.
					if (Bot::Configuration.GameHour >= 22 AND Bot::Configuration.GameHour <= 0) {
						Bot::Print(Debug::CRAFTING, "Shop is closed.");
						Bot::State.SetWait(true);
					} else {		
						BotUser::FindNpc(RestockBot::Vendor.Name);
					}
				} else {
					BotUser::FindNpc("HANWON");
					Pause();
					SocketMng::ToggleStamina(true);
				} 
			}

			// Crafting bot restocking (Crafting bot).
			else if (Bot::Configuration.mode == Mode::CRAFT) {
				if (CraftBot::State.RestockRequired) {
					BotUser::FindNpc("WAREHOUSE KEEPER");
					Pause();
				} else {
					CraftBot::Table.IsOpening = !CraftBot::Table.IsOpening;
				}
			} Bot::Waypoints.Reset();
		} else {
			Bot::Waypoints.Increment();
		}
	} else {
		UserManager::MoveRunRequest(Bot::Waypoints.GetCurrent());
	}
}

bool TargetFound(void) {

	for (auto item = UserManager::UserList.Begin(); item != UserManager::UserList.End(); item++) {

		CUser *pUser = item->second;

		if (!BotUser::IsValid(pUser))
			continue;
		if (pUser->Data->Dead)
			continue;
		if (pUser->Data->Uid == BotUser::Me->Data->Uid)
			continue;
		if (!pUser->Data->CanDie)
			continue;
		if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) > Bot::Configuration.SearchRange)
			continue;
		if (Bot::Configuration.BlackList.Contains(pUser->Data->Uid, 1) OR Bot::Configuration.BlackList.Contains(pUser->Data->Uid, 0))
			continue;
		if (Bot::Configuration.IgnoreList.Contains(pUser->Data->Name.GetString()))
			continue;
		if (pUser->IsPlayer())
			continue;
		if (pUser->IsMagicMob() AND pUser->Data->DebuffCount > 0)
			continue;

		if (pUser->IsCharAttack()) {
			if (pUser->HasTarget()) {
				if (pUser->TargetUid != BotUser::Me->Data->Uid) {
					continue;
				}
			}
			else {
				continue;
			}
		}
		else {
			if (!Bot::Configuration.TargetList.Contains(pUser->Data->Name.GetString())) {
				continue;
			}
		}

		// Iterate through the list and make sure no other player is attacking the newly found relevant mob.
		for (auto xt = UserManager::UserList.Begin();; xt++) {

			CUser *ppUser = xt->second;

			// Finished searching for players attacking the new relevant target.
			if (xt == UserManager::UserList.End()) {
				// If a target has previously been set, then check the new targets range against the previous one.
				if (BotUser::IsValid(BotUser::Me->Target)) {
					if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) < UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos)) {
						BotUser::Me->Target = pUser;
					}
				} else {
					// Set the target for further evaluation if we haven't got one already.
					BotUser::Me->Target = pUser;
				} break;
			}

			// Check if a player is hitting the newly found relevant target.
			if (BotUser::IsValid(ppUser)) {
				if (ppUser->IsPlayer() AND ppUser->Data->Uid != BotUser::Me->Data->Uid AND ppUser->HasTarget() AND ppUser->TargetUid == pUser->Data->Uid) {
					Bot::Print(Debug::TARGET, "User [%s] [Uid: %d] is attacking relevant target [%s] [Uid: %d]", ppUser->Data->Name, ppUser->Data->Uid, pUser->Data->Name, pUser->Data->Uid);
					break;
				}
			}
		}
	}

	if (BotUser::IsValid(BotUser::Me->Target)) {

		// Set our TargetUid and update PrevCellPos for later checks.
		BotUser::Me->TargetUid = BotUser::Me->Target->Data->Uid;
		BotUser::Me->Target->PrevCellPos = BotUser::Me->Target->Data->CellPos;

		// Display the newly found target on the console.
		Bot::Print(Debug::TARGET, "Found target [%s] [Uid: %d] [X: %d Y: %d] [R: %d]",
			BotUser::Me->Target->Data->Name.GetString(),
			BotUser::Me->TargetUid,
			BotUser::Me->Target->Data->CellPos.x, BotUser::Me->Target->Data->CellPos.y,
			BotUser::Me->Target->RangeFromCell(BotUser::Me->Data->CellPos));

		// Only generate a point to move to if the target is out of range.
		if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos) > Bot::Configuration.AttackDistance) {
			// Waypoints.Attack holds the point in which we must move to to begin attacking.
			Bot::Waypoints.Attack = GenerateNewPoint(BotUser::Me->Target->Data->CellPos, Bot::Configuration.AttackDistance);
		}

		Bot::State.Attack.ApproachTick = GetTickCount();
		Bot::State.HasTarget = true;
	}

	if (Bot::State.IsTrainingInt) {
		// Recheck health after kililng a mob to see if we need healing etc.
		//BotUser::EvaluateNewValues(BotUser::Me->Data->Health.Current, 0);
	}

	return Bot::State.HasTarget;
}

void Test(void) {

	//printf_s("\n\n\n\n");

	// Special attack data.

	//for (vector<SPECIALATTACK_DATA>::const_iterator xt = BotUser::SpecialAttackList.Begin(); xt != BotUser::SpecialAttackList.End(); xt++) {
	//	printf_s("Name: [%s] Id [%d]\nTargetMethod [%d]\nRange [%d]\nHp [%d]  Mp [%d]  Stam [%d]\nDuration [%d]  CoolDown [%d]\nPower [%d]\nClass [%d]\n\n", xt->Name, xt->Id, xt->TargetMethod, xt->Range, xt->Usage.Health, xt->Usage.Mana, xt->Usage.Stamina, xt->Duration, xt->CoolDown, xt->Power, xt->Class);
	//}

	// Magic data.

	//for (vector<MAGIC_DATA>::const_iterator xt = BotUser::MagicList.Begin(); xt != BotUser::MagicList.End(); xt++) {
	//	printf_s("Name: %s  Id: %d\nTargetMethod: %d\nRange: %d\Mp: %d\nPower: %d\nCastTime: %d\n\n", xt->Name, xt->Id, xt->TargetMethod, xt->Range, xt->MpUsage, xt->Power, xt->CastTime);
	//}

	//// Magic belt tests

	//int Index = Bot::Magic.GetBeltSelIndex();
	//Bot::Print("\tSpell: %s [%X]\n\tNumber: %d\n\tRange: %d\n\tMP: %d\tHP: %d\n\tDamage: %d\n\tSkillReq: %d\n\t",
	//	Bot::Magic.GetSpellName(Index),
	//	Bot::Magic.Belt[Index],
	//	Bot::Magic.GetSpellNumber(Index),
	//	Bot::Magic.GetSpellRange(Index),
	//	Bot::Magic.GetSpellMpUsage(Index),
	//	Bot::Magic.GetSpellHpUsage(Index),
	//	Bot::Magic.GetSpellPower(Index),
	//	Bot::Magic.GetSpellSkillReq(Index));

	// Inventory item tests

	/*for (int i = 10; i < 20; i++) {

		if (BotUser::Me->Data->Inventory[i].PictureNumber == -1)
		break;

		Bot::Print("Address: %X\n\tIndex: %d\n\tName: %s [%X]\n\tType: %d [%X]\n\tArmPos: %d\n\tPictureNumber: %d [%x]\n\tGender: %d [%x]\n\tMinDmg: %d\tMaxDmg: %d\n\tWeight: %d\n\tSpeed: %d\n\tStr: %d\tInt: %d\tDex: %d\tSkill: %d\n\tHP: %d\tMP: %d\n\tDura: %d\tMaxDura: %d\tOrgMaxDura: %d\n\tMagicDmgMin: %d\tMagicDmgMax: %d\n\tUpgradeLvl: %d [%X]\n\tSpecialNumber: %d\n",
		&BotUser::Me->Data->Inventory[i],
		BotUser::Me->Data->Inventory[i].Index,
		BotUser::Me->Data->Inventory[i].Name,
		&BotUser::Me->Data->Inventory[i].Name,
		BotUser::Me->Data->Inventory[i].Type,
		&BotUser::Me->Data->Inventory[i].Type,
		BotUser::Me->Data->Inventory[i].ArmPos,
		BotUser::Me->Data->Inventory[i].PictureNumber,
		&BotUser::Me->Data->Inventory[i].PictureNumber,
		BotUser::Me->Data->Inventory[i].Gender,
		&BotUser::Me->Data->Inventory[i].Gender,
		BotUser::Me->Data->Inventory[i].MinDamageOrDefence,
		BotUser::Me->Data->Inventory[i].MaxDamageOrDefence,
		BotUser::Me->Data->Inventory[i].Weight,
		BotUser::Me->Data->Inventory[i].Speed,
		BotUser::Me->Data->Inventory[i].Required.Str,
		BotUser::Me->Data->Inventory[i].Required.Int,
		BotUser::Me->Data->Inventory[i].Required.Dex,
		BotUser::Me->Data->Inventory[i].Required.Skill,
		BotUser::Me->Data->Inventory[i].HP,
		BotUser::Me->Data->Inventory[i].MP,
		BotUser::Me->Data->Inventory[i].Dura.Current,
		BotUser::Me->Data->Inventory[i].Dura.Maximum,
		BotUser::Me->Data->Inventory[i].Dura.OriginalMaximum,
		BotUser::Me->Data->Inventory[i].MagicDmgOrDef.Minimum,
		BotUser::Me->Data->Inventory[i].MagicDmgOrDef.Maximum,
		BotUser::Me->Data->Inventory[i].UpgradeLvl,
		&BotUser::Me->Data->Inventory[i].UpgradeLvl,
		BotUser::Me->Data->Inventory[i].SpecialNumber
		);
		}*/

	// Belt tests

	//for (int i = 0; i < 4; i++) {
	//	Bot::Print("Address: %X\n\tName: %s [%X]\n\tType: %d [%X]\n\tArmPos: %d\n\tPicNum: %d [%x]\n\tGender: %d [%x]\n\tMinDmg: %d\tMaxDmg: %d\n\tWeight: %d\n\tSpeed: %d\n\tStr: %d\tInt: %d\tDex: %d\tSkill: %d\n\tHP: %d\tMP: %d\n\tDura: %d\tMaxDura: %d\tOrgMaxDura: %d\n\tMagicDmgMin: %d\tMagicDmgMax: %d\n\tUpgradeLvl: %d [%x]\n",
	//		&BotUser::Me->Data->Belt[i],
	//		BotUser::Me->Data->Belt[i].Name,
	//		&BotUser::Me->Data->Belt[i].Name,
	//		BotUser::Me->Data->Belt[i].Type,
	//		&BotUser::Me->Data->Belt[i].Type,
	//		BotUser::Me->Data->Belt[i].ArmPos,
	//		BotUser::Me->Data->Belt[i].PicNum,
	//		&BotUser::Me->Data->Belt[i].PicNum,
	//		BotUser::Me->Data->Belt[i].Gender,
	//		&BotUser::Me->Data->Belt[i].Gender,
	//		BotUser::Me->Data->Belt[i].MinDamageOrDefence,
	//		BotUser::Me->Data->Belt[i].MaxDamageOrDefence,
	//		BotUser::Me->Data->Belt[i].Weight,
	//		BotUser::Me->Data->Belt[i].Speed,
	//		BotUser::Me->Data->Belt[i].Required.Str,
	//		BotUser::Me->Data->Belt[i].Required.Int,
	//		BotUser::Me->Data->Belt[i].Required.Dex,
	//		BotUser::Me->Data->Belt[i].Required.Skill,
	//		BotUser::Me->Data->Belt[i].HP,
	//		BotUser::Me->Data->Belt[i].MP,
	//		BotUser::Me->Data->Belt[i].Dura.Current,
	//		BotUser::Me->Data->Belt[i].Dura.Maximum,
	//		BotUser::Me->Data->Belt[i].Dura.OriginalMaximum,
	//		BotUser::Me->Data->Belt[i].MagicDmgOrDef.Minimum,
	//		BotUser::Me->Data->Belt[i].MagicDmgOrDef.Maximum,
	//		BotUser::Me->Data->Belt[i].UpgradeLvl,
	//		&BotUser::Me->Data->Belt[i].UpgradeLvl
	//		);
	//}

	// Local entity data

	for (auto item = UserManager::UserList.Begin(); item != UserManager::UserList.End(); item++) {
		CUser *pUser = item->second;
		if (BotUser::IsValid(pUser)) {
			//if (pUser->Data->IsMe)
			{
				//if (pUser->IsPlayer())
				{
					printf_s("[%s] [%s] [Uid: %d] [%X]\n", pUser->Data->Name, pUser->Data->Guild, pUser->Data->Uid, &pUser->Data);
					printf_s("RangeFromMe: [%d]\n", UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos));
					//printf_s("[Hp: %d / %d]\n", pUser->Data->Health.Current, pUser->Data->Health.Maximum);
					//printf_s("Current [X: %d Y: %d] [R: %d]\n", pUser->Data->CellPos.x, pUser->Data->CellPos.y, UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos));
					//printf_s("Previous [X: %d Y: %d] [R: %d]\n", pUser->PreviousPoint.x, pUser->PreviousPoint.y, UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->PreviousPoint));
					//printf_s("Dead: %d\n", pUser->Data->Dead);
					//printf_s("Me: %d\n", pUser->Data->IsMe);
					//printf_s("At: %d [%X]\n", pUser->Data->At, &pUser->Data->At);
					//printf_s("Style: %d [%X]\n", pUser->Data->Style, &pUser->Data->Style);
					//printf_s("BloodType: %d [%X]\n", pUser->Data->BloodType, &pUser->Data->BloodType);
					//printf_s("Class: %d [%X]\n", pUser->Data->Class, &pUser->Data->Class);
					//printf_s("Stance: %d [%X]\n", pUser->Data->Stance, &pUser->Data->Stance);
					//printf_s("%s\n", pUser->Data->Inventory[6].Name);
					//printf_s("Skin: %d [%X]	Hair: %d [%X]\n", pUser->Data->Skin, &pUser->Data->Skin, pUser->Data->Hair, &pUser->Data->Hair);
					//printf_s("CurrentWeapon: %d [%X]\n", pUser->Data->CurrentWeapon, &pUser->Data->CurrentWeapon);
					//printf_s("Level: [%d]\n", pUser->Data->Level);
					//printf_s("Str: [%d] Int: [%d] Dex: [%d]\n", pUser->Data->Stats.Strength, pUser->Data->Stats.Intelligence, pUser->Data->Stats.Dexterity);

				}
			}
		}
	}

	//// User stats & skills tests 

	//Bot::Print("Str: %d [%X]\nInt: %d\nDex: %d\nWis: %d\nCha: %d\nCon: %d\n\nSword: %d [%X]\nSpear: %d\nAxe: %d\nKnuck: %d\nBow: %d\nStaff: %d\n",
	//	BotUser::Me->Data->Stats.Strength,
	//	&BotUser::Me->Data->Stats.Strength,
	//	BotUser::Me->Data->Stats.Intelligence,
	//	BotUser::Me->Data->Stats.Dexterity,
	//	BotUser::Me->Data->Stats.Wisdom,
	//	BotUser::Me->Data->Stats.Charisma,
	//	BotUser::Me->Data->Stats.Constitution,
	//	BotUser::Me->Data->WeaponSkills.Sword,
	//	&BotUser::Me->Data->WeaponSkills.Sword,
	//	BotUser::Me->Data->WeaponSkills.Spear,
	//	BotUser::Me->Data->WeaponSkills.Axe,
	//	BotUser::Me->Data->WeaponSkills.Knuck,
	//	BotUser::Me->Data->WeaponSkills.Bow,
	//	BotUser::Me->Data->WeaponSkills.Staff);
}

 bool Initialize(void) {

	 try {

		 if (!BotUser::IsValid(BotUser::Me)) {
			 throw "BotUser invalid.";
		 }

		 if (Bot::Configuration.mode != Mode::HEAL AND Bot::Waypoints.Count() <= 0) {
			 throw "No waypoints loaded.";
		 }

		 switch (Bot::Configuration.mode) {

			 case Mode::LEVEL: {

				SocketMng::SetStance(Stance::BATTLE);

				// Determine if we need to first run a PRELVL_PATH before hunting. 
				if (Bot::Waypoints.PreLvlPath.size() > 0) {

					// But only if the bot is not resuming from performing repairs and potion restocks.
					if (!Bot::State.PotsAndRepairs) {

						int PreLvlNearest = UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.PreLvlPath.at(0));
						int LvlNearest = UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.LvlPath.at(0));

						for (int i = 0; i < Bot::Waypoints.PreLvlPath.size(); i++) {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.PreLvlPath.at(i)) < PreLvlNearest) {
								PreLvlNearest = UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.PreLvlPath.at(i));
							}
						}
						for (int i = 0; i < Bot::Waypoints.LvlPath.size(); i++) {
							if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.LvlPath.at(i)) < LvlNearest) {
								LvlNearest = UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Waypoints.LvlPath.at(i));
							}
						}
						if (PreLvlNearest < LvlNearest) {
							Bot::Print(Debug::MOVEMENT, "Running the PRELVL_PATH.");
							Bot::State.IsPathRunning = true;
						}
					}
				}

				// Set the attacking distance.
				switch (BotUser::Inventory.Item.Type(6)) {
				case ItemType::AXE:
				case ItemType::KNUCKLE:
				case ItemType::SWORD:
				case ItemType::FIST:
				case ItemType::STAFF:
					Bot::Configuration.AttackDistance = 2;
					break;
				case ItemType::BOW:
					Bot::Configuration.AttackDistance = 14;
					break;
				case ItemType::SPEAR:
					Bot::Configuration.AttackDistance = 4;
					break;
				}

				// Determine if we're leveling int or not.
				if (BotUser::Inventory.Item.Type(6) == ItemType::STAFF OR BotUser::Inventory.Item.Type(6) == ItemType::FIST OR BotUser::Inventory.Item.HasMagicAvailable(6)) {
					if (BotUser::MagicList.Belt.SelectedIndex() != -1 AND BotUser::MagicList.Contains(BotUser::MagicList.Belt.SpellName())) {

						// Set the attacking spell.
						BotUser::MagicList.AtkSpell = BotUser::MagicList.GetByName(BotUser::MagicList.Belt.SpellName());

						// Set the new AttackDistance. Evaluate differently if the user has the spell Weakening.
						BotUser::MagicList.Contains("Weakening") ?
							Bot::Configuration.AttackDistance = (BotUser::MagicList.GetByName("Weakening")->Range * 2) :
							Bot::Configuration.AttackDistance = (BotUser::MagicList.AtkSpell->Range * 2);

						// Reduce the attack range by 2 if we're hunting NIGHTMARE. This solves potential range issues with bugged Nightmares when casting Weakening.
						if (Bot::Configuration.TargetList.Contains("NIGHTMARE"))
							Bot::Configuration.AttackDistance -= 2;

						// Enable int hunting.
						Bot::Print(Debug::GENERAL, "Intelligence training enabled.");
						Bot::State.IsTrainingInt = true;
					}
				}
			 } break;

			 case Mode::RESTOCK:
			 case Mode::CRAFT: {
			 } break;

			 case Mode::HEAL: {
			 } break;
		 }

		 // Set the aura data.
		 SPECIALATTACK_DATA *tmpAuraData = BotUser::SpecialAttackList.GetByName(Bot::Configuration.mode == Mode::LEVEL ? Bot::Configuration.Aura : "Rapid MP Recovery");

		 // Verify the data and set the re-use time.
		 if (tmpAuraData) {

			 // Differentiate between bow dodge and knuckle dodge.
			 if (BotUser::Inventory.Item.Type(6) == ItemType::KNUCKLE OR BotUser::Inventory.Item.Type(6) == ItemType::BOW) {
				 tmpAuraData = BotUser::SpecialAttackList.GetByClassNdName(Bot::Configuration.Aura, BotUser::Inventory.Item.Type(6));
			 }

			 if (tmpAuraData->TargetMethod == 3 OR tmpAuraData->Range >= 1) {
				 Bot::Configuration.AuraReuseTime = (tmpAuraData->CoolDown + 100);
			 } else {
				 Bot::Configuration.AuraReuseTime = tmpAuraData->Duration;
			 } Bot::Configuration.AuraData = tmpAuraData;
		 }

		 Sleep(750);

		 Bot::Print(Debug::GENERAL, "Started.");

		 Bot::Waypoints.FindNearest();

		 return true;

	 } catch (LPCSTR msg) {
		 Bot::Print(Debug::GENERAL, msg);
	 } return false;
 } 

 BOOL KeyDown(int Key) {
	 if (GetAsyncKeyState(Key)) {
		 do {
			 Sleep(1);
		 } while (GetAsyncKeyState(Key)); 
		 return true;
	 } return false;
 }


 DWORD BotUser::Process(LPVOID lpParameter) {
 
	 // This works.
	 //mouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandle(NULL), NULL);

	 // These do not work.
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle("InjectableMFC.dll"), GetCurrentThreadId());
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle(NULL), GetCurrentThreadId());
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, NULL, GetCurrentThreadId());

	 // This works for the soma window but it causes bizarre system behaviour.
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle("InjectableMFC.dll"), NULL);
	 // These do not work.
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle(NULL), NULL);
	 // mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, NULL, NULL);

	 // Main thread loop
	 for (;; Sleep(10)) {

		 //// Check for mouse input.
		 //MSG Msg; 
		 //if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE)) {
			// TranslateMessage(&Msg);
			// DispatchMessage(&Msg);
		 //}

		 // Check for keypresses.
		 if (KeyDown(VK_INSERT)) {
			 if (!Bot::State.IsRunning) {
				 Bot::State.IsRunning = Initialize();
			 }
		 }
		 else if (KeyDown(VK_END)) {
			 if (KeyDown(VK_SHIFT)) {
				 SocketMng::SendLogOut();
			 }else {
				 if (Bot::State.IsRunning) {
					 Bot::State.Terminate(Debug::GENERAL, NULL);
				 }
			 }
		 }
		 else if (KeyDown(VK_HOME)) {
			 if (KeyDown(VK_SHIFT)) {
				 if (Bot::Waypoints.TmpPath.size() > 0) {
					 Bot::Waypoints.Save("Waypoints.wpf");
				 } else {
					 Bot::Print(Debug::MOVEMENT, "There are no waypoints to save.");
				 }
			 } else {
				 if (BotUser::IsValid(BotUser::Me)) {
					Bot::Waypoints.Add();
				 }
			 }
		 }
		 else if (KeyDown(VK_DELETE)) {
			 if (KeyDown(VK_SHIFT)) {
				 Bot::Waypoints.Clear(FileType::TEMPORARY_PATH);
				 Bot::Print(Debug::MOVEMENT, "Temporary waypoint list cleared.");
			 } else {
				 if (Bot::Waypoints.TmpPath.size() > 0) {
					 Bot::Print(Debug::MOVEMENT, "Removing waypoint %d %d from the list.", Bot::Waypoints.TmpPath[Bot::Waypoints.TmpPath.size() - 1].x, Bot::Waypoints.TmpPath[Bot::Waypoints.TmpPath.size() - 1].y);
					 Bot::Waypoints.TmpPath.erase(Bot::Waypoints.TmpPath.end() - 1);
				 }
			 }
		 }
		 else if (KeyDown(VK_LEFT)) {
			 //Bot::Configuration.CastFast = !Bot::Configuration.CastFast;
			 //printf_s("\n\n\tCastFast is now %s\n", Bot::Configuration.CastFast ? "TRUE" : "FALSE");
			 //Test();
		 }
		 else if (KeyDown(VK_RIGHT)) {
			// Bot::OutPut.SoundPlay(IDR_CHAT);
			// Bot::Configuration.Test = 0;
			 //printf_s("\n\n\tTest is now [%d]\n", Bot::Configuration.Test);
		 }
		 else if (KeyDown(VK_UP)) {		
			// Bot::Configuration.tmpDelay += 100;
			// printf_s("\n\n\n\t\tDELAY IS NOW [%d]\n\n\n", Bot::Configuration.tmpDelay);
			 //Bot::Configuration.Test += 10;
			// printf_s("\n\n\tTest is now [%d]\n", Bot::Configuration.Test);
		 }
		 else if (KeyDown(VK_DOWN)) {
			 //Bot::Configuration.tmpDelay -= 100;
			 //printf_s("\n\n\n\t\tDELAY IS NOW [%d]\n\n\n", Bot::Configuration.tmpDelay);	
			// Bot::OutPut.SoundPlay(IDR_PKDETECT);
			// Bot::Configuration.Test -= 10;
			 //printf_s("\n\n\tTest is now [%d]\n", Bot::Configuration.Test);
			 /*for (int deltaX = -25; deltaX <= 25; deltaX++) {
				 for (int deltaY = -25; deltaY <= 25; deltaY++) {
					 int newX = BotUser::Me->Data->CellPos.x + deltaX;
					 int newY = BotUser::Me->Data->CellPos.y + deltaY;
					 CPoint coord(newX, newY);
					 printf_s("(%d, %d): IsMoveable? %d\n", newX, newY, UserManager::IsMoveable(coord));
				 }
			 }*/
			// printf_s("IsMoveable [%d]\n", UserManager::IsMoveable(CPoint(BotUser::Me->Data->CellPos.x, BotUser::Me->Data->CellPos.y - 2)));
			 //SocketMng::SendPm("AKAH", "hello");
			 //SocketMng::SendChat("Weeeee");
			// SocketMng::SendChat((BYTE)ChatType::PARTY, NULL, "Hello");
			 Test();
		 }

		 if (Bot::State.IsRunning) {

			 // Sound a notification if a certain user is nearby (GM Live Notify also uses this).
			 if (Bot::NearbyUserDetection.IsNotifying) {
				 if (++Bot::NearbyUserDetection.NotificationCount >= 300) {
					 Bot::OutPut.SoundPlay(IDR_NEARBYUSER);
					 Bot::NearbyUserDetection.NotificationCount = 0;
					 if (++Bot::NearbyUserDetection.NotificationTotalCount >= Bot::NearbyUserDetection.NotificationLimit) {
						 Bot::NearbyUserDetection.NotificationTotalCount = 0;
						 Bot::NearbyUserDetection.IsNotifying = false;
					 }
				 }
			 }

			 //// Check if a GM has come online.
			 //if (Bot::Configuration.GmDetection.IsEnabled) {
				// if (++Bot::Configuration.GmDetection.Count >= 1000) {
				//	 //SocketMng::SendChat("/lastonline " + *Bot::Configuration.GmDetection.CurrentGm);
				//	 SocketMng::SendPm(*Bot::Configuration.GmDetection.CurrentGm, " ");
				//	 Bot::Configuration.GmDetection.Count = 0;
				// }
			 //}

			 switch (Bot::Configuration.mode) {

				 // Healing wind bot.
				 case Mode::HEAL: {

					// Detect the range from the target and begin healing if he or she is in range.
					if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos) <= Bot::Configuration.AttackDistance) {
						// Check for low health.
					}
					else { // Move to the target.
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, BotUser::Me->Target->Data->CellPos) > Bot::Configuration.AttackDistance) {
							if (BotUser::Me->Target->PrevCellPos != BotUser::Me->Target->Data->CellPos) {
								Bot::Print(Debug::TARGET, "Target moved... Generating new point.");
								BotUser::Me->Target->PrevCellPos = BotUser::Me->Target->Data->CellPos;
								Bot::Waypoints.Attack = GenerateNewPoint(BotUser::Me->Target->PrevCellPos, Bot::Configuration.AttackDistance);
							}

							//if (Bot::Waypoints.Attack.x AND Bot::Waypoints.Attack.y) 
							{
								UserManager::MoveRunRequest(Bot::Waypoints.Attack);
							}
						}
					}

									  
					//// Use Rapid MP Recovery.
					//if (Bot::Configuration.AuraData) {
					//	if ((GetTickCount() - Bot::Configuration.AuraLastUseTime) > Bot::Configuration.AuraReuseTime) {
					//		SocketMng::SendAura(Bot::Configuration.AuraData);
					//	}
					//}
				 } break;

				 // Leveling bot.
				 case Mode::LEVEL: {

					// Death handler.
					if (Bot::State.AwaitingRevival) {
						if (Me->Data->Dead) {
							if (Bot::Wait(Bot::Configuration.Death.Delay)) {
								if (Bot::Configuration.NoWarp) {
									SocketMng::SendRevival(REVIVE);
									Bot::Configuration.Death.Increment();
								} else {
									SocketMng::SendRevival(WARP);
								}
							}
						} else {
							if (Bot::Wait(2000)) {
								Bot::Configuration.Death.Increment();
								if (Bot::Waypoints.VendorAndBlkSmithLoaded()) {
									Bot::Waypoints.RunPepairAndRestock();
								} else {
									Bot::State.Terminate(Debug::GENERAL, "No Vendor or Blacksmith waypoints loaded. Terminating."); continue;
								}
							}
						} continue;
					}

					// Warping.
					if (Bot::State.Warp.Required) {
						if (Bot::State.Warp.InProgress) {
							if (Bot::State.Warp.Done) {
								if (Bot::Wait(Bot::State.Warp.Delay)) {
									Bot::State.Warp.Reset();
									if (Bot::Waypoints.VendorAndBlkSmithLoaded()) {
										Bot::Waypoints.RunPepairAndRestock();
									} else {
										Bot::State.Terminate(Debug::GENERAL, "No Vendor or Blacksmith waypoints loaded. Terminating."); continue;
									}
								}
							}
						} else if (Bot::State.IsBeingPkd) {
							// Wait to warp if we are being PK'd (Looks suspicious if a warp is too fast)
							if (Bot::Wait(1000 + ((BYTE)Bot::Configuration.behaviour * 750))) {
								Bot::State.IsBeingPkd = false;
							}
						} else {
							SocketMng::SendUseItem(Bot::Items.TpIndex);
						} continue;
					}

					// Potions.
					if (Bot::State.HealthLow) {
						BotUser::Inventory.Item.Count(Bot::Items.HpPotionIndex) > 0 ? SocketMng::SendUseItem(Bot::Items.HpPotionIndex) : Bot::State.HpOut();
					} else {
						// Only regenerate mana if health is not low (Prioritize health).
						if (Bot::State.ManaLow) {
							BotUser::Inventory.Item.Count(Bot::Items.MpPotionIndex) > 0 ? SocketMng::SendUseItem(Bot::Items.MpPotionIndex) : Bot::State.MpOut();
						}
					}

					// Healing.
					if (Bot::State.Heal.SafeHealRequired) {
						// This prevents the bot from continuing with the heal process until it is safe to do so (Determined by HandleMove on PacketParser).
					} else if (Bot::State.Heal.WepChangeRequired) {
						if (Bot::Wait(Bot::State.Heal.Delay)) {
							Bot::State.Heal.ReEquipWepRequired = true;
							Bot::State.Heal.WepChangeRequired = false;
							// Remove the shield if one is equipped.
							if (Bot::Items.ShieldIndex != -1) {
								SocketMng::SwitchItem(7, -1);
							} // Equip a staff if necessary.
							if (Bot::Items.WeaponIndex != -1) {
								SocketMng::SwitchItem(Bot::Items.WeaponIndex, 6);
							} // Otherwise just remove the current weapon.
							else {
								SocketMng::SwitchItem(6, -1);
								Bot::Items.WeaponIndex = Bot::Items.InvEmptyIndex;
							}
						} continue;
					} else if (Bot::State.Heal.Required) {
						SocketMng::SendMagicAttack(Me->Data->Uid, BotUser::MagicList.GetByName("Healing LV")->Id); continue;
					} else if (Bot::State.Heal.ReEquipWepRequired) {
						if (Bot::Wait(Bot::State.Heal.Delay)) {
							Bot::State.Heal.ReEquipWepRequired = false;
							SocketMng::SwitchItem(Bot::Items.WeaponIndex, 6);
							if (Bot::Items.ShieldIndex != -1) {
								SocketMng::SwitchItem(Bot::Items.ShieldIndex, 7);
							}
						} continue;
					}

					// Repairing & Restocking.
					if (Bot::State.PotsAndRepairs) {
						ManageMovement(); continue;
					}

					// Path running.
					if (Bot::State.IsPathRunning) {
						if (Bot::State.IsEnteringPortal) {
							FindNpc(Bot::Waypoints.Portal);
						} else {
							ManageMovement();
						} continue;
					}

					// Loot items.
					if (Bot::State.IsLooting) {
						if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, Bot::Loot.GetCurrent().Point) <= 4) {
							if (Bot::Wait(Bot::Loot.Delay)) {
								SocketMng::PickUpItem(Bot::Loot.GetCurrent());
							}
						} else {
							UserManager::MoveRunRequest(Bot::Loot.GetCurrent().Point);
						} continue;
					}

					// Attacks & Movement.
					if (Bot::State.HasTarget) {
						ManageAttack();
					} else {
						// Locate nearest target.
						if (!TargetFound()) {
							ManageMovement();
						}
					} 
				 } break;

				// Abias restock bot.
				 case Mode::RESTOCK: {
					if (Bot::State.IsWaiting) {
						Sleep(1000);
					} else if (RestockBot::State.JustBought) {
						if (Bot::Wait((BYTE)Bot::Configuration.behaviour * 1000)) {
							SocketMng::ToggleStamina(true);
							RestockBot::State.JustBought = false;
							RestockBot::State.IsWaitingToBuy = false;
						}
					} else if (RestockBot::State.IsWaitingToBuy) {
						if (Bot::Wait((BYTE)Bot::Configuration.behaviour * 1500)) {
							SocketMng::ShopBuy(RestockBot::Vendor.ID, RestockBot::Vendor.Item);
							RestockBot::State.JustBought = RestockBot::State.IsWaitingToBuy;
						} 				
					} else {
						ManageMovement();
					} 
				 } break;

				// Crafting bot.
				 case Mode::CRAFT: {
					if (CraftBot::Table.IsOpen) {
						SocketMng::SendCraftItem(); Sleep(100);
					} else if (CraftBot::Table.IsOpening) {
						FindNpc(CraftBot::Table.Name);
					} else if (CraftBot::State.NpcRequired) {
						CraftBot::Get.Type() == "Accessory" ? FindNpc("ACCESS VENDOR") : FindNpc("BLACKSMITH ");
					} else {
						ManageMovement();
					} 
				 } break;
			 }
		 }
	 } return 0;
 }

void BotUser::FindNpc(CString Name) {

	if (!Bot::State.IsOpeningNpc) {

		CUser* pUser = nullptr;

		// Dirty fix for the bot opening the wrong Warehouse during crafting.
		if (Bot::Configuration.mode == Mode::CRAFT AND CraftBot::State.RestockRequired AND CraftBot::Get.Type() == "Accessory" OR CraftBot::Get.Type() == "Cooking" OR CraftBot::Get.Type() == "Potion") {
			
			// First find any Warehouse in the userlist.
			if (UserManager::UserList.GetByName(Name, pUser)) {

				// Then iterate through the userlist to try to find a warehouse that's closer.
				for (auto xt = UserManager::UserList.Begin(); xt != UserManager::UserList.End(); xt++) {
					if (xt->second->Data->Name == "WAREHOUSE KEEPER" AND UserManager::GetCellRange(xt->second->Data->CellPos, BotUser::Me->Data->CellPos) < UserManager::GetCellRange(pUser->Data->CellPos, BotUser::Me->Data->CellPos)) {
						pUser = xt->second;
					} 
				} SocketMng::OpenNpc(pUser);
			} else {
				Bot::State.Terminate(Debug::NPC, "Could not locate NPC [" + Name + "]\n");
			}
		} else {
			if (UserManager::UserList.GetByName(Name, pUser)) {
				SocketMng::OpenNpc(pUser);
			} else {
				Bot::State.Terminate(Debug::NPC, "Could not locate NPC [" + Name + "]\n");
			}
		}
	}
}

bool BotUser::IsValid(CUser *pUser) {
	// Check the pointer.
	if (!pUser) {
		return false;
	}
	// Check the game pointer.
	if (!pUser->Data) {
		return false;
	}
	return true;
}

DWORD BotUser::ResolvePointer(DWORD pAddress, int pLevels, ...) {
	DWORD dwResult = pAddress;

	// Initialize a va_list variable.
	va_list vaarg;
	// Take the number of arguments instructed by pLevels from the "..." 
	va_start(vaarg, pLevels);

	for (int i = 0; i < pLevels; i++) {
		dwResult = *(DWORD*)(dwResult)+va_arg(vaarg, int);
	}

	va_end(vaarg);

	return dwResult;
}

BOOL BotUser::CaptureScreen(CString Type)
{
	//CImage img;
	//CString filePath = "C:\\Tmp\\File.jpeg";
	//HWND hWndParent = ::FindWindow(NULL, "Myth of Soma : The Rise of Gorshak");
	//
	//img.GetDC();

	//img.m_hDC = GetWindowDC(hWndParent);
	//img.Save(filePath);
	return TRUE;
}

void BotUser::EvaluateNewValues(int NewHp, int NewMp) {

	if (NewHp) {
		if (Bot::State.Heal.Enabled) {

			// Player is already healing.
			if (Bot::State.Heal.Required) {
				if (NewHp > (BotUser::Me->Data->Health.Maximum * Bot::Configuration.HealUntilThreshold / 100)) {
					Bot::Print(Debug::GENERAL, "Heal done.");
					Bot::State.Heal.Required = false;

					// Reset AuraLastUseTime to force an aura cast after healing is done (Only for melee)
					if (!Bot::State.IsTrainingInt) {
						Bot::Configuration.AuraLastUseTime = GetTickCount();
					}
				}
			}
			else {

				// Make sure the player actually has heal.
				if (BotUser::MagicList.Contains("Healing LV")) {

					int HealThreshold = Bot::Configuration.HealThreshold;

					// Change the HealThreshold if we're path running through dangerous areas.
					if (Bot::State.IsPathRunning) {
						if (Bot::Zone.Current() == Map::VOD) {
							if (Bot::Configuration.TargetList.Contains("WRAITH") OR Bot::Configuration.TargetList.Contains("LICH LORD") OR Bot::Configuration.TargetList.Contains("PRISMATIC SHADOW") OR Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
								if (BotUser::Me->Data->Stats.Strength <= 800) {
									HealThreshold += 30;
									if (HealThreshold > 99) {
										HealThreshold = 90;
									}
								}
							}
						}
					}

					// 
					/*if (Bot::State.IsTrainingInt) {
						if (!Bot::State.HasTarget) {
							HealThreshold += 15;
						}
					}*/

					if (NewHp < (BotUser::Me->Data->Health.Maximum * HealThreshold / 100)) {

						Bot::Print(Debug::GENERAL, "Player needs healing.");

						try {

							// Currently engaged in combat.
							if (Bot::State.HasTarget) {
								// If the mob is a magic mob then it is safe to heal in all circumstances.
								if (!BotUser::Me->Target->IsMagicMob()) {
									// Do not heal if we're melee hunting. If we're training int and are being attacked by a melee mob then we will be forced to heal.
									if (!Bot::State.IsTrainingInt) {
										throw "But it is not safe to heal because we're engaged in melee combat.";
									}
								}
							}

							// Path running or running around looking for mobs.
							else {
								// Check for aggressive nearby mobs before healing.
								for (auto xt = UserManager::UserList.Begin(); xt != UserManager::UserList.End(); xt++) {
									CUser *pUser = xt->second;
									if (IsValid(pUser) AND !pUser->Data->IsMe AND !pUser->IsMagicMob() AND pUser->IsAggressive()) {
										if (UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos) < (pUser->AggroRange + 4)) {
											// HandleMove on PacketParser will determine when it is actually safe to heal after enabling SafeHealRequired.
											Bot::Print(Debug::GENERAL, "Not safe to heal because of [%s] [Uid: %d] [RFM: %d]", pUser->Data->Name, pUser->Data->Uid, UserManager::GetCellRange(BotUser::Me->Data->CellPos, pUser->Data->CellPos));
											Bot::State.Heal.SafeHealRequired = true; break;
										}
									}
								}
							}
							
							// See if we need to change weapons to heal.
							switch (BotUser::Inventory.Item.Type(6)) {

								case ItemType::STAFF:
								case ItemType::FIST: {
									Bot::State.Heal.FlagReq(false);
								} break;

								default: {
									// Check to see if the user is wielding a shield.
									if (BotUser::Inventory.Item.IsValid(7)) {
										Bot::Items.ShieldIndex = BotUser::Inventory.NearestEmpty(NULL);
									}

									ITEM_DATA* Data;
									if (BotUser::Inventory.GetByType(ItemType::STAFF, Data, false)) {
										Bot::Items.WeaponIndex = Data->Index;
										Bot::State.Heal.FlagReq(true);
									}
									else if (BotUser::Inventory.SpaceAvailable()) {
										// Set the inventory index of the BotUsers current weapon so we know from where to equip it later. (Exclude shield index position)
										Bot::Items.InvEmptyIndex = BotUser::Inventory.NearestEmpty(Bot::Items.ShieldIndex == -1 ? NULL : Bot::Items.ShieldIndex);
										Bot::State.Heal.FlagReq(true);
									}
									else {
										Bot::State.Warp.FlagReq();
										throw "However a staff could not be located.\nAnd there is no space in invent to remove the current weapon.";
									}
								} break;
							}
						} catch (LPCSTR msg) {
							Bot::Print(Debug::GENERAL, msg);
						}
					}
				}
			}
		}

		// HP potting.
		if (!Bot::State.HealthLow) {

			int HpThreshold = Bot::Configuration.HpThreshold;

			// Change the HpThreshold if we're path running through dangerous areas.
			if (Bot::State.IsPathRunning) {
				if (Bot::Zone.Current() == Map::VOD) {
					if (Bot::Configuration.TargetList.Contains("WRAITH") OR Bot::Configuration.TargetList.Contains("LICH LORD") OR Bot::Configuration.TargetList.Contains("PRISMATIC SHADOW") OR Bot::Configuration.TargetList.Contains("NIGHTMARE")) {
						if (BotUser::Me->Data->Stats.Strength <= 800) {
							HpThreshold += 30;
							if (HpThreshold > 99) {
								HpThreshold = 90;
							}
						}
					}
				}
			}

			if (NewHp < (BotUser::Me->Data->Health.Maximum * HpThreshold / 100)) {
				Bot::Print(Debug::GENERAL, "Player HP detected as below [%d%%]", HpThreshold);

				ITEM_DATA* Data;
				if (BotUser::Inventory.GetByType(ItemType::POTION, Data, true)) {
					Bot::Items.HpPotionIndex = Data->Index;
					Bot::Print(Debug::GENERAL, "Found [%d] [%s] at index [%d]", BotUser::Inventory.Item.Count(Data->Index), BotUser::Inventory.Item.Name(Data->Index), Bot::Items.HpPotionIndex);
					Bot::State.HealthLow = true;
				}
				else {
					Bot::Print(Debug::GENERAL, "Could not find HP potions.");
					Bot::State.Warp.FlagReq();
				}
			}
		}
	}

	// MP potting.
	if (NewMp) {
		if (!Bot::State.ManaLow) {
			if (NewMp < (BotUser::Me->Data->Mana.Maximum * Bot::Configuration.MpThreshold / 100)) {
				Bot::Print(Debug::GENERAL, "Player MP detected as below [%d%%]", Bot::Configuration.MpThreshold);

				ITEM_DATA* Data;
				if (BotUser::Inventory.GetByType(ItemType::POTION, Data, false)) {
					Bot::Items.MpPotionIndex = Data->Index;
					Bot::Print(Debug::GENERAL, "Found [%d] [%s] at index [%d]", BotUser::Inventory.Item.Count(Data->Index), BotUser::Inventory.Item.Name(Data->Index), Bot::Items.MpPotionIndex);
					Bot::State.ManaLow = true;
				}
				else {
					Bot::Print(Debug::GENERAL, "Could not find MP potions.");
					if (Bot::State.IsTrainingInt) {
						Bot::State.Warp.FlagReq();
					}
				}
			}
		}
	}
}