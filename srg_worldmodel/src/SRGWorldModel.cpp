#include "srg/SRGWorldModel.h"

#include "srg/conceptnet/ConceptNet.h"
#include "srg/asp/ASPTranslator.h"

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
        , gui(nullptr)
{
    this->aspTranslator = new srg::asp::ASPTranslator(this);
    this->agentName = sc->getHostname();
    this->conceptNet = new conceptnet::ConceptNet(this);

}

SRGWorldModel::~SRGWorldModel()
{
    delete this->conceptNet;
    delete this->communication;
    delete this->aspTranslator;
    delete this->gui;
}

std::string SRGWorldModel::getAgentName()
{
    return this->agentName;
}

void SRGWorldModel::init()
{
    this->sRGSimData.init();
    this->gui = new srg::GUI(this->agentName);
    this->communication = new wm::Communication(this);
}

void SRGWorldModel::setSolver(reasoner::asp::Solver* solver) {
    this->knowledgeManager.setSolver(solver);
}
} /* namespace wumpus */
