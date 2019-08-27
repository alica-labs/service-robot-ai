#include "srg/dialogue/DialogueManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/dialogue/SpeechAct.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/dialogue/InformHandler.h"

#include <supplementary/InformationElement.h>

namespace srg
{
namespace dialogue
{
DialogueManager::DialogueManager(srg::SRGWorldModel* wm)
        : wm(wm)
{
    this->basicHumanNeeds = new BasicHumanNeeds(wm);
    this->informHandler = new InformHandler(wm);
}
DialogueManager::~DialogueManager() {
    for(auto pair : actMapping) {
        delete pair.second;
    }
}

void DialogueManager::processSpeechAct(std::shared_ptr<supplementary::InformationElement<SpeechAct>> speechAct)
{
    if(speechAct->getInformation().type == SpeechType::request) {
        AnswerGraph* answerGraph = this->basicHumanNeeds->answerNeed(speechAct->getInformation().text);
        this->actMapping.emplace(speechAct, answerGraph);
    } else if (speechAct->getInformation().type == SpeechType::inform) {
        AnswerGraph* answerGraph = this->informHandler->answerInform(speechAct->getInformation().text);
        this->actMapping.emplace(speechAct, answerGraph);
    }

}
} // namespace dialogue
} // namespace srg
