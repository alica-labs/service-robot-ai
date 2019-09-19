#pragma once

#include "srg/containers/SpeechAct.h"

#include <capnzero/CapnZero.h>

namespace essentials {
    class IDManager;
}

namespace srg {
    class ContainerUtils {
    public:
        static SpeechAct toSpeechAct(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(SpeechAct speechAct, ::capnp::MallocMessageBuilder& builder);

    private:
        ContainerUtils() = delete;
    };
}