#pragma once

#include "srg/dialogue/Task.h"
namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class IdleTask : public Task
{
public:
    IdleTask() : Task() {};
    ~IdleTask() override = default;

    bool checkSuccess(SRGWorldModel* wm) const override;
};
} // namespace dialogue
} // namespace srg
