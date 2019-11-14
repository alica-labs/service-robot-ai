#pragma once

#include <cstdint>

namespace srgsim {
    class Cell;
}

namespace srg
{
namespace robot
{
struct SearchCell
{
    SearchCell(int32_t priority, const srgsim::Cell* cell);
    int32_t priority;
    const srgsim::Cell* cell;
};
} // namespace robot
} // namespace srg
