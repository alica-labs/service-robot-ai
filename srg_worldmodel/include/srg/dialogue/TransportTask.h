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
    bool foundObject(SRGWorldModel* wm) const;
    bool pickedObject(SRGWorldModel* wm) const;
    bool movedToDestination(SRGWorldModel* wm) const;
    bool closeToObject(srg::SRGWorldModel* wm) const;

    friend std::ostream& operator<<(std::ostream& os, const srg::dialogue::TransportTask& obj);


    srgsim::ObjectType objectType;
};
} // namespace dialogue
} // namespace srg
