#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <control/SpeechActMsg.capnp.h>
#include <control/containers/SpeechAct.h>

#include <essentials/Worker.h>
#include <capnzero/Subscriber.h>

#include <QWidget>

namespace capnzero {
    class Publisher;
}

namespace essentials{
    class IDManager;
    class SystemConfig;
}

namespace Ui
{
    class Talker;
}

namespace control {
    class ControlPanel;
    class Talker : public QObject {
    Q_OBJECT
    public:
        Talker(ControlPanel* controlPanel);
        ~Talker();
        void onSpeechAct(capnp::FlatArrayMessageReader &msg);

    public Q_SLOTS:
        void run();

    private:
        bool parseInput(std::string input, SpeechAct& speechAct);
        void send(SpeechAct speechAct) const;
        std::vector<std::string> split(std::string input);


        essentials::SystemConfig* sc;
        essentials::IDManager* idManager;
        essentials::IdentifierConstPtr id;
        ControlPanel* controlPanel;

        void* ctx;
        capnzero::Publisher* speechActPublisher;
        capnzero::Subscriber* speechActSubscriber;


    };
}