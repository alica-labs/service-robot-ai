#pragma once

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

        private:
            SRGWorldModel* wm;
            supplementary::InfoBuffer<Message>* telegramMessageBuffer;
            alica::AlicaTime telegramMessageValidityDuration;
        };
    }
}
