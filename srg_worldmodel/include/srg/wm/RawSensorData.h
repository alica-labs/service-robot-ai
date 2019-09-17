#pragma once

#include "srg/dialogue/SpeechAct.h"

#include <Message.h>
#include <control/containers/AgentCommand.h>
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
            void processSpeechAct(srg::dialogue::SpeechAct act);
            void processAgentCmd(control::AgentCommand agentCmd);

        private:
            SRGWorldModel* wm;

            alica::AlicaTime telegramMessageValidityDuration;
            supplementary::InfoBuffer<Message>* telegramMessageBuffer;

            alica::AlicaTime agentCmdValidityDuration;
            supplementary::InfoBuffer<control::AgentCommand>* agentCmdBuffer;

            alica::AlicaTime speechActValidityDuration;
            supplementary::InfoBuffer<srg::dialogue::SpeechAct>* speechActBuffer;
        };
    }
}
