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
    bool checkSuccess(SRGWorldModel* wm) const;
};
} // namespace dialogue
} // namespace srg
