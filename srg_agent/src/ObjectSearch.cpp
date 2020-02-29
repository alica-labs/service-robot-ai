#include "srg/agent/ObjectSearch.h"

#include <srg/SRGWorldModel.h>
#include <engine/AlicaEngine.h>
#include <srg/dialogue/RequestHandler.h>
#include <srg/World.h>
#include <srg/world/Cell.h>
#include <srg/world/Door.h>
#include <srg/world/Room.h>

#include <cnc_geometry/Calculator.h>

#include <limits>

namespace srg
{
namespace agent
{
ObjectSearch::ObjectSearch(srg::world::ObjectType objectType, srg::SRGWorldModel* wm)
        : objectType(objectType)
        , wm(wm)
        , updateCounter(0)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
    this->fringe = new std::set<SearchCell, SearchCellSorter>(SearchCellSorter(this->wm));
    this->visited = new std::unordered_set<std::shared_ptr<const world::Cell>>();
    this->initFringeWithProbableLocations(); // Remark: activate, or deactivate for different evaluations
}

ObjectSearch::~ObjectSearch() {
    delete this->fringe;
    delete this->visited;
}

std::shared_ptr<const world::Cell> ObjectSearch::getNextCell()
{
    if (this->fringe->size() == 0) {
        return nullptr;
    }
    return this->fringe->begin()->cell;
}

void ObjectSearch::update()
{
    nonstd::optional<srg::world::Coordinate> ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return;
    }

    updateCounter++;

    std::unordered_set<std::shared_ptr<const world::Cell>> visible;
    std::unordered_set<std::shared_ptr<const world::Cell>> front;
    this->getVisibleAndFrontCells(ownCoord.value(), wm->sRGSimData.getWorld(), visible, front);
    for (std::shared_ptr<const world::Cell> cell : visible) {
        for (auto iter = this->fringe->begin(); iter != this->fringe->end(); ++iter) {
            if (iter->cell == cell) {
                this->fringe->erase(iter);
                break;
            }
        }
        this->visited->insert(cell);
    }
    for (std::shared_ptr<const world::Cell> cell : front) {
        if (this->visited->find(cell) == this->visited->end()) {
            this->fringe->insert(SearchCell(updateCounter, cell));
        }
    }

    // for debugging purposes only
    for (auto& cell : *this->fringe) {
        srg::viz::Marker marker(cell.cell->coordinate);
        marker.type = srg::viz::SpriteType::CupRed;
        wm->gui->addMarker(marker);
    }
}

void ObjectSearch::initFringeWithProbableLocations() {
    // request
    agent::SpeechAct request;
    request.text = "room"; // Future Work: this should depend on other parameters of the Object Search in future
    request.objectRequestType = this->objectType;
    request.type = agent::SpeechType::request;
    request.actID = this->wm->getEngine()->getIdManager()->generateID();
    request.senderID = this->wm->getOwnId();
    std::shared_ptr<agent::SpeechAct> answer = this->wm->dialogueManager.requestHandler->handle(request);

    // init fringe with room cells
    for (const srg::world::RoomType roomType : answer->probableRoomTypes) {
        this->roomTypes.insert(roomType);
        for (world::Room* room : this->wm->sRGSimData.getWorld()->getRooms(roomType)) {
            std::cout << "[ObjectSearch] Add cells of room " << room->getID() << " Type " << room->getType() << std::endl;
            for (auto& cellEntry : room->getCells()) {
                this->fringe->insert(SearchCell(std::numeric_limits<int32_t>::max(), cellEntry.second));
            }
        }
    }
}

/**
 * Called one time per update call of this ObjectSearch.
 * @param ownCoord
 * @param world
 * @param visible The cells that agent is currently able to see.
 * @param front The cells that are neighbours to the currently seeable cells, i.e. potentially unvisited cells to visit.
 */
void ObjectSearch::getVisibleAndFrontCells(srg::world::Coordinate& ownCoord, const srg::World* world,
        std::unordered_set<std::shared_ptr<const world::Cell>>& visible, std::unordered_set<std::shared_ptr<const world::Cell>>& front)
{
    double increment = atan2(1, sightLimit + 2);
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += increment) {
        int32_t xDelta = round(sin(currentDegree) * sightLimit);
        int32_t yDelta = round(cos(currentDegree) * sightLimit);
        srg::world::Coordinate traceEnd = srg::world::Coordinate(ownCoord.x + xDelta, ownCoord.y + yDelta);
        this->trace(world, ownCoord, traceEnd, visible, front);
    }
}

void ObjectSearch::trace(const srg::World* world, srg::world::Coordinate& from, srg::world::Coordinate& to,
        std::unordered_set<std::shared_ptr<const world::Cell>>& visible, std::unordered_set<std::shared_ptr<const world::Cell>>& front)
{
    visible.insert(world->getCell(from));
    int32_t sign_x = ((int32_t) to.x - (int32_t) from.x) > 0 ? 1 : -1;
    int32_t sign_y = ((int32_t) to.y - (int32_t) from.y) > 0 ? 1 : -1;

    srg::world::Coordinate currentPoint(from.x, from.y);
    srg::world::Coordinate pointStepX(from.x, from.y);
    srg::world::Coordinate pointStepY(from.x, from.y);
    while (currentPoint != to) {
        // set next point
        pointStepX = srg::world::Coordinate(currentPoint.x + sign_x, currentPoint.y);
        pointStepY = srg::world::Coordinate(currentPoint.x, currentPoint.y + sign_y);
        double distanceStepX = geometry::distancePointToLineSegmentCalc(pointStepX.x, pointStepX.y, from.x, from.y, to.x, to.y);
        double distanceStepY = geometry::distancePointToLineSegmentCalc(pointStepY.x, pointStepY.y, from.x, from.y, to.x, to.y);
        if (distanceStepX < distanceStepY) {
            currentPoint = pointStepX;
        } else {
            currentPoint = pointStepY;
        }

        // check if sight is blocked in this cell
        std::shared_ptr<const world::Cell> cell = world->getCell(currentPoint);
        front.erase(cell);
        if (!cell || cell->getType() == srg::world::RoomType::Wall) {
            break;
        }
        bool sightBlocked = false;
        for (auto& objectEntry : cell->getObjects()) {
            if (std::shared_ptr<srg::world::Door> door = std::dynamic_pointer_cast<srg::world::Door>(objectEntry.second)) {
                sightBlocked = !door->isOpen();
                break;
            }
        }
        if (sightBlocked) {
            break;
        }

        visible.insert(cell);
        if (visible.find(cell->up) == visible.end() && cell->up->getType() != srg::world::RoomType::Wall) {
            front.insert(cell->up);
        }
        if (visible.find(cell->down) == visible.end() && cell->down->getType() != srg::world::RoomType::Wall) {
            front.insert(cell->down);
        }
        if (visible.find(cell->right) == visible.end() && cell->right->getType() != srg::world::RoomType::Wall) {
            front.insert(cell->right);
        }
        if (visible.find(cell->left) == visible.end() && cell->left->getType() != srg::world::RoomType::Wall) {
            front.insert(cell->left);
        }
    }
}

} // namespace agent
} // namespace srg