#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/DialogueManager.h"
#include "srg/tasks/TaskHandler.h"
#include "srg/asp/SRGKnowledgeManager.h"

#include <srg/World.h>
#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/ObjectType.h>
#include <srg/world/Agent.h>

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
    this->world = new srg::World((*sc).getConfigPath() + "/textures/Department.tmx", wm->getEngine()->getIdManager());
    // add asp facts that probably don't change
    for (auto& roomEntry : this->world->getRooms()) {
        this->wm->srgKnowledgeManager->addRoom(roomEntry.second);
    }
}

srg::World* SRGSimData::getWorld()
{
    return this->world;
}

void SRGSimData::processPerception(srg::sim::containers::SimPerceptions simPerceptions)
{
    if (!world)
        return;
    for (srg::sim::containers::CellPerception cellPerception : simPerceptions.cellPerceptions) {
        const srg::world::Cell* cell = this->world->getCell(srg::world::Coordinate(cellPerception.x, cellPerception.y));
        if (cell) {
            // update objects itself
            std::vector<std::shared_ptr<world::Object>> objects;
            for (std::shared_ptr<srg::world::Object> object : cellPerception.objects) {
                objects.push_back(this->world->createOrUpdateObject(object));
            }

            // update association with cell
            this->world->updateCell(srg::world::Coordinate(cellPerception.x, cellPerception.y), objects);
            for (std::shared_ptr<world::Object> object : objects) {
                switch (object->getType()) {
                case world::ObjectType::Robot:
                case world::ObjectType::Human:{
                    this->world->addAgent(std::dynamic_pointer_cast<srg::world::Agent>(object));
                    const world::Cell* cell = dynamic_cast<const world::Cell*>(object->getParentContainer());
                    auto ownPositionInfo = std::make_shared<supplementary::InformationElement<srg::world::Coordinate>>(
                            cell->coordinate, wm->getTime(), ownPositionValidityDuration, 1.0);
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
