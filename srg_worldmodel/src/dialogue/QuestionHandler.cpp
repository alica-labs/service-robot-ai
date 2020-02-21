#include "srg/dialogue/QuestionHandler.h"

#include "srg/SRGWorldModel.h"
#include "srg/asp/SRGKnowledgeManager.h"
#include "srg/dialogue/BasicHumanNeeds.h"

#include <engine/AlicaEngine.h>

namespace srg
{
namespace dialogue
{
QuestionHandler::QuestionHandler(SRGWorldModel* wm)
        : wm(wm)
{
    this->basicHumanNeeds = new BasicHumanNeeds(wm);
}

std::shared_ptr<agent::SpeechAct> QuestionHandler::answerQuestion(const agent::SpeechAct requestAct)
{
    // Handling of Basic Human Needs
    if (requestAct.text.find("NEED") == 0) {
        return this->basicHumanNeeds->answerNeed(requestAct);
    }

    // Basic ASP testing
    this->wm->srgKnowledgeManager->ask(requestAct.text);

    // prepare answer
    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->text = "";
    answerSpeechAct->type = agent::SpeechType::request;
    answerSpeechAct->previousActID = requestAct.actID;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    return answerSpeechAct;
}
} // namespace dialogue
} // namespace wm