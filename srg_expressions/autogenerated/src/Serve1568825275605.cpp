#include "Serve1568825275605.h"
/*PROTECTED REGION ID(eph1568825275605) ENABLED START*/
#include <srg/dialogue/CommandHandler.h>
/*PROTECTED REGION END*/

using namespace alica;

namespace alicaAutogenerated
{
// Plan:Serve
/* generated comment
    Task: Serve  -> EntryPoint-ID: 1568825285315
*/
shared_ptr<UtilityFunction> UtilityFunction1568825275605::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1568825275605) ENABLED START*/
    std::shared_ptr<UtilityFunction> defaultFunction = std::make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;
    /*PROTECTED REGION END*/
}
/*
 *
 * Transition:
 *   - Name: 1568825457853, ConditionString: MovingTask received, Comment : MISSING_COMMENT
 *
 * Plans in State:
 *
 *   - Plan - (Name): Stop, (PlanID): 1555602210283
 *
 * Tasks:
 *
 *   - Serve (1555601344076) (Entrypoint: 1568825285315)
 *
 * States:
 *
 *   - WaitForTask (1568825288640)
 *   - MoveToPosition (1568825309813)
 *
 * Vars:
 */
bool PreCondition1568825457853::evaluate(shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1568825336792) ENABLED START*/
    // TODO extend speechAct to contain different kind of commands
    return this->wm->dialogueManager.commandHandler->getCommandActBuffer().getLastValidContent()->type == srg::SpeechType::command;
    /*PROTECTED REGION END*/
}
/*
 *
 * Transition:
 *   - Name: 1568825476581, ConditionString: Movement successful!, Comment : MISSING_COMMENT
 *
 * Plans in State:
 *
 *   - Plan - (Name): Move, (PlanID): 1568825137528
 *
 * Tasks:
 *
 *   - Serve (1555601344076) (Entrypoint: 1568825285315)
 *
 * States:
 *
 *   - WaitForTask (1568825288640)
 *   - MoveToPosition (1568825309813)
 *
 * Vars:
 */
bool PreCondition1568825476581::evaluate(shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1568825392354) ENABLED START*/
    return rp->isAnyChildStatus(PlanStatus::Success);
    /*PROTECTED REGION END*/
}
} // namespace alicaAutogenerated
