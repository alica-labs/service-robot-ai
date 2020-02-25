#include "srg/agent/SearchCell.h"

#include <srg/world/Cell.h>

namespace srg
{
namespace agent
{
SearchCell::SearchCell(int32_t priority, std::shared_ptr<const world::Cell> cell)
        : priority(priority)
        , cell(cell)
{
}
} // namespace agent
} // namespace srg