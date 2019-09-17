#include "srg/wm/Communication.h"

#include "srg/dialogue/SpeechAct.h"

#include <srg/SRGWorldModel.h>

#include <Message.h>
#include <srg/SpeechAct.capnp.h>
#include <control/containers/ContainerUtils.h>

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
        }

        Communication::~Communication() {
            delete this->telegramMessageSub;
            delete this->speechActSub;
        }

        void Communication::onTelegramMessage(capnp::FlatArrayMessageReader &msg) {
            std::cout << "onTelegramMessage called..." << std::endl;
            Message m;
            m.fromCapnp(msg);
            this->wm->rawSensorData.processTelegramMessage(m);
        }

        void Communication::onSpeechAct(capnp::FlatArrayMessageReader &msg) {
            srg::SpeechAct::Reader reader = msg.getRoot<srg::SpeechAct>();

            // fill container
            srg::dialogue::SpeechAct speechAct;
            speechAct.senderID = this->wm->getEngine()->getIDFromBytes(
                    reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
            speechAct.text = std::string(reader.getText().cStr());
            switch(reader.getSpeechType()) {
                case srg::SpeechType::INFORM:
                    speechAct.type = srg::dialogue::SpeechType::inform;
                    break;
                case srg::SpeechType::COMMAND:
                    speechAct.type = srg::dialogue::SpeechType::command;
                    break;
                case srg::SpeechType::REQUEST:
                    speechAct.type = srg::dialogue::SpeechType::request;
                    break;
                default:
                    std::cerr << "Communication: Unknown speach act type received!" << std::endl;
            }

            this->wm->rawSensorData.processSpeechAct(speechAct);
        }

        void Communication::onAgentCmd(capnp::FlatArrayMessageReader &msg) {
            this->wm->rawSensorData.processAgentCmd(control::ContainerUtils::toAgentCommand(msg, this->wm->getEngine()->getIdManager()));
        }
    }
}
