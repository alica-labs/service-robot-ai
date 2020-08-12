#pragma once

#include <srg/agent/containers/SpeechAct.h>

#include <srg/sim/containers/Perceptions.h>

#include <srg/agent/containers/AgentCommand.h>
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
            const supplementary::InfoBuffer<agent::AgentCommand>& getAgentCmdBuffer();
            const supplementary::InfoBuffer<agent::SpeechAct>& getSpeechActBuffer();
            supplementary::InfoBuffer<agent::SpeechAct>& editSpeechActBuffer();
            const supplementary::InfoBuffer<srg::sim::containers::Perceptions>& getPerceptionsBuffer();
            supplementary::InfoBuffer<srg::sim::containers::Perceptions>& editPerceptionsBuffer();

            // Methods for processing Messages
            void processSpeechAct(agent::SpeechAct act);
            void processAgentCmd(agent::AgentCommand agentCmd);
            void processSimPerceptions(srg::sim::containers::Perceptions perceptions);

        private:
            SRGWorldModel* wm;

            alica::AlicaTime agentCmdValidityDuration;
            supplementary::InfoBuffer<agent::AgentCommand>* agentCmdBuffer;

            alica::AlicaTime speechActValidityDuration;
            supplementary::InfoBuffer<agent::SpeechAct>* speechActBuffer;

            alica::AlicaTime perceptionsValidityDuration;
            supplementary::InfoBuffer<srg::sim::containers::Perceptions>* perceptionsBuffer;
        };
    }
}
