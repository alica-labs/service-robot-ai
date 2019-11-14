#include "srg/robot/SearchCellSorter.h"

#include <Geometry.h>
#include <srg/Robot.h>
#include <srgsim/world/Cell.h>

namespace srg
{
namespace robot
{

SearchCellSorter::SearchCellSorter(srg::SRGWorldModel* wm)
        : wm(wm)
{
}

bool SearchCellSorter::operator()(const SearchCell& lhs, const SearchCell& rhs)
{
    // filters cells with same coordinate in sets
    if (lhs.cell->coordinate == rhs.cell->coordinate) {
        return true;
    }

    // orders search cells according to priority first
    if (lhs.priority != rhs.priority) {
        return lhs.priority > rhs.priority;
    }

    // last criteria is the coordinate
    return lhs.cell->coordinate < rhs.cell->coordinate;
}

} // namespace robot
} // namespace srg