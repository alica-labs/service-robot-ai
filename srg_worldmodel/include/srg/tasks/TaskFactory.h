#pragma once

#include "srg/tasks/TaskType.h"

#include <control/containers/SpeechAct.h>
#include <srg/world/Coordinate.h>

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

    TaskSequence* createTaskSequence(const control::SpeechAct& speechAct);

private:
    void setIDFields(const control::SpeechAct& speechAct, Task* task);
    void setCoordinate(const std::string& coordToken, Task* task);
    void setObjectType(const std::string& objecTypeToken, Task* task);
    void setObjectID(const std::string& objectIDToken, Task* task);
    std::vector<std::string> split(const std::string& taskText);
    bool isValid(const srg::world::Coordinate& coord, TaskType type);

    srg::SRGWorldModel* wm;
};
} // namespace tasks
} // namespace srg