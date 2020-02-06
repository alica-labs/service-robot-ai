#include "srg/SRGWorldModel.h"

#include "srg/conceptnet/ConceptNet.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/asp/ASPTranslator.h"

#include <knowledge_manager/ASPKnowledgeManager.h>

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
        , conceptNet(new conceptnet::ConceptNet(this))
        , aspTranslator(new srg::asp::ASPTranslator(this))
        , srgKnowledgeManager(new srg::asp::SRGKnowledgeManager())
        , gui(nullptr)
{
    this->agentName = sc->getHostname();
}

SRGWorldModel::~SRGWorldModel()
{
    delete this->conceptNet;
    delete this->srgKnowledgeManager;
    delete this->aspTranslator;
    delete this->communication;
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
    this->srgKnowledgeManager->setSolver(solver);
  // add asp facts that probably don't change
//  for (auto& roomEntry : this->sRGSimData.getWorld()->getRooms()) {
//    this->srgKnowledgeManager->addRoom(roomEntry.second);
//  }
}
} /* namespace wumpus */
