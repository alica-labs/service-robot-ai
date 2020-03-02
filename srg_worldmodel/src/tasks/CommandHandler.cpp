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

    this->propagateKnowledge();

    if (this->currentTaskSequence->getActiveTask()->isCompletelySpecified()) {
        this->removeInvalidKnowledge();
    }
}

void CommandHandler::propagateKnowledge()
{
    auto activeTask = this->currentTaskSequence->getActiveTask();
    if (activeTask->type == TaskType::Search) {
        // nothing to do for search task
        return;
    }

    // the last task must have all information, because we always propagate them
    Task* taskWithInfos = this->currentTaskSequence->getTask(this->currentTaskSequence->getActiveTaskIdx()-1);

    // completely specify tasks with found object
    activeTask->addInformation(taskWithInfos->objectID, taskWithInfos->objectType, taskWithInfos->coordinate);

    /////////////////////////////////////////////////////////////////////////


    /*// find latest task, that specifies a specific object or is a Search task
    Task* taskWithInfos = nullptr;
    int32_t taskIdx = this->currentTaskSequence->getActiveTaskIdx();
    while (taskIdx >= 0) {
        taskWithInfos = this->currentTaskSequence->getTask(taskIdx--);
        if (taskWithInfos->type == TaskType::Search || taskWithInfos->objectID) {
            break;
        }
    }

    auto activeTask = this->currentTaskSequence->getActiveTask();
    if (taskWithInfos->type != TaskType::Search && !taskWithInfos->objectID) {
        std::cerr << "[CommandHandler] No task found, to complete active task: " << activeTask << std::endl;
        return;
    }

    std::shared_ptr<const srg::world::Object> foundObject = nullptr;
    if (taskWithInfos->objectID) {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(taskWithInfos->objectID);
    } else {
        foundObject = this->wm->sRGSimData.getWorld()->getObject(taskWithInfos->objectType);
    }
    if (!foundObject || !foundObject->canBePickedUp(this->wm->getOwnId())) {
        // Search task was not successful, yet!
        return;
    }

    // completely specify tasks with found object
    activeTask->addInformation(foundObject->getID(), foundObject->getType(), foundObject->getCoordinate());
     */
}

void CommandHandler::removeInvalidKnowledge()
{
    int32_t taskIdx = this->currentTaskSequence->getActiveTaskIdx();
    Task* task = this->currentTaskSequence->getTask(taskIdx);
    std::shared_ptr<const world::Object> object = this->wm->sRGSimData.getWorld()->getObject(task->objectID);
    if ((object && object->canBePickedUp(this->wm->getOwnId())) || task->type == TaskType::PutDown) {
        // everything is fine
        return;
    }

    // Otherwise Search would be reverted without having effect, while it
    // is not successful, yet.
    if (task->type == TaskType::Search && !task->isSuccessful()) {
        return;
    }

    // revert the whole sequence
    while (taskIdx >= 0) {
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
