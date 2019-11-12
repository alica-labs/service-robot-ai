#pragma once

#include "srg/robot/CustomCellSorter.h"

#include <srgsim/containers/Coordinate.h>
#include <srgsim/world/ObjectType.h>

#include <cstdint>
#include <set>
#include <vector>

namespace essentials
{
class SystemConfig;
}

namespace srgsim
{
class Cell;
class World;
} // namespace srgsim

namespace srg
{
class SRGWorldModel;
namespace robot
{
class Search
{
public:
    Search(srgsim::ObjectType objectType);

    void update(srg::SRGWorldModel* wm);
    const srgsim::Cell* getNextCell();
    const srgsim::Coordinate getOwnCoordinates();

private:
    void getVisibleAndFrontCells(
            srgsim::Coordinate& ownCoord, const srgsim::World* world, std::vector<const srgsim::Cell*>& visible, std::vector<const srgsim::Cell*>& front);
    void trace(const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::vector<const srgsim::Cell*>& visible,
            const srgsim::Cell*& frontCell);


    srgsim::ObjectType objectType;
    std::set<const srgsim::Cell*, CustomCellSorter>* fringe;
    essentials::SystemConfig* sc;
    srgsim::Coordinate ownCoordinates;
    uint32_t sightLimit;
};

} // namespace robot
} // namespace srg
