#pragma once

#include "srg/robot/CustomCellSorter.h"

#include <srgsim/containers/Coordinate.h>
#include <srgsim/world/ObjectType.h>
#include <srgsim/world/Cell.h>

#include <cstdint>
#include <set>
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
    ObjectSearch(srgsim::ObjectType objectType);

    void update(srg::SRGWorldModel* wm);
    const srgsim::Cell* getNextCell();
    const srgsim::Coordinate getOwnCoordinates();

private:
    void getVisibleAndFrontCells(srgsim::Coordinate& ownCoord, const srgsim::World* world,
            std::set<const srgsim::Cell*, decltype(&srgsim::Cell::sortByCoordinates)>& visible,
            std::set<const srgsim::Cell*, decltype(&srgsim::Cell::sortByCoordinates)>& front);
    void trace(const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::vector<const srgsim::Cell*>& visible,
            const srgsim::Cell*& frontCell);

    srgsim::ObjectType objectType;
    std::multiset<const srgsim::Cell*, CustomCellSorter>* fringe;
    std::set<const srgsim::Cell*, decltype(&srgsim::Cell::sortByCoordinates)>* visited;
    essentials::SystemConfig* sc;
    srgsim::Coordinate ownCoordinates;
    uint32_t sightLimit;
};

} // namespace robot
} // namespace srg
