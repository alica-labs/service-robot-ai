#include "srg/Robot.h"

#include <capnzero/Publisher.h>
#include <SystemConfig.h>
#include <srg/SRGWorldModel.h>
#include <engine/AlicaEngine.h>
#include <engine/teammanager/TeamManager.h>

#include <capnp/message.h>

namespace srg
{

Robot::Robot(srg::SRGWorldModel* wm) : wm(wm)
{
    this->id = this->wm->getEngine()->getTeamManager()->getLocalAgentID();

    this->sc = essentials::SystemConfig::getInstance();
    this->simCmdTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.cmdTopic", NULL);
    this->simAddress = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL);

    this->capnzeroContext = zmq_ctx_new();
    this->simPub = new capnzero::Publisher(this->capnzeroContext);
    this->simPub->setDefaultGroup(this->simCmdTopic);
    this->simPub->bind(capnzero::CommType::UDP, simAddress);
}

Robot::~Robot()
{
    delete simPub;
}

void Robot::spawn() const
{
    builder:capnp::MallocMessageBuilder msgBuilder;
    srgsim::Command::Builder commandBuilder = msgBuilder.initRoot<srgsim::Command>();
    commandBuilder.setAction(srgsim::Command::Action::SPAWN);
    capnzero::ID::Builder sender = commandBuilder.initSenderId();
    sender.setValue(kj::arrayPtr(this->id->getRaw(), this->id->getSize()));
    commandBuilder.setSenderId(sender);
    this->simPub->send(msgBuilder);
}
} // namespace srg
