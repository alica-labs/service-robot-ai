#pragma once

#include <cstdint>

namespace essentials {
    class Identifier;
}

namespace process_manager {
    struct ProcessStat {
        static const uint8_t PUBLISHING_OFF = 0;
        static const uint8_t PUBLISHING_ON = 1;

        const essentials::Identifier* robotID;
        int32_t  processKey;
        int32_t paramSet;
        uint8_t state;
        uint16_t cpu;
        uint32_t mem;
        uint8_t publishing;
    };
}