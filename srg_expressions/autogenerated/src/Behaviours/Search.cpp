#include "Behaviours/Search.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1573419059418) ENABLED START*/
#include <srg/SRGWorldModel.h>
#include <srg/dialogue/TaskHandler.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1573419059418) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

Search::Search()
        : DomainBehaviour("Search")
{
    /*PROTECTED REGION ID(con1573419059418) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
Search::~Search()
{
    /*PROTECTED REGION ID(dcon1573419059418) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void Search::run(void* msg)
{
    /*PROTECTED REGION ID(run1573419059418) ENABLED START*/
    std::cout << "[Search::run] Called! Task is " << *this->activeTask << std::endl;
    /*PROTECTED REGION END*/
}
void Search::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1573419059418) ENABLED START*/
    std::shared_ptr<const supplementary::InformationElement<srg::dialogue::Task*>> task = this->wm->dialogueManager.taskHandler->getActiveTask();
    if (task && task->getInformation()->type == srgsim::TaskType::Transport) {
        if (this->activeTask) {
            delete this->activeTask;
        }
        this->activeTask = new srg::dialogue::TransportTask();
        this->activeTask->type = task->getInformation()->type;
        this->activeTask->coordinate = task->getInformation()->coordinate;
        this->activeTask->actID = task->getInformation()->actID;
        this->activeTask->previousActID = task->getInformation()->previousActID;
        this->activeTask->senderID = task->getInformation()->senderID;
        this->activeTask->receiverID = task->getInformation()->receiverID;
        this->activeTask->objectID = static_cast<srg::dialogue::TransportTask*>(task->getInformation())->objectID;
        this->activeTask->objectType = static_cast<srg::dialogue::TransportTask*>(task->getInformation())->objectType;
    } else {
        this->activeTask = nullptr;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1573419059418) ENABLED START*/
// Add additional options here
/*PROTECTED REGION END*/

} /* namespace alica */
