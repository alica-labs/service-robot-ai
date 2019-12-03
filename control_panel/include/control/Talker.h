#pragma once

#include <essentials/IdentifierConstPtr.h>
#include <srg/agent/SpeechActMsg.capnp.h>
#include <srg/agent/Voice.h>
#include <srg/agent/containers/SpeechAct.h>


#include <QWidget>
#include <capnp/serialize.h>

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
        essentials::SystemConfig* sc;
        essentials::IDManager* idManager;
        essentials::IdentifierConstPtr id;
        ControlPanel* controlPanel;
        srg::agent::Voice* voice;
    };
}