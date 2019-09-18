#include "control/containers/ContainerUtils.h"

#include "control/AgentCommandMsg.capnp.h"
#include "control/containers/AgentCommand.h"

#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace control
{
AgentCommand ContainerUtils::toAgentCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    AgentCommand agentCommand;
    control::AgentCommandMsg::Reader reader = msg.getRoot< control::AgentCommandMsg>();
    // check whether this message is for me, 0 is a wild card for all Agents
    essentials::WildcardID wildcardId(nullptr, 0);

    if ((uint8_t) reader.getReceiverId().getType() == essentials::Identifier::WILDCARD_TYPE) {
        agentCommand.receiverID = &wildcardId;
    } else {
        agentCommand.receiverID = idManager->getIDFromBytes(
                reader.getReceiverId().getValue().asBytes().begin(), reader.getReceiverId().getValue().size(), (uint8_t) reader.getReceiverId().getType());
    }

    agentCommand.cmd = reader.getCmd();
    return agentCommand;
}

void ContainerUtils::toMsg(AgentCommand agentCommand, ::capnp::MallocMessageBuilder& builder)
{
    AgentCommandMsg::Builder msg = builder.getRoot<AgentCommandMsg>();
    capnzero::ID::Builder receiverID = msg.initReceiverId();
    receiverID.setValue(kj::arrayPtr(agentCommand.receiverID->getRaw(), (unsigned int) agentCommand.receiverID->getSize()));
    receiverID.setType(agentCommand.receiverID->getType());
    msg.setReceiverId(receiverID);
    msg.setCmd(agentCommand.cmd);
}

} // namespace control