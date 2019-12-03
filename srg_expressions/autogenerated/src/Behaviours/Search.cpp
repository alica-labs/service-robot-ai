#include "Behaviours/Search.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1573419059418) ENABLED START*/
#include <srg/Agent.h>
#include <srg/SRGWorldModel.h>
#include <srg/agent/ObjectSearch.h>
#include <srg/tasks/TaskHandler.h>
#include <srg/world/Cell.h>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1573419059418) ENABLED START*/
/*PROTECTED REGION END*/

Search::Search()
        : DomainBehaviour("Search")
{
    /*PROTECTED REGION ID(con1573419059418) ENABLED START*/
    activeTask = nullptr;
    search = nullptr;
    /*PROTECTED REGION END*/
}
Search::~Search()
{
    /*PROTECTED REGION ID(dcon1573419059418) ENABLED START*/
    /*PROTECTED REGION END*/
}
void Search::run(void* msg)
{
    /*PROTECTED REGION ID(run1573419059418) ENABLED START*/
    if (this->isSuccess() || !this->activeTask) {
        return;
    }

    if (this->activeTask->checkSuccess(this->wm)) {
        this->setSuccess();
        return;
    }

    this->search->update();
    const srg::world::Cell* cell = this->search->getNextCell();
    if (cell) {
        this->agent->move(cell->coordinate);
    } else {
        std::cout << "[Search] No cell received!" << std::endl;
    }
    /*PROTECTED REGION END*/
}
void Search::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1573419059418) ENABLED START*/
    // clean up
    delete search;

    // init
    this->taskSequence = this->wm->dialogueManager.taskHandler->getActiveTaskSequence();
    if (this->taskSequence) {
        this->activeTask = this->taskSequence->getActiveTask();
    }

    if (activeTask && activeTask->type == srg::tasks::TaskType::Search) {
        search = new srg::agent::ObjectSearch(activeTask->objectType, this->wm);
    } else {
        this->activeTask = nullptr;
        this->taskSequence = nullptr;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1573419059418) ENABLED START*/
/*PROTECTED REGION END*/

} /* namespace alica */
