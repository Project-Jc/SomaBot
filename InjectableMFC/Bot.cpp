#include "stdafx.h"

CraftBot::CraftBotState CraftBot::State;
CraftBot::ConfigurationData CraftBot::Configuration;
CraftBot::TableData CraftBot::Table;
CraftBot::CraftBotSet CraftBot::Set;
CraftBot::CraftBotGet CraftBot::Get;
CString CraftBot::CProfession;
CString CraftBot::CRecipe;

RestockBot::RestockBotState RestockBot::State;
RestockBot::RestockBotSet RestockBot::Set;
RestockBot::RestockBotGet RestockBot::Get;
RestockBot::RestockVendor RestockBot::Vendor;
vector<CString> RestockBot::MaterialList;
short RestockBot::InvItemIndex;

Bot::BotState Bot::State;
Bot::WaypointData Bot::Waypoints;
Bot::ConfigurationData Bot::Configuration;
Bot::ItemLocations Bot::Items;
Bot::LootData Bot::Loot;
Bot::ZoneData Bot::Zone;
Bot::NearbyUserDetectionData Bot::NearbyUserDetection;
Bot::StatTrackerData Bot::StatTracker;
Bot::OutPutData Bot::OutPut;

CString Bot::Version = "v3.3";

CString Bot::ExePath;

void Bot::OutPutData::SoundPlay(BYTE Type) {
	PlaySound(MAKEINTRESOURCE(Type), GetModuleHandle(szDLLName), SND_RESOURCE | SND_ASYNC);
}

bool Bot::Wait(int Time) {
	if (Configuration.WaiTtick == 0) {
		Print(Debug::WAIT, "Waiting for %dms", Time);
	} else if (Configuration.WaiTtick >= Time) {
		//printf_s("  %d\n", Configuration.WaiTtick);
		Configuration.WaiTtick = 0;
		return true;
	} //printf_s("  %d", Configuration.WaiTtick);
	Configuration.WaiTtick += 10;
	return false;
}

bool PrintProceed(CString Text) {
	if (Text.Find("Attacking") != -1 OR Text.Find("Healing") != -1) {
		if (Bot::OutPut.ShowIsAttacking)
			Bot::OutPut.ShowIsAttacking = false;
		else return false;
	}
	else if (Text.Find("Found target") != -1) {
		Bot::OutPut.TextColourChange();
		printf_s("\n\n");
	}
	else if (Text.Find("PacketType") != -1) {
		if (Text.Find("Send") != -1) {
			Bot::OutPut.TextColourChange(10);
		} else {
			Bot::OutPut.TextColourChange(12);
		}	
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Bot::OutPut.TextColour);
	return true;
}

void Bot::Print(Debug type, LPCSTR format, ...) {
#ifdef DBG
	switch (type) {
	case Debug::GENERAL:
	case Debug::LOOT:
	//case Debug::USERLIST:
	case Debug::HEAL:
	case Debug::CRAFTING:
	case Debug::MOVEMENT:
	case Debug::CHAT:
	case Debug::TARGET:
	case Debug::WAIT:
	case Debug::NPC:
	//case Debug::STAMINA:
	//case Debug::PACKETPROCESS:
	//case Debug::SOCKETMNG:
	case Debug::GMDETECTION:
	case Debug::OUTPUT:
	//case Debug::POINTGENERATION:
	case Debug::INPUT:
	case Debug::USER:
		break;
	default:
		return;
	}

	char buffer[1024];

	va_list argPtr;

	va_start(argPtr, format);
	wvsprintf(buffer, format, argPtr);
	va_end(argPtr);

	if (PrintProceed(CString(buffer))) {
		printf_s("[%d:%d:%d] %s\r\n", CTime::GetTickCount().GetHour(), CTime::GetTickCount().GetMinute(), CTime::GetTickCount().GetSecond(), buffer);
	}
#endif
}


