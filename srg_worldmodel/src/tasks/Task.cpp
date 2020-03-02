#include "srg/tasks/Task.h"

#include "srg/SRGWorldModel.h"

#include <srg/world/Agent.h>
#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/RoomType.h>

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
        , objectIDIsFixed(false)
        , objectTypeIsFixed(false)
        , coordinateIsFixed(false)
        , successful(false)
{
}

Task::~Task() {}

bool Task::checkSuccess(SRGWorldModel* wm) const
{
    if (!this->isSuccessful() && isCompletelySpecified()) {
        switch (this->type) {
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
    std::shared_ptr<const world::Cell> goalCell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    if ((goalCell->isBlocked() && diff.x < 2 && diff.y < 2) || (diff.x == 0 && diff.y == 0)) {
        std::cout << "[Task] " << this->type << " successful: " << this->coordinate << std::endl;
        return true;
    }
    return false;
}

bool Task::checkManipulationSuccess(SRGWorldModel* wm) const
{
    std::shared_ptr<const srg::world::Object> object = nullptr;
    std::shared_ptr<const srg::world::Agent> agent = nullptr;
    std::shared_ptr<const world::Cell> cell = nullptr;
    bool success;
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
        agent = wm->sRGSimData.getWorld()->getAgent(this->receiverID);
        success = agent->contains(this->objectID);
        break;
    case TaskType::PutDown:
        cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
        if (cell->contains(this->objectID)) {
            success = true;
        } else {
            auto carryingAgent = std::dynamic_pointer_cast<const srg::world::Agent>(wm->sRGSimData.getWorld()->getObject(this->objectID)->getParentContainer());
            success = carryingAgent && carryingAgent->getID() != this->receiverID;
        }
        break;
    default:
        std::cerr << "[Task] Unknown manipulation task encountered: " << this->type << std::endl;
        success = false;
    }
    if (success) {
        std::cout << "[Task] " << this->type << " successful: " << this->objectType << "(ID: " << this->objectID << ")" << std::endl;
    }
    return success;
}

bool Task::checkSearchSuccess(srg::SRGWorldModel* wm) const
{
    auto object = wm->sRGSimData.getWorld()->getObject(this->objectType);
    if (!object || !object->canBePickedUp(wm->getOwnId())) {
        return false;
    }

    std::cout << "[Task] " << this->type << " successful: " << this->objectType << "(ID: " << this->objectID << ")" << std::endl;
    return true;
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

void Task::addInformation(essentials::IdentifierConstPtr objectID, world::ObjectType objectType, world::Coordinate coordinate)
{
    if (!objectIDIsFixed) {
        this->objectID = objectID;
    }
    if (!objectTypeIsFixed) {
        this->objectType = objectType;
    }
    if (!coordinateIsFixed) {
        this->coordinate = coordinate;
    }
}

void Task::revertProgress()
{
    std::cout << "[Task] -----> Revert " << *this << std::endl;
    if (!objectIDIsFixed) {
        this->objectID = nullptr;
    }
    if (!objectTypeIsFixed) {
        this->objectType = world::ObjectType::Unknown;
    }
    if (!coordinateIsFixed) {
        this->coordinate = world::Coordinate(-1, -1);
    }
    this->successful = false;
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