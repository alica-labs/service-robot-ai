#pragma once

#include "srg/robot/Path.h"

#include <srgsim/containers/Coordinate.h>

namespace srg
{
class SRGWorldModel;
namespace robot
{
class Movement
{
public:
    Movement(srg::SRGWorldModel* wm);
    Path* searchPath(srgsim::Coordinate start, srgsim::Coordinate goal);
private:
    srg::SRGWorldModel* wm;
};
} // namespace robot
} // namespace srg
