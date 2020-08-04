#include "srg/agent/containers/ContainerUtils.h"

#include "srg/agent/SpeechActMsg.capnp.h"
#include "srg/agent/containers/SpeechAct.h"
#include "srg/agent/AgentCommandMsg.capnp.h"
#include "srg/agent/containers/AgentCommand.h"

#include <essentials/IDManager.h>
#include <srg/sim/ContainerUtils.h>

namespace srg
{
namespace agent
{
AgentCommand ContainerUtils::toAgentCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    AgentCommand agentCommand;
    AgentCommandMsg::Reader reader = msg.getRoot<AgentCommandMsg>();
    agentCommand.receiverID = idManager->getIDFromBytes(
            reader.getReceiverId().getValue().asBytes().begin(), reader.getReceiverId().getValue().size(), (uint8_t) reader.getReceiverId().getType());
    agentCommand.cmd = reader.getCmd();
    return agentCommand;
}

void ContainerUtils::toMsg(const AgentCommand& agentCommand, ::capnp::MallocMessageBuilder& builder)
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
    // TYPE
    switch (reader.getObjectRequestType()) {
        case srg::sim::PerceptionMsg::Object::Type::ROBOT:
            speechAct.objectRequestType = srg::world::ObjectType::Robot;
            break;
        case srg::sim::PerceptionMsg::Object::Type::HUMAN:
            speechAct.objectRequestType = srg::world::ObjectType::Human;
            break;
        case srg::sim::PerceptionMsg::Object::Type::DOOR:
            speechAct.objectRequestType = srg::world::ObjectType::Door;
            break;
        case srg::sim::PerceptionMsg::Object::Type::CUPRED:
            speechAct.objectRequestType = srg::world::ObjectType::CupRed;
            break;
        case srg::sim::PerceptionMsg::Object::Type::CUPBLUE:
            speechAct.objectRequestType = srg::world::ObjectType::CupBlue;
            break;
        case srg::sim::PerceptionMsg::Object::Type::CUPYELLOW:
            speechAct.objectRequestType = srg::world::ObjectType::CupYellow;
            break;
        case srg::sim::PerceptionMsg::Object::Type::UNKNOWN:
            speechAct.objectRequestType = srg::world::ObjectType::Unknown;
            break;
        default:
            std::cerr << "[ContainterUtils] Unknown object type in capnp message found!" << std::endl;
            break;
    }
    speechAct.perceptions = srg::sim::ContainerUtils::createPerceptions(reader.getPerception(), idManager);

    return speechAct;
}

void ContainerUtils::toMsg(const SpeechAct& speechAct, ::capnp::MallocMessageBuilder& builder)
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

    // TYPE
    switch (speechAct.objectRequestType) {
        case srg::world::ObjectType::Robot:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::ROBOT);
            break;
        case srg::world::ObjectType::Human:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::HUMAN);
            break;
        case srg::world::ObjectType::Door:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::DOOR);
            break;
        case srg::world::ObjectType::CupRed:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::CUPRED);
            break;
        case srg::world::ObjectType::CupBlue:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::CUPBLUE);
            break;
        case  srg::world::ObjectType::CupYellow:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::CUPYELLOW);
            break;
        case  srg::world::ObjectType::Unknown:
            msg.setObjectRequestType(srg::sim::PerceptionMsg::Object::Type::UNKNOWN);
            break;
        default:
            std::cerr << "[ContainterUtils] Unknown object type '" << speechAct.objectRequestType << "' in speech act found!" << std::endl;
            break;
    }

    ::srg::sim::PerceptionMsg::Builder perceptionsBuilder = msg.initPerception();
    srg::sim::ContainerUtils::toMsg(speechAct.perceptions, perceptionsBuilder);

    msg.setText(speechAct.text);
    msg.setSpeechType(speechAct.type);
}

} // namespace agent
} // namespace srg