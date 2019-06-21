#pragma once

#include <alica_msgs/AlicaEngineInfo.h>
#include <process_manager/ProcessStats.h>

#include <ros/macros.h>
#include <ros/ros.h>

namespace essentials {
    class SystemConfig;
}

namespace robot_control
{
class RobotsControl;
class Communication
{
public:
    Communication(RobotsControl* control);

    // ros call backs
    void receiveProcessStats(process_manager::ProcessStatsConstPtr processStats);
    void receiveAlicaInfo(alica_msgs::AlicaEngineInfoConstPtr alicaInfo);

private:
    RobotsControl* control;
    essentials::SystemConfig* sc;

    // ros stuff
    ros::NodeHandle* rosNode;
    ros::Subscriber processStateSub;
    ros::Subscriber alicaInfoSub;

    // capnzero stuff
};
} // namespace robot_control
