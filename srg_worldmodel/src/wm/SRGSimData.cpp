#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/DialogueManager.h"
#include "srg/tasks/TaskHandler.h"

#include <srg/World.h>
#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/ObjectType.h>
#include <srg/world/ServiceRobot.h>

#include <engine/AlicaEngine.h>

namespace srg
{
namespace wm
{

SRGSimData::SRGSimData(SRGWorldModel* wm)
        : world(nullptr)
{
    this->wm = wm;
    this->sc = this->wm->getSystemConfig();
    this->dialogueManager = &wm->dialogueManager;

    this->ownPositionValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Perception.ValidityDuration", NULL));
    this->ownPositionBuffer = new supplementary::InfoBuffer<srg::world::Coordinate>((*sc)["SRGWorldModel"]->get<int>("Data.Perception.BufferLength", NULL));
}

SRGSimData::~SRGSimData() {}

void SRGSimData::init()
{
    // ATTENTION: This path/file is not the same as the simulator
    // is starting, please make sure that both files have the same content!
    this->world = new srg::World((*sc).getConfigPath() + "/textures/Department.tmx", wm->getEngine()->getIdManager());
}

srg::World* SRGSimData::getWorld()
{
    return this->world;
}

void SRGSimData::processPerception(srg::sim::containers::SimPerceptions simPerceptions)
{
    if (!world)
        return;
    for (srg::sim::containers::CellPerceptions cellPerceptions : simPerceptions.cellPerceptions) {
//        if (cellPerceptions.perceptions.size() > 0) {
//            std::cout << "[SRGSimData]" << std::endl << cellPerceptions << std::endl;
//        }

        const srg::world::Cell* cell = this->world->getCell(srg::world::Coordinate(cellPerceptions.x, cellPerceptions.y));
        if (cell) {
            // update objects itself
            std::vector<world::Object*> objects;
            for (srg::sim::containers::Perception perception : cellPerceptions.perceptions) {
                objects.push_back(this->world->createOrUpdateObject(perception.objectID, perception.type, perception.state, perception.robotID));
            }

            // update association with cell
            this->world->updateCell(srg::world::Coordinate(cellPerceptions.x, cellPerceptions.y), objects);
            for (world::Object* object : objects) {
                switch (object->getType()) {
                case world::ObjectType::Robot: {
                    this->world->addRobot(static_cast<srg::world::ServiceRobot*>(object));
                    auto ownPositionInfo = std::make_shared<supplementary::InformationElement<srg::world::Coordinate>>(
                            object->getCell()->coordinate, wm->getTime(), ownPositionValidityDuration, 1.0);
                    this->ownPositionBuffer->add(ownPositionInfo);
                } break;
                default:
                    // nothing extra to do for other types
                    break;
                }
            }
        }
    }

    // call to update success status of tasks
    this->dialogueManager->taskHandler->tick();

    // draw the world from the robots perspective
    this->wm->gui->draw(world);
}

bool SRGSimData::isLocalised()
{
    nonstd::optional<srg::world::Coordinate> ownCoord = this->getOwnPositionBuffer().getLastValidContent();
    return ownCoord.has_value();
}

const supplementary::InfoBuffer<srg::world::Coordinate>& SRGSimData::getOwnPositionBuffer() const
{
    return *this->ownPositionBuffer;
}

} // namespace wm
} /* namespace srg */
