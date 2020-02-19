#include "srg/dialogue/DialogueManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/dialogue/InformHandler.h"
#include "srg/dialogue/QuestionHandler.h"
#include "srg/tasks/TaskHandler.h"

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
    this->questionHandler = new QuestionHandler(wm);
    this->informHandler = new InformHandler(wm);
    this->taskHandler = new srg::tasks::TaskHandler(wm);
}
DialogueManager::~DialogueManager()
{

}

void DialogueManager::processSpeechAct(std::shared_ptr<supplementary::InformationElement<agent::SpeechAct>> speechAct)
{
    if (speechAct->getInformation().type == agent::SpeechType::request) {
        this->speechActs.push_back(this->questionHandler->answerQuestion(speechAct->getInformation()));
    } else if (speechAct->getInformation().type == agent::SpeechType::inform) {
        this->speechActs.push_back(this->informHandler->answerInform(speechAct->getInformation()));
    } else if (speechAct->getInformation().type == agent::SpeechType::command) {
        this->taskHandler->processTaskAct(speechAct);
    }

#ifdef inconsistency_eval
    renderDot();
#endif
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
    for (auto act : speechActs) {
        act->answerGraph->renderDot(g, true);
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
