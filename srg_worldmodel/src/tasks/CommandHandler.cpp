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

//    if (!this->currentTaskSequence->getActiveTask()->isCompletelySpecified()) {
        this->propagateKnowledge();
//    }

    if (this->currentTaskSequence->getActiveTask()->isCompletelySpecified()) {
        this->removeInvalidKnowledge();
    }

//    std::cout << "[CommandHandler] Active Task is " << *this->currentTaskSequence->getActiveTask() << std::endl;
}

void CommandHandler::propagateKnowledge()
{
    // find last task before active task, that specifies a specific object or is a Search task
    // the task found is denoted as completionHelperTask
    auto activeTask = this->currentTaskSequence->getActiveTask();
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

void CommandHandler::removeInvalidKnowledge()
{
    // loop backwards over the task sequence
    int32_t taskIdx = this->currentTaskSequence->getActiveTaskIdx();
    while (taskIdx >= 0) {
        Task* task = this->currentTaskSequence->getTask(taskIdx);

        std::shared_ptr<const world::Object> object;

        // special handling for search task
        if (task->type == TaskType::Search) {
            object = this->wm->sRGSimData.getWorld()->getObject(task->objectID);
            if (!object && task->isSuccessful()) {
                task->revertProgress();
            }
            break;
        }

        // handling for all other tasks
        object = this->wm->sRGSimData.getWorld()->getObject(task->objectID);
        if ((object && object->getCoordinate().x >= 0) || task->coordinateIsFixed) {
            break;
        }

        task->revertProgress();
        taskIdx--;
    }
    this->currentTaskSequence->setActiveTaskIdx(taskIdx);
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
    std::string filename = "TaskLog_" + this->wm->getAgentName() + ".csv";
    fileWriter.open(essentials::FileSystem::combinePaths("results", filename), std::ios_base::app);
    fileWriter << std::fixed << taskSequence->toLogString(this->wm->getOwnId()) << std::endl;
}

} // namespace tasks
} // namespace srg
