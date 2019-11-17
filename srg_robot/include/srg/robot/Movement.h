#pragma once

#include "srg/robot/Path.h"

#include <srg/world/Coordinate.h>

namespace srg
{
class SRGWorldModel;
namespace robot
{
class Movement
{
public:
    Movement(srg::SRGWorldModel* wm);
    Path* searchPath(srg::world::Coordinate start, srg::world::Coordinate goal);
private:
    srg::world::Coordinate findAlternativeGoal(srg::world::Coordinate start, srg::world::Coordinate goal);
    bool checkValidity(srg::world::Coordinate coord);

    srg::SRGWorldModel* wm;
};
} // namespace robot
} // namespace srg
