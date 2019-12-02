#include "control/containers/ContainerUtils.h"

#include "control/AgentCommandMsg.capnp.h"
#include "control/SpeechActMsg.capnp.h"
#include "control/containers/AgentCommand.h"
#include "control/containers/SpeechAct.h"

#include <essentials/IDManager.h>

namespace control
{
AgentCommand ContainerUtils::toAgentCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    AgentCommand agentCommand;
    control::AgentCommandMsg::Reader reader = msg.getRoot<control::AgentCommandMsg>();
    agentCommand.receiverID = idManager->getIDFromBytes(
            reader.getReceiverId().getValue().asBytes().begin(), reader.getReceiverId().getValue().size(), (uint8_t) reader.getReceiverId().getType());
    agentCommand.cmd = reader.getCmd();
    return agentCommand;
}

void ContainerUtils::toMsg(AgentCommand agentCommand, ::capnp::MallocMessageBuilder& builder)
{
    AgentCommandMsg::Builder msg = builder.getRoot<AgentCommandMsg>();
    capnzero::ID::Builder receiverID = msg.initReceiverId();
    receiverID.setValue(kj::arrayPtr(agentCommand.receiverID->getRaw(), (unsigned int) agentCommand.receiverID->getSize()));
    receiverID.setType(agentCommand.receiverID->getType());
    msg.setCmd(agentCommand.cmd);
}

SpeechAct ContainerUtils::toSpeechAct(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    SpeechAct speechAct;

    SpeechActMsg::Reader reader = msg.getRoot<SpeechActMsg>();
    speechAct.senderID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
    speechAct.receiverID = idManager->getIDFromBytes(
            reader.getReceiverID().getValue().asBytes().begin(), reader.getReceiverID().getValue().size(), (uint8_t) reader.getReceiverID().getType());
    speechAct.actID = idManager->getIDFromBytes(
            reader.getActID().getValue().asBytes().begin(), reader.getActID().getValue().size(), (uint8_t) reader.getActID().getType());
    speechAct.previousActID = idManager->getIDFromBytes(
            reader.getPreviousActID().getValue().asBytes().begin(), reader.getPreviousActID().getValue().size(), (uint8_t) reader.getPreviousActID().getType());
    speechAct.text = std::string(reader.getText().cStr());
    speechAct.type = (SpeechType) reader.getSpeechType();

    return speechAct;
}

void ContainerUtils::toMsg(SpeechAct speechAct, ::capnp::MallocMessageBuilder& builder)
{
    SpeechActMsg::Builder msg = builder.getRoot<SpeechActMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(speechAct.senderID->getRaw(), (unsigned int) speechAct.senderID->getSize()));
    senderID.setType(speechAct.senderID->getType());

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(speechAct.receiverID->getRaw(), (unsigned int) speechAct.receiverID->getSize()));
    receiverID.setType(speechAct.receiverID->getType());

    capnzero::ID::Builder actID = msg.initActID();
    actID.setValue(kj::arrayPtr(speechAct.actID->getRaw(), (unsigned int) speechAct.actID->getSize()));
    actID.setType(speechAct.actID->getType());

    capnzero::ID::Builder previousID = msg.initPreviousActID();
    previousID.setValue(kj::arrayPtr(speechAct.previousActID->getRaw(), (unsigned int) speechAct.previousActID->getSize()));
    previousID.setType(speechAct.previousActID->getType());

    msg.setText(speechAct.text);
    msg.setSpeechType(speechAct.type);
}

} // namespace control