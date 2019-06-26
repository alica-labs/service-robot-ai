#pragma once

#include "control/containers/AgentCommand.h"

#include <capnzero/CapnZero.h>
#include <essentials/IdentifierConstPtr.h>
#include <process_manager/containers/ProcessCommand.h>


namespace essentials
{
class SystemConfig;
}

namespace control
{
class ControlPanel;

class Communication
{
public:
    Communication(ControlPanel* controlPanel);

    // capnzero call backs
    void handleProcessStats(capnp::FlatArrayMessageReader& msg);
    void handleAlicaInfo(capnp::FlatArrayMessageReader& msg);
    void send(process_manager::ProcessCommand pc);
    void send(AgentCommand rc);
private:
    essentials::SystemConfig* sc;
    ControlPanel* controlPanel;

    // capnzero stuff
    void* ctx;
    capnzero::Subscriber* processStatsSub;
    std::string processStatsTopic;
    capnzero::Subscriber* alicaInfoSub;
    std::string alicaInfoTopic;

    capnzero::Publisher* processCommandPub;
    std::string processCommandTopic;
    capnzero::Publisher* agentCommandPub;
    std::string agentCommandTopic;

};
} // namespace control
