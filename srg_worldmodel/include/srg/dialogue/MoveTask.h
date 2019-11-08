#pragma once

#include "srg/dialogue/Task.h"

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class MoveTask : public Task
{
public:
    MoveTask() : Task() {};
    ~MoveTask() override = default;
    bool checkSuccess(SRGWorldModel* wm) const override;
};
} // namespace dialogue
} // namespace srg
