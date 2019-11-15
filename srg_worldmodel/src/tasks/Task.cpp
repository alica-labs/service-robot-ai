#include "srg/tasks/Task.h"

#include "srg/SRGWorldModel.h"

#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/ServiceRobot.h>
#include <srg/world/RoomType.h>

namespace srg
{
namespace tasks
{
Task::Task(srg::tasks::TaskType type)
        : coordinate(-1, -1)
        , type(type)
        , nextTask(nullptr)
        , senderID(nullptr)
        , receiverID(nullptr)
        , actID(nullptr)
        , previousActID(nullptr)
        , objectID(nullptr)
        , objectType(world::ObjectType::Unknown)
        , specifiedCompletely(false)
{
}

bool Task::checkSuccess(SRGWorldModel* wm) const
{
    switch (type) {
    case TaskType::Move:
        return this->checkMoveSuccess(wm);
    default:
        return false;
    }
}

bool Task::checkMoveSuccess(SRGWorldModel* wm) const
{
    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }

    bool goalIsBlocked = false;
    const srg::world::Cell* goalCell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    if (goalCell->getType() == srg::world::RoomType::Wall) {
        goalIsBlocked = true;
    }

    if (!goalIsBlocked) {
        for (auto object : goalCell->getObjects()) {
            if (object->getType() == srg::world::ObjectType::Door) {
                goalIsBlocked = true;
                break;
            }
        }
    }

    srg::world::Coordinate diff = this->coordinate - ownCoord.value();
    if ((goalIsBlocked && abs(diff.x) < 2 && abs(diff.y) < 2) || (diff.x == 0 && diff.y == 0)) {
        std::cout << "MoveTask::checkSuccess(): SUCCESS! Goal " << this->coordinate << " OwnPos " << ownCoord.value() << std::endl;
        return true;
    }
    return false;
}

bool Task::checkManipulationSuccess(SRGWorldModel* wm) const
{
    const srg::world::Object* object = nullptr;
    const srg::world::ServiceRobot* robot = nullptr;
    const srg::world::Cell* cell = nullptr;
    switch (this->type) {
    case TaskType::Open:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srg::world::ObjectState::Open;
    case TaskType::Close:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srg::world::ObjectState::Closed;
    case TaskType::PickUp:
        robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
        if (robot) {
            std::cout << "ManipulationTask::checkSuccess(): " << *robot << std::endl;
        }
        return robot->isCarrying(this->objectID);
    case TaskType::PutDown:
        cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
        for (const srg::world::Object* object : cell->getObjects()) {
            if (object->getID() == this->objectID) {
                return true;
            }
        }
        return false;
    default:
        std::cerr << "ManipulationTask::checkSuccess(): Unknown manipulation task encountered: " << this->type << std::endl;
        return false;
    }
}

bool Task::checkSearchSuccess(srg::SRGWorldModel* wm) const
{
    return wm->sRGSimData.getWorld()->getObject(this->objectType);
}

std::ostream& operator<<(std::ostream& os, const srg::tasks::Task& obj)
{
    os << "[Task] " << obj.type;
    return os;
}
} // namespace tasks
} // namespace srg