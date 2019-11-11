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

    srgsim::ObjectType objectType;
};
} // namespace dialogue
} // namespace srg
