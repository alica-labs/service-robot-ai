#pragma once

#include "srg/dialogue/Task.h"
namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class ManipulationTask : public Task
{
public:
    ManipulationTask() : Task() {};
    ~ManipulationTask() override = default;
    virtual bool checkSuccess(SRGWorldModel* wm) const override;

    essentials::IdentifierConstPtr objectID;
};
} // namespace dialogue
} // namespace srg
