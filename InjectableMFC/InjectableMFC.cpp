#include "stdafx.h"

#include "DDrawHook.h"
#include "PacketParserHook.h"
#include "SocketMngHook.h"
#include "UserManagerHook.h"

#include "InjectableMFC.h"

#include <conio.h>

#ifdef _Debug
#define new Debug_NEW
#endif

BEGIN_MESSAGE_MAP(CInjectableMFCApp, CWinApp)
	//ON_MESSAGE(WM_KEYDOWN, OnKeyDown) // Add your own message map entry
END_MESSAGE_MAP()

CInjectableMFCApp::CInjectableMFCApp() {
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CInjectableMFCApp theApp;


void smearId(DWORD* id)
{
	int mask[] = { 0x4E, 0x9E, 0x54};

	for (int i = 0; i < 3; i++)
		id[i] += mask[0] * mask[1] - mask[2];

	CString name = "THERMALTAKE";

	id[0] += 0x0F5E ^ 0xDE * (BYTE)name[0] ^ 0x08 % 0x3E8 * (BYTE)name[name.GetLength() - 3];
	id[1] += 0x0005 ^ 0xC0 * (BYTE)name[1] ^ 0x05 % 0x2E7 * (BYTE)name[name.GetLength() - 2];
	id[2] += 0x4BCD ^ 0XA5 * (BYTE)name[2] ^ 0x03 % 0x1E6 * (BYTE)name[name.GetLength() - 1];
}

DWORD* generateId()
{
	DWORD id[3];

	// Get the CPU ID.
	int cpuinfo[4] = { 0, 0, 0, 0 };
	__cpuid(cpuinfo, 1);
	id[0] = cpuinfo[0];

	// Get the HDD ID.
	DWORD serialNum = 0;  
	GetVolumeInformation("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
	id[1] = (DWORD)((serialNum + (serialNum >> 16)) & 0xFFFF);

	id[2] = 0;
	for (int i = 0; i < 2; i++)
		id[2] += id[i];
	id[2] += 0xBF4A ^ 0x41 ^ 0xFF;

	return id;
}


void encryptUser(string users[], int size)
{
	vector<string> encUsers;

	for (int i = 0; i < size; i++) {

		string New;

		for (int x = 0, hex = 0x3; x < users[i].size(); x++, hex++) {

			char aChar = users[i][x] + hex;

			if (aChar > 0x7E) {
				aChar = 0x4F;
			}

			New += aChar;
		}

		New += '\0';

		printf_s("Old [%s] New [%s]\n", users[i].c_str(), New.c_str());

		encUsers.push_back(New);
	}

	for (auto xt = encUsers.begin(); xt != encUsers.end(); xt++) {
		printf_s("\"%s\", ", xt->c_str());
	}
} 


void loadIni(void) {

	CString CfgFile = "Dabura.ini";

	if (!PathFileExists(CfgFile)) {

		printf_s("Could not locate: Dabura.ini. Will create a new file instead.\n\n");

		ofstream NewIni(CfgFile);
		NewIni
			// Main bot Behaviour.
			<< "## The main bot Mode. Modes are: LEVEL, HEAL, RESTOCK and CRAFT." << endl
			<< "MODE=LEVEL" << endl << endl

			// Useful macros.
			<< "## Enable or disable auto serum usage." << endl
			<< "AUTO_SERUM=0" << endl << endl

			// Client modifications.
			<< "## Reduce the casting time of spells." << endl
			<< "CAST_REDUCTION=1" << endl
			<< "## Enable or disable darkness." << endl
			<< "NO_DARKNESS=1" << endl
			<< "## Enable or disable damage display above other players." << endl
			<< "DAMAGE_OVERHEAD=1" << endl
			<< "## Enable tracking of the values for the current target." << endl
			<< "TARGET_TRACKING=1" << endl
			<< "## Display information about the program on the soma game window." << endl
			<< "DRAW_INFO=0" << endl << endl

			// Chat notifications.
			<< "## Sound notifications for nearby chat and incoming private messages (Only if the bot is running)." << endl
			<< "CHAT_NOTIFY=1" << endl
			<< "## Enable or disable the responding to nearby chat or PM." << endl
			<< "CHAT_RESPOND=0" << endl << endl

			// GM notifications.
			<< "## Sound a notification, disconnect or do nothing when a GM becomes live." << endl
			<< "## Options are: DISCONNECT and NOTIFY. Leave blank to not act." << endl
			<< "GM_LIVE_RESPONSE=NOTIFY" << endl
			<< "## The list of GM names to search for." << endl
			<< "GM_LIST=ISYLVER,DISYLVER,GHOSTLORD,FINITO,DFINITO,MACHEONRU" << endl << endl

			// Nearby user notifications.
			<< "## Sound a notification, disconnect or do nothing when another player is detected as being nearby." << endl
			<< "## Options are: DISCONNECT and NOTIFY. Leave blank to not act." << endl
			<< "NEARBYUSER_RESPONSE=NOTIFY" << endl
			<< "## The amount of times the sound notification will play for any given player." << endl
			<< "NOTIFY_LIMIT=1" << endl
			<< "## The list of usernames that are to be ignored when one of those users are detected as being nearby." << endl
			<< "## To exclude entire guilds of users, input the guild name." << endl
			<< "EXCLUSION_LIST=" << endl << endl

			// Looting options.
			<< "## If LOOT_AFTERWARDS is true, the bot will loot items only when it has finished killing a group of mobs." << endl
			<< "LOOT_AFTERWARDS=0" << endl << endl

			// The bot behaviour.
			<< "## Behaviour controls how the bot behaves." << endl
			<< "## Current Behaviours are: INSANE, AGGRESSIVE, CASUAL and PENSIONER." << endl
			<< "BEHAVIOUR=AGGRESSIVE" << endl << endl

			// Stat tracker.
			<< "## Enable the tracking of time and percentages between stat gains (STR, DEX, INT & CON)." << endl
			<< "STAT_TRACKER=0" << endl << endl

			// General bot options.
			<< "## Enable or disable looting." << endl
			<< "NO_LOOT=0" << endl
			<< "## Enable or disable warping." << endl
			<< "NO_WARP=0" << endl
			<< "## Do or do not act if the player is being attacked by another player." << endl
			<< "IGNORE_PK=0" << endl
			<< "## If enabled, the bot will stand and regain stamina instead of walking." << endl
			<< "REGAIN_STAMINA=1" << endl
			<< "## The number of times the bot will permit dying before terminating." << endl
			<< "DEATH_LIMIT=2" << endl
			<< "## Enable or disable more random bot behaviour." << endl
			<< "RANDOMIZATION=0" << endl << endl

			// Previously used configuration file.
			<< "## The previous configuration file that was used (This will get loaded every time the program is run)." << endl
			<< "## If the creation of a new default configuration file is desired, input the new name of the file and run Dabura." << endl
			<< "PREVIOUS_CFG=Default.cfg" << endl << endl;
	}

	printf_s("Loading program configuration from: Dabura.ini\n\n");

	char Line[1024], Value[1024];

	ifstream Ini(CfgFile);

	while (Ini.getline(Line, sizeof(Line)))	{

		if (Line[0] != '#') {

			int Seperator = CString(Line).Find('=');

			copy((Line + (Seperator + 1)), (Line + (Seperator + 1)) + CString(Line).GetLength(), Value);

			// Main bot Behaviour.
			if (CString(Line).Find("MODE") != -1) {
				if (CString(Value) == "LEVEL")
					Bot::Configuration.mode = Mode::LEVEL;
				else if (CString(Value) == "RESTOCK")
					Bot::Configuration.mode = Mode::RESTOCK;
				else if (CString(Value) == "CRAFT")
					Bot::Configuration.mode = Mode::CRAFT;
				else if (CString(Value) == "HEAL")
					Bot::Configuration.mode = Mode::HEAL;
				printf_s("\tMode: %s\n\n", Value);
			}

			// Useful macros.
			else if (CString(Line).Find("AUTO_SERUM") != -1) {
				Bot::Configuration.AutoSerum = atoi(Value); printf_s("\tAutoSerum: %s\n\n", Bot::Configuration.AutoSerum ? "TRUE" : "FALSE");
			}

			// Client modifications.
			else if (CString(Line).Find("CAST_REDUCTION") != -1) {
				Bot::Configuration.CastReduction = atoi(Value); printf_s("\tCastReduction: %s\n", Bot::Configuration.CastReduction ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("NO_DARKNESS") != -1) {
				Bot::Configuration.NoDark = atoi(Value); printf_s("\tNoDark: %s\n", Bot::Configuration.NoDark ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("DAMAGE_OVERHEAD") != -1) {
				Bot::Configuration.DamageOverHead = atoi(Value); printf_s("\tDamageOverHead: %s\n", Bot::Configuration.DamageOverHead ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("TARGET_TRACKING") != -1) {
				Bot::Configuration.TrackTarget = atoi(Value); printf_s("\tTargetTracking: %s\n", Bot::Configuration.TrackTarget ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("DRAW_INFO") != -1) {
				Bot::Configuration.DrawInfo = atoi(Value); 	printf_s("\tDrawInfo: %s\n\n", Bot::Configuration.DrawInfo ? "TRUE" : "FALSE");
			}

			// Chat notifications.
			else if (CString(Line).Find("CHAT_NOTIFY") != -1) {
				Bot::Configuration.ChatNotify = atoi(Value); printf_s("\tChatNotify: %s\n", Bot::Configuration.ChatNotify ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("CHAT_RESPOND") != -1) {
				Bot::Configuration.ChatRespond = atoi(Value); printf_s("\tChatRespond: %s\n\n", Bot::Configuration.ChatRespond ? "TRUE" : "FALSE");
			}

			// GM notifications.
			else if (CString(Line).Find("GM_LIVE_RESPONSE") != -1) {
				if (CString(Value) == "DISCONNECT")
					Bot::Configuration.GmDetection.LiveResponse = Response::DISCONNECT;
				else if (CString(Value) == "NOTIFY")
					Bot::Configuration.GmDetection.LiveResponse = Response::NOTIFY;
				Bot::Configuration.GmDetection.IsEnabled = (BYTE)Bot::Configuration.GmDetection.LiveResponse;	
				printf_s("\tGmDetectionEnabled: %s\n\tGmLiveResponse: %s\n\n", Bot::Configuration.GmDetection.IsEnabled ? "TRUE" : "FALSE", Value);			
			}
			else if (CString(Line).Find("GM_LIST") != -1) {
				if (Bot::Configuration.GmDetection.IsEnabled) {
					Bot::Configuration.GmDetection.ParseList(Value);
				}
			}

			// Nearby user notifications.
			else if (CString(Line).Find("NEARBYUSER_RESPONSE") != -1) {
				if (CString(Value) == "DISCONNECT")
					Bot::NearbyUserDetection.DetectedUsrResponse = Response::DISCONNECT;
				else if (CString(Value) == "NOTIFY")
					Bot::NearbyUserDetection.DetectedUsrResponse = Response::NOTIFY;
				Bot::NearbyUserDetection.IsEnabled = (BYTE)Bot::NearbyUserDetection.DetectedUsrResponse;
				printf_s("\tNearbyUserDetectionEnabled: %s\n\tNearbyUserResponse: %s\n", Bot::NearbyUserDetection.IsEnabled ? "TRUE" : "FALSE", Value);		
			}
			else if (CString(Line).Find("NOTIFY_LIMIT") != -1) {
				Bot::NearbyUserDetection.NotificationLimit = atoi(Value); printf_s("\tNearbyNotifyLimit: %d\n\n", Bot::NearbyUserDetection.NotificationLimit); Bot::NearbyUserDetection.NotificationLimit -= 1;
			}
			else if (CString(Line).Find("EXCLUSION_LIST") != -1) {
				Bot::NearbyUserDetection.ExclusionList.ParseList(Value);
			}

			// Loot options.
			else if (CString(Line).Find("LOOT_AFTERWARDS") != -1) {
				Bot::Configuration.LootAfterwards = atoi(Value); printf_s("\tLootAfterwards: %s\n\n", Bot::Configuration.LootAfterwards ? "TRUE" : "FALSE");
			}

			// Bot behaviour.
			else if (CString(Line).Find("BEHAVIOUR") != -1) {
				if (CString(Value) == "INSANE")
					Bot::Configuration.ModifyBehaviour(Behaviour::INSANE);
				else if (CString(Value) == "AGGRESSIVE")
					Bot::Configuration.ModifyBehaviour(Behaviour::AGGRESSIVE);
				else if (CString(Value) == "CASUAL")
					Bot::Configuration.ModifyBehaviour(Behaviour::CASUAL);
				else if (CString(Value) == "PENSIONER")
					Bot::Configuration.ModifyBehaviour(Behaviour::PENSIONER);
				printf_s("\tBehaviour: %s [%d]\n\n", Bot::Configuration.BehaviourAsCString(), Bot::Configuration.behaviour);
			}

			else if (CString(Line).Find("STAT_TRACKER") != -1) {
				Bot::StatTracker.IsEnabled = atoi(Value); printf_s("\tTrackingStats: %s\n\n", Bot::StatTracker.IsEnabled ? "TRUE" : "FALSE");
			}

			// General bot options.	
			else if (CString(Line).Find("NO_LOOT") != -1) {
				Bot::Configuration.NoLoot = atoi(Value); printf_s("\tNoLoot: %s\n", Bot::Configuration.NoLoot ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("NO_WARP") != -1) {
				Bot::Configuration.NoWarp = atoi(Value); printf_s("\tNoWarp: %s\n", Bot::Configuration.NoWarp ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("IGNORE_PK") != -1) {
				Bot::Configuration.IgnorePk = atoi(Value); printf_s("\tIgnorePk: %s\n", Bot::Configuration.IgnorePk ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("REGAIN_STAMINA") != -1) {
				Bot::Configuration.RegainStam = atoi(Value); printf_s("\tRegainStam: %s\n", Bot::Configuration.RegainStam ? "TRUE" : "FALSE");
			}
			else if (CString(Line).Find("DEATH_LIMIT") != -1) {
				Bot::Configuration.Death.Limit = atoi(Value); printf_s("\tDeathLimit: %d\n", Bot::Configuration.Death.Limit);
			}
			else if (CString(Line).Find("RANDOMIZATION") != -1) {
				Bot::Configuration.Randomization = atoi(Value); printf_s("\tRandomization: %s\n\n", Bot::Configuration.Randomization ? "TRUE" : "FALSE");
			}

			// Previous configuration file.
			else if (CString(Line).Find("PREVIOUS_CFG") != -1) {
				Bot::Configuration.Load(Value);
			}
		}
	}	

	// Warnings.
	if (!Bot::Configuration.GmDetection.IsEnabled) {
		printf_s("\tWARNING: GM detection is not enabled.\n");
	}
	if (!Bot::NearbyUserDetection.IsEnabled) {
		printf_s("\tWARNING: Nearby user detection is not enabled.\n");
	} printf_s("\n\n");
}

void loadSubs(void) {

	// Me
	vector<string> Users = { "PGFX[I", "EEGOKQ", "GSWO[W", "FEHZ\[SKNW", "D]S" };
	BotUser::Subscribers.Add(Subscriber(Users, 0x37C21, 0x11B83, 0x51243));
}


BOOL CInjectableMFCApp::InitInstance() {

	CWinApp::InitInstance();

	// Load the list of subscribers.
	loadSubs();

#ifdef DBG
	// Allocate a console for Debugging.
	if (AllocConsole()) {
		FILE* file;
		freopen_s(&file, "conin$", "r+t", stdin);
		freopen_s(&file, "conout$", "w+t", stdout);
		freopen_s(&file, "conout$", "w+t", stderr);
	}
#endif

	// Set the ExePath so we can load files from the Dabura directory later on. Once we inject the programs current directory changes to the Myth of Soma directory.
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	Bot::ExePath = (CString(NPath) + "\\");

	// Code for generating encrypted character names. 
	BOOL GenEncUsers = false;
	//if (GenEncUsers) {
	//	string Users[] = { "CALIGO", "LIISA" };
	//	encryptUser(Users, 2);
	//}

	// Get the system IDs.
	DWORD* Id = generateId();

	//Id[0] = 198313; Id[1] = 25431; Id[2] = 272884;

	// Show some system IDs.
	//BOOL ShowSystemId = false; 
	//if (ShowSystemId) {
	//	printf_s("\n\t%d\n\t%d\n\t%d\n", Id[0], Id[1], Id[2]);
	//} else {
	//	// Smear the ID.
	//	smearId(Id);
	//	BOOL ShowSmeared = false;
	//	if (ShowSmeared) {
	//		printf_s("\n\t0x%X\n\t0x%X\n\t0x%X\n\t", Id[0], Id[1], Id[2]); _getch();
	//	}
	//}

	//if (GenEncUsers OR ShowSystemId) {
	//	_getch();
	//}

	// Only continue if the combination of ID's is correct.
	//if (BotUser::Subscribers.VerifyId(Id)) 
	{

		// Patch MsgFlt.dll
		HMODULE hMod_MsgFlt = NULL;
		hMod_MsgFlt = GetModuleHandle("MsgFlt.dll");

		if (hMod_MsgFlt) {

			DWORD offSet = 0x1BB1D;

			// Allow writing to the address.
			DWORD oldProtect = 0;
			if (VirtualProtect((LPVOID)((DWORD)hMod_MsgFlt + offSet), 2, PAGE_EXECUTE_READWRITE, &oldProtect)) {

				// NOP the calls.
				*(short *)((DWORD)hMod_MsgFlt + offSet) = 0x9090;

				// Change the permissions back.
				VirtualProtect((LPVOID)((DWORD)hMod_MsgFlt + offSet), 2, PAGE_EXECUTE_READ, &oldProtect);

				// Verify that the patch has been applied.
				if ((BYTE)*(BYTE *)((DWORD)hMod_MsgFlt + offSet) == 0x90) {
					//printf_s("Successfully patched MsgFlt.dll\n\n");
				} else {
					//printf_s("\nCould not patch MsgFlt.dll.\n\n");
					return FALSE;
				}
			}
		} else {
			//printf_s("\nCould not locate MsgFlt.dll.");
			return FALSE;
		}

		// Initialize the DDraw hook.
		if (!DDrawHook::Init()) {
			printf_s("Could not initialize the DDraw hook.");
			return FALSE;
		}

		// Initialize the PacketParser hook.
		if (!PacketParserHook::Init()) {
			printf_s("Could not initialize the PacketParser hook.");
			DDrawHook::Exit();
			return FALSE;
		}

		// Initialize the UserManager hook.
		if (!UserManagerHook::Init()) {
			printf_s("Could not initialize the UserManager hook.");
			DDrawHook::Exit();
			PacketParserHook::Exit();
			return FALSE;
		}

		// Initialize the SocketMng hook.
		if (!SocketMngHook::Init()) {
			printf_s("Could not initialize the SocketMngHook.");
			DDrawHook::Exit();
			PacketParserHook::Exit();
			UserManagerHook::Exit();
			return FALSE;
		}

		// Load the program configuration.
		loadIni();

		// Creat the bot thread.
		::CreateThread(NULL, NULL, BotUser::Process, NULL, NULL, NULL);

		return TRUE;

	} 
//else {
//		//printf_s("\n\nIncorrect ID's.");
//	}

	ExitProcess(NULL);
	return FALSE;
}

BOOL CInjectableMFCApp::ExitInstance() {
	return TRUE;
}
