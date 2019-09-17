#include "srg/Robot.h"

#include <SystemConfig.h>
#include <capnzero/Publisher.h>
#include <engine/AlicaEngine.h>
#include <engine/teammanager/TeamManager.h>
#include <srg/SRGWorldModel.h>

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
    capnp::MallocMessageBuilder msgBuilder;
    srgsim::Command::Builder commandBuilder = msgBuilder.initRoot<srgsim::Command>();
    commandBuilder.setAction(srgsim::Command::Action::SPAWN);
    capnzero::ID::Builder sender = commandBuilder.initSenderId();
    //    std::cout << "SenderID: " << this->id->getRaw() << std::endl;
    sender.setValue(kj::arrayPtr(this->id->getRaw(), this->id->getSize()));
    //    std::cout << commandBuilder.toString().flatten().cStr() << std::endl;
    this->simPub->send(msgBuilder);
}
} // namespace srg
