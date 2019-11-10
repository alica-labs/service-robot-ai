#include "Serve1568825275605.h"
/*PROTECTED REGION ID(eph1568825275605) ENABLED START*/
#include <srg/dialogue/TaskHandler.h>
/*PROTECTED REGION END*/

using namespace alica;

namespace alicaAutogenerated
{
    //Plan:Serve
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
*   - Name: 1568825457853, ConditionString: MovingTask received!, Comment : MISSING_COMMENT
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
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1568825457853::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1568825336792) ENABLED START*/
                auto activeCommand = this->wm->dialogueManager.taskHandler->getActiveTask();
                return activeCommand && activeCommand->getInformation()->type == srgsim::TaskType::Move;
    /*PROTECTED REGION END*/
}
/*
*
* Transition:
*   - Name: 1571661980674, ConditionString: Manipulate command!, Comment : MISSING_COMMENT
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
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1571661980674::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1571661739802) ENABLED START*/
                auto activeCommand = this->wm->dialogueManager.taskHandler->getActiveTask();
                return activeCommand && (activeCommand->getInformation()->type == srgsim::TaskType::Open
                || activeCommand->getInformation()->type == srgsim::TaskType::Close
                || activeCommand->getInformation()->type == srgsim::TaskType::PutDown
                || activeCommand->getInformation()->type == srgsim::TaskType::PickUp);
    /*PROTECTED REGION END*/
}
/*
*
* Transition:
*   - Name: 1573418732991, ConditionString: Received Catch Task, Comment : MISSING_COMMENT
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
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1573418732991::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1573418725423) ENABLED START*/
            return implement_me_1573418725423;
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
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1568825476581::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1568825392354) ENABLED START*/
            //    for (auto child : rp->getChildren()) {
            //        std::cout << "PreCondition1568825476581::evaluate(): " << *child << std::endl;
            //    }
                return rp->isAnyChildStatus(PlanStatus::Success);
    /*PROTECTED REGION END*/
}
/*
*
* Transition:
*   - Name: 1571661864299, ConditionString: Manipulation successful!, Comment : MISSING_COMMENT
*
* Plans in State: 
*     
*   - Plan - (Name): Manipulate, (PlanID): 1571687572903 
*
* Tasks: 
*     
*   - Serve (1555601344076) (Entrypoint: 1568825285315)
*
* States: 
*     
*   - WaitForTask (1568825288640)
*   - MoveToPosition (1568825309813)
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1571661864299::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1571661809581) ENABLED START*/
                return rp->isAnyChildStatus(PlanStatus::Success);
    /*PROTECTED REGION END*/
}
/*
*
* Transition:
*   - Name: 1573418838905, ConditionString: Any children success, Comment : MISSING_COMMENT
*
* Plans in State: 
*     
*   - Plan - (Name): Transport, (PlanID): 1573418869596 
*
* Tasks: 
*     
*   - Serve (1555601344076) (Entrypoint: 1568825285315)
*
* States: 
*     
*   - WaitForTask (1568825288640)
*   - MoveToPosition (1568825309813)
*   - ManipulateObject (1571661663929)
*   - BringToPosition (1573418710533)
*
* Vars:
*/
bool PreCondition1573418838905::evaluate(shared_ptr<RunningPlan> rp)
 {
    /*PROTECTED REGION ID(1573418821209) ENABLED START*/
            return implement_me_1573418821209;
    /*PROTECTED REGION END*/
}
}
