#include "Behaviours/Manipulate.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1571687572903) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/tasks/TaskHandler.h>
#include <srg/robot/Movement.h>
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

    if (this->activeTask->checkSuccess(this->wm) ||
            (this->activeTask->type != srg::tasks::TaskType::Open && this->activeTask->type != srg::tasks::TaskType::Close &&
                    this->activeTask->type != srg::tasks::TaskType::PickUp && this->activeTask->type != srg::tasks::TaskType::PutDown)) {
        this->setSuccess();
        return;
    }

    std::cout << "Manipulate::run(void* msg): Call to manipulate environment!" << std::endl;
    this->robot->manipulate(activeTask);
    /*PROTECTED REGION END*/
}
void Manipulate::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1571687572903) ENABLED START*/
    std::shared_ptr<const supplementary::InformationElement<srg::tasks::Task*>> task = this->wm->dialogueManager.taskHandler->getActiveTask();
    if (task && (task->getInformation()->type == srg::tasks::TaskType::PickUp || task->getInformation()->type == srg::tasks::TaskType::Close ||
                        task->getInformation()->type == srg::tasks::TaskType::Open || task->getInformation()->type == srg::tasks::TaskType::PutDown)) {
        if (this->activeTask) {
            delete this->activeTask;
        }
        this->activeTask = new srg::tasks::Task(task->getInformation()->type);
        this->activeTask->coordinate = task->getInformation()->coordinate;
        this->activeTask->actID = task->getInformation()->actID;
        this->activeTask->previousActID = task->getInformation()->previousActID;
        this->activeTask->senderID = task->getInformation()->senderID;
        this->activeTask->receiverID = task->getInformation()->receiverID;
        this->activeTask->objectID = static_cast<srg::tasks::Task*>(task->getInformation())->objectID;
    } else {
        this->activeTask = nullptr;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1571687572903) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
