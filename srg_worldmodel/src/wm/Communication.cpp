#include "srg/wm/Communication.h"

#include <srg/SRGWorldModel.h>

#include <srg/agent/SpeechActMsg.capnp.h>
#include <srg/agent/containers/ContainerUtils.h>
#include <srg/sim/ContainerUtils.h>

#include <engine/AlicaContext.h>

#include <capnzero/Common.h>
#include <capnzero/Subscriber.h>

namespace srg
{
namespace wm
{
Communication::Communication(SRGWorldModel* wm)
        : wm(wm)
        , sc(essentials::SystemConfig::getInstance())
{
    this->ctx = zmq_ctx_new();

    std::string speechActTopic = sc["Voice"]->get<std::string>("SpeechAct.topic", NULL);
    this->speechActSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->speechActSub->setTopic(speechActTopic);
    this->speechActSub->addAddress(sc["Voice"]->get<std::string>("SpeechAct.address", NULL));
    this->speechActSub->setReceiveQueueSize(1000);
    this->speechActSub->subscribe(&Communication::onSpeechAct, &(*this));

    this->speechActPub = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->speechActPub->setDefaultTopic(speechActTopic);
    this->speechActPub->addAddress(sc["Voice"]->get<std::string>("SpeechAct.address", NULL));
    this->speechActPub->setSendQueueSize(1000);

    std::string agendCmdTopic = sc["Voice"]->get<std::string>("AgentCmd.topic", NULL);
    this->agentCommandSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->agentCommandSub->setTopic(agendCmdTopic);
    this->agentCommandSub->addAddress(sc["Voice"]->get<std::string>("AgentCmd.address", NULL));
    this->agentCommandSub->setReceiveQueueSize(1000);
    this->agentCommandSub->subscribe(&Communication::onAgentCmd, &(*this));

    std::string perceptionTopic = sc["SRGSim"]->get<std::string>("SRGSim.Communication.perceptionsTopic", NULL);
    this->perceptionSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->perceptionSub->setTopic(perceptionTopic);
    this->perceptionSub->addAddress(sc["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL));
    this->perceptionSub->setReceiveQueueSize(1000);
    this->perceptionSub->subscribe(&Communication::onSimPerceptions, &(*this));

}

Communication::~Communication()
{
    delete this->speechActSub;
    delete this->perceptionSub;
    delete this->agentCommandSub;
}

void Communication::onSpeechAct(capnp::FlatArrayMessageReader& msg)
{
    agent::SpeechAct speechAct = agent::ContainerUtils::toSpeechAct(msg, const_cast<essentials::IDManager&>(this->wm->getAlicaContext()->getIDManager()));
    if (speechAct.receiverID == this->wm->getOwnId() || speechAct.receiverID.get()->getType() == essentials::Identifier::WILDCARD_TYPE) {
        this->wm->rawSensorData.processSpeechAct(speechAct);
    }
}

void Communication::onAgentCmd(capnp::FlatArrayMessageReader& msg)
{
    this->wm->rawSensorData.processAgentCmd(agent::ContainerUtils::toAgentCommand(msg, const_cast<essentials::IDManager&>(this->wm->getAlicaContext()->getIDManager())));
}

void Communication::onSimPerceptions(capnp::FlatArrayMessageReader& msg)
{
    auto simPerceptions = srg::sim::ContainerUtils::toPerceptions(msg, const_cast<essentials::IDManager&>(this->wm->getAlicaContext()->getIDManager()));
    if (simPerceptions.receiverID == this->wm->getOwnId()) {
        std::chrono::duration<double, std::milli> sendTime = std::chrono::system_clock::now().time_since_epoch() - simPerceptions.timestamp;
        if (sendTime > std::chrono::milliseconds (15)) {
            std::cerr << "[Communication] SimPerceptions took " << sendTime.count() << "ms" << std::endl;
        }
        this->wm->rawSensorData.processSimPerceptions(simPerceptions);
    }
}

void Communication::sendSpeechAct(std::shared_ptr<const srg::agent::SpeechAct> speechAct) const
{
    ::capnp::MallocMessageBuilder msgBuilder;
    srg::agent::ContainerUtils::toMsg(*speechAct, msgBuilder);
    this->speechActPub->send(msgBuilder);
}

} // namespace wm
} // namespace srg
