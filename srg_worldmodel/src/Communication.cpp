#include "srg/wm/Communication.h"
#include <SRGWorldModel.h>

#include <capnzero/Subscriber.h>
#include <SystemConfig.h>

namespace srg {
    namespace wm{
        Communication::Communication(SRGWorldModel *wm) : wm(wm) {
            this->ctx = zmq_ctx_new();
            this->sc = essentials::SystemConfig::getInstance();
            std::string TelegramMessageTopic = (*sc)["wm-config"]->get<std::string>("Topics.TelegramMessageTopic");

            this->TelegramMessageSub = new capnzero::Subscriber(this->ctx, TelegramMessageTopic);

            this->TelegramMessageSub->connect(capnzero::CommType::UDP, "224.0.0.2:5555");

            this->TelegramMessageSub->subscribe(&srg::wm::Communication::onTelegramMessage, &(*this));
        }
        Communication::~Communication() {
            zmq_ctx_term(this->ctx);
            delete this->TelegramMessageSub;
        }
        void Communication::onTelegramMessage() {

        }
    }
}