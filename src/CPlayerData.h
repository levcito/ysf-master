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

#ifndef YSF_CPLAYERDATA_H
#define YSF_CPLAYERDATA_H

#include <bitset>
#include <chrono>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "CGangZonePool.h"
#include "CPickupPool.h"
#include "Structs.h"
#include "utils/bimap.h"

using default_clock = std::chrono::steady_clock;

class CPlayerObjectAttachAddon
{
public:
/*
	CPlayerObjectAttachAddon::CPlayerObjectAttachAddon() :
		: wObjectID(INVALID_OBJECT_ID), wAttachPlayerID(INVALID_PLAYER_ID)
	{

	}
	CPlayerObjectAttachAddon::CPlayerObjectAttachAddon(WORD &objectid, WORD &attachplayer, CVector &vecoffset, CVector &vecrot)
		: wObjectID(objectid), wAttachPlayerID(attachplayer), vecOffset(vecoffset), vecRot(vecrot)
	{
	
	}
*/
	WORD wObjectID = INVALID_OBJECT_ID;
	WORD wAttachPlayerID = INVALID_PLAYER_ID;
	CVector vecOffset;
	CVector vecRot;
	default_clock::time_point creation_timepoint;
	bool bCreated = false;
	bool bAttached = false;
	//std::unordered_map<BYTE, std::string> strMaterialText;
};

class CPlayerData
{
public:
	CPlayerData(WORD playerid);
	CPlayerData(size_t playerid) : CPlayerData(static_cast<WORD>(playerid))
	{

	}
	~CPlayerData();

	WORD wPlayerID;

	bool SetPlayerTeamForPlayer(WORD teamplayerid, int team);
	int GetPlayerTeamForPlayer(WORD teamplayerid);
	inline void ResetPlayerTeam(WORD playerid) { m_iTeams[playerid] = 0; }

	bool SetPlayerSkinForPlayer(WORD skinplayerid, int skin);
	int GetPlayerSkinForPlayer(WORD skinplayerid);
	inline void ResetPlayerSkin(WORD playerid) { m_iSkins[playerid] = 0; }

	bool SetPlayerNameForPlayer(WORD nameplayerid, const char *name);
	const char *GetPlayerNameForPlayer(WORD nameplayerid);
	inline void ResetPlayerName(WORD playerid) { m_PlayerNames.erase(playerid); }

	bool SetPlayerFightingStyleForPlayer(WORD styleplayerid, int style);
	int GetPlayerFightingStyleForPlayer(WORD styleplayerid);
	inline void ResetPlayerFightingStyle(WORD playerid){ m_iFightingStyles[playerid] = 0; }

	void ResetPlayerMarkerForPlayer(WORD resetplayerid);

	WORD GetGangZoneIDFromClientSide(WORD zoneid, bool bPlayer = false);
	bool DestroyObject(WORD objectid);

	void Process(void);

	bool HiddenInQuery() { return bCustomNameInQuery && strNameInQuery.empty(); }
	void DeleteObjectAddon(WORD objectid);

	void ShowObject(WORD objectid, bool sync);
	void HideObject(WORD objectid, bool sync);
	bool NewObjectsHidden() const;
	void HideNewObjects(bool toggle);
	bool IsObjectHidden(WORD objectid) const;
	void SetBuildingsRemoved(int modelid, const CVector &pos, float range);
	size_t GetBuildingsRemoved() const;
	bool IsBuildingRemoved(int modelid, const CVector &pos, float range) const;

	int iNPCProcessID = -1;
	WORD wSurfingInfo = 0;
	WORD wDialogID = -1;

	// Exclusive RPC broadcast
	bool bBroadcastTo = false;

	// Variables to store disabled keys
	WORD wDisabledKeys = 0;
	WORD wDisabledKeysUD = 0;
	WORD wDisabledKeysLR = 0;

	bool ghostMode = false;

	// Per-player things
	float fGravity = 0.0f;
	BYTE byteWeather = 0;
	float fBounds[4] = { 20000.0f, -20000.0f, 20000.0f, -20000.0f };

	// Per-player pos
	std::bitset<MAX_PLAYERS> bCustomQuat;
	std::unordered_map<WORD, CVector> customPos;
	float fCustomQuat[MAX_PLAYERS][4] = {};

	std::shared_ptr<CPlayerObjectAttachAddon> GetObjectAddon(WORD objectid);
	std::shared_ptr<CPlayerObjectAttachAddon> const FindObjectAddon(WORD objectid);

	// Containers to store attached offset of AttachPlayerObjectToPlayer
	std::unordered_map<WORD, std::shared_ptr<CPlayerObjectAttachAddon>> m_PlayerObjectsAddon;
	std::set<WORD> m_PlayerObjectsAttachQueue;

	// Fix for GetPlayerObjectMaterial/MaterialText - i keep this outside from containers above
	std::multimap<WORD, std::pair<BYTE, std::string>> m_PlayerObjectMaterialText;

	// Other
	bool bCustomNameInQuery = false;
	std::string strNameInQuery;

	// Gangzones
	class CGangZone *pPlayerZone[MAX_GANG_ZONES] = {};

	// [clientsideid] = serversideid
	// initialized to 0xFF
	BYTE byteClientSideZoneIDUsed[MAX_GANG_ZONES];
	WORD wClientSideGlobalZoneID[MAX_GANG_ZONES];
	WORD wClientSidePlayerZoneID[MAX_GANG_ZONES];

	std::bitset<MAX_GANG_ZONES> bInGangZone;
	std::bitset<MAX_GANG_ZONES> bIsGangZoneFlashing;
	DWORD dwClientSideZoneColor[MAX_GANG_ZONES] = {};
	DWORD dwClientSideZoneFlashColor[MAX_GANG_ZONES] = {};
#ifdef NEW_PICKUP_SYSTEM
	// Pickpus - clientside (global/player)
	PickupMap ClientPlayerPickups;
	std::bitset<MAX_PICKUPS> bClientPickupSlots;
	std::bitset<MAX_PICKUPS> bClientPickupStreamedIn;

	// Pickups - per-player
	PickupMap PlayerPickups;
	std::bitset<MAX_PICKUPS> bPlayerPickup;
#endif
	DWORD dwFakePingValue = 0;
	default_clock::time_point LastUpdateTick;

	bool bObjectsRemoved = false;
	bool bWidescreen = false;
	bool bUpdateScoresPingsDisabled = false;
	bool bFakePingToggle = false;
	bool bAFKState = false;
	bool bEverUpdated = false;
	bool bControllable = false;
	bool bAttachedObjectCreated = false;

private:
	struct RemovedBuilding
	{
		int ModelId;
		CVector Position;
		float Range;

		RemovedBuilding(int modelid, const CVector &pos, float range) : ModelId(modelid), Position(pos), Range(range)
		{

		}
	};

	int m_iTeams[MAX_PLAYERS] = {};
	int m_iSkins[MAX_PLAYERS] = {};
	int m_iFightingStyles[MAX_PLAYERS] = {};
	std::unordered_map<WORD, std::string> m_PlayerNames;
	std::unordered_set<WORD> m_HiddenObjects;
	bool m_HideNewObjects = false;
	std::vector<RemovedBuilding> m_RemovedBuildings;
};

#endif
