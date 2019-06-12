#include "srg/dialogue/BasicHumanNeeds.h"

#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/CNManager.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptNet.h"
#include "srg/conceptnet/Relations.h"
#include "srg/dialogue/AnswerGraph.h"

namespace srg
{
namespace dialogue
{
const int BasicHumanNeeds::bestWeightedEdges = 5;
#define HUMAN_NEEDS_DEBUG
BasicHumanNeeds::BasicHumanNeeds(SRGWorldModel* wm)
        : wm(wm)
{
    this->cn = this->wm->conceptNet;
}

AnswerGraph* BasicHumanNeeds::answerNeed(std::string need)
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
    conceptnet::Concept* root = this->cn->getConcept(answerGraph, need);
    answerGraph->setRoot(root);

    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need) and CausesDesire(need, WILDCARD)
    root->addEdges(this->cn->getIncomingEdges(answerGraph, conceptnet::MotivatedByGoal, need, bestWeightedEdges));
    root->addEdges(this->cn->getOutgoingEdges(answerGraph, conceptnet::CausesDesire, need, bestWeightedEdges));

    // 2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
    // => Synonym, SimilarTo, InstanceOf
    std::vector<conceptnet::Concept*> connectedConcepts = root->getConnectedConcepts({conceptnet::MotivatedByGoal, conceptnet::CausesDesire}, false);
    for (conceptnet::Concept* concept : connectedConcepts) {
        concept->addEdges(this->cn->getEquivalentOutgoingEdges(answerGraph, concept, bestWeightedEdges));
    }

    // 3. ask ConceptNet for UsedFor(WILDCARD, concept)
    connectedConcepts = root->getConnectedConcepts({conceptnet::MotivatedByGoal, conceptnet::CausesDesire}, true); // include synonyms
    for (conceptnet::Concept* concept : connectedConcepts) {
        concept->addEdges(this->cn->getIncomingEdges(answerGraph, conceptnet::UsedFor, concept->term, bestWeightedEdges));
    }

    for (conceptnet::Concept* concept : connectedConcepts) {
        std::cout << "BasicHumanNeeds: Concept " << concept->term << std::endl;
        for (conceptnet::Concept* connectedConcept : concept->getConnectedConcepts(conceptnet::UsedFor, true)) {
            std::cout << "BasicHumanNeeds: ConnectedConcept " << connectedConcept->term << std::endl;
            answerGraph->answerConcepts.push_back(connectedConcept);
        }
    }

    answerGraph->renderDot();

//    // 6. return answer graph
//    std::vector<srg::conceptnet::Edge*> bestAnswers;
//    for (const conceptnet::Edge* edge1 : root->edges) {
//        for (const conceptnet::Edge* edge2 : edge1->toConcept->edges) {
//            for (const conceptnet::Edge* edge3 : edge2->toConcept->edges) {
//                insertNewEdges(edge3->fromConcept->edges, bestAnswers);
//            }
//        }
//    }
//    for (const conceptnet::Edge* edge1 : root->edges) {
//        for (const conceptnet::Edge* edge2 : edge1->fromConcept->edges) {
//            for (const conceptnet::Edge* edge3 : edge2->toConcept->edges) {
//                insertNewEdges(edge3->fromConcept->edges, bestAnswers);
//            }
//        }
//    }
//    std::sort(bestAnswers.begin(), bestAnswers.end(), [](conceptnet::Edge* first, conceptnet::Edge* second) { return first->weight > second->weight; });
//    //    std::cout << "Best Answers: " << std::endl;
//    for (size_t i = 0; i < bestAnswers.size(); i++) {
//        //        std::cout << "\t" << bestAnswers.at(i)->toString() << std::endl;
//        if (bestAnswers.at(i)->relation == conceptnet::Relation::UsedFor || bestAnswers.at(i)->relation == conceptnet::Relation::IsA) {
//            insertNewConcepts(bestAnswers.at(i)->fromConcept, answerGraph->answerConcepts);
//        } else {
//            insertNewConcepts(bestAnswers.at(i)->toConcept, answerGraph->answerConcepts);
//        }
//        if (answerGraph->answerConcepts.size() == this->bestWeightedEdges) {
//            break;
//        }
//    }
    //    std::cout << "AnswerGraph: " << answerGraph->toString() << std::endl;

    return answerGraph;
}

//void BasicHumanNeeds::getUsedFor(const conceptnet::Concept* current) const
//{
//    for (size_t j = 0; j < current->edges.size(); j++) {
//        std::vector<srg::conceptnet::Edge*> tmp;
//        if (current->edges.at(j)->relation == conceptnet::Relation::MotivatedByGoal) {
//            tmp = cn->getIncomingEdges(conceptnet::UsedFor, current->edges.at(j)->fromConcept->term, bestWeightedEdges);
//        } else {
//            tmp = cn->getIncomingEdges(conceptnet::UsedFor, current->edges.at(j)->toConcept->term, bestWeightedEdges);
//        }
//        insertNewEdges(tmp, current->edges.at(j)->toConcept->edges);
//        getSynonyms(current->edges.at(j)->toConcept->edges, false);
//    }
//}

void BasicHumanNeeds::insertNewEdges(std::vector<conceptnet::Edge*>& from, std::vector<conceptnet::Edge*>& to) const
{
    bool insert;
    for (conceptnet::Edge* f : from) {
        insert = true;
        for (conceptnet::Edge* t : to) {
            if (&t == &f) {
                insert = false;
                break;
            }
        }
        if (insert) {
            to.push_back(f);
        }
    }
}

void BasicHumanNeeds::insertNewConcepts(srg::conceptnet::Concept* concept, std::vector<srg::conceptnet::Concept*>& to) const
{
    bool insert = true;
    for (conceptnet::Concept* c : to) {
        if (c->term == concept->term) {
            insert = false;
            break;
        }
    }
    if (insert) {
        to.push_back(concept);
    }
}

} // namespace dialogue
} // namespace srg