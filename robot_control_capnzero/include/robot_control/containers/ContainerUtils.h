#pragma once

#include "robot_control/containers/RobotCommand.h"

#include <capnzero/CapnZero.h>

namespace essentials {
    class IDManager;
}

namespace robot_control {
    class ContainerUtils {
    public:
        static RobotCommand  toRobotCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
        static void toMsg(RobotCommand rc, ::capnp::MallocMessageBuilder& builder);
    private:
    private:
        ContainerUtils() = delete;
    };
}