#pragma once

#include "srg/conceptnet/ConceptNet.h"
#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/dialogue/DialogueManager.h"
#include "srg/wm/RawSensorData.h"
#include "wm/Communication.h"
#include "wm/SRGSimData.h"

#include <essentials/SystemConfig.h>
#include <essentials/EventTrigger.h>
#include <srg/GUI.h>
#include <supplementary/InformationElement.h>
#include <supplementary/WorldModel.h>

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
class SRGKnowledgeManager;
} // namespace asp

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

    // Public Data Access Classes
    wm::SRGSimData sRGSimData;
    conceptnet::ConceptNet* conceptNet;
    dialogue::DialogueManager dialogueManager;
    wm::RawSensorData rawSensorData;
    wm::Communication* communication;
    srg::asp::ASPTranslator* aspTranslator;
    srg::asp::SRGKnowledgeManager* srgKnowledgeManager;
    srg::GUI* gui;

private:
    SRGWorldModel(); /**< Private Singleton Constructor */
};

} /* namespace srg */
