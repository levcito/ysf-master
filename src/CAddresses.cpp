/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved. The development was abandobed
*  around 2010, afterwards kurta999 has continued it.
*
*  Contributor(s):
*
*	0x688, balika011, Gamer_Z, iFarbod, karimcambridge, Mellnik, P3ti, Riddick94
*	Slice, sprtik, uint32, Whitetigerswt, Y_Less, ziggi and complete SA-MP community
*
*  Special Thanks to:
*
*	SA:MP Team past, present and future
*	Incognito, maddinat0r, OrMisicL, Zeex
*
*/

#include "CAddresses.h"
#include "Globals.h"
#include "Memory.h"

#undef DECLARE_FUNC_PTR

//#define print_addresses
#ifdef _WIN32
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03Z = 0x00486CB0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR2_2 = 0x00487310;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR3 = 0x00487460;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR4 = 0x004875F0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_037 = 0x0048A0B0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_037_R2_1 = 0x0048C8D0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03DL_R1 = 0x00491FA0;
#else
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03Z = 0x080A7440;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR2_2 = 0x080A77D0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR3 = 0x080A78E0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03ZR4 = 0x80A7A90;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_037 = 0x080A9000;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_037_R2_1 = 0x080A91D0;
ADDR<CAddress::logprintf_t> CAddress::FUNC_Logprintf_03DL_R1 = 0x080B1CA0;
#endif

// Variables
ADDR<float> CAddress::VAR_pRestartWaitTime = nullptr;
ADDR<float> CAddress::VAR_pPosSyncBounds[4] = {};
ADDR<WORD> CAddress::VAR_wRCONUser = nullptr;
ADDR<bool> CAddress::VAR_bRconSocketReply = nullptr;
ADDR<ConsoleCommand_s[]> CAddress::ARRAY_ConsoleCommands = nullptr;

// Functions
DEFINE_FUNC_PTR(CConsole__AddStringVariable);
DEFINE_FUNC_PTR(CConsole__GetStringVariable);
DEFINE_FUNC_PTR(CConsole__SetStringVariable);
DEFINE_FUNC_PTR(CConsole__GetIntVariable);
DEFINE_FUNC_PTR(CConsole__SetIntVariable);
DEFINE_FUNC_PTR(CConsole__GetBoolVariable);
DEFINE_FUNC_PTR(CConsole__ModifyVariableFlags);
DEFINE_FUNC_PTR(CConsole__FindVariable);
DEFINE_FUNC_PTR(CConsole__SendRules);
DEFINE_FUNC_PTR(CConsole__Execute);

DEFINE_FUNC_PTR(CNetGame__SetWeather);
DEFINE_FUNC_PTR(CNetGame__SetGravity);

DEFINE_FUNC_PTR(CFilterscripts__LoadFilterscript);
DEFINE_FUNC_PTR(CFilterscripts__UnLoadFilterscript);
DEFINE_FUNC_PTR(ContainsInvalidChars);

DEFINE_FUNC_PTR(CPlayer__SpawnForWorld);
DEFINE_FUNC_PTR(CVehicle__Respawn);
DEFINE_FUNC_PTR(CPlayerPool__HandleVehicleRespawn);
DEFINE_FUNC_PTR(CObject__SpawnForPlayer);

DEFINE_FUNC_PTR(ProcessQueryPacket);
DEFINE_FUNC_PTR(Packet_WeaponsUpdate);
DEFINE_FUNC_PTR(Packet_StatsUpdate);
DEFINE_FUNC_PTR(format_amxstring);

DEFINE_FUNC_PTR(ReplaceBadChars);
DEFINE_FUNC_PTR(RemoveColorTags);
DEFINE_FUNC_PTR(OnDialogResponse);

// Others
ADDR<BYTE> CAddress::ADDR_GetNetworkStats_VerbosityLevel;
ADDR<BYTE> CAddress::ADDR_GetPlayerNetworkStats_VerbosityLevel;

ADDR<const char*> CAddress::ADDR_RecordingDirectory;

