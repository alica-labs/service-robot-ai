#pragma once

#include <cstdint>

namespace srg
{
namespace world
{
class Cell;
}
namespace robot
{
struct SearchCell
{
    SearchCell(int32_t priority, const srg::world::Cell* cell);
    int32_t priority;
    const srg::world::Cell* cell;
};
} // namespace robot
} // namespace srg
