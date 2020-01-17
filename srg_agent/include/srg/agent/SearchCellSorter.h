#pragma once

#include "srg/agent/SearchCell.h"

#include <srg/SRGWorldModel.h>

#include <utility>
#include <cstdint>

namespace srg
{
namespace agent
{
struct SearchCellSorter
{
    explicit SearchCellSorter(srg::SRGWorldModel* wm);

    srg::SRGWorldModel* wm;
    bool operator()(const SearchCell& lhs,const SearchCell& rhs);
};
} // namespace agent
} // namespace srg