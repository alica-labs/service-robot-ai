#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/world/Coordinate.h>
#include <srg/world/ObjectType.h>
#include <srg/tasks/TaskType.h>
namespace srg
{
class SRGWorldModel;
namespace tasks
{
class Task
{
public:
    Task(srg::tasks::TaskType type);
    virtual ~Task();
    bool isSuccessful() const;
    bool checkSuccess(SRGWorldModel* wm) const;
    bool checkMoveSuccess(SRGWorldModel* wm) const;
    bool checkManipulationSuccess(SRGWorldModel* wm) const;
    bool checkSearchSuccess(srg::SRGWorldModel* wm) const;
    bool isCompletelySpecified() const;
    friend std::ostream& operator<<(std::ostream& os, const srg::tasks::Task& task);

    // Dialogue and Communication
    essentials::IdentifierConstPtr senderID;
    essentials::IdentifierConstPtr receiverID;
    essentials::IdentifierConstPtr actID;
    essentials::IdentifierConstPtr previousActID;

    // Description of the task
    srg::tasks::TaskType type;
    world::Coordinate coordinate;
    essentials::IdentifierConstPtr objectID;
    world::ObjectType objectType;
    mutable bool successful;
};
} // namespace dialogue
} // namespace srg
