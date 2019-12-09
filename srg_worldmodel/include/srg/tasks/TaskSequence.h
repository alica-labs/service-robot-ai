#pragma once

#include <vector>
#include <cstdint>
#include <iosfwd>
#include <engine/AlicaClock.h>

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
    void setStartTime(alica::AlicaTime startTime);
    void setEndTime(alica::AlicaTime endTime);
    std::string toLogString();
    friend std::ostream& operator<<(std::ostream& os, const srg::tasks::TaskSequence& taskSequence);
private:
    alica::AlicaTime startTime;
    alica::AlicaTime endTime;
    int32_t activeTaskIdx;
    std::vector<Task*> taskSequence;
};
} // namespace tasks
} // namespace srg
