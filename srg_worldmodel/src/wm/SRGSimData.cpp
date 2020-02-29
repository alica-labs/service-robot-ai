#include "srg/wm/SRGSimData.h"

#include "srg/SRGWorldModel.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/dialogue/DialogueManager.h"
#include "srg/tasks/CommandHandler.h"

#include <srg/World.h>
#include <srg/world/Agent.h>
#include <srg/world/Cell.h>
#include <srg/world/Object.h>
#include <srg/world/ObjectType.h>

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

void SRGSimData::processPerception(srg::sim::containers::Perceptions perceptions)
{
    if (!world)
        return;
    for (srg::sim::containers::CellPerception cellPerception : perceptions.cellPerceptions) {
        std::shared_ptr<const world::Cell> cell = this->world->getCell(srg::world::Coordinate(cellPerception.x, cellPerception.y));
        if (!cell || cell->timeOfLastUpdate > cellPerception.time) {
            // invalid or old information
            continue;
        }

        // update objects itself
        std::vector<std::shared_ptr<world::Object>> objects;
        for (std::shared_ptr<srg::world::Object> object : cellPerception.objects) {
            objects.push_back(this->world->createOrUpdateObject(object));
        }
        // update asp knowledge about objects
        std::cout << "[SRGSimData] " << cellPerception << std::endl;
        this->wm->srgKnowledgeManager->handleObjects(cellPerception.objects, true);

        // update association with cell
        this->world->updateCell(srg::world::Coordinate(cellPerception.x, cellPerception.y), objects, cellPerception.time);
        for (std::shared_ptr<world::Object> object : objects) {
            switch (object->getType()) {
            case world::ObjectType::Robot:
            case world::ObjectType::Human: {
                this->world->addAgent(std::dynamic_pointer_cast<srg::world::Agent>(object));
                std::shared_ptr<const world::Cell> cell = std::dynamic_pointer_cast<const world::Cell>(object->getParentContainer());
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

    // remove objects that don't have coordinates -> you don't know them anymore
    // && update asp knowledge about vanished objects
    this->wm->srgKnowledgeManager->handleObjects(this->world->removeUnknownObjects(), false);

    // updates task sequences and sends pending answers
    this->dialogueManager->tick();

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
