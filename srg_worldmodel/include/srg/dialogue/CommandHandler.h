#pragma once

#include "srg/SRGWorldModel.h"
#include "srg/containers/SpeechAct.h"

namespace essentials{
    class SystemConfig;
}

namespace srg {
    namespace dialogue {
        class CommandHandler {
        public:
            CommandHandler(SRGWorldModel* wm);

            void processCommandAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> commandAct);
            const supplementary::InfoBuffer<srg::SpeechAct>& getCommandActBuffer();

        private:
            srg::SRGWorldModel* wm;
            essentials::SystemConfig* sc;
            alica::AlicaTime commandActValidityDuration;
            supplementary::InfoBuffer<srg::SpeechAct>* commandActBuffer;
        };
    }
}
