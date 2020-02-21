#include "srg/tasks/TaskSequence.h"

#include "srg/tasks/Task.h"

#include <iostream>
#include <sstream>

namespace srg
{
namespace tasks
{
TaskSequence::TaskSequence()
        : activeTaskIdx(0)
{
    taskSequence.reserve(6); /**< make some space, by default */
}

TaskSequence::~TaskSequence()
{
    for (Task* task : taskSequence) {
        delete task;
    }
}

void TaskSequence::setStartTime(alica::AlicaTime startTime) {
    this->startTime = startTime;
}

void TaskSequence::setEndTime(alica::AlicaTime endTime) {
    this->endTime = endTime;
}

bool TaskSequence::isSuccessful()
{
    while (this->activeTaskIdx < this->taskSequence.size() && this->taskSequence[this->activeTaskIdx]->isSuccessful()) {
        this->activeTaskIdx++;
    }
    return this->activeTaskIdx >= this->taskSequence.size();
}

Task* TaskSequence::getActiveTask()
{
    return this->taskSequence[this->activeTaskIdx];
}

int32_t TaskSequence::getActiveTaskIdx() {
    return this->activeTaskIdx;
}

void TaskSequence::setActiveTaskIdx(int32_t newActiveTaskIdx) {
    this->activeTaskIdx = newActiveTaskIdx;
}

Task* TaskSequence::getTask(int32_t taskIdx)
{
    return this->taskSequence[taskIdx];
}

void TaskSequence::addTask(srg::tasks::Task* task)
{
    this->taskSequence.push_back(task);
}

int32_t TaskSequence::size() const
{
    return this->taskSequence.size();
}

std::string TaskSequence::toLogString(essentials::IdentifierConstPtr agentID) {
    std::stringstream logStringStream;
    logStringStream << this->endTime-this->startTime << "\t" << this->size() << std::endl;
    for (Task* task : this->taskSequence) {
        logStringStream << *task << std::endl;
    }
    return logStringStream.str();
}

std::ostream& operator<<(std::ostream& os, const srg::tasks::TaskSequence& taskSequence)
{
    os << "[TaskSequence] Size: " << taskSequence.taskSequence.size() << "\t" << std::endl;
    for (Task* task : taskSequence.taskSequence) {
        os << *task << std::endl;
    }
    return os;
}

} // namespace tasks
} // namespace srg
