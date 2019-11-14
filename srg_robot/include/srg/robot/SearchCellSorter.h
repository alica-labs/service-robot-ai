#pragma once

#include "srg/robot/SearchCell.h"

#include <srg/SRGWorldModel.h>

#include <utility>
#include <cstdint>

namespace srg
{
namespace robot
{
struct SearchCellSorter
{
    explicit SearchCellSorter(srg::SRGWorldModel* wm);

    srg::SRGWorldModel* wm;
    bool operator()(const SearchCell& lhs,const SearchCell& rhs);
};
} // namespace robot
} // namespace srg