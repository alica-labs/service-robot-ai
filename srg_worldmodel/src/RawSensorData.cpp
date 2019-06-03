#include <supplementary/InfoBuffer.h>
#include "srg/wm/RawSensorData.h"
#include <srg/SRGWorldModel.h>
#include <memory>
#include <utility>


namespace srg {
    namespace wm {
        RawSensorData::RawSensorData(srg::SRGWorldModel* wm) {
            std::cout << "Creating RawSensorData\n";
            this->wm = wm;
            auto sc = this->wm->getSystemConfig();
            this->telegramMessageValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.TelegramMessage.ValidityDuration", NULL));
            this->telegramMessageBuffer = new supplementary::InfoBuffer<Message>((*sc)["SRGWorldmodel"]->get<int>("Data.TelegramMessage.BufferLength", NULL));
        }

        RawSensorData::~RawSensorData() {

        }

        void RawSensorData::processTelegramMessage(Message message) {
            auto messageInfo = std::make_shared<supplementary::InformationElement<Message>>(message, wm->getTime(), telegramMessageValidityDuration, 1.0);
        }
    }
}