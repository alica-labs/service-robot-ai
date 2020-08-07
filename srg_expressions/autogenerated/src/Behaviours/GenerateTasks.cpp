#include "Behaviours/GenerateTasks.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1575291385685) ENABLED START*/
#include <srg/Agent.h>
#include <srg/world/Room.h>
#include <srg/world/RoomType.h>
#include <engine/AlicaContext.h>
#include <engine/teammanager/TeamManager.h>
#include <engine/teammanager/Agent.h>
#include <sstream>
/*PROTECTED REGION END*/

namespace alica
{
/*PROTECTED REGION ID(staticVars1575291385685) ENABLED START*/
// initialise static variables here
/*PROTECTED REGION END*/

GenerateTasks::GenerateTasks()
        : DomainBehaviour("GenerateTasks")
{
    /*PROTECTED REGION ID(con1575291385685) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
GenerateTasks::~GenerateTasks()
{
    /*PROTECTED REGION ID(dcon1575291385685) ENABLED START*/
    // Add additional options here
    /*PROTECTED REGION END*/
}
void GenerateTasks::run(void* msg)
{
    /*PROTECTED REGION ID(run1575291385685) ENABLED START*/
    if (this->isSuccess()) {
        return;
    }

    if (sentCounter < this->speechActs.size()) {
        this->agent->speak(this->speechActs[sentCounter++]);
    } else {
        this->setSuccess();
    }
    /*PROTECTED REGION END*/
}
void GenerateTasks::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1575291385685) ENABLED START*/
    this->speechActs.clear();
    generateHumanKnowledge();
    this->sentCounter = 0;
    generateSpeechActs(1000);
    // debug
    for (srg::agent::SpeechAct sa : this->speechActs) {
        std::cout << "[GenerateTasks] " << sa << std::endl;
    }

    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1575291385685) ENABLED START*/
void GenerateTasks::generateHumanKnowledge()
{
    srg::agent::SpeechAct sa;
    sa.senderID = this->wm->getOwnId();
    sa.actID = this->wm->getAlicaContext()->generateID();
    sa.previousActID = this->wm->getAlicaContext()->getIDManager().getWildcardID();
    sa.type = srg::agent::SpeechType::inform;
    sa.objectRequestType = srg::world::ObjectType::Unknown;

    // receiver ID
    auto agents = this->wm->getAlicaContext()->getActiveAgents();
    const Agent* agent;
    do {
        ActiveAgentIterator agentIter = agents.begin();
        std::advance(agentIter, rand() % agents.size());
    } while (agent->getId() == this->wm->getOwnId());
    sa.receiverID = agent->getId();
    sa.perceptions.receiverID = sa.receiverID;

    // knowledge text
    sa.text = "cs_AtLocation(cup,office,450).";

    this->speechActs.push_back(sa);
}

void GenerateTasks::generateSpeechActs(int numberOfTasks)
{
    for (int i = 0; i < numberOfTasks; i++) {
        srg::agent::SpeechAct sa;
        sa.senderID = this->wm->getOwnId();
        sa.actID = this->wm->getAlicaContext()->generateID();
        sa.previousActID = this->wm->getAlicaContext()->getIDManager().getWildcardID();
        sa.type = srg::agent::SpeechType::command;
        sa.objectRequestType = srg::world::ObjectType::Unknown;

        // receiver ID
        auto agents = this->wm->getAlicaContext()->getActiveAgents();
        const Agent* agent;
        do {
            ActiveAgentIterator agentIter = agents.begin();
            std::advance(agentIter, rand() % agents.size());
            agent = *agentIter;
        } while (agent->getId() == this->wm->getOwnId());
        sa.receiverID = agent->getId();
        sa.perceptions.receiverID = sa.receiverID;

        // task text
        sa.text = "transport ";

        sa.text += getRandomObjectString() + " ";

        srg::world::Coordinate coordinate = getRandomCoordinate();
        std::stringstream ss(" ");
        ss << coordinate;
        sa.text += ss.str();

        this->speechActs.push_back(sa);
    }
}

std::string GenerateTasks::getRandomObjectString()
{
    int idx = rand() % 3;
    if (idx == 0) {
        return "CupYellow";
    } else if (idx == 1) {
        return "CupRed";
    } else if (idx == 2) {
        return "CupBlue";
    }
    std::cerr << "[GenerateTasks] Unknown random object string! " << idx << std::endl;
    return "Unknown";
}

const srg::world::Coordinate GenerateTasks::getRandomCoordinate()
{
    // statistic evaluation of room
    int randRoomValue = rand() % 100;
    std::vector<srg::world::Room*> rooms;
    if (randRoomValue < 30) {
        // kitchen
        rooms = this->wm->sRGSimData.getWorld()->getRooms(srg::world::RoomType::Kitchen);
    } else if (randRoomValue < 80) {
        // office
        rooms = this->wm->sRGSimData.getWorld()->getRooms(srg::world::RoomType::Office);
    } else {
        // other room types
        for (auto& roomEntry : this->wm->sRGSimData.getWorld()->getRooms()) {
            switch (roomEntry.second->getType()) {
            case srg::world::RoomType::Kitchen:
            case srg::world::RoomType::Office:
            case srg::world::RoomType::Wall:
                continue;
            default:
                rooms.push_back(roomEntry.second);
            }
        }
    }

    // random coordinate in a random room
    randRoomValue = rand() % rooms.size();
    srg::world::Room* room = rooms[randRoomValue];

    auto& cells = room->getCells();
    std::map<srg::world::Coordinate, std::shared_ptr<srg::world::Cell>>::const_iterator cellIter;
    cellIter = cells.begin();
    std::advance(cellIter, rand() % cells.size());
    return cellIter->first;
}
/*PROTECTED REGION END*/

} /* namespace alica */
