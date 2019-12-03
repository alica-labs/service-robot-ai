#include "srg/Agent.h"

#include "srg/agent/Movement.h"
#include "srg/agent/Voice.h"

#include <srg/SRGWorldModel.h>
#include <srg/sim/ContainerUtils.h>
#include <srg/tasks/Task.h>
#include <srg/tasks/TaskType.h>

#include <SystemConfig.h>
#include <capnzero/Publisher.h>
#include <engine/AlicaEngine.h>
#include <engine/teammanager/TeamManager.h>

#include <capnp/message.h>

namespace srg
{

Agent* Agent::getInstance()
{
    static Agent instance(srg::SRGWorldModel::getInstance());
    return &instance;
}

Agent::Agent(srg::SRGWorldModel* wm)
        : wm(wm)
        , movement(new srg::agent::Movement(wm))

{
    this->id = this->wm->getEngine()->getTeamManager()->getLocalAgentID();
    this->voice = new srg::agent::Voice(this->id, this->wm->getEngine()->getIdManager());

    this->sc = essentials::SystemConfig::getInstance();
    this->simCmdTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.cmdTopic", NULL);
    this->simAddress = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL);

    this->capnzeroContext = zmq_ctx_new();
    this->simPub = new capnzero::Publisher(this->capnzeroContext, capnzero::Protocol::UDP);
    this->simPub->setDefaultTopic(this->simCmdTopic);
    this->simPub->addAddress(simAddress);
}

Agent::~Agent()
{
    delete simPub;
}

void Agent::spawn() const
{
    srg::sim::containers::SimCommand sc;
    sc.senderID = this->id.get();
    sc.objectID = this->id.get();
    sc.action = srg::sim::containers::SimCommand::SPAWN;
    send(sc);
}

void Agent::speak(std::string input) const
{
    this->voice->speak(input);
}

void Agent::speak(srg::agent::SpeechAct sa) const
{
    this->voice->speak(sa);
}

int32_t Agent::getPathCost(srg::world::Coordinate goal) const
{
    auto ownCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoordinate.has_value()) {
        std::cerr << "[Agent] Not localised!" << std::endl;
        return true;
    }

    agent::Path* path = this->movement->searchPath(ownCoordinate.value(), goal);
    if (path) {
        return path->getTotalCosts();
    } else {
        return -1;
    }
}

/**
 * Plans a path, and send a move command to the simulator for making the first step.
 * @param goal
 * @return True, if movement is necessary. False, otherwise.
 */
bool Agent::move(srg::world::Coordinate goal) const
{
    auto ownCoordinate = this->wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoordinate.has_value()) {
        std::cerr << "[Agent] Not localised!" << std::endl;
        return true;
    }

    agent::Path* path = this->movement->searchPath(ownCoordinate.value(), goal);
    //    std::cout << "Agent::move(): Result " << *path << std::endl;
    srg::sim::containers::SimCommand sc;
    sc.senderID = this->id.get();
    sc.objectID = this->id.get();
    switch (path->getDirection()) {
    case srg::world::Direction::Up:
        sc.action = srg::sim::containers::SimCommand::GOUP;
        break;
    case srg::world::Direction::Down:
        sc.action = srg::sim::containers::SimCommand::GODOWN;
        break;
    case srg::world::Direction::Right:
        sc.action = srg::sim::containers::SimCommand::GORIGHT;
        break;
    case srg::world::Direction::Left:
        sc.action = srg::sim::containers::SimCommand::GOLEFT;
        break;
    default:
        std::cout << "Agent::move(): No movement necessary or found!" << std::endl;
        delete path;
        return false;
    }
    //    std::cout << "Agent::move(): OwnPos: " << ownCoordinate.value() << " Moving " << path->getDirection() << std::endl;
    delete path;
    send(sc);
    return true;
}

void Agent::manipulate(const srg::tasks::Task* task) const
{
    srg::sim::containers::SimCommand sc;
    switch (task->type) {
    case srg::tasks::TaskType::Open:
        sc.action = srg::sim::containers::SimCommand::Action::OPEN;
        break;
    case srg::tasks::TaskType::Close:
        sc.action = srg::sim::containers::SimCommand::Action::CLOSE;
        break;
    case srg::tasks::TaskType::PickUp:
        sc.action = srg::sim::containers::SimCommand::Action::PICKUP;
        break;
    case srg::tasks::TaskType::PutDown:
        sc.action = srg::sim::containers::SimCommand::Action::PUTDOWN;
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

void Agent::send(srg::sim::containers::SimCommand sc) const
{
    ::capnp::MallocMessageBuilder msgBuilder;
    srg::sim::ContainerUtils::toMsg(sc, msgBuilder);
    this->simPub->send(msgBuilder);
}

} // namespace srg
