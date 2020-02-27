#include "srg/tasks/CommandHandler.h"
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
CommandHandler::CommandHandler(SRGWorldModel* wm)
        : wm(wm)
        , currentTaskSequence(nullptr)
        , taskFactory(new TaskFactory(wm))
{
    auto sc = essentials::SystemConfig::getInstance();
    this->taskValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.ValidityDuration", NULL));
    this->taskActBuffer = new supplementary::InfoBuffer<agent::SpeechAct>((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.BufferLength", NULL));
}

CommandHandler::~CommandHandler()
{
    delete taskFactory;
    delete taskActBuffer;
}

const supplementary::InfoBuffer<agent::SpeechAct>& CommandHandler::getTaskActBuffer()
{
    return *this->taskActBuffer;
}

std::shared_ptr<TaskSequence> CommandHandler::getActiveTaskSequence()
{
    return this->currentTaskSequence;
}

void CommandHandler::tick()
{
    this->updateCurrentTaskSequence();
    this->setNextTaskSequence();
}

void CommandHandler::updateCurrentTaskSequence()
{
    if (!this->currentTaskSequence) {
        return;
    }
    if (this->currentTaskSequence->isSuccessful()) {
        std::cout << "[CommandHandler] TaskSequence successful and therefore removed!" << std::endl;
        this->logTaskSequence(this->currentTaskSequence);
        this->currentTaskSequence = nullptr;
        return;
    }
    Task* activeTask = this->currentTaskSequence->getActiveTask();
    if (activeTask->isCompletelySpecified()) {
        this->removeInvalidKnowledge(this->currentTaskSequence);
        return;
    }

    // search tasks are always completely specified, so the active task must not be a search task
    assert(activeTask->type != TaskType::Search);

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
        std::cerr << "[CommandHandler] No task found, to complete active task: " << activeTask << std::endl;
        return;
    }

    std::shared_ptr<const srg::world::Object> foundObject = nullptr;
    if (completionHelperTask->objectID) {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(completionHelperTask->objectID);
    } else {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(completionHelperTask->objectType);
    }
    if (!foundObject) {
        std::cerr << "[CommandHandler] No object of type " << completionHelperTask->objectType << ", although search task was successful!" << std::endl;
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
        std::cout << "[CommandHandler] Incompletely specified task not handled: " << *activeTask << std::endl;
        break;
    }
}

void CommandHandler::removeInvalidKnowledge(std::shared_ptr<TaskSequence> taskSequence)
{
    assert(!taskSequence->isSuccessful());
    assert(taskSequence->getActiveTask()->isCompletelySpecified());

    // loop backwards over the task sequence
    int32_t taskIdx = taskSequence->getActiveTaskIdx();
    while (taskIdx >= 0) {
        Task* task = taskSequence->getTask(taskIdx);
        if (task->type == TaskType::Search) {
            // a search task does not contain knowledge that could render to be invalid
            break;
        }

        if (!task->objectID) {
            // no knowledge to invalidate
            break;
        }

        std::shared_ptr<const world::Object> object = this->wm->sRGSimData.getWorld()->getObject(task->objectID);
        if (object && object->getCoordinate().x >= 0) {
            // everything is fine for this task
            break;
        }

        if (object && object->getCoordinate().x < 0) {
            // object still exists, but has not coordinates so remove it from the world
            this->wm->sRGSimData.getWorld()->removeObjectIfUnknown(task->objectID);
        }
        task->objectID = nullptr;
        task->objectType = world::ObjectType::Unknown;
        if (task->type != TaskType::PutDown) {
            task->coordinate = world::Coordinate(-1, -1);
        }
        task->successful = false;
        taskIdx--;
    }
    taskSequence->setActiveTaskIdx(taskIdx);
}

void CommandHandler::setNextTaskSequence()
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
        TaskSequence* rawCurrentTaskSequence = this->taskFactory->createTaskSequence(taskSpeechAct->getInformation(), taskSpeechAct->getCreationTime());
        if (!rawCurrentTaskSequence) {
            return;
        }
        this->currentTaskSequence = std::shared_ptr<TaskSequence>(rawCurrentTaskSequence);

        if (this->currentTaskSequence && !this->currentTaskSequence->isSuccessful()) {
            // new task sequence is not finished yet - that is fine
            std::cout << "[CommandHandler] New task sequence set " << *currentTaskSequence << std::endl;
            break;
        } else {
            this->currentTaskSequence = nullptr;
        }
    }
}

std::shared_ptr<agent::SpeechAct> CommandHandler::handle(std::shared_ptr<supplementary::InformationElement<agent::SpeechAct>> commandAct)
{
    this->taskActBuffer->add(commandAct);
    return nullptr; // todo: maybe send an acknowledge for receiving this task
}

void CommandHandler::logTaskSequence(std::shared_ptr<TaskSequence> taskSequence)
{
    taskSequence->setEndTime(this->wm->getTime());
    std::ofstream fileWriter;
    fileWriter.open(essentials::FileSystem::combinePaths("results", "TaskLog.csv"), std::ios_base::app);
    fileWriter << std::fixed << taskSequence->toLogString(this->wm->getOwnId()) << std::endl;
}

} // namespace tasks
} // namespace srg
