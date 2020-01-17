#pragma once

#include "srg/agent/containers/SpeechAct.h"
#include "srg/agent/containers/AgentCommand.h"

#include <capnzero/CapnZero.h>

namespace essentials
{
class IDManager;
}

namespace srg
{
namespace agent
{
class ContainerUtils
{
public:

    static AgentCommand toAgentCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
    static void toMsg(const AgentCommand& agentCommand, ::capnp::MallocMessageBuilder& builder);

    static SpeechAct toSpeechAct(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
    static void toMsg(const SpeechAct& speechAct, ::capnp::MallocMessageBuilder& builder);

private:
private:
    ContainerUtils() = delete;
};
} // namespace human
} // namespace srg