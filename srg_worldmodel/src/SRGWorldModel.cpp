#include "srg/SRGWorldModel.h"

#include "srg/wm/ConceptNet.h"

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
        , basicHumanNeeds(this)
        , communication(nullptr)
{
    this->agentName = sc->getHostname();
    this->conceptNet = new wm::ConceptNet(this);
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
