#pragma once

#include "process_manager/containers/ProcessStat.h"

#include <vector>

namespace essentials {
    class Identifier;
}

namespace process_manager {
    struct ProcessStats {
        const essentials::Identifier* ownID;
        std::vector<ProcessStat> processStats;
    };
}