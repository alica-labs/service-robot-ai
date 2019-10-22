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
            std::cerr << "Robot::move(): No movement necessary or found!" << std::endl;
            delete path;
            return;
    }
    std::cout << "Robot::move(): Moving " << sc.action << std::endl;
    delete path;
    send(sc);
}

void Robot::manipulate(std::string cmd) const {

    std::string actionString = cmd.substr(0, cmd.find(" "));
    std::string objectIdString;
    std::string xCoordString = "-1";
    std::string yCoordString = "-1";;
    size_t objectIdEnd = cmd.find(" ", actionString.length() + 1);
    if (objectIdEnd == std::string::npos) {
        objectIdString = cmd.substr(actionString.length() + 1);
    } else {
        objectIdString = cmd.substr(actionString.length() + 1, objectIdEnd - (actionString.length() + 1));
        xCoordString = cmd.substr(objectIdEnd + 1, cmd.find(","));
        yCoordString = cmd.substr(cmd.find(",") + 1);
    }

    std::cout << "Robot::manipulate(): Action: '" << actionString << "' ID: '" << objectIdString << "' xCoord: '" << xCoordString << "' yCoord: '" << yCoordString << "'" << std::endl;

    srgsim::SimCommand::Action action;

    if (actionString.compare("open") == 0) {
        action = srgsim::SimCommand::Action::OPEN;
    } else if (actionString.compare("close") == 0) {
        action = srgsim::SimCommand::Action::CLOSE;
    } else if (actionString.compare("pick") == 0) {
        action = srgsim::SimCommand::Action::PICKUP;
    } else if (actionString.compare("put") == 0) {
        action = srgsim::SimCommand::Action::PUTDOWN;
    } else {
        std::cout << "Robot::manipulate(): Current command is unknown: " << actionString << std::endl;
        return;
    }

    uint32_t idInt = std::stoi(objectIdString);
    essentials::IdentifierConstPtr objectID = this->wm->getEngine()->getId<uint32_t>(idInt);

    srgsim::Coordinate coord = srgsim::Coordinate(std::stoi(xCoordString), std::stoi(yCoordString));

    srgsim::SimCommand sc;
    sc.senderID = this->id.get();
    sc.objectID = objectID;
    sc.action = action;
    sc.x = coord.x;
    sc.y = coord.y;
    send(sc);
}

void Robot::send(srgsim::SimCommand sc) const {
    ::capnp::MallocMessageBuilder msgBuilder;
    srgsim::ContainerUtils::toMsg(sc, msgBuilder);
    this->simPub->send(msgBuilder);
}

} // namespace srg
