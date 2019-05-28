#pragma once

#include <Message.h>
#include <supplementary/InfoBuffer.h>
#include <supplementary/InformationElement.h>


namespace srg {
    class SRGWorlModel;

    namespace wm {
        class RawSensorData {
        public:
            RawSensorData(SRGWorlModel* wm);
            virtual ~RawSensorData();

            // Methods for processing Messages
            void processTelegramMessage(Message message);

        private:
            SRGWorlModel* wm;
            supplementary::InfoBuffer<Message>* telegramMessageBuffer;
            
        };
    }
}
