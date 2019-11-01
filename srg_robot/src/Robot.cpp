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
    sc.objectID = this->id.get();
    sc.action = srgsim::SimCommand::SPAWN;
    send(sc);
}

/**
 * Plans a path, and send a move command to the simulator for making the first step.
 * @param goal
 * @return True, if movement is necessary. False, otherwise.
 */
bool Robot::move(srgsim::Coordinate goal) const {
    auto ownCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoordinate.has_value()) {
        std::cerr << "Robot::move(): Not localised!" << std::endl;
        return true;
    }

    robot::Path* path = this->movement->searchPath(ownCoordinate.value(), goal);
    std::cout << "Robot::move(): Result " << path->toString() << std::endl;
    srgsim::SimCommand sc;
    sc.senderID = this->id.get();
    sc.objectID = this->id.get();
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
            std::cout << "Robot::move(): No movement necessary or found!" << std::endl;
            delete path;
            return false;
    }
    std::cout << "Robot::move(): Moving " << sc.action << std::endl;
    delete path;
    send(sc);
    return true;
}

void Robot::manipulate(const srg::dialogue::ManipulationTask* task) const {
    srgsim::SimCommand sc;
    switch (task->type) {
        case srgsim::TaskType::Open:
            sc.action = srgsim::SimCommand::Action::OPEN;
            break;
        case srgsim::TaskType::Close:
            sc.action = srgsim::SimCommand::Action::CLOSE;
            break;
        case srgsim::TaskType::PickUp:
            sc.action = srgsim::SimCommand::Action::PICKUP;
            break;
        case srgsim::TaskType::PutDown:
            sc.action = srgsim::SimCommand::Action::PUTDOWN;
            break;
        default:
            return;
    }

    sc.senderID = this->id.get();
    sc.objectID = task->objectID;
    sc.x = task->coordinate.x;
    sc.y = task->coordinate.y;
    send(sc);
}

void Robot::send(srgsim::SimCommand sc) const {
    ::capnp::MallocMessageBuilder msgBuilder;
    srgsim::ContainerUtils::toMsg(sc, msgBuilder);
    this->simPub->send(msgBuilder);
}

} // namespace srg