#ifdef _WIN32
ADDR<unsigned char[82]> CAddress::ADDR_WrongPacketIDBranch;
#else
ADDR<unsigned char[114]> CAddress::ADDR_WrongPacketIDBranch;
#endif

// Callback hooks
DEFINE_FUNC_PTR(CGameMode__OnPlayerConnect);
DEFINE_FUNC_PTR(CGameMode__OnPlayerDisconnect);
DEFINE_FUNC_PTR(CGameMode__OnPlayerSpawn);
DEFINE_FUNC_PTR(CGameMode__OnPlayerStreamIn);
DEFINE_FUNC_PTR(CGameMode__OnPlayerStreamOut);

void CAddress::Initialize(SAMPVersion sampVersion)
{
	// Thx for Whitetiger
	DWORD dwTemp;
#ifdef _WIN32
	dwTemp =									FindPattern("\xD9\x15\x00\x00\x00\x00\xD8\x1D\x00\x00\x00\x00\xDF\xE0\xF6\xC4\x41\x75\x07", "xx????xx????xxxxxxx") + 6;
	VAR_pRestartWaitTime =						*(DWORD*)(dwTemp + 2);

	// Sync bounds addresses
	dwTemp = FindPattern("Shot out of bounds.\0", "xxxxxxxxxxxxxxxxxxxx");
	if (dwTemp)
	{
		VAR_pPosSyncBounds[0] = dwTemp + 20;
		VAR_pPosSyncBounds[1] = VAR_pPosSyncBounds[0] + 4;
	}
	dwTemp = FindPattern("Offset out of bounds.\0\0\0", "xxxxxxxxxxxxxxxxxxxxxxxx");
	if (dwTemp)
	{
		VAR_pPosSyncBounds[2] = dwTemp + 24;
	}
	VAR_pPosSyncBounds[3] =						FindPattern("\x00\x50\x43\x48\x00\x00\xC8\xC2\x00\x00\x00\x3F\xB2\x00\x00\x00\xAD\x00\x00\x00\xAE\x00\x00\x00maxnpc", "????xxxxxxxxxxxxxxxxxxxxxxxxxx");

	VAR_wRCONUser =								0x004E5874;
	VAR_bRconSocketReply =						0x00515D18;
	ARRAY_ConsoleCommands =						FindPattern("echo", "xxxx");

	FUNC_CConsole__AddStringVariable =			FindPattern("\x53\x56\x57\x8B\x7C\x24\x18\x85\xFF", "xxxxxxxxx");
	FUNC_CConsole__GetStringVariable =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0B", "xxxxxx????xxxx");
	FUNC_CConsole__SetStringVariable =			FindPattern("\x8B\x44\x24\x04\x53\x50\xE8\xD5\xFE\xFF\xFF\x8B\xD8\x85\xDB", "xxxxxxx???xxxx");
	FUNC_CConsole__GetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0D\x83\x38\x01\x75\x08", "xxxxxx????xxxxxxxxx");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x8B\x44\x24\x04\x50\xE8\xF6\xFD\xFF\xFF\x85\xC0\x74\xE0\x83\x38\x01", "xxxxxx????xx??xxx") + 0x20;
	FUNC_CConsole__GetBoolVariable =			FindPattern("\x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x85\xC0\x74\x0D", "xxxxxx????xxxx") + 0x90;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x8B\x44\x24\x04\x50\xE8\x16\xFF\xFF\xFF\x85\xC0\x74\x07", "xxxxxx????xxxx");
	FUNC_CConsole__FindVariable =				FindPattern("\x8B\x84\x24\x30\x01\x00\x00\x53\x56\x57", "xxxxxxxxxx") - 0x1B;
	FUNC_CConsole__SendRules =					FindPattern("\x81\xEC\x08\x04\x00\x00\x53\x55\x56\x57\x8B\xF9\x8B\x77\x04", "xx????xxxxxxxxx");
	FUNC_CConsole__Execute =					FindPattern("\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x0C\x01\x00\x00", "xxxxxxxxxxxx");

	FUNC_CNetGame__SetWeather =					FindPattern("\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x98\x01\x00\x00\x56\x8B\xF1", "xxx????xxxxxxxxxxxxxxxxx?xxxxx"); // 00490A40
	FUNC_CNetGame__SetGravity =					FUNC_CNetGame__SetWeather + 0xD0; // 0x00490B10;

	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x8B\x44\x24\x04\x81\xEC\x04\x01\x00\x00", "xxxxxxxxxx");
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\xCC\x51\x53\x8B\x5C\x24\x0C\x55\x56\x57\x89", "xxxxxxxxxxx") + 0x1;

	FUNC_ContainsInvalidChars =					FindPattern("\x8B\x4C\x24\x04\x8A\x01\x84\xC0", "xxxxxxxx");

	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x56\x8B\xF1\x8B\x86\x3B\x26\x00\x00\x85\xC0\x0F\x84", "xxxxx????xxxx");
	FUNC_CVehicle__Respawn =					FindPattern("\x53\x33\xC0\x56\x8B\xF1\x57\xB9\x10\x00\x00\x00\x8D\x7E\x0C", "xxxxxxxxx???xxx");
	FUNC_CPlayerPool__HandleVehicleRespawn =	FindPattern("\x53\x55\x56\x8B\xF1\x8B\xAE\x68\x09", "xxxxxx?xx");
	FUNC_CObject__SpawnForPlayer =				0x00497790;

	FUNC_ProcessQueryPacket =					FindPattern("\x83\xEC\x24\x53\x55\x56\x57\x8B\x7C\x24", "xxxxxxxxxx");
	FUNC_Packet_WeaponsUpdate =					FindPattern("\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x0\x0\x0\x0\x50\x64\x89\x25\x0\x0\x0\x0\x81\xEC\x2C\x01\x00\x00\x55\x56", "xx????xx????xxxx????xxxx??xx");
	FUNC_Packet_StatsUpdate =					FindPattern("\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x20\x01\x00\x00\x56\x57", "xxx????xx????xxxxxxxxxxx???xx");
	FUNC_format_amxstring =						FindPattern("\x8B\x54\x24\x08\x56\x8B\x74\x24\x08\x57\x33\xC0", "xxxxxxxxxxxx");

	ADDR_GetNetworkStats_VerbosityLevel =		1 + FindPattern("\x6A\x01\x8D\x4C\x0C", "xxxxx"); // 0x0047362A
	ADDR_GetPlayerNetworkStats_VerbosityLevel = 1 + FindPattern("\x6A\x01\x8D\x44\x04", "xxxxx"); // 0x004730B9;

	ADDR_RecordingDirectory =					1 + FindPattern("\x75\xDF\x8D\x44\x24\x18\x50\x8D\x8C\x24", "xxxxx?xxxx") + 14; // 0x00482265

	FUNC_CGameMode__OnPlayerConnect =			FindPattern("\x83\xEC\x08\x56\x8B\xF1\x8A\x46\x68", "xxxxxxxxx"); // 0x0046D910
	FUNC_CGameMode__OnPlayerDisconnect =		FUNC_CGameMode__OnPlayerConnect + 0x60; // 0x0046D970;
	FUNC_CGameMode__OnPlayerSpawn =				FUNC_CGameMode__OnPlayerDisconnect + 0x60; // 0x0046D9D0;
	FUNC_CGameMode__OnPlayerStreamIn =			FUNC_CGameMode__OnPlayerSpawn + 0xF10; // 0x0046E8E0
	FUNC_CGameMode__OnPlayerStreamOut =			FUNC_CGameMode__OnPlayerStreamIn + 0x70; // 0x0046E950;

	ADDR_WrongPacketIDBranch =					0x004591FC;

	FUNC_OnDialogResponse = 					FindPattern("\x55\x8B\xEC\x83\xE4\x00\x6A\x00\x68\x00\x00\x00\x00\x64\x00\x00\x00\x00\x00\x50\x64\x89\x00\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x8B\x45\x00\x8B\x0D", "xxxxx?x?x????x?????xxx?????xx????xx?xx"); // 0x00493750

	FUNC_ReplaceBadChars =						FindPattern("\x56\x8B\x74\x00\x00\x56\xE8\x00\x00\x00\x00\x8A\x06", "xxx??xx????xx"); // 0x00468F20
	FUNC_RemoveColorTags = 						FindPattern("\x53\x8B\x5C\x24\x08\x56\x57\x8A\x03", "xxxxxxxxx"); // 0x468ED0

