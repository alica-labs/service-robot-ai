#pragma once

#include <cstdint>
#include <memory>

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
    SearchCell(int32_t priority, std::shared_ptr<const world::Cell> cell);
    int32_t priority;
    std::shared_ptr<const world::Cell> cell;
};
} // namespace agent
} // namespace srg
