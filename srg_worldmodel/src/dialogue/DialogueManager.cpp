#include "srg/dialogue/DialogueManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/dialogue/SpeechAct.h"

#include <supplementary/InformationElement.h>

namespace srg
{
namespace dialogue
{
DialogueManager::DialogueManager(srg::SRGWorldModel* wm)
        : wm(wm)
{
    this->basicHumanNeeds = new BasicHumanNeeds(wm);
}

void DialogueManager::processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct)
{
    AnswerGraph* answerGraph = this->basicHumanNeeds->answerNeed(speechAct->getInformation().text);

}
} // namespace dialogue
} // namespace srg
