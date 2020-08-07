#include "srg/SRGWorldModel.h"

#include "srg/conceptnet/ConceptNet.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/asp/ASPTranslator.h"

#include <engine/AlicaContext.h>

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
}

SRGWorldModel::~SRGWorldModel()
{
    delete this->conceptNet;
    delete this->srgKnowledgeManager;
    delete this->aspTranslator;
    delete this->communication;
    delete this->gui;
}

void SRGWorldModel::init()
{
    this->sRGSimData.init();
    this->gui = new srg::GUI(this->getAlicaContext()->getLocalAgentName());
    this->communication = new wm::Communication(this);
}

/**
 * Must be called before init().
 * @param solver
 */
void SRGWorldModel::setSolver(reasoner::asp::Solver* solver) {
    this->srgKnowledgeManager->setSolver(solver);
}
} /* namespace wumpus */
