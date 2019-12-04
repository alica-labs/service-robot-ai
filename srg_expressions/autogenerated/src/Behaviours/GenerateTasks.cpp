#include "Behaviours/GenerateTasks.h"
#include <memory>

/*PROTECTED REGION ID(inccpp1575291385685) ENABLED START*/
#include <srg/Agent.h>
#include <srg/world/Room.h>
#include <srg/world/RoomType.h>
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
    this->agent->speak(this->speechActs[sentCounter++]);
    /*PROTECTED REGION END*/
}
void GenerateTasks::initialiseParameters()
{
    /*PROTECTED REGION ID(initialiseParameters1575291385685) ENABLED START*/
    this->sentCounter = 0;
    generateSpeechActs(10);
    /*PROTECTED REGION END*/
}
/*PROTECTED REGION ID(methods1575291385685) ENABLED START*/
void GenerateTasks::generateSpeechActs(int numberOfTasks) {
    for (int i = 0; i < numberOfTasks; i++) {
        srg::agent::SpeechAct sa;
        sa.senderID = this->wm->getOwnId();
        sa.actID = this->wm->getEngine()->getIdManager()->generateID();
        sa.previousActID = this->wm->getEngine()->getIdManager()->getWildcardID();
        sa.type = srg::agent::SpeechType::command;

        // receiver ID
        auto& agents = this->wm->getEngine()->getTeamManager()->getAllAgents();
        std::map<essentials::IdentifierConstPtr, Agent*>::const_iterator agentIter;
        do {
            agentIter = agents.begin();
            std::advance(agentIter, rand() % agents.size());
        } while (agentIter->first != this->wm->getOwnId() && agentIter->second->isActive());
        sa.receiverID = agentIter->first;


        srg::world::Coordinate coordinate = getRandomCoordinate();
        // task text
        std::string taskText = "transport";
        taskText += "CupBlue";
        taskText += "5,5";
        sa.text = taskText;

        this->speechActs.push_back(sa);
    }
}

const srg::world::Coordinate GenerateTasks::getRandomCoordinate() {
    int randRoomValue = rand() % 100;
    std::vector<srg::world::Room*> rooms;
    if (randRoomValue < 40) {
        // kitchen
        rooms = this->wm->sRGSimData.getWorld()->getRooms(srg::world::RoomType::Kitchen);
    } else if (randRoomValue < 80) {
        // office
        rooms = this->wm->sRGSimData.getWorld()->getRooms(srg::world::RoomType::Workroom);
    } else {
        // other room types
        // TODO get all other rooms and choose random one room that is not kitchen or workroom
    }
    return srg::world::Coordinate(0,0);
}
/*PROTECTED REGION END*/

} /* namespace alica */
