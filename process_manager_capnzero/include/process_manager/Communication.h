#pragma once

#include "process_manager/containers/ProcessCommand.h"
#include "process_manager/containers/ProcessStats.h"

#include <capnp/serialize.h>

namespace capnzero {
    class Publisher;
    class Subscriber;
}

namespace essentials
{
class SystemConfig;
}

namespace process_manager
{
class ProcessManager;

class Communication
{
public:
    Communication(ProcessManager* processManager);
    void handleProcessCommand(capnp::FlatArrayMessageReader& msg);
    void sendProcessStats(ProcessStats psts);

private:
    essentials::SystemConfig* sc;
    ProcessManager* processManager;
    void* ctx;

    capnzero::Subscriber* processCommandSub;
    std::string processCmdTopic;
    capnzero::Publisher* processStatePub;
    std::string processStatsTopic;
};
} // namespace process_manager