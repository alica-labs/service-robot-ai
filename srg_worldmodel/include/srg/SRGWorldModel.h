#pragma once

#include "srg/conceptnet/ConceptNet.h"
#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/dialogue/DialogueManager.h"
#include "srg/wm/RawSensorData.h"
#include "wm/Communication.h"
#include "wm/SRGSimData.h"

#include <SystemConfig.h>
#include <essentials/EventTrigger.h>
#include <knowledge_manager/ASPKnowledgeManager.h>
#include <supplementary/InformationElement.h>
#include <supplementary/WorldModel.h>
#include <srg/GUI.h>

namespace essentials
{
class SystemConfig;
}

namespace reasoner
{
namespace asp
{
class Solver;
}
} // namespace reasoner

namespace srg
{
namespace asp
{
class ASPTranslator;
}

namespace conceptnet
{
class ConceptNet;
}

namespace dialogue
{
class DialogueManager;
}

class SRGWorldModel : public supplementary::WorldModel
{
public:
    static SRGWorldModel* getInstance(); /**< Singleton Getter */

    virtual ~SRGWorldModel();
    void init();
    void setSolver(::reasoner::asp::Solver* solver);
    std::string getAgentName();

    // Public Data Access Classes
    wm::SRGSimData sRGSimData;
    conceptnet::ConceptNet* conceptNet;
    dialogue::DialogueManager dialogueManager;
    wm::RawSensorData rawSensorData;
    wm::Communication* communication;
    knowledge_manager::ASPKnowledgeManager knowledgeManager;
    srg::asp::ASPTranslator* aspTranslator;
    srg::GUI* gui;

private:
    SRGWorldModel(); /**< Private Singleton Constructor */
    std::string agentName;
};

} /* namespace srg */
