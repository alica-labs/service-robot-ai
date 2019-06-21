#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <vector>

namespace process_manager {
    /**
     * Example: Hairy's ProcessManager should stop the processes with the Id 1, 2 and 3, if they run with ROBOT=nase
     * receiverId = 8, robotId = [9], cmd = 1, processKeys = [1,2,3]
     */

    struct ProcessCommand {
        // constants for cmd
        static const uint8_t START=0;
        static const uint8_t STOP = 1;
        static const uint8_t START_LOG_PUBLISHING = 2;
        static const uint8_t STOP_LOG_PUBLISHING = 3;

        // Cmd has to be set to one of the values above.
        uint8_t cmd;

        // ReceiverId determines the ProcessManager-Instanz, which is addressed by this message. 0 means every ProcessManager
        const essentials::Identifier* receiverID;

        // RobotId determines the ROBOT-Environment Variable
        std::vector<const essentials::Identifier*> robotIDs;

        // Vector of process IDs from the Processes.conf file.
        std::vector<int32_t> processKeys;

        // Vector of paramSet IDs from the Processes.conf file.
        std::vector<int32_t> paramSets;
    };
}