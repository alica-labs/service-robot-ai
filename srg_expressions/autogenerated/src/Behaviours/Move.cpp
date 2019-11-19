#include "Behaviours/Move.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1568825137528) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/robot/Movement.h>
#include <srg/tasks/TaskHandler.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1568825137528) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Move::Move()
        : DomainBehaviour("Move")
{
    /*PROTECTED REGION ID(con1568825137528) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Move::~Move()
{
    /*PROTECTED REGION ID(dcon1568825137528) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Move::run(void* msg)
{
    /*PROTECTED REGION ID(run1568825137528) ENABLED START*/
    if (this->isSuccess()) {
        return;
    }

    if (this->activeTask && this->activeTask->checkSuccess(this->wm)) {
        this->setSuccess();
        return;
    }

    this->robot->move(activeTask->coordinate);
    /*PROTECTED REGION END*/
}
void Move::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1568825137528) ENABLED START*/
    this->taskSequence = this->wm->dialogueManager.taskHandler->getActiveTaskSequence();
    this->activeTask = this->taskSequence->getActiveTask();

    if (activeTask && activeTask->type != srg::tasks::TaskType::Move) {
        this->activeTask = nullptr;
        this->taskSequence = nullptr;
    }
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1568825137528) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