#else

	// Thx for Mellnik
	VAR_pRestartWaitTime = 						NULL;
	VAR_wRCONUser = 							NULL;
	VAR_bRconSocketReply = 						NULL;
	ARRAY_ConsoleCommands =						FindPattern("echo", "xxxx");


	FUNC_CConsole__AddStringVariable = 			FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\x85\xF6\x74\x00\x89\x34\x24", "xxxxxxx?xx?xxx?xxx");
	FUNC_CConsole__GetStringVariable =			FUNC_CConsole__AddStringVariable - 0x760;//0x80A0190;
	FUNC_CConsole__SetStringVariable =			FindPattern("\x55\x89\xE5\x83\xEC\x00\x89\x75\x00\x8B\x45\x00\x89\x7D\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00\x8B\x45\x00", "xxxxx?xx?xx?xx?xx?xx?xxx?xx?");
	FUNC_CConsole__SetIntVariable =				FindPattern("\x83\x38\x00\x74\x00\xC9\xC3\x8B\x50\x00\x8B\x45\x00", "xx?x?xxxx?xx?") - 0x1C;
	FUNC_CConsole__GetIntVariable =				FUNC_CConsole__SetIntVariable + 0x30; // 0x80A0070;
	FUNC_CConsole__GetBoolVariable =			FUNC_CConsole__SetIntVariable - 0x30;//0x80A0010;
	FUNC_CConsole__ModifyVariableFlags =		FindPattern("\x89\x04\x24\xE8\x00\x00\x00\x00\x85\xC0\x89\xC2\x74\x00\x8B\x45\x00", "xxxx????xxxxx?xx?") - 0x10;
	FUNC_CConsole__FindVariable =				FindPattern("\xB9\xFF\x00\x00\x00\x89\xE5\x81\xEC\x68\x01\x00\x00", "xxxxxxxxxxxxx") - 0x1;
	FUNC_CConsole__SendRules =					FindPattern("\x55\x31\xD2\x89\xE5\x57\x56\x53\x81\xEC\x4C\x04", "xxxxxxxxxxxx");
	FUNC_CConsole__Execute =					FindPattern("\x55\x89\xE5\x57\x56\x53\x81\xEC\x3C\x01\x00\x00\x8B\x45\x0C", "xxxxxxxxxxxxxxx");

	FUNC_CNetGame__SetWeather = 				0x80AE6D0;
	FUNC_CNetGame__SetGravity = 				0x80AE7D0;

	FUNC_CFilterscripts__LoadFilterscript =		FindPattern("\x89\x7D\x00\x8B\x45\x00\x8B\x7D\x00\x89\x5D\x00\x89\x44\x24\x00", "xx?xx?xx?xx?xxx?") - 0x9;
	FUNC_CFilterscripts__UnLoadFilterscript =	FindPattern("\x31\xF6\x53\x83\xEC\x00\x8B\x45\x00\x8B\x7D\x00\x89\xC3", "xxxxx?xx?xx?xx") - 0x5;

	FUNC_ContainsInvalidChars =					FindPattern("\x53\x8B\x5D\x00\x0F\xB6\x0B\x84\xC9\x74\x00\x66\x90", "xxx?xxxxxx?xx") - 0x3;

	FUNC_CPlayer__SpawnForWorld =				FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x00\x8B\x75\x00\xA1\x00\x00\x00\x00", "xxxxxxx?xx?x????");
	FUNC_CVehicle__Respawn =					FindPattern("\x55\x31\xD2\x89\xE5\x57\xB9\x00\x00\x00\x00\x53", "xxxxxxx????x");
	FUNC_CObject__SpawnForPlayer =				0x80C8080;

	//
	DWORD addr = 								FindPattern("\x55\x31\xD2\x89\xE5\x57\xB9\x40\x00\x00\x00", "xxxxxxxxxxx");
	//logprintf("addr: %x", addr);

	FUNC_ProcessQueryPacket =					FindPattern("\x81\xEC\xA8\x00\x00\x00\x89\x5D\xF4\x8B\x5D\x14\x89\x75\xF8", "xxxxxxxxxxxxxxx") - 0x3;
	FUNC_Packet_WeaponsUpdate =					FindPattern("\x55\x31\xC0\x89\xE5\x81\xEC\x58\x01\x00\x00", "xxxxxxxxxxx");
	FUNC_Packet_StatsUpdate =					FindPattern("\x55\x31\xD2\x89\xE5\x81\xEC\x58\x01\x00\x00\x89\x5D\xF4", "xxxxxxxxxxxxxx"); // 80AD430
	FUNC_format_amxstring = 					FindPattern("\x55\xB8\x00\x10\x00\x00\x89\xE5\x56\x53\x83\xEC\x20", "xxxxxxxxxxxxx");

	ADDR_GetNetworkStats_VerbosityLevel =		1 + FindPattern("\xB8\x01\x00\x00\x00\x83\xD9\x03", "xxxxxxxx");
	ADDR_GetPlayerNetworkStats_VerbosityLevel = 1 + FindPattern("\xBB\x01\x00\x00\x00\x83\xD9\x03", "xxxxxxxx"); // 080DD7FA

	FUNC_CGameMode__OnPlayerConnect = 			0x80A5160;
	FUNC_CGameMode__OnPlayerDisconnect = 		0x80A51D0;
	FUNC_CGameMode__OnPlayerSpawn = 			0x80A5250;
	FUNC_CGameMode__OnPlayerStreamIn = 			0x80A6450;
	FUNC_CGameMode__OnPlayerStreamOut = 		0x80A64D0;

	ADDR_WrongPacketIDBranch = 					0x080752FC;

	FUNC_OnDialogResponse =						FindPattern("\x55\x89\xE5\x81\xEC\x00\x00\x00\x00\x89\x5D\xF4\x8B\x55\x08\x89\x75\xF8\x89", "xxxxx????xxxxxxxxxx"); // 080B2A00

	FUNC_ReplaceBadChars =						FindPattern("\x55\x89\xE5\x53\x83\xEC\x04\x8B\x5D\x08\x89\x1C\x24\xE8\x00\x00\x00\x00\x0F", "xxxxxxxxxxxxxx????x"); // 080D5CA0
	FUNC_RemoveColorTags = 						FindPattern("\x55\x89\xE5\x57\x56\x53\x83\xEC\x0C\x8B\x7D\x08\x8D", "xxxxxxxxxxxxx"); // 80D5C40

	switch(sampVersion)
	{
		case SAMPVersion::VERSION_037:
		{
			VAR_pRestartWaitTime =						0x081A0840;
			break;
		}
		case SAMPVersion::VERSION_037_R2:
		{
			VAR_pRestartWaitTime =						0x0815A528; // 12.0
			VAR_pPosSyncBounds[0] =						0x08150710;
			VAR_pPosSyncBounds[1] =						0x0815070C;
			VAR_pPosSyncBounds[2] =						0x08150718;
			VAR_pPosSyncBounds[3] =						0x08150714;
			ADDR_RecordingDirectory =					0x080CC7D2;
			FUNC_CPlayerPool__HandleVehicleRespawn =	0x80D1480;
			VAR_wRCONUser =								0x08197DF0;
			VAR_bRconSocketReply =						0x081CB608;
			ARRAY_ConsoleCommands =						0x08196920;
			break;
		}
	}
