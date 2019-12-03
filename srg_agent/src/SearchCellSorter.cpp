#include "srg/agent/SearchCellSorter.h"

#include <Geometry.h>
#include <srg/Agent.h>
#include <srg/world/Cell.h>

namespace srg
{
namespace agent
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

} // namespace agent
} // namespace srg