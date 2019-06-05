#pragma once

#include "srg/container/SpeechAct.h"

#include <Message.h>
#include <supplementary/InformationElement.h>

namespace supplementary{
    template <typename T> class InfoBuffer;
}

namespace srg {
    class SRGWorldModel;

    namespace wm {
        class RawSensorData {
        public:
            RawSensorData(srg::SRGWorldModel* wm);
            virtual ~RawSensorData();

            // Methods for processing Messages
            void processTelegramMessage(Message message);
            void processSpeechAct(srg::container::SpeechAct act);

        private:
            SRGWorldModel* wm;

            alica::AlicaTime telegramMessageValidityDuration;
            supplementary::InfoBuffer<Message>* telegramMessageBuffer;

            alica::AlicaTime speechActValidityDuration;
            supplementary::InfoBuffer<srg::container::SpeechAct>* speechActBuffer;
        };
    }
}
