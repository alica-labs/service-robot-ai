#include "srg/dialogue/DialogueManager.h"

#include "srg/SRGWorldModel.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/dialogue/InformHandler.h"
#include "srg/dialogue/CommandHandler.h"
#include "srg/containers/SpeechAct.h"

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
    this->basicHumanNeeds = new BasicHumanNeeds(wm);
    this->informHandler = new InformHandler(wm);
    this->commandHandler = new CommandHandler(wm);
}
DialogueManager::~DialogueManager()
{

}

void DialogueManager::processSpeechAct(std::shared_ptr<supplementary::InformationElement<srg::SpeechAct>> speechAct)
{
    if (speechAct->getInformation().type == SpeechType::request) {
        this->speechActs.push_back(this->basicHumanNeeds->answerNeed(speechAct->getInformation()));
    } else if (speechAct->getInformation().type == SpeechType::inform) {
        this->speechActs.push_back(this->informHandler->answerInform(speechAct->getInformation()));
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
