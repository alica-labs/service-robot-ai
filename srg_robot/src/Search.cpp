#include "srg/robot/Search.h"

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
Search::Search(srgsim::ObjectType objectType)
        : objectType(objectType)
        , ownCoordinates(-1, -1)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
    this->fringe = new std::set<const srgsim::Cell*, CustomCellSorter>(CustomCellSorter(this));
}

const srgsim::Coordinate Search::getOwnCoordinates()
{
    return this->ownCoordinates;
}

const srgsim::Cell* Search::getNextCell()
{
    return *this->fringe->begin();
}

void Search::update(srg::SRGWorldModel* wm)
{
    nonstd::optional<srgsim::Coordinate> ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return;
    }
    ownCoordinates = ownCoord.value();

    std::vector<const srgsim::Cell*> visible;
    std::vector<const srgsim::Cell*> front;
    this->getVisibleAndFrontCells(ownCoord.value(), wm->sRGSimData.getWorld(), visible, front);

    for (const srgsim::Cell* cell : visible) {
        this->fringe->erase(cell);
    }
    for (const srgsim::Cell* cell : front) {
        this->fringe->insert(cell);
    }
}

void Search::getVisibleAndFrontCells(
        srgsim::Coordinate& ownCoord, const srgsim::World* world, std::vector<const srgsim::Cell*>& visible, std::vector<const srgsim::Cell*>& front)
{
    double increment = atan2(1, sightLimit + 2);
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += increment) {
        // +1 for new front cell
        int32_t xDelta = round(sin(currentDegree) * sightLimit + 1);
        int32_t yDelta = round(cos(currentDegree) * sightLimit + 1);
        srgsim::Coordinate traceEnd = srgsim::Coordinate(ownCoord.x + xDelta, ownCoord.y + yDelta);

        std::vector<const srgsim::Cell*> visibleTrace;
        const srgsim::Cell* frontCell = nullptr;
        this->trace(world, ownCoord, traceEnd, visibleTrace, frontCell);
        for (const srgsim::Cell* cell : visibleTrace) {
            visible.push_back(cell);
        }
        if (frontCell) {
            front.push_back(frontCell);
        }
    }
}

void Search::trace(
        const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::vector<const srgsim::Cell*>& visible, const srgsim::Cell*& frontCell)
{
    frontCell = nullptr;

    visible.push_back(world->getCell(from));
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
        if (!cell || cell->getType() != srgsim::RoomType::Floor) {
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
        if (currentPoint != to) {
            visible.push_back(cell);
        } else {
            frontCell = cell;
        }
    }
}

} // namespace robot
} // namespace srg