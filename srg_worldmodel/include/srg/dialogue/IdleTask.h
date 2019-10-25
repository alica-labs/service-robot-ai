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
            bool checkSuccess(SRGWorldModel* wm);
        };
    } // namespace dialogue
} // namespace srg
