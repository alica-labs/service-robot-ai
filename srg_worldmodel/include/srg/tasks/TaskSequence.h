#pragma once

#include <vector>
#include <cstdint>
#include <iosfwd>

namespace srg
{
namespace tasks
{
class Task;
class TaskSequence
{
public:
    TaskSequence();
    virtual ~TaskSequence();

    void addTask(Task* task);
    Task* getTask(int32_t taskIdx);
    Task* getActiveTask();
    int32_t getActiveTaskIdx();
    int32_t size() const;
    bool isSuccessful();
    friend std::ostream& operator<<(std::ostream& os, const srg::tasks::TaskSequence& taskSequence);
private:
    int32_t activeTaskIdx;
    std::vector<Task*> taskSequence;
};
} // namespace tasks
} // namespace srg
