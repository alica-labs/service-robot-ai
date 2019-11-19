#include "srg/tasks/Task.h"

#include "srg/SRGWorldModel.h"

#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/RoomType.h>
#include <srg/world/ServiceRobot.h>

namespace srg
{
namespace tasks
{
Task::Task(srg::tasks::TaskType type)
        : coordinate(-1, -1)
        , type(type)
        , senderID(nullptr)
        , receiverID(nullptr)
        , actID(nullptr)
        , previousActID(nullptr)
        , objectID(nullptr)
        , objectType(world::ObjectType::Unknown)
        , successful(false)
{
}

Task::~Task()
{
}

bool Task::checkSuccess(SRGWorldModel* wm) const
{
    if (!this->isSuccessful() && isCompletelySpecified()) {
        switch (type) {
        case TaskType::Move:
            this->successful = this->checkMoveSuccess(wm);
        case TaskType::Search:
            this->successful = this->checkSearchSuccess(wm);
        case TaskType::Close:
            this->successful = this->checkManipulationSuccess(wm);
        case TaskType::Open:
            this->successful = this->checkManipulationSuccess(wm);
        case TaskType::PickUp:
            this->successful = this->checkManipulationSuccess(wm);
        case TaskType::PutDown:
            this->successful = this->checkManipulationSuccess(wm);
        default:
            this->successful = false;
        }
    }
    return isSuccessful();
}

bool Task::isSuccessful() const
{
    return this->successful;
}

bool Task::checkMoveSuccess(SRGWorldModel* wm) const
{
    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }
    srg::world::Coordinate diff = (this->coordinate - ownCoord.value()).abs();
    const srg::world::Cell* goalCell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    if ((goalCell->isBlocked() && diff.x < 2 && diff.y < 2) || (diff.x == 0 && diff.y == 0)) {
        std::cout << "[Task] Move-Task to goal " << this->coordinate << " successful!" << std::endl;
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
        return robot->isCarrying(this->objectID);
    case TaskType::PutDown:
        cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
        return cell->contains(this->objectID);
    default:
        std::cerr << "[Task] Unknown manipulation task encountered: " << this->type << std::endl;
        return false;
    }
}

bool Task::checkSearchSuccess(srg::SRGWorldModel* wm) const
{
    return wm->sRGSimData.getWorld()->getObject(this->objectType);
}

bool Task::isCompletelySpecified() const
{
    switch (type) {
    case TaskType::Move:
        return this->coordinate.x >= 0;
    case TaskType::PutDown:
        return this->coordinate.x >= 0 && this->objectID.get();
    case TaskType::Close:
    case TaskType::Open:
    case TaskType::PickUp:
        return this->objectID.get();
    case TaskType::Search:
        return this->objectType != srg::world::ObjectType::Unknown;
    default:
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const srg::tasks::Task& task)
{
    os << "[Task] " << task.type;
    os << " Object: " << task.objectType << "(" << task.objectID << ")";
    os << " Coordinate: " << task.coordinate;
    return os;
}
} // namespace tasks
} // namespace srg