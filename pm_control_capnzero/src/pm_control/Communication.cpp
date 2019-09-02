#include "pm_control/Communication.h"

#include "pm_control/PMControl.h"

#include <process_manager/containers/ContainerUtils.h>

#include <SystemConfig.h>

namespace pm_control {
    Communication::Communication(PMControl* control) {
        this->control = control;
        this->sc = essentials::SystemConfig::getInstance();

        // initialise capnzero stuff
        this->ctx = zmq_ctx_new();
        this->processStatsTopic = (*sc)["ProcessManaging"]->get<std::string>("Topics.processStatsTopic", NULL);
        this->processStatsSub = new capnzero::Subscriber(this->ctx, this->processStatsTopic, &Communication::handleProcessStats, &(*this));
        this->processStatsSub->addAddress(capnzero::CommType::UDP, "224.0.0.2:5555");
        this->processStatsSub->connect();

        this->processCommandTopic = (*sc)["RobotControl"]->get<std::string>("Topics.robotCommandTopic", NULL);
        this->processCommandPub = new capnzero::Publisher(this->ctx);
        this->processCommandPub->setDefaultGroup(this->processCommandTopic);
        this->processCommandPub->bind(capnzero::CommType::UDP, "224.0.0.2:5555");
    }

    void Communication::handleProcessStats(capnp::FlatArrayMessageReader& msg)
    {
        this->control->receiveProcessStats(process_manager::ContainerUtils::toProcessStats(msg));
    }

    void Communication::sendProcessCommand(process_manager::ProcessCommand pc) {
        capnp::MallocMessageBuilder builder;
        process_manager::ContainerUtils::toMsg(pc,builder);
        this->processCommandPub->send(builder);
    }
}
