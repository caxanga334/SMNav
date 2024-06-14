#include <extension.h>
#include <util/helpers.h>
#include <entities/tf2/tf_entities.h>
#include <mods/tf2/tf2lib.h>
#include <bot/tf2/tf2bot.h>
#include <bot/tf2/tasks/tf2bot_find_ammo_task.h>
#include "tf2bot_engineer_upgrade_object.h"

CTF2BotEngineerUpgradeObjectTask::CTF2BotEngineerUpgradeObjectTask(CBaseEntity* object) :
	m_object(object)
{
}

TaskResult<CTF2Bot> CTF2BotEngineerUpgradeObjectTask::OnTaskStart(CTF2Bot* bot, AITask<CTF2Bot>* pastTask)
{
	if (m_object.Get() == nullptr)
	{
		return Done("Goal object is no longer valid");
	}

	return Continue();
}

TaskResult<CTF2Bot> CTF2BotEngineerUpgradeObjectTask::OnTaskUpdate(CTF2Bot* bot)
{
	if (m_object.Get() == nullptr)
	{
		return Done("Goal object is no longer valid");
	}

	tfentities::HBaseObject object(m_object.Get());

	if (object.GetLevel() == object.GetMaxLevel())
	{
		return Done("Object is at max level!");
	}

	if (bot->GetAmmoOfIndex(TeamFortress2::TF_AMMO_METAL) == 0)
	{
		return PauseFor(new CTF2BotFindAmmoTask, "Need more metal!");
	}

	auto myweapon = bot->GetActiveBotWeapon();

	if (myweapon && myweapon->GetModWeaponID<TeamFortress2::TFWeaponID>() != TeamFortress2::TFWeaponID::TF_WEAPON_WRENCH)
	{
		CBaseEntity* wrench = bot->GetWeaponOfSlot(TeamFortress2::TFWeaponSlot::TFWeaponSlot_Melee);

		if (wrench)
		{
			bot->SelectWeapon(wrench);
		}
	}

	if (bot->GetRangeTo(object.WorldSpaceCenter()) > get_object_melee_range())
	{
		if (!m_nav.IsValid() || m_nav.GetAge() > 3.0f)
		{
			CTF2BotPathCost cost(bot);

			if (!m_nav.ComputePathToPosition(bot, object.GetAbsOrigin(), cost))
			{
				return Done("No path to object.");
			}
		}

		m_nav.Update(bot);
	}
	else
	{
		bot->GetControlInterface()->AimAt(object.WorldSpaceCenter(), IPlayerController::LOOK_VERY_IMPORTANT, 0.5f, "Looking at object to upgrade it.");
		bot->GetControlInterface()->PressAttackButton(0.5f);
	}

	return Continue();
}
