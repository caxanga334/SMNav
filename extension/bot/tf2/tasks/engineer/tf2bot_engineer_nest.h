#ifndef NAVBOT_TF2BOT_TASKS_ENGINEER_NEST_H_
#define NAVBOT_TF2BOT_TASKS_ENGINEER_NEST_H_

#include <sdkports/sdk_timers.h>
#include <bot/interfaces/path/meshnavigator.h>

class CTF2Bot;

/**
 * @brief Task responsible for taking care of the engineer's own buildings
 */
class CTF2BotEngineerNestTask : public AITask<CTF2Bot>
{
public:
	static constexpr auto max_travel_distance() { return 2048.0f; }

	CTF2BotEngineerNestTask();

	TaskResult<CTF2Bot> OnTaskUpdate(CTF2Bot* bot) override;
	bool OnTaskPause(CTF2Bot* bot, AITask<CTF2Bot>* nextTask) override;
	TaskResult<CTF2Bot> OnTaskResume(CTF2Bot* bot, AITask<CTF2Bot>* pastTask) override;


	TaskEventResponseResult<CTF2Bot> OnMoveToFailure(CTF2Bot* bot, CPath* path, IEventListener::MovementFailureType reason) override;
	TaskEventResponseResult<CTF2Bot> OnMoveToSuccess(CTF2Bot* bot, CPath* path) override;

	const char* GetName() const override { return "EngineerNest"; }



private:
	CMeshNavigator m_nav;
	Vector m_goal;

	AITask<CTF2Bot>* NestTask(CTF2Bot* me);
	bool FindSpotToBuildSentryGun(CTF2Bot* me, Vector& out);
	bool FindSpotToBuildTeleEntrance(CTF2Bot* me, Vector& out);
};

#endif // !NAVBOT_TF2BOT_TASKS_ENGINEER_NEST_H_
