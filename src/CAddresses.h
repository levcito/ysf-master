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

#ifndef YSF_ADDRESSES_H
#define YSF_ADDRESSES_H

#include <type_traits>
#include <stdexcept>
#include "CFunctions.h"
#include "Memory.h"

#ifdef _WIN32
	#define RAKNET_START_OFFSET							1
	#define RAKNET_SEND_OFFSET							7
	#define RAKNET_RECEIVE_OFFSET						10
	#define RAKNET_SET_ALLOWED_PLAYERS_OFFSET			13
	#define RAKNET_GET_LAST_PING_OFFSET					19
	#define RAKNET_REGISTER_RPC_OFFSET					29
	#define RAKNET_UNREGISTER_RPC_OFFSET				31
	#define RAKNET_RPC_OFFSET							32
	#define RAKNET_GET_LOCAL_IP_OFFSET					52
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET		57
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET		58
	#define RAKNET_ADD_BAN_OFFSET						60
	#define RAKNET_REMOVE_BAN_OFFSET					61
	#define RAKNET_CLEAR_BAN_OFFSET						62
	#define RAKNET_SET_TIMEOUT_OFFSET					65
#else
	#define RAKNET_START_OFFSET							2
	#define RAKNET_SEND_OFFSET							9
	#define RAKNET_RECEIVE_OFFSET						11
	#define RAKNET_SET_ALLOWED_PLAYERS_OFFSET			14
	#define RAKNET_GET_LAST_PING_OFFSET					20
	#define RAKNET_REGISTER_RPC_OFFSET					30
	#define RAKNET_UNREGISTER_RPC_OFFSET				32
	#define RAKNET_RPC_OFFSET							35
	#define RAKNET_GET_LOCAL_IP_OFFSET					53
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET		58
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET		59
	#define RAKNET_ADD_BAN_OFFSET						61
	#define RAKNET_REMOVE_BAN_OFFSET					62
	#define RAKNET_CLEAR_BAN_OFFSET						63
	#define RAKNET_SET_TIMEOUT_OFFSET					65
#endif

#include <vector>
#include "includes/types.h"

enum class SAMPVersion
{
	VERSION_UNKNOWN,
	VERSION_037,
	VERSION_037_R2
};

namespace aux
{
	template <class T>
	struct is_function_pointer : std::false_type
	{

	};

#define DEF_IS_FUNCTION_POINTER_CQ(CALLCONV, QUALIFIERS) \
	template <class TRet, class... TArgs> \
	struct is_function_pointer<TRet(CALLCONV * QUALIFIERS)(TArgs...)> : std::true_type { }

#define DEF_IS_FUNCTION_POINTER(CALLCONV) \
	DEF_IS_FUNCTION_POINTER_CQ(CALLCONV,); \
	DEF_IS_FUNCTION_POINTER_CQ(CALLCONV, const); \
	DEF_IS_FUNCTION_POINTER_CQ(CALLCONV, volatile); \
	DEF_IS_FUNCTION_POINTER_CQ(CALLCONV, const volatile)

#ifdef _WIN32
	DEF_IS_FUNCTION_POINTER(__cdecl);
	DEF_IS_FUNCTION_POINTER(__stdcall);
	DEF_IS_FUNCTION_POINTER(__fastcall);
	DEF_IS_FUNCTION_POINTER(__thiscall);
#else
	DEF_IS_FUNCTION_POINTER();
#endif

	template <class TRet, class... TArgs>
	struct is_function_pointer<TRet(*)(TArgs..., ...)> : std::true_type { };
#undef DEF_IS_FUNCTION_POINTER_CQ
#undef DEF_IS_FUNCTION_POINTER
}

template <class T>
class ADDR
{
	// __thiscall without * produces errors
	typedef typename std::conditional<aux::is_function_pointer<T>::value, T, T*>::type ptr_type;

	ptr_type ptr;
public:
	ADDR() : ptr(nullptr)
	{

	}

	ADDR(decltype(nullptr)) : ptr(nullptr)
	{

	}

	ADDR(DWORD addr) : ptr(reinterpret_cast<ptr_type>(addr))
	{

	}

	operator DWORD() const
	{
		return reinterpret_cast<DWORD>(ptr);
	}

	ADDR &operator=(DWORD addr)
	{
		ptr = reinterpret_cast<ptr_type>(addr);
		return *this;
	}

	auto operator*() -> decltype(*ptr)
	{
		if (ptr == nullptr) throw std::logic_error("Attempt to dereference null pointer.");
		return *ptr;
	}

	void unlock()
	{
		if (ptr == nullptr) throw std::logic_error("Attempt to dereference null pointer.");
		Unlock(reinterpret_cast<void*>(ptr), sizeof(T));
	}

