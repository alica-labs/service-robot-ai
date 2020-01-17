#pragma once

#include <iosfwd>

namespace srg
{
namespace tasks
{
enum class TaskType
{
    Move,
    PickUp,
    PutDown,
    Open,
    Close,
    Search
};
std::ostream& operator<<(std::ostream& os, const TaskType& taskType);
} // namespace tasks
} // namespace srg
