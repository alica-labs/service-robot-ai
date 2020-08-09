#include "srg/tasks/CommandHandler.h"
#include "srg/tasks/TaskFactory.h"

#include <srg/world/Cell.h>
#include <srg/world/Coordinate.h>
#include <srg/world/Object.h>

#include <engine/AlicaEngine.h>

#include <essentials/SystemConfig.h>

namespace srg
{
namespace tasks
{
CommandHandler::CommandHandler(SRGWorldModel* wm)
        : wm(wm)
        , currentTaskSequence(nullptr)
        , taskFactory(new TaskFactory(wm))
        , fileWriter()
{
    auto& sc = essentials::SystemConfig::getInstance();
    this->taskValidityDuration = alica::AlicaTime::nanoseconds(sc["SRGWorldModel"]->get<int64_t>("Data.TaskAct.ValidityDuration", NULL));
    this->taskActBuffer = new supplementary::InfoBuffer<agent::SpeechAct>(sc["SRGWorldModel"]->get<int64_t>("Data.TaskAct.BufferLength", NULL));

    // init logging for experiments
    this->logPath = "results";
    if (!essentials::FileSystem::isDirectory(logPath)) {
        if (!essentials::FileSystem::createDirectory(logPath, 0777)) {
            std::cerr << "[CommandHandler] Cannot create log folder: " << logPath << std::endl;
        }
    }
    std::stringstream sb;
    struct tm timestruct;
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sb << logPath << "/" << std::put_time(localtime_r(&time, &timestruct), "%Y-%Om-%Od_%OH-%OM-%OS") << "_TaskLog_" << sc.getHostname() << ".csv";
    this->fileWriter.open(sb.str().c_str(), std::ios_base::app);
}

CommandHandler::~CommandHandler()
{
    delete taskFactory;
    delete taskActBuffer;
    this->fileWriter.close();
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
    if (!task->isKnowledgeValid(this->wm)) {
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
        return;
    }

    // revert from putdown to move, if you are not at a suitable putdown position (happens due to simulator delay)
    if (task->type == TaskType::PutDown && !this->wm->sRGSimData.checkMoveSuccess(task->coordinate)) {
        Task* moveTask = this->currentTaskSequence->getTask(this->currentTaskSequence->getActiveTaskIdx() - 1);
        if (moveTask->type == TaskType::Move) {
            moveTask->successful = false;
            this->currentTaskSequence->setActiveTaskIdx(this->currentTaskSequence->getActiveTaskIdx() - 1);
        }
    }
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
        TaskSequence* rawCurrentTaskSequence = this->taskFactory->createTaskSequence(taskSpeechAct->getInformation(), this->wm->getTime());
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
    this->fileWriter << std::fixed << taskSequence->toLogString(this->wm->getOwnId());
    this->fileWriter.flush();
}

} // namespace tasks
} // namespace srg