	explicit operator bool() const
	{
		return ptr != nullptr;
	}

	template <class... Args>
	auto operator()(Args&&... args) -> decltype(ptr(std::forward<Args>(args)...))
	{
		if (ptr == nullptr) throw std::logic_error("Attempt to dereference null pointer.");
		return ptr(std::forward<Args>(args)...);
	}
};

#define DECLARE_FUNC_PTR(name) static ADDR<name ## _t> FUNC_ ## name
#define DEFINE_FUNC_PTR(name) ADDR<name ## _t> CAddress::FUNC_ ## name = nullptr

class CAddress
{
	typedef void(logprintf_t)(const char* format, ...);
public:
	static void	Initialize(SAMPVersion version);

	static ADDR<logprintf_t> FUNC_Logprintf_03Z;
	static ADDR<logprintf_t> FUNC_Logprintf_03ZR2_2;
	static ADDR<logprintf_t> FUNC_Logprintf_03ZR3;
	static ADDR<logprintf_t> FUNC_Logprintf_03ZR4;
	static ADDR<logprintf_t> FUNC_Logprintf_037;
	static ADDR<logprintf_t> FUNC_Logprintf_037_R2;
	static ADDR<logprintf_t> FUNC_Logprintf_037_R2_1;
	static ADDR<logprintf_t> FUNC_Logprintf_03DL_R1;

	// Variables
	static ADDR<float>		VAR_pRestartWaitTime;
	static ADDR<float>		VAR_pPosSyncBounds[4];
	static ADDR<WORD>		VAR_wRCONUser;
	static ADDR<bool>		VAR_bRconSocketReply;
	static ADDR<ConsoleCommand_s[]> ARRAY_ConsoleCommands;

	// Functions
	DECLARE_FUNC_PTR(CConsole__AddStringVariable);
	DECLARE_FUNC_PTR(CConsole__GetStringVariable);
	DECLARE_FUNC_PTR(CConsole__SetStringVariable);
	DECLARE_FUNC_PTR(CConsole__GetIntVariable);
	DECLARE_FUNC_PTR(CConsole__SetIntVariable);
	DECLARE_FUNC_PTR(CConsole__GetBoolVariable);
	DECLARE_FUNC_PTR(CConsole__ModifyVariableFlags);
	DECLARE_FUNC_PTR(CConsole__FindVariable);
	DECLARE_FUNC_PTR(CConsole__SendRules);
	DECLARE_FUNC_PTR(CConsole__Execute);

	DECLARE_FUNC_PTR(CNetGame__SetWeather);
	DECLARE_FUNC_PTR(CNetGame__SetGravity);

	DECLARE_FUNC_PTR(CFilterscripts__LoadFilterscript);
	DECLARE_FUNC_PTR(CFilterscripts__UnLoadFilterscript);
	DECLARE_FUNC_PTR(ContainsInvalidChars);

	DECLARE_FUNC_PTR(CPlayer__SpawnForWorld);
	DECLARE_FUNC_PTR(CVehicle__Respawn);
	DECLARE_FUNC_PTR(CPlayerPool__HandleVehicleRespawn);
	DECLARE_FUNC_PTR(CObject__SpawnForPlayer);

	DECLARE_FUNC_PTR(ProcessQueryPacket);
	DECLARE_FUNC_PTR(Packet_WeaponsUpdate);
	DECLARE_FUNC_PTR(Packet_StatsUpdate);
	DECLARE_FUNC_PTR(format_amxstring);

	DECLARE_FUNC_PTR(ReplaceBadChars);
	DECLARE_FUNC_PTR(RemoveColorTags);

	DECLARE_FUNC_PTR(OnDialogResponse);

	// Others
	static ADDR<BYTE> ADDR_GetNetworkStats_VerbosityLevel;
	static ADDR<BYTE> ADDR_GetPlayerNetworkStats_VerbosityLevel;

	static ADDR<const char*> ADDR_RecordingDirectory;

#ifdef _WIN32
	static ADDR<unsigned char[82]> ADDR_WrongPacketIDBranch;
#else
	static ADDR<unsigned char[114]> ADDR_WrongPacketIDBranch;
#endif

	// Callback hooks
	DECLARE_FUNC_PTR(CGameMode__OnPlayerConnect);
	DECLARE_FUNC_PTR(CGameMode__OnPlayerDisconnect);
	DECLARE_FUNC_PTR(CGameMode__OnPlayerSpawn);
	DECLARE_FUNC_PTR(CGameMode__OnPlayerStreamIn);
	DECLARE_FUNC_PTR(CGameMode__OnPlayerStreamOut);
};

#endif
