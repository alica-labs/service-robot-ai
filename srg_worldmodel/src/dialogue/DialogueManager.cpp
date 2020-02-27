#include "srg/dialogue/DialogueManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/dialogue/InformHandler.h"
#include "srg/dialogue/RequestHandler.h"
#include "srg/tasks/CommandHandler.h"

#include <srg/agent/containers/SpeechAct.h>

#include <gvc.h>
#include <gvcext.h>
#include <supplementary/InformationElement.h>

namespace srg
{
namespace dialogue
{
DialogueManager::DialogueManager(srg::SRGWorldModel* wm)
        : wm(wm)
{
    this->requestHandler = new RequestHandler(wm);
    this->informHandler = new InformHandler(wm);
    this->commandHandler = new srg::tasks::CommandHandler(wm);
}
DialogueManager::~DialogueManager() {}

void DialogueManager::processSpeechAct(std::shared_ptr<supplementary::InformationElement<agent::SpeechAct>> speechActInfo)
{
    // make_shared calls implicit copy constructor - maybe that is not what you want (create your own then)
    this->dialogueMap.emplace(speechActInfo->getInformation().actID, std::make_shared<agent::SpeechAct>(speechActInfo->getInformation()));

    std::shared_ptr<agent::SpeechAct> answerSpeechAct = nullptr;
    if (speechActInfo->getInformation().type == agent::SpeechType::request) {
        answerSpeechAct = this->requestHandler->handle(speechActInfo->getInformation());
    } else if (speechActInfo->getInformation().type == agent::SpeechType::inform) {
        answerSpeechAct = this->informHandler->handle(speechActInfo->getInformation());
    } else if (speechActInfo->getInformation().type == agent::SpeechType::command) {
        answerSpeechAct = this->commandHandler->handle(speechActInfo);
    }

    if (answerSpeechAct) {
        this->pendingSpeechActs.push_back(answerSpeechAct);
    }

#ifdef inconsistency_eval
    renderDot();
#endif
}

void DialogueManager::tick()
{
    // call to update success status of tasks
    this->commandHandler->tick();

    // send pending speech act answers
    for (std::shared_ptr<srg::agent::SpeechAct> speechAct : this->pendingSpeechActs) {
        this->dialogueMap.emplace(speechAct->actID, speechAct);
        this->wm->communication->sendSpeechAct(speechAct);
    }
}

void DialogueManager::renderDot() const
{
    Agraph_t* g;
    /* set up a graphviz context - but only once even for multiple graphs */
    static GVC_t* gvc;
    if (!gvc) {
        gvc = gvContext();
    }
    /* Create a simple digraph */
    g = agopen("g", Agdirected, NULL);
    agsafeset(g, "rankdir", "RL", "");
    for (auto& act : dialogueMap) {
        act.second->answerGraph->renderDot(g, true);
    }
    /* Set an attribute - in this case one that affects the visible rendering */

    /* Use the directed graph layout engine */
    gvLayout(gvc, g, "dot");
    /* Output in .dot format */
    FILE* fptr;
    fptr = fopen("test.dot", "w");

    gvRender(gvc, g, "dot", fptr);
    fclose(fptr);

    gvFreeLayout(gvc, g);
    agclose(g);

    // call this to translate into ps format and open with evince
    //    dot -Tps ~/test.dot -o outfile.ps
}
} // namespace dialogue
} // namespace srg
