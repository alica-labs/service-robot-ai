#include "Transport1573418869596.h"
/*PROTECTED REGION ID(eph1573418869596) ENABLED START*/
#include <srg/World.h>
#include <srg/tasks/CommandHandler.h>
#include <srg/tasks/Task.h>
#include <srg/world/Cell.h>
#include <srg/world/Object.h>
/*PROTECTED REGION END*/

namespace alica
{
// Plan:Transport
/**
 * Task: Fetch  -> EntryPoint-ID: 1573418933317
 */
std::shared_ptr<UtilityFunction> UtilityFunction1573418869596::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1573418869596) ENABLED START*/
    std::shared_ptr<UtilityFunction> defaultFunction = std::make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1573419765536, ConditionString: Object found, now move to it!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Search (1573419059418)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1573419765536::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1573419007368) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx > 0;
    if (transitionHolds) {
        std::cout << "{Search} -> {MoveClose}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1573419810456, ConditionString: Object picked up, now bring it somewhere!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Manipulate (1571687572903)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1573419810456::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1573418998111) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx > 2;
    if (transitionHolds) {
        std::cout << "{PickUp} -> {MoveDestination}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1582991345020, ConditionString: PickUp is not executable, because its object vanished. Go back to Search!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Manipulate (1571687572903)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1582991345020::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1582991345018) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx < 2;
    if (transitionHolds) {
        std::cout << "{PickUp} -> {Search}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1573419828606, ConditionString: Object carried to destination, now put it down., Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Move (1568825137528)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1573419828606::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1573419000441) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx > 3;
    if (transitionHolds) {
        std::cout << "{MoveDestination} -> {PutDown}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1583008562201, ConditionString: , Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Manipulate (1571687572903)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1583008562201::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1583008562199) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    int activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx >= taskSequence->size() || activeTaskIdx == 1 || activeTaskIdx == 2;
    if (transitionHolds) {
        std::cout << "{PutDown} -> {PseudoSuccess}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1583070277040, ConditionString: , Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Manipulate (1571687572903)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1583070277040::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1583070277037) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    auto activeTask = taskSequence->getActiveTask();
    bool transitionHolds = activeTask && activeTask->type == srg::tasks::TaskType::Search && activeTaskIdx == 0;
    if (transitionHolds) {
        std::cout << "{PutDown} -> {Search}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1596916629821, ConditionString: , Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Manipulate (1571687572903)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1596916629821::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1596916629819) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTask = taskSequence->getActiveTask();
    int activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTask && activeTask->type == srg::tasks::TaskType::Move && activeTaskIdx == 3;
    if (transitionHolds) {
        std::cout << "{PutDown} -> {Move}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1573419800282, ConditionString: At the object, now pick it up!, Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Move (1568825137528)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1573419800282::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1573419009128) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    bool transitionHolds = activeTaskIdx > 1;
    if (transitionHolds) {
        std::cout << "{MoveClose} -> {PickUp}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
/**
 * Outgoing transition:
 *   - Name: 1582991323350, ConditionString: MoveClose does not work anymore, because its target object vanished! Go back to Search., Comment: MISSING_COMMENT
 *
 * Abstract plans in current state:
 *   - Move (1568825137528)
 *
 * Tasks in plan:
 *   - Fetch (1555601323994) (Entrypoint: 1573418933317)
 *
 * States in plan:
 *   - Search (1573418936395)
 *   - PickUp (1573418937725)
 *   - MoveDestination (1573418938883)
 *   - PutDown (1573418939737)
 *   - MoveClose (1573418941076)
 *   - PseudoSuccess (1583008553235)
 *
 * Variables of preconditon:
 */
bool PreCondition1582991323350::evaluate(std::shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1582991323347) ENABLED START*/
    auto taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (!taskSequence) {
        return true;
    }
    auto activeTaskIdx = taskSequence->getActiveTaskIdx();
    auto activeTask = taskSequence->getActiveTask();
    bool transitionHolds = activeTask && activeTask->type == srg::tasks::TaskType::Search && activeTaskIdx == 0;
    if (transitionHolds) {
        std::cout << "{MoveClose} -> {Search}" << std::endl;
    }
    return transitionHolds;
    /*PROTECTED REGION END*/
}
} // namespace alica
