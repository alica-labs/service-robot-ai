#pragma once

namespace srgsim {
    class Cell;
}

namespace srg
{
namespace robot
{
class Search;
struct CustomCellSorter
{
    CustomCellSorter(srg::robot::Search* search);
    srg::robot::Search* search;
    bool operator()(const srgsim::Cell* const& lhs, const srgsim::Cell* const& rhs);
};
} // namespace robot
} // namespace srg