#pragma once

namespace srgsim {
    class Cell;
}

namespace srg
{
namespace robot
{
class ObjectSearch;
struct CustomCellSorter
{
    CustomCellSorter(srg::robot::ObjectSearch* search);
    srg::robot::ObjectSearch* search;
    bool operator()(const srgsim::Cell* const& lhs, const srgsim::Cell* const& rhs);
};
} // namespace robot
} // namespace srg