#pragma once

#include <cstdint>

namespace srg
{
namespace world
{
class Cell;
}
namespace agent
{
struct SearchCell
{
    SearchCell(int32_t priority, const srg::world::Cell* cell);
    int32_t priority;
    const srg::world::Cell* cell;
};
} // namespace agent
} // namespace srg
