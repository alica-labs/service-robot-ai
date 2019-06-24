#pragma once

#include <capnzero/CapnZero.h>
#include <essentials/IdentifierConstPtr.h>

namespace essentials
{
class SystemConfig;
}

namespace robot_control
{
class RobotsControl;

class Communication
{
public:
    Communication(RobotsControl* control);

    // capnzero call backs
    void handleProcessStats(capnp::FlatArrayMessageReader& msg);
    void handleAlicaInfo(capnp::FlatArrayMessageReader& msg);
    void sendRobotCommand(bool start, essentials::IdentifierConstPtr id);

private:
    essentials::SystemConfig* sc;
    RobotsControl* control;

    // capnzero stuff
    void* ctx;
    capnzero::Subscriber* processStatsSub;
    std::string processStatsTopic;
    capnzero::Subscriber* alicaInfoSub;
    std::string alicaInfoTopic;

    capnzero::Publisher* robotCommandPub;
    std::string robotCommandTopic;

};
} // namespace robot_control
