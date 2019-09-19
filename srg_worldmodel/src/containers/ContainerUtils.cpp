#include "srg/containers/ContainerUtils.h"

#include <srg/SpeechActMsg.capnp.h>
#include <srg/containers/SpeechAct.h>
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace srg
{
    SpeechAct ContainerUtils::toSpeechAct(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
    {
        srg::SpeechAct speechAct;

        srg::SpeechActMsg::Reader reader = msg.getRoot<srg::SpeechActMsg>();
        speechAct.senderID = idManager->getIDFromBytes(
                reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
        speechAct.actID = idManager->getIDFromBytes(
                reader.getActID().getValue().asBytes().begin(), reader.getActID().getValue().size(), (uint8_t) reader.getActID().getType());
        speechAct.previousActID = idManager->getIDFromBytes(
                reader.getPreviousActID().getValue().asBytes().begin(), reader.getPreviousActID().getValue().size(), (uint8_t) reader.getPreviousActID().getType());
        speechAct.text = std::string(reader.getText().cStr());
        speechAct.type = (srg::SpeechType) reader.getSpeechType();

        return speechAct;
    }

    void ContainerUtils::toMsg(SpeechAct speechAct, ::capnp::MallocMessageBuilder& builder)
    {
        srg::SpeechActMsg::Builder msg = builder.getRoot<srg::SpeechActMsg>();

        capnzero::ID::Builder senderID = msg.initSenderID();
        senderID.setValue(kj::arrayPtr(speechAct.senderID->getRaw(), (unsigned int) speechAct.senderID->getSize()));
        senderID.setType(speechAct.senderID->getType());

        capnzero::ID::Builder actID = msg.initActID();
        actID.setValue(kj::arrayPtr(speechAct.actID->getRaw(), (unsigned int) speechAct.actID->getSize()));
        actID.setType(speechAct.actID->getType());

        capnzero::ID::Builder previousID = msg.initActID();
        previousID.setValue(kj::arrayPtr(speechAct.previousActID->getRaw(), (unsigned int) speechAct.previousActID->getSize()));
        previousID.setType(speechAct.previousActID->getType());

        msg.setText(speechAct.text);
        msg.setSpeechType(speechAct.type);
    }

} // namespace srg