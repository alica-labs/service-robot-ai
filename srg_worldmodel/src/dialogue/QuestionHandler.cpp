#include "srg/dialogue/QuestionHandler.h"

#include "srg/SRGWorldModel.h"
#include "srg/asp/SRGKnowledgeManager.h"

#include <engine/AlicaEngine.h>

namespace srg
{
namespace dialogue
{
QuestionHandler::QuestionHandler(SRGWorldModel* wm)
        : wm(wm)
{
}

std::shared_ptr<agent::SpeechAct> QuestionHandler::answerQuestion(const agent::SpeechAct requestAct)
{
    // TODO test code
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