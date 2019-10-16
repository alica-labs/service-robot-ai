#include "srg/Robot.h"

#include "srg/robot/Movement.h"

#include <srg/SRGWorldModel.h>
#include <srgsim/containers/ContainerUtils.h>

#include <SystemConfig.h>
#include <capnzero/Publisher.h>
#include <engine/AlicaEngine.h>
#include <engine/teammanager/TeamManager.h>


#include <capnp/message.h>

namespace srg
{

Robot* Robot::getInstance()
{
    static Robot instance(srg::SRGWorldModel::getInstance());
    return &instance;
}

Robot::Robot(srg::SRGWorldModel* wm)
        : wm(wm), movement(new srg::robot::Movement(wm))
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
    send(sc);
}

void Robot::move(srgsim::Coordinate goal) const {
    auto ownCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoordinate.has_value()) {
        std::cerr << "Robot::move(): Not localised!" << std::endl;
        return;
    }

    robot::Path* path = this->movement->searchPath(ownCoordinate.value(), goal);
    std::cout << "Robot::move(): Result " << path->toString() << std::endl;
    srgsim::SimCommand sc;
    sc.senderID = this->id.get();
    switch (path->getDirection()) {
        case srgsim::Direction::Up:
            sc.action = srgsim::SimCommand::GOUP;
            break;
        case srgsim::Direction::Down:
            sc.action = srgsim::SimCommand::GODOWN;
            break;
        case srgsim::Direction::Right:
            sc.action = srgsim::SimCommand::GORIGHT;
            break;
        case srgsim::Direction::Left:
            sc.action = srgsim::SimCommand::GOLEFT;
            break;
        default:
            std::cerr << "Robot::move(): No movement necessary or found!" << std::endl;
            delete path;
            return;
    }
    std::cout << "Robot::move(): Moving " << sc.action << std::endl;
    delete path;
    send(sc);
}

void Robot::manipulate(essentials::IdentifierConstPtr objectID, srgsim::SimCommand::Action action) const {
    std::cout << "Robot::manipulate(): Not yet implemented!" << std::endl;
    srgsim::SimCommand sc;
    sc.senderID = this->id.get();
    sc.objectID = objectID;
    sc.action = action;
    send(sc);
}

void Robot::send(srgsim::SimCommand sc) const {
    ::capnp::MallocMessageBuilder msgBuilder;
    srgsim::ContainerUtils::toMsg(sc, msgBuilder);
    this->simPub->send(msgBuilder);
}

} // namespace srg
