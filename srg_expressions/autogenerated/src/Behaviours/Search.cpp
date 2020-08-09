#include "Behaviours/Search.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1573419059418) ENABLED START*/
#include <srg/Agent.h>
#include <srg/SRGWorldModel.h>
#include <srg/agent/ObjectSearch.h>
#include <srg/tasks/CommandHandler.h>
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
    search = new srg::agent::ObjectSearch(this->wm);
    /*PROTECTED REGION END*/
}
Search::~Search()
{
    /*PROTECTED REGION ID(dcon1573419059418) ENABLED START*/
    delete search;
    /*PROTECTED REGION END*/
}
void Search::run(void* msg)
{
    /*PROTECTED REGION ID(run1573419059418) ENABLED START*/
    if (!this->activeTask) {
        std::cout << "[Search] No task!" << std::endl;
        return;
    }

    if (this->activeTask->checkSuccess(this->wm)) {
        std::cout << "[Search] Task success!" << std::endl;
        this->setSuccess();
        return;
    }

    this->search->update();
    std::shared_ptr<const srg::world::Cell> cell = this->search->getNextCell();
    if (cell) {
        this->agent->move(cell->coordinate);
    } else {
        std::cout << "[Search] No cell received!" << std::endl;
        return;
    }
    /*PROTECTED REGION END*/
}
void Search::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1573419059418) ENABLED START*/
    // init
    this->taskSequence = this->wm->dialogueManager.commandHandler->getActiveTaskSequence();
    if (this->taskSequence) {
        this->activeTask = this->taskSequence->getActiveTask();
        std::cout << "[Search] " << *this->activeTask << std::endl;
    }

    if (activeTask && activeTask->type == srg::tasks::TaskType::Search) {
        this->search->init(activeTask->objectType);
    } else {
        this->search->reset();
        this->activeTask = nullptr;
        this->taskSequence = nullptr;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1573419059418) ENABLED START*/
/*PROTECTED REGION END*/

} /* namespace alica */
