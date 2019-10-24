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
    for (srgsim::CellPerceptions cellPerceptions : simPerceptions.cellPerceptions) {
        std::cout << "SRGSimData::processPerception(): " << cellPerceptions << std::endl;

        std::vector<srgsim::Object*> objects = this->world->updateCell(cellPerceptions);
        for (srgsim::Object* object : objects) {
            switch (object->getType()) {
                case srgsim::Type::Robot: {
                    this->world->addRobot(static_cast<srgsim::ServiceRobot*>(object));
                    auto ownPositionInfo = std::make_shared<supplementary::InformationElement<srgsim::Coordinate>>(
                            object->getCell()->coordinate, wm->getTime(), ownPositionValidityDuration, 1.0);
                    this->ownPositionBuffer->add(ownPositionInfo);
                } break;
                default:
                    // nothing extra to do for other types
                    break;
            }
        }
    }
    std::cout << "SRGSimData::processPerception(): -------------- " << std::endl;
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
