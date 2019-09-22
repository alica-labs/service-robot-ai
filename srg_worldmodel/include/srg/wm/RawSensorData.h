#pragma once

#include <control/containers/SpeechAct.h>

#include <srgsim/containers/SimPerceptions.h>

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

            // Methods for accessing the buffers
            const supplementary::InfoBuffer<Message>& getTelegramMessageBuffer();
            const supplementary::InfoBuffer<control::AgentCommand>& getAgentCmdBuffer();
            const supplementary::InfoBuffer<control::SpeechAct>& getSpeechActBuffer();
            const supplementary::InfoBuffer<srgsim::SimPerceptions>& getPerceptionsBuffer();

            // Methods for processing Messages
            void processTelegramMessage(Message message);
            void processSpeechAct(control::SpeechAct act);
            void processAgentCmd(control::AgentCommand agentCmd);
            void processSimPerceptions(srgsim::SimPerceptions perceptions);

        private:
            SRGWorldModel* wm;

            alica::AlicaTime telegramMessageValidityDuration;
            supplementary::InfoBuffer<Message>* telegramMessageBuffer;

            alica::AlicaTime agentCmdValidityDuration;
            supplementary::InfoBuffer<control::AgentCommand>* agentCmdBuffer;

            alica::AlicaTime speechActValidityDuration;
            supplementary::InfoBuffer<control::SpeechAct>* speechActBuffer;

            alica::AlicaTime perceptionsValidityDuration;
            supplementary::InfoBuffer<srgsim::SimPerceptions>* perceptionsBuffer;
        };
    }
}
