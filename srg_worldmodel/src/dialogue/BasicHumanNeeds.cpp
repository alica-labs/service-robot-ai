#include "srg/dialogue/BasicHumanNeeds.h"

#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/CNManager.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptNet.h"
#include "srg/conceptnet/ConceptPath.h"
#include "srg/conceptnet/Relations.h"
#include "srg/dialogue/AnswerGraph.h"

#include <engine/AlicaEngine.h>

#include <iostream>

#define HUMAN_NEEDS_DEBUG

namespace srg
{
namespace dialogue
{
const int BasicHumanNeeds::bestNumberOfElements = 5;

BasicHumanNeeds::BasicHumanNeeds(SRGWorldModel* wm)
        : wm(wm)
{
    this->cn = this->wm->conceptNet;
}

std::shared_ptr<agent::SpeechAct> BasicHumanNeeds::answerNeed(const agent::SpeechAct needAct)
{
    /**
     *  1. ask ConceptNet for MotivatedByGoal(WILDCARD, need) and CausesDesire(need, WILDCARD)
     *  2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  3. ask ConceptNet for UsedFor(concept, WILDCARD) for MotivatedByGoal edges
     *  4. ask ConceptNet for UsedFor(concept, WILDCARD) for CausesDesire edges
     *  5. ask ConceptNet for Synonyms for top bestWeightedEdges results from 4
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  6. return answer graph
     */
    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph();
    conceptnet::Concept* root = this->cn->getConcept(answerGraph, needAct.text);
    answerGraph->setRoot(root);
    if (!root) {
        // did not find any concept for the given text
        std::cout << "[BasicHumanNeeds] No root concept found for '" << needAct.text << "'" << std::endl;
        return createAnswerSpeechAct(needAct.actID, answerGraph);
    }

    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need) and CausesDesire(need, WILDCARD)
    root->addEdges(this->cn->getIncomingEdges(answerGraph, conceptnet::MotivatedByGoal, needAct.text, bestNumberOfElements));
    root->addEdges(this->cn->getOutgoingEdges(answerGraph, conceptnet::CausesDesire, needAct.text, bestNumberOfElements));

    // 2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
    // => Synonym, SimilarTo, InstanceOf
    std::vector<conceptnet::Concept*> connectedConcepts = root->getConnectedConcepts({conceptnet::MotivatedByGoal, conceptnet::CausesDesire}, false);
    for (conceptnet::Concept* concept : connectedConcepts) {
        concept->addEdges(this->cn->getEquivalentOutgoingEdges(answerGraph, concept, bestNumberOfElements));
    }

    // 3. ask ConceptNet for UsedFor(WILDCARD, concept)
    connectedConcepts = root->getConnectedConcepts({conceptnet::MotivatedByGoal, conceptnet::CausesDesire}, true); // include synonyms
    for (conceptnet::Concept* concept : connectedConcepts) {
        concept->addEdges(this->cn->getIncomingEdges(answerGraph, conceptnet::UsedFor, concept->term, bestNumberOfElements));
    }

    for (conceptnet::Concept* concept : connectedConcepts) {
        //        std::cout << "BasicHumanNeeds: Concept " << concept->term << std::endl;
        for (conceptnet::Concept* connectedConcept : concept->getConnectedConcepts(conceptnet::UsedFor, true)) {
            //            std::cout << "BasicHumanNeeds: ConnectedConcept " << connectedConcept->term << std::endl;
            answerGraph->answerConcepts.push_back(connectedConcept);
        }
    }
    this->createAnswerPaths(answerGraph, root);
#ifdef HUMAN_NEEDS_DEBUG
//    for (conceptnet::ConceptPath* conceptPath : answerGraph->answerPaths) {
//        std::cout << "BasicHumanNeeds:" << conceptPath->toString() << std::endl;
//    }
#endif

    // TODO add again later
    // answerGraph->renderDot();
    return createAnswerSpeechAct(needAct.actID, answerGraph);
}

std::shared_ptr<agent::SpeechAct> BasicHumanNeeds::createAnswerSpeechAct(essentials::IdentifierConstPtr previousActID, srg::dialogue::AnswerGraph* answerGraph)
{
    std::shared_ptr<agent::SpeechAct> answerSpeechAct = std::make_shared<agent::SpeechAct>();
    answerSpeechAct->text = "";
    answerSpeechAct->answerGraph = answerGraph;
    answerSpeechAct->type = agent::SpeechType::request;
    answerSpeechAct->previousActID = previousActID;
    answerSpeechAct->actID = this->wm->getEngine()->getIdManager()->generateID();
    answerSpeechAct->senderID = this->wm->getOwnId();
    return answerSpeechAct;
}

void BasicHumanNeeds::createAnswerPaths(AnswerGraph* answerGraph, conceptnet::Concept* start)
{
    // init fringe
    std::vector<conceptnet::ConceptPath*> fringe;
    for (conceptnet::Edge* edge : start->getEdges()) {
        conceptnet::ConceptPath* path = new conceptnet::ConceptPath(start);
        path->addToPath(edge);
        fringe.push_back(path);
    }

    while (!fringe.empty()) {
        conceptnet::ConceptPath* currentPath = fringe[0];
        fringe.erase(fringe.begin());

        if (std::find(answerGraph->answerConcepts.begin(), answerGraph->answerConcepts.end(), currentPath->getEnd()) != answerGraph->answerConcepts.end()) {
            answerGraph->answerPaths.push_back(currentPath);
            continue;
        }

        bool expanded = false;
        for (conceptnet::Edge* edge : currentPath->getEnd()->getEdges()) {
            conceptnet::Concept* otherNode;
            if (edge->fromConcept == currentPath->getEnd()) {
                otherNode = edge->toConcept;
            } else {
                otherNode = edge->fromConcept;
            }

            if (currentPath->containsNode(otherNode)) {
                continue;
            }
            conceptnet::ConceptPath* nextPath = new conceptnet::ConceptPath(*currentPath);
            nextPath->addToPath(edge);
            fringe.push_back(nextPath);
            expanded = true;
        }
        if (!expanded) {
            delete currentPath;
        }
    }
}

} // namespace dialogue
} // namespace srg