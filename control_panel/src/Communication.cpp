#include "control/Communication.h"

#include "control/ControlPanel.h"
#include "control/containers/ContainerUtils.h"

#include <process_manager/containers/ContainerUtils.h>
#include <process_manager/ProcessStatsMsg.capnp.h>

#include <SystemConfig.h>

namespace control {
    Communication::Communication(ControlPanel* controlPanel) {
        this->controlPanel = controlPanel;
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

        this->processCommandTopic = (*sc)["RobotControl"]->get<std::string>("Topics.robotCommandTopic", NULL);
        this->processCommandPub = new capnzero::Publisher(this->ctx);
        this->processCommandPub->setDefaultGroup(this->processCommandTopic);
        this->processCommandPub->bind(capnzero::CommType::UDP, "224.0.0.2:5555");

        this->agentCommandTopic = (*sc)["RobotControl"]->get<std::string>("Topics.robotCommandTopic", NULL);
        this->agentCommandPub = new capnzero::Publisher(this->ctx);
        this->agentCommandPub->setDefaultGroup(this->agentCommandTopic);
        this->agentCommandPub->bind(capnzero::CommType::UDP, "224.0.0.2:5555");
    }

    void Communication::handleAlicaInfo(capnp::FlatArrayMessageReader& msg)
    {
        // TODO container utils for alica info container in alica capnzero proxy
//        this->controlPanel->enqueue(alica::ContainerUtils::(msg));
    }

    void Communication::handleProcessStats(capnp::FlatArrayMessageReader& msg)
    {
        std::cout << "control::Communication: " << msg.getRoot<process_manager::ProcessStatsMsg>().toString().flatten().cStr() << std::endl;
        this->controlPanel->enqueue(process_manager::ContainerUtils::toProcessStats(msg, this->controlPanel->getIDManager()));
    }

    void Communication::send(process_manager::ProcessCommand pc) {
        capnp::MallocMessageBuilder builder;
        process_manager::ContainerUtils::toMsg(pc,builder);
        this->processCommandPub->send(builder);
    }

    void Communication::send(AgentCommand agentCommand) {
        capnp::MallocMessageBuilder builder;
        control::ContainerUtils::toMsg(agentCommand,builder);
        this->agentCommandPub->send(builder);
    }
}
