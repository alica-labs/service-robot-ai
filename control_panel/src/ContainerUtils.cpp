#include "control/containers/ContainerUtils.h"

#include "control/AgentCommand.capnp.h"
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace control
{
    AgentCommand ContainerUtils::toAgentCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
    {
        AgentCommand agentCommand;
        //        process_manager::ProcessCommandMsg::Reader reader = msg.getRoot<process_manager::ProcessCommandMsg>();
        //        // check whether this message is for me, 0 is a wild card for all ProcessManagers
        //        essentials::WildcardID wildcardId(nullptr, 0);
        //        if ((uint8_t) reader.getSenderID().getType() == essentials::Identifier::WILDCARD_TYPE) {
        //            pc.receiverID = &wildcardId;
        //        } else {
        //            pc.receiverID = idManager->getIDFromBytes(
        //                    reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
        //        }
        //
        //        pc.cmd = reader.getCmd();
        //        for (::capnzero::ID::Reader id : reader.getRobotIds()) {
        //            pc.robotIDs.push_back(idManager->getIDFromBytes(id.getValue().asBytes().begin(), id.getValue().size(), (uint8_t) id.getType()));
        //        }
        //        for (int32_t paramSet : reader.getParamSets()) {
        //            pc.paramSets.push_back(paramSet);
        //        }
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