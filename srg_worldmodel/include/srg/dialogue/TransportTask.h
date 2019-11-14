#pragma once

#include "srg/dialogue/ManipulationTask.h"
#include <srgsim/world/ObjectType.h>

namespace srg
{
namespace dialogue
{
class TransportTask : public ManipulationTask
{
public:

    TransportTask() : ManipulationTask() {};
    ~TransportTask() override = default;
    bool checkSuccess(SRGWorldModel* wm) const override;
    // different parts of the task
    bool foundObject(SRGWorldModel* wm);
    bool pickedObject(SRGWorldModel* wm);
    bool movedToDestination(SRGWorldModel* wm);
    bool closeToObject(srg::SRGWorldModel* wm);

    friend std::ostream& operator<<(std::ostream& os, const srg::dialogue::TransportTask& obj);

    srgsim::ObjectType objectType;
};
} // namespace dialogue
} // namespace srg
