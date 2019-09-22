#pragma once

#include "srg/SRGWorldModel.h"
#include <control/containers/SpeechAct.h>

namespace essentials{
    class SystemConfig;
}

namespace srg {
    namespace dialogue {
        class CommandHandler {
        public:
            CommandHandler(SRGWorldModel* wm);

            void processCommandAct(std::shared_ptr<supplementary::InformationElement<control::SpeechAct>> commandAct);
            const supplementary::InfoBuffer<control::SpeechAct>& getCommandActBuffer();

        private:
            srg::SRGWorldModel* wm;
            essentials::SystemConfig* sc;
            alica::AlicaTime commandActValidityDuration;
            supplementary::InfoBuffer<control::SpeechAct>* commandActBuffer;
        };
    }
}
