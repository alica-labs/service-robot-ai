#include "srg/wm/Communication.h"

#include <srg/SRGWorldModel.h>

#include <Message.h>
#include <srg/SpeechActMsg.capnp.h>
#include <control/containers/ContainerUtils.h>
#include <srgsim/containers/ContainerUtils.h>
#include <srg/containers/ContainerUtils.h>

#include <engine/AlicaEngine.h>

#include <capnzero/Common.h>
#include <capnzero/Subscriber.h>
#include <SystemConfig.h>

namespace srg {
    namespace wm{
        Communication::Communication(SRGWorldModel *wm) : wm(wm) {
            this->ctx = zmq_ctx_new();
            this->sc = essentials::SystemConfig::getInstance();

            std::string telegramMessageTopic = (*sc)["SRGWorldModel"]->get<std::string>("Data.Telegram.Topic", NULL);
            this->telegramMessageSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
            this->telegramMessageSub->setTopic(telegramMessageTopic);
            this->telegramMessageSub->addAddress((*sc)["SRGWorldModel"]->get<std::string>("Data.Telegram.Address", NULL));
            this->telegramMessageSub->subscribe(&Communication::onTelegramMessage, &(*this));

            std::string speechActTopic = (*sc)["SRGWorldModel"]->get<std::string>("Data.SpeechAct.Topic", NULL);
            this->speechActSub = new capnzero::Subscriber(this->ctx,capnzero::Protocol::UDP);
            this->speechActSub->setTopic(speechActTopic);
            this->speechActSub->addAddress((*sc)["SRGWorldModel"]->get<std::string>("Data.SpeechAct.Address", NULL));
            this->speechActSub->subscribe(&Communication::onSpeechAct, &(*this));

            std::string agendCmdTopic = (*sc)["ControlPanel"]->get<std::string>("AgentCmd.topic", NULL);
            this->agentCommandSub = new capnzero::Subscriber(this->ctx,capnzero::Protocol::UDP);
            this->agentCommandSub->setTopic(agendCmdTopic);
            this->agentCommandSub->addAddress((*sc)["ControlPanel"]->get<std::string>("AgentCmd.address", NULL));
            this->agentCommandSub->subscribe(&Communication::onAgentCmd, &(*this));

            std::string perceptionTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.perceptionsTopic", NULL);
            this->agentCommandSub = new capnzero::Subscriber(this->ctx,capnzero::Protocol::UDP);
            this->agentCommandSub->setTopic(perceptionTopic);
            this->agentCommandSub->addAddress((*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL));
            this->agentCommandSub->subscribe(&Communication::onSimPerceptions, &(*this));
        }

        Communication::~Communication() {
            delete this->telegramMessageSub;
            delete this->speechActSub;
        }

        void Communication::onTelegramMessage(capnp::FlatArrayMessageReader &msg) {
            Message m;
            m.fromCapnp(msg);
            this->wm->rawSensorData.processTelegramMessage(m);
        }

        void Communication::onSpeechAct(capnp::FlatArrayMessageReader &msg) {
            this->wm->rawSensorData.processSpeechAct(srg::ContainerUtils::toSpeechAct(msg, this->wm->getEngine()->getIdManager()));
        }

        void Communication::onAgentCmd(capnp::FlatArrayMessageReader &msg) {
            this->wm->rawSensorData.processAgentCmd(control::ContainerUtils::toAgentCommand(msg, this->wm->getEngine()->getIdManager()));
        }

        void Communication::onSimPerceptions(capnp::FlatArrayMessageReader &msg) {
            auto simPerceptions = srgsim::ContainerUtils::toSimPerceptions(msg, this->wm->getEngine()->getIdManager());
            if (simPerceptions.receiverID == this->wm->getOwnId()) {
                this->wm->rawSensorData.processSimPerceptions(simPerceptions);
            }
        }
    }
}
