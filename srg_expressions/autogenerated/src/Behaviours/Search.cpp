#include "Behaviours/Search.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1573419059418) ENABLED START*/
#include <srg/Robot.h>
#include <srg/SRGWorldModel.h>
#include <srg/robot/ObjectSearch.h>
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
    if (this->isSuccess()) {
        return;
    }

    if (this->activeTask && this->activeTask->checkSuccess(this->wm)) {
        this->setSuccess();
        return;
    }

    this->search->update();
    const srg::world::Cell* cell = this->search->getNextCell();
    if (cell) {
        this->robot->move(cell->coordinate);
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
    activeTask = nullptr;
    task = nullptr;

    // init
    task = this->wm->dialogueManager.taskHandler->getActiveTask();
    int32_t progress = task->getInformation()->getProgress(this->wm);
    while (progress > 0) {
        activeTask = task->getInformation()->nextTask;
        progress--;
    }

    if (activeTask && activeTask->type == srg::tasks::TaskType::Search) {
        search = new srg::robot::ObjectSearch(activeTask->objectType, this->wm);
    } else {
        this->activeTask = nullptr;
        this->task = nullptr;
    }
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1573419059418) ENABLED START*/
/*PROTECTED REGION END*/

} /* namespace alica */
