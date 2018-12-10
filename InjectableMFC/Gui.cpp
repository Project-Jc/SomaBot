#include "stdafx.h"
#include "DDrawHook.h"
#include "Colours.h"

BOOL Gui::IsShowing = false;

CString Gui::Version = "v1.0";

unordered_map<Purpose, RECT> Gui::Controls;

RECT Gui::Position = { 100, 50, 475, 480 };
RECT Gui::Extras = { (Position.left + 20), (Position.bottom - 102), 217, 132 };
RECT Gui::Config = { (Position.left + Extras.right + 43), (Position.top + 40), 190, 420 };
RECT Gui::Program = { (Position.left + 20), (Position.top + 40), 217, 272 };

int yItr, xItr, vItr;

void Gui::Draw(HDC hDC) {

	// Draw the top information display box.
	//DDrawHook::MyDrawBitMap(hDC, IDB_BITMAP2, 200, 200);

	return;

	//// Draw the main UI.
	//Border border(24, 4, 4, 4);
	//DDrawHook::MyDrawFillRectWithBorder(hDC, BLACK, LIGHT_GREY, Position.left, Position.top, Position.right, Position.bottom, Border(1, 1, 1, 1));
	//DDrawHook::MyDrawFillRectWithBorder(hDC, BLACK, LIGHT_BLUE, (Position.left + border.Left), (Position.top + border.Top), (Position.right - (border.Left + border.Right)), (Position.bottom - (border.Top + border.Bottom)), Border(1, 1, 1, 1));
	//DDrawHook::MyDrawTextWithCustomFont(hDC, BLACK, NULL, CreateFont(22, 12, 0, 0,
	//														FW_DEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
	//														CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Buxton Sketch")), (Position.left + (Position.right / 3)), (Position.top + border.Bottom), "Dabura GUI");
	//
	/** Program **/
		yItr = 25; xItr = 20; vItr = 25;
		DDrawHook::MyDrawGroupBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Program", Program.left, Program.top, Program.right, Program.bottom, Border(3, 3, 3, 3));

		//// The main bot mode.
		//DDrawHook::MyDrawMultipleOptionBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Mode", Purpose::SELECT_MODE, (Position.left + 500), (Position.top + 100), Border(3, 3, 3, 3));

		//// Death limit.
		//DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Death Limit:", Purpose::MODIFY_VALUE_DEATHLIMIT, (Position.left + 400), (Position.top + 350));

		DDrawHook::MyDrawButtonWithBorder(hDC, LIGHT_GREY, LIGHT_BLUE, "Test Sound", Purpose::TEST_SOUND, (Program.left + 50), (Program.top + 50), Border(3, 3, 3, 3));
		DDrawHook::MyDrawButtonWithBorder(hDC, LIGHT_GREY, LIGHT_BLUE, "Repair All", Purpose::REPAIR_ALL, (Program.left + 50), (Program.top + 100), Border(3, 3, 3, 3));
	/** End program **/


	/** Extras **/
		yItr = 25; xItr = 20; vItr = 25;
		DDrawHook::MyDrawGroupBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Extras", Extras.left, Extras.top, Extras.right, Extras.bottom, Border(3, 3, 3, 3));

		DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Auto Serum", Purpose::TOGGLE_AUTOSERUM, (Extras.left + xItr), (Extras.top + yItr)); yItr += vItr;
		DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Overhead Damage", Purpose::TOGGLE_DAMAGEOVERHEAD, (Extras.left + xItr), (Extras.top + yItr)); yItr += vItr;
		DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Draw Info", Purpose::TOGGLE_DRAWINFO, (Extras.left + xItr), (Extras.top + yItr)); yItr += vItr;

		// Cast time reduction.
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Cast Time Reduction:", Purpose::MODIFY_VALUE_CASTREDUCTION, (Extras.left + xItr + 142), ((Extras.top + yItr) - 4));
	/** End Extras **/


	/** Configuration **/
		yItr = 30; xItr = 35; vItr = 30;
		DDrawHook::MyDrawGroupBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Configuration", Config.left, Config.top, Config.right, Config.bottom, Border(3, 3, 3, 3));

		CString text = Bot::Configuration.Cfg; text.Delete(Bot::Configuration.Cfg.GetLength() - 4, 4);
		DDrawHook::MyDrawTextWithFont(hDC, BLACK, TA_CENTER, (Config.left + (Config.right / 2)), (Config.top + 25), text);
		NewControl(Purpose::CFG_SCROLL, RECT{ (Config.left + (Config.right / 2) - 75), (Config.top + 20), (Config.left + (Config.right / 2) + 75), (Config.top + 45) }); yItr += 25;

		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Health:", Purpose::MODIFY_PERCENT_HP, (Config.left + (Config.right - 120)), (Config.top + yItr));
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Mana:", Purpose::MODIFY_PERCENT_MP, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;

		DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Heal", Purpose::TOGGLE_HEALENABLE, (Config.left + (Config.right - 165)), (Config.top + (yItr + 5)));
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Percent: ", Purpose::MODIFY_PERCENT_HEAL, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;

		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Dura:", Purpose::MODIFY_PERCENT_DURA, (Config.left + (Config.right - 120)), (Config.top + yItr));
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Aura:", Purpose::MODIFY_VALUE_AURAREUSE, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;

		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Search Range:", Purpose::MODIFY_VALUE_SEARCHRANGE, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Approach TimeOut:", Purpose::MODIFY_VALUE_APPROACHTIMEOUT, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Attack TimeOut:", Purpose::MODIFY_VALUE_ATTACKTIMEOUT, (Config.left + (Config.right - xItr)), (Config.top + yItr)); yItr += vItr;
		DDrawHook::MyDrawModifiableValueBox(hDC, LIGHT_GREY, "Loot TimeOut:", Purpose::MODIFY_VALUE_LOOTTIMEOUT, (Config.left + (Config.right - xItr)), (Config.top + yItr));

		DDrawHook::MyDrawMultipleOptionBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Aura", Purpose::SELECT_AURA, (Config.left + 15), (Config.top + yItr + 45), Border(3, 3, 3, 3));  yItr += 100;
		DDrawHook::MyDrawMultipleOptionBox(hDC, LIGHT_GREY, LIGHT_BLUE, "Behaviour", Purpose::SELECT_BEHAVIOUR, (Config.left + (Config.right / 4)), (Config.top + yItr), Border(3, 3, 3, 3)); yItr += 40;

		DDrawHook::MyDrawButtonWithBorder(hDC, LIGHT_GREY, LIGHT_BLUE, "New", Purpose::CFG_NEW, (Config.left + 15), (Config.top + yItr), Border(3, 3, 3, 3));
		DDrawHook::MyDrawButtonWithBorder(hDC, LIGHT_GREY, LIGHT_BLUE, "Load", Purpose::CFG_LOAD, (Config.left + 68), (Config.top + yItr), Border(3, 3, 3, 3));
		DDrawHook::MyDrawButtonWithBorder(hDC, LIGHT_GREY, LIGHT_BLUE, "Save", Purpose::CFG_SAVE, (Config.left + 128), (Config.top + yItr), Border(3, 3, 3, 3));
	/** End Configuration **/

	/** Checkboxes **/
		//// Chat notify.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Chat Notify", Purpose::TOGGLE_CHATNOTIFY, (Position.left + 50), (Position.top + 125));
		//// Chat respond.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Chat Respond", Purpose::TOGGLE_CHATRESPOND, (Position.left + 50), (Position.top + 150));
		//// Gm live notify.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "GM Live Notify", Purpose::TOGGLE_GMLIVENOTIFY, (Position.left + 50), (Position.top + 175));
		//// Randomization.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Randomize", Purpose::TOGGLE_RANDOMIZATION, (Position.left + 50), (Position.top + 250));
		//// Nearby notify.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Nearby Notify", Purpose::TOGGLE_NEARBYNOTIFY, (Position.left + 50), (Position.top + 275));	
		//// Draw info.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "No Loot", Purpose::TOGGLE_NOLOOT, (Position.left + 50), (Position.top + 325));
		//// Draw info.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "No Warp", Purpose::TOGGLE_NOWARP, (Position.left + 50), (Position.top + 350));
		//// Draw info.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Ignore Pk", Purpose::TOGGLE_IGNOREPK, (Position.left + 50), (Position.top + 375));
		//// Draw info.
		//DDrawHook::MyDrawCheckBox(hDC, LIGHT_BLUE, "Regain Stam", Purpose::TOGGLE_REGAINSTAM, (Position.left + 50), (Position.top + 400));
	/** End checkboxes **/
}

