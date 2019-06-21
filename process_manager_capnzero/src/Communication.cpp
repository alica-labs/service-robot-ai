#include "process_manager/Communication.h"
#include "process_manager/ProcessManager.h"
#include "process_manager/RobotExecutableRegistry.h"


#include <SystemConfig.h>
#include <essentials/WildcardID.h>
#include <process_manager/containers/ContainerUtils.h>

namespace process_manager
{
Communication::Communication(ProcessManager* processManager)
{
    this->processManager = processManager;

    // initialise ROS stuff
    this->ctx = zmq_ctx_new();

    this->processCmdTopic = (*sc)["ProcessManaging"]->get<std::string>("Topics.processCmdTopic", NULL);
    this->processCommandSub = new capnzero::Subscriber(this->ctx, this->processCmdTopic);
    this->processCommandSub->connect(capnzero::CommType::UDP, "224.0.0.2:5555");
    this->processCommandSub->subscribe(&Communication::handleProcessCommand, &(*this));

    this->processStatsTopic = (*sc)["ProcessManaging"]->get<std::string>("Topics.processStatsTopic", NULL);
    this->processStatePub = new capnzero::Publisher(this->ctx);
    this->processStatePub->bind(capnzero::CommType::UDP, "224.0.0.2:5555");
    this->processStatePub->setDefaultGroup(this->processStatsTopic);
}

/**
 * The callback of the ROS subscriber - it inits the message processing.
 * @param pc
 */
void Communication::handleProcessCommand(capnp::FlatArrayMessageReader& msg)
{
    std::cout << "Communication: ProcessCommand received..." << std::endl;
    ProcessCommand pc = ContainerUtils::toProcessCommand(msg, this->processManager->getPMRegistry());
    if (pc.receiverID != this->processManager->getOwnID() &&
        !(this->processManager->getSimMode() && dynamic_cast<const essentials::WildcardID*>(pc.receiverID))) {
        return;
    }

    switch (pc.cmd) {
    case process_manager::ProcessCommand::START:
        this->processManager->changeDesiredProcessStates(pc, true);
        break;
    case process_manager::ProcessCommand::STOP:
        this->processManager->changeDesiredProcessStates(pc, false);
        break;
    case process_manager::ProcessCommand::START_LOG_PUBLISHING:
        this->processManager->changeLogPublishing(pc, true);
        break;
    case process_manager::ProcessCommand::STOP_LOG_PUBLISHING:
        this->processManager->changeLogPublishing(pc, false);
        break;
    }
}

void Communication::sendProcessStats(process_manager::ProcessStats psts)
{
    ::capnp::MallocMessageBuilder builder;
    ContainerUtils::toMsg(psts, builder);
    this->processStatePub->send(builder);
}

} // namespace process_manager
