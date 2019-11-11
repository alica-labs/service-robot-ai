#include "srg/dialogue/TransportTask.h"

#include "srg/SRGWorldModel.h"

#include <srgsim/world/Object.h>
#include <srgsim/world/World.h>
#include <srgsim/world/Cell.h>
#include <srgsim/world/ServiceRobot.h>

namespace srg
{
namespace dialogue
{
    bool TransportTask::checkSuccess(SRGWorldModel* wm) const
    {
        std::cout << "[Transport] Not implemented yet!" << std::endl;
        const srgsim::Cell* cell = nullptr;
        switch (this->type) {
            case srgsim::TaskType::Transport:
                cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
                for (const srgsim::Object* object : cell->getObjects()) {
                    if (object->getType() == this->objectType) {
                        return true;
                    }
                }
                return false;
            default:
                std::cerr << "Transport::checkSuccess(): Unknown transport task encountered: " << this->type << std::endl;
                return false;
        }
    }
}
} // namespace srg
