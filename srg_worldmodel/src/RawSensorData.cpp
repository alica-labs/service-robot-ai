#include "srg/wm/RawSensorData.h"

#include "srg/SRGWorldModel.h"

#include <supplementary/InfoBuffer.h>
#include <SystemConfig.h>

#include <memory>
#include <utility>

namespace srg {
    namespace wm {
        RawSensorData::RawSensorData(srg::SRGWorldModel* wm) {
            std::cout << "RawSensorData: Creating RawSensorData" << std::endl;
            this->wm = wm;
            auto sc = essentials::SystemConfig::getInstance();
            this->telegramMessageValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.Telegram.ValidityDuration", NULL));
            this->telegramMessageBuffer = new supplementary::InfoBuffer<Message>((*sc)["SRGWorldModel"]->get<int>("Data.Telegram.BufferLength", NULL));

            this->speechActValidityDuration = alica::AlicaTime::nanoseconds((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.ValidityDuration", NULL));
            this->speechActBuffer = new supplementary::InfoBuffer<srg::container::SpeechAct>((*sc)["SRGWorldModel"]->get<int>("Data.SpeechAct.BufferLength", NULL));
        }

        RawSensorData::~RawSensorData() {

        }

        void RawSensorData::processTelegramMessage(Message message) {
            std::cout << "RawSensorData: processTelegramMessage called" << std::endl;
            auto messageInfo = std::make_shared<supplementary::InformationElement<Message>>(message, wm->getTime(), telegramMessageValidityDuration, 1.0);
            telegramMessageBuffer->add(messageInfo);
        }

        void RawSensorData::processSpeechAct(srg::container::SpeechAct act) {
            std::cout << "RawSensorData: processSpeechAct called" << std::endl;
            auto speechActInfo = std::make_shared<supplementary::InformationElement<srg::container::SpeechAct>>(act, wm->getTime(), speechActValidityDuration, 1.0);
            speechActBuffer->add(speechActInfo);
            auto stuff = this->wm->basicHumanNeeds.answerNeed(act.text);
            for (std::string answer : stuff) {
                std::cout << "RawSensorData: " << answer << std::endl;
            }
        }
    }
}