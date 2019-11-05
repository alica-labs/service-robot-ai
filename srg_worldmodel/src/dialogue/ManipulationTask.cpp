#include "srg/dialogue/ManipulationTask.h"

#include "srg/SRGWorldModel.h"

#include <srgsim/world/Object.h>
#include <srgsim/world/World.h>
#include <srgsim/world/ServiceRobot.h>

namespace srg
{
namespace dialogue
{
bool ManipulationTask::checkSuccess(SRGWorldModel* wm) const
{
    const srgsim::Object* object = nullptr;
    const srgsim::ServiceRobot* robot = nullptr;
    switch (this->type) {
    case srgsim::TaskType::Open:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srgsim::ObjectState::Open;
    case srgsim::TaskType::Close:
        object = wm->sRGSimData.getWorld()->getObject(this->objectID);
        return object && object->getState() == srgsim::ObjectState::Closed;
    case srgsim::TaskType::PickUp:
        robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
        return robot->getCarriedObject()->getID() == this->objectID;
    case srgsim::TaskType::PutDown:
        std::cout << "ManipulationTask::checkSuccess(): PutDown-CheckSuccess not implemented, yet!" << std::endl;
        const srgsim::Cell* cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
//        for (const srgsim::Object* object : cell->getObjects()) {
//
//        }
        return false;
    default:
        std::cerr << "ManipulationTask::checkSuccess(): Unknown manipulation task encountered: " << this->type << std::endl;
        return false;
    }
}
} // namespace dialogue
} // namespace srg