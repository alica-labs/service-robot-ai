#include "srg/dialogue/ManipulationTask.h"

#include "srg/SRGWorldModel.h"

#include <srgsim/world/Cell.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/ServiceRobot.h>
#include <srgsim/world/World.h>

namespace srg
{
namespace dialogue
{
bool ManipulationTask::checkSuccess(SRGWorldModel* wm) const
{
    const srgsim::Object* object = nullptr;
    const srgsim::ServiceRobot* robot = nullptr;
    const srgsim::Cell* cell = nullptr;
    switch (this->type) {
    case srgsim::TaskType::Open:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srgsim::ObjectState::Open;
    case srgsim::TaskType::Close:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srgsim::ObjectState::Closed;
    case srgsim::TaskType::PickUp:
        robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
        if (robot) {
            std::cout << "ManipulationTask::checkSuccess(): " << *robot << std::endl;
        }
        return robot->getCarriedObject() && robot->getCarriedObject()->getID() == this->objectID;
    case srgsim::TaskType::PutDown:
        cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
        for (const srgsim::Object* object : cell->getObjects()) {
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
} // namespace dialogue
} // namespace srg