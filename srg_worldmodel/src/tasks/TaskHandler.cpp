#include "srg/tasks/TaskHandler.h"
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
        , currentTaskSequence(nullptr)
        , taskFactory(new TaskFactory(wm))
{
    auto sc = essentials::SystemConfig::getInstance();
    this->taskValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.ValidityDuration", NULL));
    this->taskActBuffer = new supplementary::InfoBuffer<agent::SpeechAct>((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.BufferLength", NULL));
}

TaskHandler::~TaskHandler()
{
    delete taskFactory;
    delete taskActBuffer;
}

const supplementary::InfoBuffer<agent::SpeechAct>& TaskHandler::getTaskActBuffer()
{
    return *this->taskActBuffer;
}

std::shared_ptr<TaskSequence> TaskHandler::getActiveTaskSequence()
{
    return this->currentTaskSequence;
}

void TaskHandler::tick()
{
    this->updateCurrentTaskSequence();
    this->setNextTaskSequence();
}

void TaskHandler::updateCurrentTaskSequence()
{
    if (!this->currentTaskSequence) {
        return;
    }
    if (this->currentTaskSequence->isSuccessful()) {
        std::cout << "[TaskHandler] TaskSequence successful and therefore removed!" << std::endl;
        this->currentTaskSequence = nullptr;
        return;
    }
    Task* activeTask = this->currentTaskSequence->getActiveTask();
    if (activeTask->isCompletelySpecified()) {
        return;
    }

    // find last search task before active task
    int32_t taskIdx = this->currentTaskSequence->getActiveTaskIdx();
    Task* completionHelperTask = nullptr;
    while (taskIdx >= 0) {
        completionHelperTask = this->currentTaskSequence->getTask(taskIdx--);
        if (completionHelperTask->type == TaskType::Search || completionHelperTask->objectID) {
            break;
        }
    }
    if (completionHelperTask->type != TaskType::Search && !completionHelperTask->objectID) {
        std::cerr << "[TaskHandler] No task found, to complete active task: " << activeTask << std::endl;
        return;
    }

    const srg::world::Object* foundObject = nullptr;
    if (completionHelperTask->objectID) {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(completionHelperTask->objectID);
    } else {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(completionHelperTask->objectType);
    }
    if (!foundObject) {
        std::cerr << "[TaskHandler] No object of type " << completionHelperTask->objectType << ", although search task was successful!" << std::endl;
        return;
    }
    completionHelperTask->coordinate = foundObject->getCoordinate();
    completionHelperTask->objectID = foundObject->getID();
    completionHelperTask->objectType = foundObject->getType();

    // completely specify tasks with found object
    switch (activeTask->type) {
    case TaskType::Move:
    case TaskType::PickUp:
    case TaskType::PutDown:
        if (activeTask->coordinate.x < 0) {
            activeTask->coordinate = completionHelperTask->coordinate;
        }
        activeTask->objectID = completionHelperTask->objectID;
        activeTask->objectType = completionHelperTask->objectType;
        break;
    default:
        std::cout << "[TaskHandler] Incompletely specified task not handled: " << *activeTask << std::endl;
        break;
    }
}

void TaskHandler::setNextTaskSequence()
{
    if (this->currentTaskSequence) {
        return;
    }
    while (true) {
        auto taskSpeechAct = this->taskActBuffer->popLast();
        if (!taskSpeechAct) {
            // stop when no new task speech act is available
            break;
        }
        TaskSequence* rawCurrentTaskSequence = this->taskFactory->createTaskSequence(taskSpeechAct->getInformation());
        if (!rawCurrentTaskSequence) {
            return;
        }
        this->currentTaskSequence = std::shared_ptr<TaskSequence>(rawCurrentTaskSequence);

        if (this->currentTaskSequence && !this->currentTaskSequence->isSuccessful()) {
            // new task sequence is not finished yet - that is fine
            std::cout << "[TaskHandler] New task sequence set " << *currentTaskSequence << std::endl;
            break;
        } else {
            this->currentTaskSequence = nullptr;
        }
    }
}

void TaskHandler::processTaskAct(std::shared_ptr<supplementary::InformationElement<agent::SpeechAct>> taskAct)
{
    this->taskActBuffer->add(taskAct);
}

} // namespace tasks
} // namespace srg
