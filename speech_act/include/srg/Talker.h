#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/SpeechAct.capnp.h>

#include <essentials/Worker.h>

namespace capnzero {
    class Publisher;
}

namespace essentials{
    class IDManager;
    class SystemConfig;
}

namespace srg {
    class Talker : public essentials::Worker {
    public:
        Talker();
        ~Talker();

        void run();
        void send(srg::SpeechType speechType, const std::string& text) const;

        static void s_signal_handler(int signal_value);
        static bool operating;

    private:

        essentials::SystemConfig* sc;
        essentials::IDManager* idManager;
        essentials::IdentifierConstPtr id;

        void* ctx;
        std::string url;
        std::string topic;
        capnzero::Publisher* speechActPublisher;
    };
}