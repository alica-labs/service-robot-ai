#include "srg/dialogue/InformHandler.h"

#include "srg/asp/ASPTranslator.h"
#include "srg/dialogue/AnswerGraph.h"

#include "srg/SRGWorldModel.h"

#include <engine/AlicaEngine.h>

namespace srg
{
namespace dialogue
{

InformHandler::InformHandler(SRGWorldModel* wm)
        : wm(wm)
{
    this->basicHumanNeeds = new BasicHumanNeeds(wm);
    this->cn = this->wm->conceptNet;
}

std::shared_ptr<agent::SpeechAct> InformHandler::handle(const agent::SpeechAct informAct)
{
    return testInconsistencyStuff(informAct);
}

std::shared_ptr<agent::SpeechAct> InformHandler::createAnswerSpeechAct(essentials::IdentifierConstPtr previousActID, srg::dialogue::AnswerGraph* answerGraph, agent::SpeechType type)
{
    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->text = "";
    answerSpeechAct->answerGraph = answerGraph;
    answerSpeechAct->type = type;
    answerSpeechAct->previousActID = previousActID;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    return answerSpeechAct;
}

std::shared_ptr<agent::SpeechAct> InformHandler::testInconsistencyStuff(const agent::SpeechAct informAct)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    answerGraph->root = this->cn->getConcept(answerGraph, informAct.text);

    answerGraph->root->addEdges(this->cn->getEdges(answerGraph, answerGraph->root->term, -1));

    this->cn->findInconsistencies(answerGraph);
    // answerGraph->renderDot(true);
    std::ofstream file;
    file.open("example.lp");
    file << this->wm->aspTranslator->addToKnowledgeBase(answerGraph,
                                                        asp::ASPTranslator::InconsistencyRemoval::UseExternals);
    file.close();
    std::cout << answerGraph->toString() << std::endl;

   return createAnswerSpeechAct(informAct.actID, answerGraph, agent::SpeechType::inform);
}
} // namespace dialogue
} // namespace srg
