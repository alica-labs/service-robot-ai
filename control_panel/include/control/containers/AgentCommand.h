#pragma once

#include <essentials/IdentifierConstPtr.h>

namespace control {
    struct AgentCommand {
    public:
        // constants for cmd
        static const uint8_t START = 0;
        static const uint8_t STOP = 1;

        // ReceiverId determines the Agent-Instanz, which is addressed by this message. WildcardID means every Agent
        essentials::IdentifierConstPtr receiverID;

        // Cmd has to be set to one of the values above.
        uint8_t cmd;
    };
}