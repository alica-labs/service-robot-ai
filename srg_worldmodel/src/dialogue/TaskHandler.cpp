#include "srg/dialogue/TaskHandler.h"
#include "srg/dialogue/ManipulationTask.h"
#include "srg/dialogue/MoveTask.h"
#include "srg/dialogue/Task.h"

#include <srgsim/world/World.h>
#include <srgsim/containers/Coordinate.h>

#include <engine/AlicaEngine.h>

#include <SystemConfig.h>

namespace srg
{
namespace dialogue
{
TaskHandler::TaskHandler(SRGWorldModel* wm)
        : wm(wm)
        , activeTask(nullptr)
{
    auto sc = essentials::SystemConfig::getInstance();
    this->taskValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.ValidityDuration", NULL));
    this->taskActBuffer = new supplementary::InfoBuffer<Task*>((*sc)["SRGWorldModel"]->get<int64_t>("Data.TaskAct.BufferLength", NULL));
}

void TaskHandler::processTaskAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> taskAct)
{
    Task* task = this->createTask(taskAct);
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
        std::cout << "TaskHandler::tick(): Active task in progress: " << *this->activeTask->getInformation() << std::endl;
        return;
    }

    auto newTask = this->taskActBuffer->popLast();
    if (!newTask || newTask->getInformation()->checkSuccess(wm)) {
        // no new task that is not successful already
        if (newTask) {
//            std::cout << "TaskHandler::tick(): New task already successful: " << *newTask->getInformation() << std::endl;
        } else {
//            std::cout << "TaskHandler::tick(): No new task! " << std::endl;
        }
        this->activeTask = nullptr;
    } else {
        std::cout << "TaskHandler::tick(): New task set: " << *newTask->getInformation() << std::endl;
        this->activeTask = newTask;
    }
}

// INTERNAL METHODS

/**
 * Creates a task instance from by parsing the text of the speech act.
 * @param taskAct
 * @return The created Task.
 */
Task* TaskHandler::createTask(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> taskAct)
{
    Task* task;
    if (taskAct->getInformation().text.find("move") != std::string::npos) {
        task = createMoveTask(taskAct->getInformation().text);
    } else if (taskAct->getInformation().text.find("put") != std::string::npos || taskAct->getInformation().text.find("pick") != std::string::npos ||
               taskAct->getInformation().text.find("close") != std::string::npos || taskAct->getInformation().text.find("open") != std::string::npos) {
        task = createManipulationTask(taskAct->getInformation().text);
    } else {
        std::cerr << "TaskHandler::createTask(): Unkown task type encountered  - '" << taskAct->getInformation().text << "'" << std::endl;
        return nullptr;
    }
    if (!task) {
        return nullptr;
    }

    task->senderID = taskAct->getInformation().senderID;
    task->receiverID = taskAct->getInformation().receiverID;
    task->actID = taskAct->getInformation().actID;
    task->previousActID = taskAct->getInformation().previousActID;
    return task;
}

MoveTask* TaskHandler::createMoveTask(std::string taskText)
{
    MoveTask* task = new MoveTask();
    size_t moveIdx = taskText.find("move");
    if (moveIdx == std::string::npos) {
        std::cerr << "TaskHandler::createMoveTask(): Task is no move task!" << std::endl;
        task->type = srgsim::TaskType::Idle;
        return task;
    }

    size_t commaIdx = taskText.find(",", moveIdx);
    if (commaIdx == std::string::npos) {
        std::cerr << "TaskHandler::createMoveTask(): Move task has no comma!" << std::endl;
        task->type = srgsim::TaskType::Idle;
        return task;
    }

    int xCoord = std::stoi(taskText.substr(moveIdx + 4, commaIdx - (moveIdx + 4)));
    int yCoord = std::stoi(taskText.substr(commaIdx + 1));
    srgsim::Coordinate coord = srgsim::Coordinate(xCoord, yCoord);
    if (!this->wm->sRGSimData.getWorld()->getCell(coord)) {
        std::cerr << "TaskHandler::createManipulationTask(): Coordinates outside of the world: " << coord << std::endl;
        return nullptr;
    }
    task->coordinate = coord;
    task->type = srgsim::TaskType::Move;

    return task;
}

ManipulationTask* TaskHandler::createManipulationTask(std::string taskText)
{
    std::string taskString = taskText.substr(0, taskText.find(" "));
    std::string objectIdString;
    std::string xCoordString = "-1";
    std::string yCoordString = "-1";

    size_t objectIdEnd = taskText.find(" ", taskString.length() + 1);
    if (objectIdEnd == std::string::npos) {
        objectIdString = taskText.substr(taskString.length() + 1);
    } else {
        objectIdString = taskText.substr(taskString.length() + 1, objectIdEnd - (taskString.length() + 1));
        xCoordString = taskText.substr(objectIdEnd + 1, taskText.find(","));
        yCoordString = taskText.substr(taskText.find(",") + 1);
    }
    std::cout << "TaskHandler::createManipulationTask(): Task: '" << taskString << "' ID: '" << objectIdString << "' xCoord: '" << xCoordString << "' yCoord: '"
              << yCoordString << "'" << std::endl;

    ManipulationTask* task = new ManipulationTask();
    srgsim::Coordinate coord = srgsim::Coordinate(std::stoi(xCoordString), std::stoi(yCoordString));
    if (taskString.compare("open") == 0) {
        task->type = srgsim::TaskType::Open;
    } else if (taskString.compare("close") == 0) {
        task->type = srgsim::TaskType::Close;
    } else if (taskString.compare("pick") == 0) {
        task->type = srgsim::TaskType::PickUp;
    } else if (taskString.compare("put") == 0) {
        task->type = srgsim::TaskType::PutDown;
        if (!this->wm->sRGSimData.getWorld()->getCell(coord)) {
            std::cerr << "TaskHandler::createManipulationTask(): Coordinates outside of the world: " << coord << std::endl;
            return nullptr;
        }
    } else {
        std::cerr << "TaskHandler::createManipulationTask(): Current task type is unknown: " << taskString << std::endl;
        task->type = srgsim::TaskType::Idle;
    }
    uint32_t idInt = std::stoi(objectIdString);
    task->objectID = this->wm->getEngine()->getId<uint32_t>(idInt);

    task->coordinate = coord;

    return task;
}

} // namespace dialogue
} // namespace srg
