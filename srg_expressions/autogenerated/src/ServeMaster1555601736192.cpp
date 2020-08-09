#include "ServeMaster1555601736192.h"
/*PROTECTED REGION ID(eph1555601736192) ENABLED START*/
#include <engine/IRoleAssignment.h>
/*PROTECTED REGION END*/

namespace alica
{
// Plan:ServeMaster
// Check of PreCondition - (Name): 1558533725689, (ConditionString): Stop received , (Comment) :

/**
 * Available Vars:
 */
bool PreCondition1558533725689::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1558533725689) ENABLED START*/
    // TODO implement wm and check whether stop/start message was received
    return true;
    /*PROTECTED REGION END*/
}
/**
 * Task: Default  -> EntryPoint-ID: 1555601746711
 */
std::shared_ptr<UtilityFunction> UtilityFunction1555601736192::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1555601736192) ENABLED START*/
    std::shared_ptr<UtilityFunction> defaultFunction = std::make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1558533620473, ConditionString: Start received!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Stop (1555602210283)
 *
 * Tasks in plan:
 *   - Default (1575654042058) (Entrypoint: 1555601746711)
 *
 * States in plan:
 *   - Stop (1555601748848)
 *   - Spawn (1558533460297)
 *   - Operate (1558533461982)
 *
 * Variables of preconditon:
 */
bool PreCondition1558533620473::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1558533492085) ENABLED START*/
    auto lastCmd = this->wm->rawSensorData.getAgentCmdBuffer().getLastValidContent();
    return lastCmd.has_value() && lastCmd->cmd == srg::agent::AgentCommand::START;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1558533654035, ConditionString: Stop received, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Spawn (1558533534825)
 *
 * Tasks in plan:
 *   - Default (1575654042058) (Entrypoint: 1555601746711)
 *
 * States in plan:
 *   - Stop (1555601748848)
 *   - Spawn (1558533460297)
 *   - Operate (1558533461982)
 *
 * Variables of preconditon:
 */
bool PreCondition1558533654035::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1558533494595) ENABLED START*/
    auto lastCmd = this->wm->rawSensorData.getAgentCmdBuffer().getLastValidContent();
    return lastCmd.has_value() && lastCmd->cmd == srg::agent::AgentCommand::STOP;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1558533667052, ConditionString: Spawn successful (acknowledge through received sim sensor values) && I am a robot, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Spawn (1558533534825)
 *
 * Tasks in plan:
 *   - Default (1575654042058) (Entrypoint: 1555601746711)
 *
 * States in plan:
 *   - Stop (1555601748848)
 *   - Spawn (1558533460297)
 *   - Operate (1558533461982)
 *
 * Variables of preconditon:
 */
bool PreCondition1558533667052::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1558533514929) ENABLED START*/
    return this->wm->sRGSimData.isLocalised();
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1560934538125, ConditionString: Stop received!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Operate (1575655614484)
 *
 * Tasks in plan:
 *   - Default (1575654042058) (Entrypoint: 1555601746711)
 *
 * States in plan:
 *   - Stop (1555601748848)
 *   - Spawn (1558533460297)
 *   - Operate (1558533461982)
 *
 * Variables of preconditon:
 */
bool PreCondition1560934538125::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1558533714287) ENABLED START*/
    auto lastCmd = this->wm->rawSensorData.getAgentCmdBuffer().getLastValidContent();
    return lastCmd.has_value() && lastCmd->cmd == srg::agent::AgentCommand::STOP;
    /*PROTECTED REGION END*/
}
} // namespace alica
