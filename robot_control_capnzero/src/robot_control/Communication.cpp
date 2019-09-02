#include "robot_control/Communication.h"
#include "robot_control/RobotsControl.h"
#include "robot_control/containers/RobotCommand.h"
#include "robot_control/containers/ContainerUtils.h"

#include <process_manager/containers/ContainerUtils.h>

#include <SystemConfig.h>

namespace robot_control {
    Communication::Communication(RobotsControl* control) {
        this->control = control;
        this->sc = essentials::SystemConfig::getInstance();

        // initialise capnzero stuff
        this->ctx = zmq_ctx_new();
        this->processStatsTopic = (*sc)["ProcessManaging"]->get<std::string>("Topics.processStatsTopic", NULL);
        this->processStatsSub = new capnzero::Subscriber(this->ctx, this->processStatsTopic, &Communication::handleProcessStats, &(*this));
        this->processStatsSub->addAddress(capnzero::CommType::UDP, "224.0.0.2:5555");
        this->processStatsSub->connect();

        this->alicaInfoTopic = (*sc)["AlicaCapnzProxy"]->get<std::string>("Topics.alicaEngineInfoTopic", NULL);
        this->alicaInfoSub = new capnzero::Subscriber(this->ctx, this->alicaInfoTopic, &Communication::handleAlicaInfo, &(*this));
        this->alicaInfoSub->addAddress(capnzero::CommType::UDP, "224.0.0.2:5555");
        this->alicaInfoSub->connect();

        this->robotCommandTopic = (*sc)["RobotControl"]->get<std::string>("Topics.robotCommandTopic", NULL);
        this->robotCommandPub = new capnzero::Publisher(this->ctx, this->robotCommandTopic);
        this->robotCommandPub->bind(capnzero::CommType::UDP, "224.0.0.2:5555");
    }

    void Communication::handleProcessStats(capnp::FlatArrayMessageReader& msg)
    {
        this->control->addProcessStats(process_manager::ContainerUtils::toProcessStats(msg));
    }

    void Communication::handleAlicaInfo(capnp::FlatArrayMessageReader& msg)
    {
        this->control->addAlicaInfo();

//        lock_guard<mutex> lck(alicaInfoMsgQueueMutex);
//        this->alicaInfoMsgQueue.emplace(std::chrono::system_clock::now(), alicaInfo);
    }

    void Communication::sendRobotCommand(bool start, essentials::IdentifierConstPtr id) {
        RobotCommand rc;
        rc.receiverID = id;
        if (start) {
            rc.cmd = RobotCommand::START;
        } else {
            rc.cmd = RobotCommand::STOP;
        }
        capnp::MallocMessageBuilder builder;
        ContainerUtils::toMsg(rc,builder);
        this->robotCommandPub->send(builder);
    }
}
