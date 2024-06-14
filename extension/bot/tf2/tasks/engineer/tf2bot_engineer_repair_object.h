#ifndef NAVBOT_TF2BOT_TASKS_ENGINEER_REPAIR_OBJECT_H_
#define NAVBOT_TF2BOT_TASKS_ENGINEER_REPAIR_OBJECT_H_

#include <sdkports/sdk_ehandle.h>
#include <bot/interfaces/path/meshnavigator.h>

class CTF2Bot;

class CTF2BotEngineerRepairObjectTask : public AITask<CTF2Bot>
{
public:
	CTF2BotEngineerRepairObjectTask(CBaseEntity* object);

	TaskResult<CTF2Bot> OnTaskStart(CTF2Bot* bot, AITask<CTF2Bot>* pastTask) override;
	TaskResult<CTF2Bot> OnTaskUpdate(CTF2Bot* bot) override;

	const char* GetName() const override { return "RepairObject"; }

private:
	CHandle<CBaseEntity> m_object;
	CMeshNavigator m_nav;

	static constexpr auto get_object_melee_range() { return 72.0f; }
};

#endif // !NAVBOT_TF2BOT_TASKS_ENGINEER_REPAIR_OBJECT_H_
