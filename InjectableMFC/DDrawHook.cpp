#include "stdafx.h"
#include "DDrawHook.h"
#include "Colours.h"

int DDrawHook::SurfaceCount = 0;
IDirectDrawSurface7 FAR *DDrawHook::HookedSurface;

struct DDrawHook::Screen DDrawHook::Screen = { 0, 0 };

CodeCave DDrawHook::CC::DirectDrawCreateEx;
CodeCave DDrawHook::CC::IDD7::CreateSurface;
CodeCave DDrawHook::CC::IDDSurface7::Lock;

DDrawHook::BotUserDmgData DDrawHook::BotUserDmg;

// Initiate the hook.
bool DDrawHook::Init(void) {
	// Find DDraw.dll.
	HMODULE hMod_DDraw = NULL;

	for (; hMod_DDraw == NULL; Sleep(10)) {
		hMod_DDraw = GetModuleHandle("DDraw");
	}

	// Log DDraw.dll.
	//std::cout << "DDraw.dll:\t\t\t" << hMod_DDraw << std::endl;

	// Find DDraw.DirectDrawCreateEx.
	FARPROC fDDraw_DirectDrawCreateEx = GetProcAddress(hMod_DDraw, "DirectDrawCreateEx");
	if (fDDraw_DirectDrawCreateEx == NULL) {
		return false;
	}

	// Hook DDraw.DirectDrawCreateEx.
	CC::DirectDrawCreateEx.Hook(fDDraw_DirectDrawCreateEx, MY::DirectDrawCreateEx);

	return true;
}
void DDrawHook::Exit(void) {
	// Unhook DDraw.DirectDrawCreateEx.
	if (CC::DirectDrawCreateEx.IsHooked()) {
		CC::DirectDrawCreateEx.Unhook();
	}

	// Unhook IDirectDraw7.CreateSurface.
	if (CC::IDD7::CreateSurface.IsHooked()) {
		CC::IDD7::CreateSurface.Unhook();
	}

	// Unhook IDirectDrawSurface.Lock.
	if (CC::IDDSurface7::Lock.IsHooked()) {
		CC::IDDSurface7::Lock.Unhook();
	}
}

// DDraw
HRESULT WINAPI DDrawHook::MY::DirectDrawCreateEx(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown FAR *pUnkOuter) {
	// Call the original function.
	HRESULT result = ((tDDraw_DirectDrawCreateEx) CC::DirectDrawCreateEx.OriginalFunction())(lpGUID, lplpDD, iid, pUnkOuter);

	if (result == DD_OK) {
		VTABLE(lplpDD);

		// Hook IDirectDraw7.CreateSurface.
		if (!CC::IDD7::CreateSurface.IsHooked()) {
			CC::IDD7::CreateSurface.Hook((LPVOID) vTable[VTable_IDirectDraw7::CreateSurface], MY::IDD7::CreateSurface);
		}
	}

	return result;
}

HRESULT WINAPI DDrawHook::MY::IDD7::CreateSurface(LPDIRECTDRAW7 FAR *pDevice, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 FAR *lplpDDSurface, IUnknown FAR *pUnkOuter) {
	// Call the original function.
	HRESULT result = ((tDDraw_CreateSurface) CC::IDD7::CreateSurface.OriginalFunction())(pDevice, lpDDSurfaceDesc2, lplpDDSurface, pUnkOuter);

	if (result == DD_OK) {

		if (++SurfaceCount == 3) {
			// Save the third surface (seems to be the whole game screen size).
			HookedSurface = *lplpDDSurface;

			// Store the screen size for drawing.
			Screen.Width = lpDDSurfaceDesc2->dwWidth;
			Screen.Height = lpDDSurfaceDesc2->dwHeight;
		}

		VTABLE(lplpDDSurface);

		// Hook IDirectDrawSurface7.Lock.
		if (!CC::IDDSurface7::Lock.IsHooked()) {
			CC::IDDSurface7::Lock.Hook((LPVOID) vTable[VTable_IDirectDrawSurface7::Lock], MY::IDDSurface7::Lock);
		}
	}
	return result;
}

int Hc[3] = { 0, 0, 0 };

DWORD PreviousTick;

