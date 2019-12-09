#pragma once

#include "srg/tasks/TaskType.h"

#include <srg/agent/containers/SpeechAct.h>
#include <srg/world/Coordinate.h>
#include <engine/AlicaClock.h>

namespace srg
{
class SRGWorldModel;
namespace tasks
{
class Task;
class TaskSequence;
class TaskFactory
{
public:
    TaskFactory(srg::SRGWorldModel* wm);

    TaskSequence* createTaskSequence(const agent::SpeechAct& speechAct, const alica::AlicaTime creationTime);

private:
    void setIDFields(const agent::SpeechAct& speechAct, Task* task);
    void setCoordinate(const std::string& coordToken, Task* task);
    void setObjectType(const std::string& objectTypeToken, Task* task);
    void setObjectID(const std::string& objectIDToken, Task* task);
    std::vector<std::string> split(const std::string& taskText);
    bool isValid(const srg::world::Coordinate& coord, TaskType type);

    srg::SRGWorldModel* wm;
};
} // namespace tasks
} // namespace srg