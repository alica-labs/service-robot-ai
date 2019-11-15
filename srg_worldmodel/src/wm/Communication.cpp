#include "srg/wm/Communication.h"

#include <srg/SRGWorldModel.h>

#include <Message.h>
#include <control/containers/ContainerUtils.h>
#include <srg/sim/ContainerUtils.h>

#include <control/SpeechActMsg.capnp.h>
#include <control/containers/ContainerUtils.h>

#include <engine/AlicaEngine.h>

#include <SystemConfig.h>
#include <capnzero/Common.h>
#include <capnzero/Subscriber.h>

namespace srg
{
namespace wm
{
Communication::Communication(SRGWorldModel* wm)
        : wm(wm)
{
    this->ctx = zmq_ctx_new();
    this->sc = essentials::SystemConfig::getInstance();

    std::string telegramMessageTopic = (*sc)["SRGWorldModel"]->get<std::string>("Data.Telegram.Topic", NULL);
    this->telegramMessageSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->telegramMessageSub->setTopic(telegramMessageTopic);
    this->telegramMessageSub->addAddress((*sc)["SRGWorldModel"]->get<std::string>("Data.Telegram.Address", NULL));
    this->telegramMessageSub->subscribe(&Communication::onTelegramMessage, &(*this));

    std::string speechActTopic = (*sc)["Talker"]->get<std::string>("topic", NULL);
    this->speechActSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->speechActSub->setTopic(speechActTopic);
    this->speechActSub->addAddress((*sc)["Talker"]->get<std::string>("address", NULL));
    this->speechActSub->subscribe(&Communication::onSpeechAct, &(*this));

    std::string agendCmdTopic = (*sc)["ControlPanel"]->get<std::string>("AgentCmd.topic", NULL);
    this->agentCommandSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->agentCommandSub->setTopic(agendCmdTopic);
    this->agentCommandSub->addAddress((*sc)["ControlPanel"]->get<std::string>("AgentCmd.address", NULL));
    this->agentCommandSub->subscribe(&Communication::onAgentCmd, &(*this));

    std::string perceptionTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.perceptionsTopic", NULL);
    this->agentCommandSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->agentCommandSub->setTopic(perceptionTopic);
    this->agentCommandSub->addAddress((*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL));
    this->agentCommandSub->subscribe(&Communication::onSimPerceptions, &(*this));
}

Communication::~Communication()
{
    delete this->telegramMessageSub;
    delete this->speechActSub;
}

void Communication::onTelegramMessage(capnp::FlatArrayMessageReader& msg)
{
    Message m;
    m.fromCapnp(msg);
    this->wm->rawSensorData.processTelegramMessage(m);
}

void Communication::onSpeechAct(capnp::FlatArrayMessageReader& msg)
{
    control::SpeechAct speechAct = control::ContainerUtils::toSpeechAct(msg, this->wm->getEngine()->getIdManager());
    if (speechAct.receiverID == this->wm->getOwnId() || speechAct.receiverID.get()->getType() == essentials::Identifier::WILDCARD_TYPE) {
        this->wm->rawSensorData.processSpeechAct(speechAct);
    }
}

void Communication::onAgentCmd(capnp::FlatArrayMessageReader& msg)
{
    this->wm->rawSensorData.processAgentCmd(control::ContainerUtils::toAgentCommand(msg, this->wm->getEngine()->getIdManager()));
}

void Communication::onSimPerceptions(capnp::FlatArrayMessageReader& msg)
{
    auto simPerceptions = srg::sim::ContainerUtils::toSimPerceptions(msg, this->wm->getEngine()->getIdManager());
    if (simPerceptions.receiverID == this->wm->getOwnId()) {
        this->wm->rawSensorData.processSimPerceptions(simPerceptions);
    }
}
} // namespace wm
} // namespace srg
