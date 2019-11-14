#pragma once

#include "srg/robot/SearchCellSorter.h"
#include "srg/robot/SearchCell.h"

#include <srgsim/containers/Coordinate.h>
#include <srgsim/world/Cell.h>
#include <srgsim/world/ObjectType.h>

#include <cstdint>
#include <set>
#include <unordered_set>
#include <vector>

namespace essentials
{
class SystemConfig;
}

namespace srgsim
{
class World;
} // namespace srgsim

namespace srg
{
class SRGWorldModel;
namespace robot
{
class ObjectSearch
{
public:
    ObjectSearch(srgsim::ObjectType objectType, srg::SRGWorldModel* wm);

    void addRoomType(srgsim::RoomType type);
    void update();
    const srgsim::Cell* getNextCell();

private:
    void getVisibleAndFrontCells(srgsim::Coordinate& ownCoord, const srgsim::World* world, std::unordered_set<const srgsim::Cell*>& visible,
            std::unordered_set<const srgsim::Cell*>& front);
    void trace(const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::unordered_set<const srgsim::Cell*>& visible,
            std::unordered_set<const srgsim::Cell*>& front);

    essentials::SystemConfig* sc;
    srg::SRGWorldModel* wm;
    uint32_t sightLimit;
    uint32_t updateCounter;

    srgsim::ObjectType objectType;
    std::unordered_set<srgsim::RoomType> roomTypes;

    std::set<SearchCell, SearchCellSorter>* fringe;
    std::unordered_set<const srgsim::Cell*>* visited;

};

} // namespace robot
} // namespace srg
