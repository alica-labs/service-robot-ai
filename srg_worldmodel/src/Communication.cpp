#include "srg/wm/Communication.h"

#include "srg/container/SpeechAct.h"

#include <srg/SRGWorldModel.h>

#include <Message.h>
#include <srg/SpeechAct.capnp.h>

#include <engine/AlicaEngine.h>

#include <capnzero/Common.h>
#include <capnzero/Subscriber.h>
#include <SystemConfig.h>

namespace srg {
    namespace wm{
        Communication::Communication(SRGWorldModel *wm) : wm(wm) {
            this->ctx = zmq_ctx_new();
            this->sc = essentials::SystemConfig::getInstance();

            std::string telegramMessageTopic = (*sc)["SRGWorldModel"]->get<std::string>("Telegram.Topic");
            this->telegramMessageSub = new capnzero::Subscriber(this->ctx, telegramMessageTopic);
            this->telegramMessageSub->connect(capnzero::CommType::UDP, "224.0.0.2:5555");
            this->telegramMessageSub->subscribe(&Communication::onTelegramMessage, &(*this));

            std::string speechActTopic = (*sc)["SRGWorldModel"]->get<std::string>("SpeechAct.Topic");
            this->speechActSub = new capnzero::Subscriber(this->ctx, speechActTopic);
            this->speechActSub->connect(capnzero::CommType::UDP, "224.0.0.2:5555");
            this->speechActSub->subscribe(&Communication::onSpeechAct, &(*this));
        }

        Communication::~Communication() {
            zmq_ctx_term(this->ctx);

            delete this->telegramMessageSub;
            delete this->speechActSub;
        }

        void Communication::onTelegramMessage(capnp::FlatArrayMessageReader &msg) {
            Message m;
            m.fromCapnp(msg);
            this->wm->rawSensorData.processTelegramMessage(m);
        }

        void Communication::onSpeechAct(capnp::FlatArrayMessageReader &msg) {
            srg::SpeechAct::Reader reader = msg.getRoot<srg::SpeechAct>();

            // fill container
            srg::container::SpeechAct speechAct;
            speechAct.senderID = this->wm->getEngine()->getIDFromBytes(
                    reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), (uint8_t) reader.getSenderID().getType());
            speechAct.text = std::string(reader.getText().cStr());
            switch(reader.getSpeechType()) {
                case srg::SpeechType::INFORM:
                    speechAct.type = srg::container::SpeechType::inform;
                    break;
                case srg::SpeechType::COMMAND:
                    speechAct.type = srg::container::SpeechType::command;
                    break;
                case srg::SpeechType::REQUEST:
                    speechAct.type = srg::container::SpeechType::request;
                    break;
                default:
                    std::cerr << "Communication: Unknown speach act type received!" << std::endl;
            }

            this->wm->rawSensorData.processSpeechAct(speechAct);
        }
    }
}