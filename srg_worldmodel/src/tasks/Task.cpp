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
{
}

Task::~Task() {}

bool Task::checkAndUpdateSuccess(SRGWorldModel* wm)
{
    if (!isCompletelySpecified()) {
        return false;
    }

    switch (this->type) {
    case TaskType::Move:
        return this->checkMoveSuccess(wm);
    case TaskType::Search:
        return this->checkSearchSuccess(wm);
    case TaskType::Close:
    case TaskType::Open:
    case TaskType::PickUp:
    case TaskType::PutDown:
        return this->checkManipulationSuccess(wm);
    default:
        return false;
    }
}

bool Task::checkMoveSuccess(SRGWorldModel* wm) const
{
    if (wm->sRGSimData.checkMoveSuccess(this->coordinate)) {
        std::cout << "[Task] " << this->type << " successful: " << this->coordinate << std::endl;
        return true;
    } else {
        return false;
    }
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
        if (!success) {
            std::cout << "[PickUp-SuccessTaskCheck] " << *agent << std::endl;
        }
        break;
    case TaskType::PutDown:
        // note: A stronger condition with regard to the target coordinates cannot
        // be evaluated correctly in all cases, because the simulator could displace
        // the object before the object is recognised at the target coordinates.
        agent = wm->sRGSimData.getWorld()->getAgent(this->receiverID);
        success = !agent->contains(this->objectID);
        if (!success) {
            std::cout << "[PutDown-SuccessTaskCheck] " << *agent << std::endl;
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

bool Task::checkSearchSuccess(srg::SRGWorldModel* wm)
{
    auto object = wm->sRGSimData.getWorld()->getObject(this->objectType);
    if (!object || !object->canBePickedUp(wm->getOwnId())) {
        return false;
    }
    // remember details for knowledge propagation in task sequence
    this->objectID = object->getID();
    this->coordinate = object->getCoordinate();

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

void Task::addKnowledge(essentials::IdentifierConstPtr objectID, world::ObjectType objectType, world::Coordinate coordinate)
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

bool Task::isKnowledgeValid(SRGWorldModel* wm)
{
    if (this->type == TaskType::PutDown) {
        // PutDown cannot be invalid, because it can only fail, when the target position is invalid, which is fixed.
        return true;
    }

    if (this->type == TaskType::Search && !this->checkSearchSuccess(wm)) {
        // Otherwise Search would be reverted without having effect, while it is not successful, yet.
        return true;
    }

    std::shared_ptr<const world::Object> object = wm->sRGSimData.getWorld()->getObject(this->objectID);
    if (!object) {
        // object does not exist anymore
        std::cout << "[Task] REVERT-OBJECT-UNKNOWN: " << *this << std::endl;
        return false;
    }

    if (!object->canBePickedUp(wm->getOwnId()) || (this->type == TaskType::PickUp && !wm->sRGSimData.checkMoveSuccess(this->coordinate))) {
        // object cannot be picked by me anymore || I try to pick it up and am not in reach
        std::cout << "[Task] REVERT-NOPICK: " << *this << std::endl;
        return false;
    }

    if (!this->coordinateIsFixed && object->getCoordinate() != this->coordinate) {
        // object is not located at the original position anymore
        std::cout << "[Task] REVERT-OBJECT-MOVED: " << *this << std::endl;
        return false;
    }

    return true;
}

void Task::revertKnowledge()
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