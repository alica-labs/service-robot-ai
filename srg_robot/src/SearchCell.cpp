#include "srg/robot/SearchCell.h"

#include <srg/world/Cell.h>

namespace srg
{
namespace robot
{
SearchCell::SearchCell(int32_t priority, const srg::world::Cell* cell)
        : priority(priority)
        , cell(cell)
{
}
} // namespace robot
} // namespace srg