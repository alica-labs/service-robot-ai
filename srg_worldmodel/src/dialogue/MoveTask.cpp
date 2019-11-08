#include "srg/dialogue/MoveTask.h"

#include "srg/SRGWorldModel.h"
#include <srgsim/containers/Coordinate.h>
#include <srgsim/world/Cell.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/RoomType.h>
#include <srgsim/world/ObjectType.h>
#include <srgsim/world/World.h>

namespace srg
{
namespace dialogue
{
bool MoveTask::checkSuccess(SRGWorldModel* wm) const
{
    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }

    bool goalIsBlocked = false;
    const srgsim::Cell* goalCell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    if (goalCell->getType() == srgsim::RoomType::Wall) {
        goalIsBlocked = true;
    }

    if (!goalIsBlocked) {
        for (auto object : goalCell->getObjects()) {
            if (object->getType() == srgsim::ObjectType::Door) {
                goalIsBlocked = true;
                break;
            }
        }
    }

    srgsim::Coordinate diff = this->coordinate - ownCoord.value();
    if ((goalIsBlocked && abs(diff.x) < 2 && abs(diff.y) < 2) || (diff.x == 0 && diff.y == 0)) {
        std::cout << "MoveTask::checkSuccess(): SUCCESS! Goal " << this->coordinate << " OwnPos " << ownCoord.value() << std::endl;
        return true;
    }
    return false;
}
} // namespace dialogue
} // namespace srg
