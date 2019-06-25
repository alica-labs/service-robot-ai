#pragma once

#include <process_manager/containers/ProcessCommand.h>
#include <capnzero/CapnZero.h>
#include <essentials/IdentifierConstPtr.h>

namespace essentials
{
    class SystemConfig;
}

namespace pm_control
{
    class PMControl;

    class Communication
    {
    public:
        Communication(PMControl* control);

        // capnzero call backs
        void handleProcessStats(capnp::FlatArrayMessageReader& msg);
        void sendProcessCommand(process_manager::ProcessCommand pc);

    private:
        essentials::SystemConfig* sc;
        PMControl* control;

        // capnzero stuff
        void* ctx;
        capnzero::Subscriber* processStatsSub;
        std::string processStatsTopic;
        capnzero::Publisher* processCommandPub;
        std::string processCommandTopic;

    };
} // namespace robot_control