void Gui::ProcessInput(POINT Mouse, BOOL Scroll, BOOL Increase) {
	// Iterate through the list of controls.
	for (auto xt = Controls.begin(); xt != Controls.end(); xt++) {
		// Mouse has been clicked on the control.
		if (Mouse.x >= xt->second.left AND Mouse.x <= xt->second.right AND Mouse.y >= xt->second.top AND Mouse.y <= xt->second.bottom) {
			// Process the control.
			switch (xt->first) {
				// Configuration file scroll.
				case Purpose::CFG_SCROLL: {
					if (Scroll) {
						vector<CString> files;

						WIN32_FIND_DATA ffd;
						HANDLE hFind = FindFirstFile(Bot::ExePath + "Configs\\*.*", &ffd);

						do {
							if (ffd.cFileName[0] != '.') {
								files.push_back(CString(ffd.cFileName));
							}
						} while (FindNextFile(hFind, &ffd));

						for (int i = 0; i < files.size(); i++) {
							if (files[i] == Bot::Configuration.Cfg) {
								if (Increase) {
									if (++i >= files.size()) {
										Bot::Configuration.Cfg = files[0]; break;
									} else {
										Bot::Configuration.Cfg = files[i]; break;
									}
								} else {
									if (--i < 0) {
										Bot::Configuration.Cfg = files[(files.size() - 1)]; break;
									} else {
										Bot::Configuration.Cfg = files[i]; break;
									}
								}
							}
						}
					}
				} break;
				// Modifiable boxes.
				case Purpose::MODIFY_PERCENT_HP: {
					Scroll ? Increase ? Bot::Configuration.HpThreshold < 99 ? Bot::Configuration.HpThreshold++ : false : Bot::Configuration.HpThreshold > 1 ? Bot::Configuration.HpThreshold-- : false : false;
				} break;
				case Purpose::MODIFY_PERCENT_MP: {
					Scroll ? Increase ? Bot::Configuration.MpThreshold < 99 ? Bot::Configuration.MpThreshold++ : false : Bot::Configuration.MpThreshold > 1 ? Bot::Configuration.MpThreshold-- : false : false;
				} break;
				case Purpose::MODIFY_PERCENT_HEAL: {
					Scroll ? Increase ? Bot::Configuration.HealThreshold < 99 ? Bot::Configuration.HealThreshold++ : false : Bot::Configuration.HealThreshold > 1 ? Bot::Configuration.HealThreshold-- : false : false;
				} break;
				case Purpose::MODIFY_PERCENT_DURA: {
					Scroll ? Increase ? Bot::Configuration.DuraThreshold < 99 ? Bot::Configuration.DuraThreshold++ : false : Bot::Configuration.DuraThreshold > 1 ? Bot::Configuration.DuraThreshold-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_AURAREUSE: {
					Scroll ? Increase ? Bot::Configuration.AuraReuseTime < 60 ? Bot::Configuration.AuraReuseTime++ : false : Bot::Configuration.AuraReuseTime > 1 ? Bot::Configuration.AuraReuseTime-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_SEARCHRANGE: {
					Scroll ? Increase ? Bot::Configuration.SearchRange < 32 ? Bot::Configuration.SearchRange++ : false : Bot::Configuration.SearchRange > 1 ? Bot::Configuration.SearchRange-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_APPROACHTIMEOUT: {
					Scroll ? Increase ? Bot::Configuration.ApproachTimeOut < 20 ? Bot::Configuration.ApproachTimeOut++ : false : Bot::Configuration.ApproachTimeOut > 1 ? Bot::Configuration.ApproachTimeOut-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_ATTACKTIMEOUT: {
					Scroll ? Increase ? Bot::Configuration.AttackTimeOut < 20 ? Bot::Configuration.AttackTimeOut++ : false : Bot::Configuration.AttackTimeOut > 1 ? Bot::Configuration.AttackTimeOut-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_LOOTTIMEOUT: {
					Scroll ? Increase ? Bot::Configuration.LootTimeOut < 10 ? Bot::Configuration.LootTimeOut++ : false : Bot::Configuration.LootTimeOut > 1 ? Bot::Configuration.LootTimeOut-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_CASTREDUCTION: {
					//Scroll ? Increase ? Bot::Configuration.CastReduction < 999 ? Bot::Configuration.CastReduction++ : false : Bot::Configuration.CastReduction > 1 ? Bot::Configuration.CastReduction-- : false : false;
				} break;
				case Purpose::MODIFY_VALUE_DEATHLIMIT: {
					Scroll ? Increase ? Bot::Configuration.Death.Limit < 5 ? Bot::Configuration.Death.Limit++ : false : Bot::Configuration.Death.Limit > 1 ? Bot::Configuration.Death.Limit-- : false : false;
				} break;

				// Buttons.
				case Purpose::TEST_SOUND: {
//					Bot::OutPut.SoundPlay(BotUser::RandomNumberGenerator(105, 108), NULL);
				} break;
				case Purpose::REPAIR_ALL: {
					// Repair items.
					for (int i = 0; i < INV_COUNT; i++) {
						if (BotUser::Inventory.Item.IsRepairable(i)) {
							SocketMng::SendRepair(i);
						}
					}
				} break;
				case Purpose::CFG_NEW: {
					Bot::Configuration.Create("NewDefault.cfg");
				} break;
				case Purpose::CFG_LOAD: {					
					Bot::Configuration.Load(Bot::Configuration.Cfg);		
				} break;
				case Purpose::CFG_SAVE: {
					Bot::Configuration.Save(Bot::Configuration.Cfg);
				} break;

				// Multiple option boxes.
				case Purpose::SELECT_MODE: {
					((Mode)((byte)Bot::Configuration.mode + 1) > Mode::CRAFT) ? Bot::Configuration.mode = Mode::LEVEL : Bot::Configuration.mode = (Mode)((byte)Bot::Configuration.mode + 1);
				} break;
				case Purpose::SELECT_AURA: {
					for (auto xt = BotUser::SpecialAttackList.Begin();; xt++) {
						if (xt->Name == Bot::Configuration.Aura) {
							if (++xt == BotUser::SpecialAttackList.End()) {
								Bot::Configuration.Aura = BotUser::SpecialAttackList.Begin()->Name; break;
							} else {
								Bot::Configuration.Aura = (xt)->Name; break;
							}
						}
					}
				} break;
				case Purpose::SELECT_BEHAVIOUR: {
					((Behaviour)((byte)Bot::Configuration.behaviour + 1) > Behaviour::PENSIONER) ? Bot::Configuration.ModifyBehaviour(Behaviour::INSANE) : Bot::Configuration.ModifyBehaviour((Behaviour)((byte)Bot::Configuration.behaviour + 1));
					Bot::Print(Debug::GENERAL, "New behaviour: %s [%d]", Bot::Configuration.BehaviourAsCString(), Bot::Configuration.behaviour);
				} break;

				// Checkboxes.
				case Purpose::TOGGLE_CHATNOTIFY: {
					Bot::Configuration.ChatNotify = !Bot::Configuration.ChatNotify;
				} break;
				case Purpose::TOGGLE_CHATRESPOND: {
					Bot::Configuration.ChatRespond = !Bot::Configuration.ChatRespond;
				} break;
				case Purpose::TOGGLE_GMLIVENOTIFY: {
					//Bot::Configuration.GmLiveNotify = !Bot::Configuration.GmLiveNotify;
				} break;
				case Purpose::TOGGLE_AUTOSERUM: {
					Bot::Configuration.AutoSerum = !Bot::Configuration.AutoSerum;
				} break;
				case Purpose::TOGGLE_RANDOMIZATION: {
					Bot::Configuration.Randomization = !Bot::Configuration.Randomization;
				} break;
				case Purpose::TOGGLE_NEARBYNOTIFY: {
					//Bot::Configuration.NearbyNotify = !Bot::Configuration.NearbyNotify;
				} break;
				case Purpose::TOGGLE_DAMAGEOVERHEAD: {
					Bot::Configuration.DamageOverHead = !Bot::Configuration.DamageOverHead;
				} break;
				case Purpose::TOGGLE_DRAWINFO: {
					Bot::Configuration.DrawInfo = !Bot::Configuration.DrawInfo;
				} break;
				case Purpose::TOGGLE_NOLOOT: {
					Bot::Configuration.NoLoot = !Bot::Configuration.NoLoot;
				} break;
				case Purpose::TOGGLE_NOWARP: {
					Bot::Configuration.NoWarp = !Bot::Configuration.NoWarp;
				} break;
				case Purpose::TOGGLE_IGNOREPK: {
					Bot::Configuration.IgnorePk = !Bot::Configuration.IgnorePk;
				} break;
				case Purpose::TOGGLE_REGAINSTAM: {
					Bot::Configuration.RegainStam = !Bot::Configuration.RegainStam;
				} break;
				case Purpose::TOGGLE_HEALENABLE: {
					Bot::State.Heal.Enabled = !Bot::State.Heal.Enabled;
				} break;
			} return;
		}
	}
}
