#include "srg/robot/CustomCellSorter.h"

#include <srg/robot/Search.h>
#include <srgsim/world/Cell.h>

namespace srg
{
namespace robot
{

CustomCellSorter::CustomCellSorter(srg::robot::Search* search)
        : search(search)
{
}

bool CustomCellSorter::operator()(const srgsim::Cell* const& lhs, const srgsim::Cell* const& rhs)
{
    const srgsim::Coordinate ownCoordinates = this->search->getOwnCoordinates();
    srgsim::Coordinate lhsDiff = (lhs->coordinate - ownCoordinates).abs();
    srgsim::Coordinate rhsDiff = (lhs->coordinate - ownCoordinates).abs();
    int32_t distLHS = lhsDiff.x + lhsDiff.y;
    int32_t distRHS = rhsDiff.x + rhsDiff.y;
    return distLHS < distRHS;
}

} // namespace robot
} // namespace srg