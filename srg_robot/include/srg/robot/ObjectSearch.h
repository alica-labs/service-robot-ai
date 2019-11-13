#pragma once

#include "srg/robot/CustomCellSorter.h"

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
    ObjectSearch(srgsim::ObjectType objectType);

    void update(srg::SRGWorldModel* wm);
    const srgsim::Cell* getNextCell();
    const srgsim::Coordinate getOwnCoordinates();

private:
    void getVisibleAndFrontCells(srgsim::Coordinate& ownCoord, const srgsim::World* world, std::unordered_set<const srgsim::Cell*>& visible,
            std::unordered_set<const srgsim::Cell*>& front);
    void trace(const srgsim::World* world, srgsim::Coordinate& from, srgsim::Coordinate& to, std::unordered_set<const srgsim::Cell*>& visible,
            std::unordered_set<const srgsim::Cell*>& front);

    srgsim::ObjectType objectType;
    std::set<const srgsim::Cell*>* fringe;
    std::unordered_set<const srgsim::Cell*>* visited;
    essentials::SystemConfig* sc;
    srgsim::Coordinate ownCoordinates;
    uint32_t sightLimit;
};

} // namespace robot
} // namespace srg
