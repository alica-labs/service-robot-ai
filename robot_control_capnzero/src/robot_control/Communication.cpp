#include "robot_control/Communication.h"
#include "robot_control/RobotsControl.h"

#include <SystemConfig.h>

namespace robot_control {
    Communication::Communication(RobotsControl* control) {
        this->control = control;
        this->sc = essentials::SystemConfig::getInstance();
        std::string middleware = (*sc)["RobotControl"]->tryGet<std::string>("capnzero", "RobotControl.communication", NULL);
        if (middleware.compare("capnzero")) {

        } else if (middleware.compare("ros")) {
            // Initialise the ROS Communication
            rosNode = new ros::NodeHandle();
            processStateSub = rosNode->subscribe("/process_manager/ProcessStats", 10, &Communication::receiveProcessStats, (Communication*)this);
            alicaInfoSub = rosNode->subscribe("/AlicaEngine/AlicaEngineInfo", 10, &Communication::receiveAlicaInfo, (Communication*)this);
        } else {
            std::cerr << "Communication: Unknown middleware " << middleware << " requested!" << std::endl;
        }
    }

    void Communication::receiveProcessStats(process_manager::ProcessStatsConstPtr processStats)
    {
        this->control->addProcessStats();

        std::lock_guard<mutex> lck(processStatsMsgQueueMutex);
        this->processStatMsgQueue.emplace(std::chrono::system_clock::now(), processStats);
    }

    void Communication::receiveAlicaInfo(alica_msgs::AlicaEngineInfoConstPtr alicaInfo)
    {
        this->control->addAlicaInfo();

        lock_guard<mutex> lck(alicaInfoMsgQueueMutex);
        this->alicaInfoMsgQueue.emplace(std::chrono::system_clock::now(), alicaInfo);
    }
}
