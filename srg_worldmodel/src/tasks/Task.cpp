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

Task::~Task() {}

bool Task::checkSuccess(SRGWorldModel* wm) const
{
    if (!this->isSuccessful() && isCompletelySpecified()) {
        switch (type) {
        case TaskType::Move:
            this->successful = this->checkMoveSuccess(wm);
            break;
        case TaskType::Search:
            this->successful = this->checkSearchSuccess(wm);
            break;
        case TaskType::Close:
            this->successful = this->checkManipulationSuccess(wm);
            break;
        case TaskType::Open:
            this->successful = this->checkManipulationSuccess(wm);
            break;
        case TaskType::PickUp:
            this->successful = this->checkManipulationSuccess(wm);
            break;
        case TaskType::PutDown:
            this->successful = this->checkManipulationSuccess(wm);
            break;
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
        std::cout << "[Task] Move to " << this->coordinate << " successful!" << std::endl;
        return true;
    }
    return false;
}

bool Task::checkManipulationSuccess(SRGWorldModel* wm) const
{
    const srg::world::Object* object = nullptr;
    const srg::world::ServiceRobot* robot = nullptr;
    const srg::world::Cell* cell = nullptr;
    bool success = false;
    switch (this->type) {
    case TaskType::Open:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        success = object && object->getState() == srg::world::ObjectState::Open;
        break;
    case TaskType::Close:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        success = object && object->getState() == srg::world::ObjectState::Closed;
        break;
    case TaskType::PickUp:
        robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
        success = robot->isCarrying(this->objectID);
        break;
    case TaskType::PutDown:
        cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
        success = cell->contains(this->objectID);
        break;
    default:
        std::cerr << "[Task] Unknown manipulation task encountered: " << this->type << std::endl;
        success = false;
    }
    if (success) {
        std::cout << "[Task] " << this->type << " of " << this->objectType << "(ID: "<< this->objectID << ") to  successful!" << std::endl;
    }
    return success;
}

bool Task::checkSearchSuccess(srg::SRGWorldModel* wm) const
{
    if (wm->sRGSimData.getWorld()->getObject(this->objectType)) {
        std::cout << "[Task] Search for " << this->objectType << " successful!" << std::endl;
        return true;
    }
    return false;
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