#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/tasks/TaskType.h>
#include <srg/world/Coordinate.h>
#include <srg/world/ObjectType.h>
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
    bool checkSuccess(SRGWorldModel* wm);
    bool checkMoveSuccess(SRGWorldModel* wm) const;
    bool checkManipulationSuccess(SRGWorldModel* wm) const;
    bool checkSearchSuccess(srg::SRGWorldModel* wm);
    bool isCompletelySpecified() const;
    bool isKnowledgeValid(SRGWorldModel* wm) const;
    void revertKnowledge();
    void addKnowledge(essentials::IdentifierConstPtr objectID, world::ObjectType objectType, world::Coordinate coordinate);
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
    bool objectIDIsFixed;
    bool coordinateIsFixed;
    bool objectTypeIsFixed;
    mutable bool successful;
};
} // namespace tasks
} // namespace srg