// IDirectDrawSurface7
HRESULT WINAPI DDrawHook::MY::IDDSurface7::Lock(IDirectDrawSurface7 FAR *pSurface, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent) {
	// See if this is the surface that we want to draw on.
	if (DDrawHook::HookedSurface == pSurface) {
		HDC hDC;

		// Get the device context.
		if (pSurface->GetDC(&hDC) == DD_OK) {

			// Store the old settings and change the settings.
			int oldBkMode = SetBkMode(hDC, TRANSPARENT);


			// Draw damage overhead.
			if (Bot::Configuration.DamageOverHead) {
				// Draw floating damage.
				if (BotUserDmg.FloatingDamageList.size() > 0) {

					for (auto xt = BotUserDmg.FloatingDmgBegin(); xt != BotUserDmg.FloatingDmgEnd(); xt++) {
						if ((GetTickCount() - xt->Time) >= 1500) {
							xt = BotUserDmg.FloatingDmgRemove(xt);
							if (xt == BotUserDmg.FloatingDmgEnd()) {
								break;
							}
						}
						else {

							CPoint tmpPos = CPoint(	xt->DrawForMe ? 	
													(Screen.Width / 2) + 50 : (Screen.Width / 2) - 75, 
													(Screen.Height / 2) - ((GetTickCount() - xt->Time) / 20) - 100);

							switch (xt->Type) {
								case DmgType::ATTACK: {
									MyDrawBotUserDmg(hDC, RGB(0, 0, 0), TA_LEFT, tmpPos.x, tmpPos.y, "-%d", xt->Amount);
									MyDrawBotUserDmg(hDC, RGB(255, 0, 0), TA_LEFT, tmpPos.x + 1, tmpPos.y + 1, "-%d", xt->Amount);
								} break;

								case DmgType::HEAL: {
									MyDrawBotUserDmg(hDC, RGB(0, 0, 0), TA_LEFT, tmpPos.x, tmpPos.y, "+%d", xt->Amount);
									MyDrawBotUserDmg(hDC, RGB(0, 255, 0), TA_LEFT, tmpPos.x + 1, tmpPos.y + 1, "+%d", xt->Amount);
								} break;
							}
						}
					}
				}

				//// Draw static damage done to the bot user.
				//if (BotUserDmg.StaticDamageList.size() > 0) {

				//	int yPlus = 20;
				//	if (BotUserDmg.StaticDamageList.size() > 6) {
				//		BotUserDmg.StaticDmgRemove(BotUserDmg.StaticDamageList.begin());
				//	}

				//	for (auto xt = BotUserDmg.StaticDmgBegin(); xt != BotUserDmg.StaticDmgEnd(); xt++, yPlus += 20) {

				//		CPoint tmpPos = CPoint(xt->DrawForMe ? Screen.Width - 100 : 100, (Screen.Height / 3) + yPlus);

				//		switch (xt->Type) {
				//			case DmgType::ATTACK: {
				//				MyDrawBotUserDmg(hDC, RGB(0, 0, 0), TA_LEFT, tmpPos.x, tmpPos.y, "- %d", xt->Amount);
				//				MyDrawBotUserDmg(hDC, RGB(255, 0, 0), TA_LEFT, tmpPos.x + 1, tmpPos.y + 1, "- %d", xt->Amount);
				//			} break;

				//			case DmgType::HEAL: {
				//				MyDrawBotUserDmg(hDC, RGB(0, 0, 0), TA_LEFT, tmpPos.x, tmpPos.y, "+ %d", xt->Amount);
				//				MyDrawBotUserDmg(hDC, RGB(0, 255, 0), TA_LEFT, tmpPos.x + 1, tmpPos.y + 1, "+ %d", xt->Amount);
				//			} break;
				//		}
				//	}
				//}
			}


			// Target tracking.
			if (Bot::Configuration.TrackTarget) {
				if (BotUser::IsValid(BotUser::Me)) {
					if (BotUser::IsValid(BotUser::Me->Target)) {
						if (!BotUser::Me->Target->Data->Dead) {
							MyDrawTrackTarget(hDC, (Screen.Width / 8), (Screen.Height / 2) + 100, BotUser::Me->Target);
							//MyDrawText(hDC, RGB(255, 255, 255), TA_LEFT, 5, 200, "Current: %d Prev: %d", BotUser::Me->Target->Data->Health.Current, BotUser::Me->Target->PreviousHp);
						}
					}
				}
			}


			// Display miscellaneous information.
			if (Bot::Configuration.DrawInfo) {

				// Print the bot version.
				MyDrawText(hDC, RGB(255, 255, 255), TA_LEFT, 2, 2, "%s", Bot::Version);

				// Show nearby notify users.
				if (Bot::NearbyUserDetection.DetectedUsrResponse == Response::NOTIFY) {
					if (Bot::NearbyUserDetection.DetectedUsers.Count() > 0) {

						// Do not process the NearbyUser display if the user is warping. Warping can mess with the UserList which can cause a crash here.
						if (!Bot::State.Warp.Required AND !Bot::State.Warp.InProgress AND !Bot::State.Warp.Done AND !Bot::State.PotsAndRepairs AND !Bot::State.IsPathRunning) {

							vector<CString> Names;

							for (auto xt = Bot::NearbyUserDetection.DetectedUsers.List.begin(); xt != Bot::NearbyUserDetection.DetectedUsers.List.end(); xt++) {
								if (UserManager::UserList.ContainsName(*xt)) {
									Names.push_back(*xt);
								}
							}

							if (Names.size() > 0) {
								MyDrawText(hDC, LIGHT_GREY, TA_LEFT, 10, Screen.Height / 6, "NearbyUsers:");
								for (int i = 0, yPos = (Screen.Height / 6) + 15; i < Names.size(); i++, yPos += 15) {
									MyDrawText(hDC, LIGHT_GREY, TA_LEFT, 20, yPos, "%s", Names[i]);
								}
							}
						}
					}
				}

				if (BotUser::IsValid(BotUser::Me)) {

					// Draw the top information display box.
					MyDrawBitMap(hDC, IDB_BITMAP3, (Screen.Width / 4), -26);

					// Get the bot state.
					CString State;
					if (!Bot::State.IsRunning) {
						State = "Terminated";
					}
					else if (Bot::State.AwaitingRevival) {
						State = "Dead";
					}
					else if (Bot::State.Warp.Required OR Bot::State.Warp.InProgress OR Bot::State.Warp.Done) {
						State = "Warping";
					}
					else if (Bot::State.IsBeingPkd) {
						State = "Being PK'd";
					}
					else if (Bot::State.HealthLow) {
						State = "Potting HP";
					}
					else if (Bot::State.ManaLow) {
						State = "Potting MP";
					}
					else if (Bot::State.Heal.Required) {
						State = "Healing";
					}
					else if (Bot::State.Heal.WepChangeRequired OR Bot::State.Heal.ReEquipWepRequired) {
						State = "Changing weapon";
					}
					else if (Bot::State.IsRepairing) {
						State = "Repairing";
					}
					else if (Bot::State.IsRestocking) {
						State = "Restocking";
					}
					else if (Bot::State.IsPathRunning) {
						State = "Path running";
					}
					else if (Bot::State.IsLooting) {
						State = "Looting";
					}
					else if (Bot::State.IsRegainingStamina) {
						State = "Chillin'";
					}
					else if (Bot::State.HasTarget) {
						if (Bot::State.Attack.IsAttacking) {
							State = "Attacking";
						}
						else {
							State = "Approaching target";
						}
					}
					else {
						State = "Moving";
					}

					// Place comma's in the barr amount.
					CString Barr;
					Barr.Format("%d", BotUser::Me->Data->Barr);
					if (Barr.GetLength() > 3) {
						for (int i = (Barr.GetLength() - 3); i > 0; i -= 3) {
							Barr.Insert(i, ',');
						}
					}

					// Print the information.
					MyDrawText(hDC, LIGHT_GREY, TA_LEFT, (Screen.Width / 4) + 112, 14,
						"%s   Barr: %s   Status: %s",
						BotUser::Me->Data->Name.GetString(), Barr, State);
				}
			}

			if (Gui::Visible()) {
				if (BotUser::IsValid(BotUser::Me)) {
					Gui::Draw(hDC);
				}
			}

			if (Bot::Configuration.DrawMinimap) {
				if (BotUser::IsValid(BotUser::Me)) {
					DrawMinimap(hDC, (Screen.Width / 4), (Screen.Height / 4), 400);
				}
			}

			// Revert the settings.
			SetBkMode(hDC, oldBkMode);

			// Release the device context.
			pSurface->ReleaseDC(hDC);
		}
	}

	// Call the original function.
	HRESULT result = ((tDDraw_Lock) CC::IDDSurface7::Lock.OriginalFunction())(pSurface, lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);

	return result;
}