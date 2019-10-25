#include "srg/dialogue/MoveTask.h"

#include "srg/SRGWorldModel.h"

namespace srg
{
namespace dialogue
{
bool MoveTask::checkSuccess(SRGWorldModel* wm) const
{
    std::cout << "MoveTask::checkSuccess(): Not yet implemented!" << std::endl;
    if (Task::checkSuccess(wm)) {
        return true;
    }
    return false;
}
} // namespace dialogue
} // namespace srg
