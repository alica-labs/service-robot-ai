#include "srg/robot/SearchCell.h"

#include <srgsim/world/Cell.h>

namespace srg
{
namespace robot
{
SearchCell::SearchCell(int32_t priority, const srgsim::Cell* cell)
        : priority(priority)
        , cell(cell)
{
}
} // namespace robot
} // namespace srg