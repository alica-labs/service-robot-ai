#include "srg/SRGWorldModel.h"

#include <engine/AlicaEngine.h>

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
        , knowledgeManager(this)
        , conceptNet(this)
        , rawSensorData(this)
{
    this->agentName = sc->getHostname();
    std::cout << "Creating a SRGWorldModel\n";
}

SRGWorldModel::~SRGWorldModel()
{
}


std::string SRGWorldModel::getAgentName()
{
    return this->agentName;
}

void SRGWorldModel::init()
{

}
} /* namespace wumpus */
