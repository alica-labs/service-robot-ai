#include "srg/tasks/TaskSequence.h"

#include "srg/tasks/Task.h"

#include <iostream>

namespace srg
{
namespace tasks
{
TaskSequence::TaskSequence()
        : activeTaskIdx(0)
        , taskSequence(6) // make some space, by default
{
}

TaskSequence::~TaskSequence()
{
    for (Task* task : taskSequence) {
        delete task;
    }
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

std::ostream& operator<<(std::ostream& os, const srg::tasks::TaskSequence& taskSequence)
{
    os << "[TaskSequence] Size: " << taskSequence.taskSequence.size();
    for (Task* task : taskSequence.taskSequence) {
        os << task;
    }
    return os;
}

} // namespace tasks
} // namespace srg
