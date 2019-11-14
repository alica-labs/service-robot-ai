#include "srg/dialogue/TransportTask.h"

#include "srg/SRGWorldModel.h"

#include <srgsim/world/Cell.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/ServiceRobot.h>

namespace srg
{
namespace dialogue
{
bool TransportTask::checkSuccess(SRGWorldModel* wm) const
{
    const srgsim::Cell* cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    for (const srgsim::Object* object : cell->getObjects()) {
        if (object->getType() == this->objectType) {
            return true;
        }
    }
    return false;
}

bool TransportTask::foundObject(srg::SRGWorldModel* wm)
{
    if (this->objectID) {
        return true;
    }

    const srgsim::Object* object = wm->sRGSimData.getWorld()->getObject(this->objectType);
    if (object) {
        this->objectID = object->getID();
        return true;
    }
    return false;
}

bool TransportTask::closeToObject(srg::SRGWorldModel* wm)
{
    if (!foundObject(wm)) {
        return false;
    }
    const srgsim::Object* object = wm->sRGSimData.getWorld()->getObject(this->objectID);
    const srgsim::Cell* goalCell = object->getCell();
    if (!goalCell) {
        return false;
    }

    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }
    srgsim::Coordinate diff = (goalCell->coordinate - ownCoord.value()).abs();
    return diff.x < 2 && diff.y < 2;
}

bool TransportTask::movedToDestination(srg::SRGWorldModel* wm)
{
    if (!pickedObject(wm)) {
        return false;
    }
    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }
    srgsim::Coordinate diff = (this->coordinate - ownCoord.value()).abs();
    return diff.x < 2 && diff.y < 2;
}

bool TransportTask::pickedObject(srg::SRGWorldModel* wm)
{
    if (!foundObject(wm)) {
        return false;
    }

    const srgsim::ServiceRobot* robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
    if (!robot) {
        std::cerr << "[TransportTask] World does not know myself!" << std::endl;
        return false;
    }

    return robot->getCarriedObject() && robot->getCarriedObject()->getType() == this->objectType;
}

std::ostream& operator<<(std::ostream& os, const srg::dialogue::TransportTask& obj)
{
    os << "[TransportTask] " << obj.objectType << "(ID: " << obj.objectID << ")"
       << " to " << obj.coordinate;
    return os;
}

} // namespace dialogue
} // namespace srg
