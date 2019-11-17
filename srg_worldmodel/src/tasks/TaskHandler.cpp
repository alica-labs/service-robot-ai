#include "srg/tasks/TaskHandler.h"
#include "srg/tasks/Task.h"
#include "srg/tasks/TaskFactory.h"

#include <srg/world/Cell.h>
#include <srg/world/Coordinate.h>
#include <srg/world/Object.h>

#include <engine/AlicaEngine.h>

#include <SystemConfig.h>

namespace srg
{
namespace tasks
{
TaskHandler::TaskHandler(SRGWorldModel* wm)
        : wm(wm)
        , activeTask(nullptr)
        , taskFactory(new TaskFactory(wm))
{
    auto sc = essentials::SystemConfig::getInstance();
    this->taskValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.ValidityDuration", NULL));
    this->taskActBuffer = new supplementary::InfoBuffer<Task*>((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.BufferLength", NULL));
}

TaskHandler::~TaskHandler() {
    delete taskFactory;
    delete taskActBuffer;
}

const supplementary::InfoBuffer<Task*>& TaskHandler::getTaskActBuffer()
{
    return *this->taskActBuffer;
}

std::shared_ptr<const supplementary::InformationElement<Task*>> TaskHandler::getActiveTask() const
{
    return this->activeTask;
}

void TaskHandler::tick()
{
    if (this->activeTask && !this->activeTask->getInformation()->checkSuccess(wm)) {
        // active task still in progress
        return;
    }

    auto newTask = this->taskActBuffer->popLast();
    if (!newTask || newTask->getInformation()->checkSuccess(wm)) {
        // no new task that is not successful already
        this->activeTask = nullptr;
    } else {
        std::cout << "[TaskHandler::tick] New task set: " << *newTask->getInformation() << std::endl;
        this->activeTask = newTask;
    }
}

void TaskHandler::processTaskAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> taskAct)
{
    Task* task = this->taskFactory->createTask(taskAct->getInformation());
    if (!task) {
        return;
    }
    // dont ask, but I think that this fixes a memory leak ;-)
    auto taskInfo = std::shared_ptr<supplementary::InformationElement<Task*>>(
            new supplementary::InformationElement<Task*>(task, wm->getTime(), taskValidityDuration, 1.0),
            [task](supplementary::InformationElement<Task*>*) mutable { delete task; });

    if (!activeTask) {
        this->activeTask = taskInfo;
    } else {
        this->taskActBuffer->add(taskInfo);
    }
}

} // namespace tasks
} // namespace srg
