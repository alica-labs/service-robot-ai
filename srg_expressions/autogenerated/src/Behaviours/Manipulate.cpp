#include "Behaviours/Manipulate.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1571687572903) ENABLED START*/
#include <srg/Agent.h>
#include <srg/SRGWorldModel.h>
#include <srg/agent/Movement.h>
#include <srg/tasks/TaskHandler.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1571687572903) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Manipulate::Manipulate()
        : DomainBehaviour("Manipulate")
{
    /*PROTECTED REGION ID(con1571687572903) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Manipulate::~Manipulate()
{
    /*PROTECTED REGION ID(dcon1571687572903) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Manipulate::run(void* msg)
{
    /*PROTECTED REGION ID(run1571687572903) ENABLED START*/
    if (this->isSuccess() || !this->activeTask) {
        return;
    }

    if (this->activeTask->checkSuccess(this->wm)) {
        this->setSuccess();
        return;
    }

    if (!this->activeTask->isCompletelySpecified()) {
        return;
    }
    this->agent->manipulate(activeTask);
    /*PROTECTED REGION END*/
}
void Manipulate::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1571687572903) ENABLED START*/
    this->taskSequence = this->wm->dialogueManager.taskHandler->getActiveTaskSequence();
    if (this->taskSequence) {
        this->activeTask = this->taskSequence->getActiveTask();
    }

    if (activeTask && activeTask->type != srg::tasks::TaskType::PickUp && activeTask->type != srg::tasks::TaskType::Close &&
            activeTask->type != srg::tasks::TaskType::Open && activeTask->type != srg::tasks::TaskType::PutDown) {
        this->activeTask = nullptr;
        this->taskSequence = nullptr;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1571687572903) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
