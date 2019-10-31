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
    bool checkSuccess(SRGWorldModel* wm) const;

    essentials::IdentifierConstPtr objectID;
};
} // namespace dialogue
} // namespace srg
