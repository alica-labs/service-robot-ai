#pragma once

#include "process_manager/containers/ProcessStats.h"
#include "process_manager/containers/ProcessCommand.h"

#include <capnzero/CapnZero.h>

namespace essentials {
    class IDManager;
}

namespace process_manager {
    class ContainerUtils {
    public:
        static ProcessStats toProcessStats(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static ProcessCommand toProcessCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(ProcessStats psts, ::capnp::MallocMessageBuilder& builder);
        static void toMsg(ProcessCommand pc, ::capnp::MallocMessageBuilder& builder);
    private:
        ContainerUtils() = delete;
    };
}