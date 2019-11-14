#include "srg/robot/ObjectSearch.h"

#include <srg/SRGWorldModel.h>
#include <srgsim/containers/Coordinate.h>
#include <srgsim/world/Cell.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/RoomType.h>
#include <srgsim/world/World.h>

#include <cnc_geometry/Calculator.h>

namespace srg
{
namespace robot
{
ObjectSearch::ObjectSearch(srgsim::ObjectType objectType, srg::SRGWorldModel* wm)
        : objectType(objectType)
        , wm(wm)
        , updateCounter(0)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
    this->fringe = new std::set<SearchCell, SearchCellSorter>(SearchCellSorter(this->wm));
    this->visited = new std::unordered_set<const srgsim::Cell*>();
}

void ObjectSearch::addRoomType(srgsim::RoomType type){
    this->roomTypes.insert(type);
};

const srgsim::Cell* ObjectSearch::getNextCell()
{
    if (this->fringe->size() == 0) {
        return nullptr;
    }
    return this->fringe->begin()->cell;

    //    std::vector<std::pair<int32_t, const srgsim::Cell*>> cellsInPriorityOrder;
    //    for (SearchCell sCell : *this->fringe) {
    //        cellsInPriorityOrder.emplace_back(-1, cell);
    //    }
    //    std::sort(cellsInPriorityOrder.begin(), cellsInPriorityOrder.end(), CustomCellSorter(this->wm));
    //    return cellsInPriorityOrder.begin()->second;
}

void ObjectSearch::update()
{
    nonstd::optional<srgsim::Coordinate> ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return;
    }

    updateCounter++;

    std::unordered_set<const srgsim::Cell*> visible;
    std::unordered_set<const srgsim::Cell*> front;
    this->getVisibleAndFrontCells(ownCoord.value(), wm->sRGSimData.getWorld(), visible, front);
    for (const srgsim::Cell* cell : visible) {
        for (auto iter = this->fringe->begin(); iter != this->fringe->end(); ++iter) {
            if (iter->cell == cell) {
                this->fringe->erase(iter);
                break;
            }
        }
        this->visited->insert(cell);
    }
    for (const srgsim::Cell* cell : front) {
        if (this->visited->find(cell) == this->visited->end()) {
            this->fringe->insert(SearchCell(updateCounter, cell));
        }
    }

    for (auto& cell : *this->fringe) {
        srgsim::Perception p;
        p.type = srgsim::ObjectType::CupRed;
        p.x = cell.cell->coordinate.x;
        p.y = cell.cell->coordinate.y;
        wm->sRGSimData.getWorld()->addMarker(p);
    }
}

void ObjectSearch::getVisibleAndFrontCells(srgsim::Coordinate& ownCoord, const srgsim::World* world, std::unordered_set<const srgsim::Cell*>& visible,
        std::unordered_set<const srgsim::Cell*>& front)
{
    double increment = atan2(1, sightLimit + 2);
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += increment) {
        int32_t xDelta = round(sin(currentDegree) * sightLimit);
        int32_t yDelta = round(cos(currentDegree) * sightLimit);
        srgsim::Coordinate traceEnd = srgsim::Coordinate(ownCoord.x + xDelta, ownCoord.y + yDelta);
        this->trace(world, ownCoord, traceEnd, visible, front);
    }
}

void ObjectSearch::trace(const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::unordered_set<const srgsim::Cell*>& visible,
        std::unordered_set<const srgsim::Cell*>& front)
{
    visible.insert(world->getCell(from));
    int32_t sign_x = ((int32_t) to.x - (int32_t) from.x) > 0 ? 1 : -1;
    int32_t sign_y = ((int32_t) to.y - (int32_t) from.y) > 0 ? 1 : -1;

    srgsim::Coordinate currentPoint(from.x, from.y);
    srgsim::Coordinate pointStepX(from.x, from.y);
    srgsim::Coordinate pointStepY(from.x, from.y);
    while (currentPoint != to) {
        // set next point
        pointStepX = srgsim::Coordinate(currentPoint.x + sign_x, currentPoint.y);
        pointStepY = srgsim::Coordinate(currentPoint.x, currentPoint.y + sign_y);
        double distanceStepX = geometry::distancePointToLineSegmentCalc(pointStepX.x, pointStepX.y, from.x, from.y, to.x, to.y);
        double distanceStepY = geometry::distancePointToLineSegmentCalc(pointStepY.x, pointStepY.y, from.x, from.y, to.x, to.y);
        if (distanceStepX < distanceStepY) {
            currentPoint = pointStepX;
        } else {
            currentPoint = pointStepY;
        }

        // check if sight is blocked in this cell
        const srgsim::Cell* cell = world->getCell(currentPoint);
        front.erase(cell);
        if (!cell || cell->getType() == srgsim::RoomType::Wall) {
            break;
        }
        bool sightBlocked = false;
        for (auto object : cell->getObjects()) {
            if (object->getType() == srgsim::ObjectType::Door && object->getState() == srgsim::ObjectState::Closed) {
                sightBlocked = true;
                break;
            }
        }
        if (sightBlocked) {
            break;
        }

        visible.insert(cell);
        if (visible.find(cell->up) == visible.end() && cell->up->getType() != srgsim::RoomType::Wall) {
            front.insert(cell->up);
        }
        if (visible.find(cell->down) == visible.end() && cell->down->getType() != srgsim::RoomType::Wall) {
            front.insert(cell->down);
        }
        if (visible.find(cell->right) == visible.end() && cell->right->getType() != srgsim::RoomType::Wall) {
            front.insert(cell->right);
        }
        if (visible.find(cell->left) == visible.end() && cell->left->getType() != srgsim::RoomType::Wall) {
            front.insert(cell->left);
        }
    }
}

} // namespace robot
} // namespace srg