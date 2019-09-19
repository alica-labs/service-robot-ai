#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/SpeechActMsg.capnp.h>
#include <srg/containers/SpeechAct.h>

#include <essentials/Worker.h>
#include <capnzero/Subscriber.h>

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
        void onSpeechAct(capnp::FlatArrayMessageReader &msg);

    private:
        SpeechAct* parseInput(std::string input);
        void send(SpeechAct* speechAct) const;

        essentials::SystemConfig* sc;
        essentials::IDManager* idManager;
        essentials::IdentifierConstPtr id;

        void* ctx;
        capnzero::Publisher* speechActPublisher;
        capnzero::Subscriber* speechActSubscriber;
    };
}