#endif

	/*logprintf("VAR_pRestartWaitTime: %X", VAR_pRestartWaitTime);

	logprintf("FUNC_CConsole__AddStringVariable: %X", FUNC_CConsole__AddStringVariable);
	logprintf("FUNC_CConsole__GetStringVariable: %X", FUNC_CConsole__GetStringVariable);
	logprintf("FUNC_CConsole__SetStringVariable: %X", FUNC_CConsole__SetStringVariable);
	logprintf("FUNC_CConsole__GetIntVariable: %X", FUNC_CConsole__GetIntVariable);
	logprintf("FUNC_CConsole__SetIntVariable: %X", FUNC_CConsole__SetIntVariable);
	logprintf("FUNC_CConsole__GetBoolVariable: %X", FUNC_CConsole__GetBoolVariable);
	logprintf("FUNC_CConsole__FindVariable: %X", FUNC_CConsole__FindVariable);
	logprintf("FUNC_CConsole__SendRules: %X", FUNC_CConsole__SendRules);
	logprintf("FUNC_CConsole__Execute: %X", FUNC_CConsole__Execute);

	logprintf("FUNC_CNetGame__SetWeather: %X", FUNC_CNetGame__SetWeather);
	logprintf("FUNC_CNetGame__SetGravity: %X", FUNC_CNetGame__SetGravity);

	logprintf("FUNC_CFilterscripts__LoadFilterscript: %X", FUNC_CFilterscripts__LoadFilterscript);
	logprintf("FUNC_CFilterscripts__UnLoadFilterscript: %X", FUNC_CFilterscripts__UnLoadFilterscript);

	logprintf("FUNC_ContainsInvalidChars: %X", FUNC_ContainsInvalidChars);

	logprintf("FUNC_CPlayer__SpawnForWorld: %X", FUNC_CPlayer__SpawnForWorld);
	logprintf("FUNC_CVehicle__Respawn: %X", FUNC_CVehicle__Respawn);
	logprintf("FUNC_CPlayerPool__HandleVehicleRespawn: %X", FUNC_CPlayerPool__HandleVehicleRespawn);

	logprintf("FUNC_ProcessQueryPacket: %X", FUNC_ProcessQueryPacket);
	logprintf("FUNC_Packet_WeaponsUpdate: %X", FUNC_Packet_WeaponsUpdate);
	logprintf("FUNC_Packet_StatsUpdate: %X", FUNC_Packet_StatsUpdate);
	logprintf("FUNC_format_amxstring: %X", FUNC_format_amxstring);

	logprintf("ADDR_GetNetworkStats_VerbosityLevel: %X", ADDR_GetNetworkStats_VerbosityLevel);
	logprintf("ADDR_GetPlayerNetworkStats_VerbosityLevel: %X", ADDR_GetPlayerNetworkStats_VerbosityLevel);

	logprintf("FUNC_CGameMode__OnPlayerConnect: %X", FUNC_CGameMode__OnPlayerConnect);
	logprintf("FUNC_CGameMode__OnPlayerDisconnect: %X", FUNC_CGameMode__OnPlayerDisconnect);
	logprintf("FUNC_CGameMode__OnPlayerSpawn: %X", FUNC_CGameMode__OnPlayerSpawn);
	logprintf("FUNC_CGameMode__OnPlayerStreamIn: %X", FUNC_CGameMode__OnPlayerStreamIn);
	logprintf("FUNC_CGameMode__OnPlayerStreamOut: %X", FUNC_CGameMode__OnPlayerStreamOut);
	*/

	//logprintf("ARRAY_ConsoleCommands: %x", ARRAY_ConsoleCommands);
	// Unlock restart wait time
	if (VAR_pRestartWaitTime)
		VAR_pRestartWaitTime.unlock();

	for (BYTE i = 0; i < 4; i++)
	{
		if (VAR_pPosSyncBounds[i])
			VAR_pPosSyncBounds[i].unlock();
	}

	// Patch GetNetworkStats to get more advanced stats than default
	if(ADDR_GetNetworkStats_VerbosityLevel)
	{
		ADDR_GetNetworkStats_VerbosityLevel.unlock();
	}

	// Patch GetPlayerNetworkStats to get more advanced stats than default
	if(ADDR_GetPlayerNetworkStats_VerbosityLevel)
	{
		ADDR_GetPlayerNetworkStats_VerbosityLevel.unlock();
	}

	if(ADDR_WrongPacketIDBranch)
	{
		ADDR_WrongPacketIDBranch.unlock();
	}
}