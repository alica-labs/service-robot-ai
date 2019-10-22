#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/Object.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"

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

    this->ownPositionValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Perception.ValidityDuration", NULL));
    this->ownPositionBuffer = new supplementary::InfoBuffer<srgsim::Coordinate>((*sc)["SRGWorldModel"]->get<int>("Data.Perception.BufferLength", NULL));
}

SRGSimData::~SRGSimData() {}

const srgsim::World* SRGSimData::getWorld()
{
    return this->world;
}

void SRGSimData::processPerception(srgsim::SimPerceptions simPerceptions)
{
    for (srgsim::Perception perception : simPerceptions.perceptions) {
        switch (perception.type) {
        case srgsim::Type::Robot: {
            srgsim::Object* robot = this->world->addObject(perception.objectID, perception.type);
            if (this->world->placeObject(robot, srgsim::Coordinate(perception.x, perception.y))) {
                this->world->addRobot(static_cast<srgsim::ServiceRobot*>(robot));
            }
            auto ownPositionInfo = std::make_shared<supplementary::InformationElement<srgsim::Coordinate>>(
                    robot->getCell()->coordinate, wm->getTime(), ownPositionValidityDuration, 1.0);
            this->ownPositionBuffer->add(ownPositionInfo);
        } break;
        case srgsim::Type::Door:
            break;
        case srgsim::Type::CupYellow:
        case srgsim::Type::CupRed:
        case srgsim::Type::CupBlue:
            break;
        default:
            std::cerr << "SRGSimData::processPerception(): Unknown perception received!" << std::endl;
        }
    }
}

bool SRGSimData::isLocalised()
{
    const srgsim::Object* robot = this->world->getObject(this->wm->getOwnId());
    return (robot && ((srgsim::ServiceRobot*) robot)->getCell());
}

const supplementary::InfoBuffer<srgsim::Coordinate>& SRGSimData::getOwnPositionBuffer() const
{
    return *this->ownPositionBuffer;
}

} // namespace wm
} /* namespace srg */
