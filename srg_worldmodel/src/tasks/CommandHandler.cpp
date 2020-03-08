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
    Task* taskWithInfos = this->currentTaskSequence->getTask(this->currentTaskSequence->getActiveTaskIdx() - 1);

    // completely specify tasks with found object
    activeTask->addKnowledge(taskWithInfos->objectID, taskWithInfos->objectType, taskWithInfos->coordinate);
}

void CommandHandler::removeInvalidKnowledge()
{
    Task* task = this->currentTaskSequence->getActiveTask();
    if (task->isKnowledgeValid(this->wm)) {
        return;
    }

    // revert the whole sequence
    int32_t taskIdx = this->currentTaskSequence->getActiveTaskIdx();
    while (true) {
        task->revertKnowledge();
        if (taskIdx > 0) {
            task = this->currentTaskSequence->getTask(--taskIdx);
        } else {
            break;
        }
    }
    this->currentTaskSequence->setActiveTaskIdx(0);
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
