#include "srg/Robot.h"

#include <SystemConfig.h>
#include <capnzero/Publisher.h>
#include <engine/AlicaEngine.h>
#include <engine/teammanager/TeamManager.h>
#include <srg/SRGWorldModel.h>

#include <srgsim/containers/SimCommand.h>
#include <srgsim/containers/ContainerUtils.h>

#include <capnp/message.h>

namespace srg
{

Robot* Robot::getInstance()
{
    static Robot instance(srg::SRGWorldModel::getInstance());
    return &instance;
}

Robot::Robot(srg::SRGWorldModel* wm)
        : wm(wm)
{
    this->id = this->wm->getEngine()->getTeamManager()->getLocalAgentID();
    this->sc = essentials::SystemConfig::getInstance();
    this->simCmdTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.cmdTopic", NULL);
    this->simAddress = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL);

    this->capnzeroContext = zmq_ctx_new();
    this->simPub = new capnzero::Publisher(this->capnzeroContext, capnzero::Protocol::UDP);
    this->simPub->setDefaultTopic(this->simCmdTopic);
    this->simPub->addAddress(simAddress);
}

Robot::~Robot()
{
    delete simPub;
}

void Robot::spawn() const
{
    srgsim::SimCommand sc;
    sc.senderID = this->id.get();
    sc.action = srgsim::SimCommand::SPAWN;
    ::capnp::MallocMessageBuilder msgBuilder;
    srgsim::ContainerUtils::toMsg(sc, msgBuilder);
    this->simPub->send(msgBuilder);
}
} // namespace srg
