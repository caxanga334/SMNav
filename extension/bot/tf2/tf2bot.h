#ifndef NAVBOT_TEAM_FORTRESS_2_BOT_H_
#define NAVBOT_TEAM_FORTRESS_2_BOT_H_
#pragma once

#include <memory>

#include <sdkports/sdk_timers.h>
#include <sdkports/sdk_ehandle.h>
#include <mods/tf2/teamfortress2_shareddefs.h>
#include <bot/basebot.h>
#include <bot/interfaces/path/basepath.h>

#include "tf2bot_behavior.h"
#include "tf2bot_controller.h"
#include "tf2bot_movement.h"
#include "tf2bot_sensor.h"
#include "tf2bot_inventory.h"
#include "tf2bot_spymonitor.h"
#include "tf2bot_upgrades.h"

struct edict_t;
class CTFWaypoint;

class CTF2Bot : public CBaseBot
{
public:
	CTF2Bot(edict_t* edict);
	~CTF2Bot() override;

	// Reset the bot to it's initial state
	void Reset() override;
	// Function called at intervals to run the AI 
	void Update() override;
	// Function called every server frame to run the AI
	void Frame() override;

	void TryJoinGame() override;
	void Spawn() override;
	void FirstSpawn() override;

	CTF2BotPlayerController* GetControlInterface() const override { return m_tf2controller.get(); }
	CTF2BotMovement* GetMovementInterface() const override { return m_tf2movement.get(); }
	CTF2BotSensor* GetSensorInterface() const override { return m_tf2sensor.get(); }
	CTF2BotBehavior* GetBehaviorInterface() const override { return m_tf2behavior.get(); }
	CTF2BotSpyMonitor* GetSpyMonitorInterface() const { return m_tf2spymonitor.get(); }
	CTF2BotInventory* GetInventoryInterface() const { return m_tf2inventory.get(); }
	int GetMaxHealth() const override;

	TeamFortress2::TFClassType GetMyClassType() const;
	TeamFortress2::TFTeam GetMyTFTeam() const;
	void JoinClass(TeamFortress2::TFClassType tfclass) const;
	void JoinTeam() const;
	edict_t* GetItem() const;
	bool IsCarryingAFlag() const;
	edict_t* GetFlagToFetch() const;
	edict_t* GetFlagToDefend(bool stolenOnly = false, bool ignoreHome = false) const;
	edict_t* GetFlagCaptureZoreToDeliver() const;
	bool IsAmmoLow() const;
	void FindMyBuildings();
	bool IsDisguised() const;
	bool IsCloaked() const;
	bool IsInvisible() const;
	int GetCurrency() const;
	bool IsInUpgradeZone() const;
	bool IsUsingSniperScope() const;

	inline void BeginBuilding(TeamFortress2::TFObjectType type, TeamFortress2::TFObjectMode mode)
	{
		switch (type)
		{
		case TeamFortress2::TFObject_Dispenser:
			DelayedFakeClientCommand("build 0 0");
			break;
		case TeamFortress2::TFObject_Teleporter:
		{
			if (mode == TeamFortress2::TFObjectMode::TFObjectMode_Entrance)
			{
				DelayedFakeClientCommand("build 1 0");
			}
			else
			{
				DelayedFakeClientCommand("build 1 1");
			}

			break;
		}
		case TeamFortress2::TFObject_Sentry:
			DelayedFakeClientCommand("build 2 0");
			break;
		default:
			break;
		}
	}

	inline void DisguiseAs(TeamFortress2::TFClassType classtype, bool myTeam = false)
	{
		std::unique_ptr<char[]> buffer = std::make_unique<char[]>(128);
		ke::SafeSprintf(buffer.get(), 128, "disguise %i %i", static_cast<int>(classtype), myTeam ? -2 : -1);
		DelayedFakeClientCommand(buffer.get());
	}

	/**
	 * @brief Toggles the ready status in Tournament modes. Also used in Mann vs Machine.
	 * @param isready Is the bot ready?
	 */
	void ToggleTournamentReadyStatus(bool isready = true) const;
	bool TournamentIsReady() const;

	CTF2BotUpgradeManager& GetUpgradeManager() { return m_upgrademan; }

	bool IsInsideSpawnRoom() const;

	CBaseEntity* GetMySentryGun() const;
	CBaseEntity* GetMyDispenser() const;
	CBaseEntity* GetMyTeleporterEntrance() const;
	CBaseEntity* GetMyTeleporterExit() const;
	void SetMySentryGun(CBaseEntity* entity);
	void SetMyDispenser(CBaseEntity* entity);
	void SetMyTeleporterEntrance(CBaseEntity* entity);
	void SetMyTeleporterExit(CBaseEntity* entity);

private:
	std::unique_ptr<CTF2BotMovement> m_tf2movement;
	std::unique_ptr<CTF2BotPlayerController> m_tf2controller;
	std::unique_ptr<CTF2BotSensor> m_tf2sensor;
	std::unique_ptr<CTF2BotBehavior> m_tf2behavior;
	std::unique_ptr<CTF2BotSpyMonitor> m_tf2spymonitor;
	std::unique_ptr<CTF2BotInventory> m_tf2inventory;
	TeamFortress2::TFClassType m_desiredclass; // class the bot wants
	IntervalTimer m_classswitchtimer;
	CHandle<CBaseEntity> m_mySentryGun;
	CHandle<CBaseEntity> m_myDispenser;
	CHandle<CBaseEntity> m_myTeleporterEntrance;
	CHandle<CBaseEntity> m_myTeleporterExit;
	CTF2BotUpgradeManager m_upgrademan;

	static constexpr float medic_patient_health_critical_level() { return 0.3f; }
	static constexpr float medic_patient_health_low_level() { return 0.6f; }
};

class CTF2BotPathCost : public IPathCost
{
public:
	CTF2BotPathCost(CTF2Bot* bot, RouteType routetype = FASTEST_ROUTE);

	float operator()(CNavArea* toArea, CNavArea* fromArea, const CNavLadder* ladder, const NavOffMeshConnection* link, const CNavElevator* elevator, float length) const override;

private:
	CTF2Bot* m_me;
	RouteType m_routetype;
	float m_stepheight;
	float m_maxjumpheight;
	float m_maxdropheight;
	float m_maxdjheight; // max double jump height
	float m_maxgapjumpdistance;
	bool m_candoublejump;
};

#endif // !NAVBOT_TEAM_FORTRESS_2_BOT_H_
