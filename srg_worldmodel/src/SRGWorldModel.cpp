#include "srg/SRGWorldModel.h"

#include "srg/conceptnet/ConceptNet.h"
#include "srg/dialogue/DialogueManager.h"

namespace srg
{

SRGWorldModel* SRGWorldModel::getInstance()
{
    static SRGWorldModel instance;
    return &instance;
}

SRGWorldModel::SRGWorldModel()
        : WorldModel()
        , sRGSimData(this)
        , rawSensorData(this)
        , dialogueManager(this)
        , communication(nullptr)
{
    this->agentName = sc->getHostname();
    this->conceptNet = new conceptnet::ConceptNet(this);
    std::cout << "Creating a SRGWorldModel\n";
}

SRGWorldModel::~SRGWorldModel()
{
    delete this->conceptNet;
    delete this->communication;
}

std::string SRGWorldModel::getAgentName()
{
    return this->agentName;
}

void SRGWorldModel::init()
{
    this->communication = new wm::Communication(this);
}

void SRGWorldModel::setSolver(reasoner::asp::Solver* solver) {
    this->knowledgeManager.setSolver(solver);
}
} /* namespace wumpus */
