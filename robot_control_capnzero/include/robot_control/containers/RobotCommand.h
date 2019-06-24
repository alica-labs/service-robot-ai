#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace robot_control {
    struct RobotCommand {
    public:
        // constants for cmd
        static const uint8_t START = 0;
        static const uint8_t STOP = 1;

        // ReceiverId determines the ProcessManager-Instanz, which is addressed by this message. 0 means every ProcessManager
        essentials::IdentifierConstPtr receiverID;

        // Cmd has to be set to one of the values above.
        uint8_t cmd;
    };
}