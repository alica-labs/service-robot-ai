#pragma once

#include <capnzero/CapnZero.h>
#include <SystemConfig.h>

namespace srg{
    class SRGWorldModel;

    namespace wm{
        class Communication{
        public:
            Communication(SRGWorldModel *wm);
            virtual ~Communication();

        private:
            SRGWorldModel *wm;
            essentials::SystemConfig *sc;

            void* ctx;
            capnzero::Subscriber *telegramMessageSub;
            capnzero::Subscriber *speechActSub;

            void onTelegramMessage(capnp::FlatArrayMessageReader &msg);
            void onSpeechAct(capnp::FlatArrayMessageReader &msg);
        };
    }
}