#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srgsim/Object.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/ServiceRobot.h"
#include "srgsim/World.h"

namespace srg
{
namespace wm
{

SRGSimData::SRGSimData(SRGWorldModel* wm)
{
    this->wm = wm;
    this->sc = this->wm->getSystemConfig();

    // ATTENTION: This path/file is not the same as the simulator
    // is starting, please make sure that both files have the same content!
    this->world = new srgsim::World((*sc).getConfigPath() + "/textures/Department.tmx");
}

SRGSimData::~SRGSimData() {}

void SRGSimData::processPerception(srgsim::SimPerceptions simPerceptions)
{
    for (srgsim::Perception perception : simPerceptions.perceptions) {
        switch (perception.type) {
        case srgsim::Type::Robot: {
            srgsim::Object* robot = this->world->addObject(perception.objectID, perception.type);
            if (this->world->placeObject(robot, srgsim::Coordinate(perception.x, perception.y))) {
                this->world->addRobot(static_cast<srgsim::ServiceRobot*>(robot));
            }
        } break;
        default:
            std::cerr << "SRGSimData::processPerception(): Unknown perception received!" << std::endl;
        }
    }
}

bool SRGSimData::isLocalised() {
    const srgsim::Object* robot = this->world->getObject(this->wm->getOwnId());
    return (robot && ((srgsim::ServiceRobot*) robot)->getCell());
}

} // namespace wm
} /* namespace srg */
