#include "srg/dialogue/InformHandler.h"

#include "srg/asp/ASPTranslator.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptNet.h"
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
    this->cn = this->wm->conceptNet;
}

std::shared_ptr<agent::SpeechAct> InformHandler::answerInform(const agent::SpeechAct informAct)
{
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    conceptnet::Concept* root = this->cn->getConcept(answerGraph, informAct.text);
    answerGraph->setRoot(root);

    root->addEdges(this->cn->getEdges(answerGraph, root->term, -1));

    this->cn->findInconsistencies(answerGraph);
    // answerGraph->renderDot(true);
    std::ofstream file;
    file.open("example.lp");
    file << this->wm->aspTranslator->extractASPProgram(answerGraph, asp::ASPTranslator::InconsistencyRemoval::UseExternals);
    file.close();
    std::cout << answerGraph->toString() << std::endl;

    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->text = "";
    answerSpeechAct->answerGraph = answerGraph;
    answerSpeechAct->type = agent::SpeechType::inform;
    answerSpeechAct->previousActID = informAct.actID;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    return answerSpeechAct;
}
} // namespace dialogue
} // namespace srg
