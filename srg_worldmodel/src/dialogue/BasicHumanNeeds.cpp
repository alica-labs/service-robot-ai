#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/Relations.h"
#include "srg/conceptnet/ConceptNet.h"
#include "srg/conceptnet/Concept.h"
#include "srg/dialogue/AnswerGraph.h"

namespace srg
{
namespace dialogue
{
const int BasicHumanNeeds::bestWeightedEdges = 10;
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
     *  3.a. select top bestWeightedEdges from MotivatedByGoal concepts
     *  4.a. ask ConceptNet for UsedFor(concept, WILDCARD)
     *  3.b. select top bestWeightedEdges from CausesDesire concepts
     *  4.b. ask ConceptNet for UsedFor(concept, WILDCARD)
     *  bestWeightedEdges. ask ConceptNet for Synonyms for top bestWeightedEdges results from 4
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  6. return top bestWeightedEdges
     */

    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph(this->cn->getConcept(need));

    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need) and CausesDesire(need, WILDCARD)
    std::vector<::srg::conceptnet::Edge*> motivatedEdges = this->cn->getIncomingEdges(srg::conceptnet::Relation::MotivatedByGoal, need, bestWeightedEdges);
    std::vector<::srg::conceptnet::Edge*> causesDesireEdges = this->cn->getOutgoingEdges(srg::conceptnet::Relation::CausesDesire, need, bestWeightedEdges);

    answerGraph->graph->incomingEdges.insert(answerGraph->graph->incomingEdges.end(), motivatedEdges.begin(), motivatedEdges.end());
    answerGraph->graph->outgoingEdges.insert(answerGraph->graph->outgoingEdges.end(), causesDesireEdges.begin(), causesDesireEdges.end());

#ifdef HUMAN_NEEDS_DEBUG
    int size = (motivatedEdges.size() < bestWeightedEdges ? motivatedEdges.size() : bestWeightedEdges);
    std::cout << "BasicHumanNeeds: Motivated Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << motivatedEdges.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "BasicHumanNeeds: Desired Edges: " << std::endl;
    for (size_t i = 0; i < (causesDesireEdges.size() < bestWeightedEdges ? causesDesireEdges.size() : bestWeightedEdges); i++) {
        std::cout << "\t" << causesDesireEdges.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif

    // 2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    size = (motivatedEdges.size() < bestWeightedEdges ? motivatedEdges.size() : bestWeightedEdges);

    std::cout << std::endl;
    std::vector<::srg::conceptnet::Edge*> synonymsMotivated;
    std::vector<::srg::conceptnet::Edge*> synonymsDesire;
    getSynonyms(motivatedEdges, size, synonymsMotivated, false);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for MotivatedByGoal Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonymsMotivated.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif

    size = (causesDesireEdges.size() < bestWeightedEdges ? causesDesireEdges.size() : bestWeightedEdges);
    getSynonyms(causesDesireEdges, size, synonymsDesire, true);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for CausesDesire Edges : " << std::endl;
    for (size_t i = 0; i < synonymsDesire.size(); i++) {
        std::cout << "\t" << synonymsDesire.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif

    // 3.a. select top bestWeightedEdges from MotivatedByGoal concepts
    size = (synonymsMotivated.size() < bestWeightedEdges ? synonymsMotivated.size() : bestWeightedEdges);
    std::sort(synonymsMotivated.begin(), synonymsMotivated.end());
    // 4.a. ask ConceptNet for UsedFor(concept, WILDCARD)
    std::vector<conceptnet::Edge*> usedForEdges;
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(conceptnet::Relation::UsedFor, synonymsMotivated.at(i)->fromConcept->term, bestWeightedEdges);
        synonymsMotivated.at(i)->toConcept->incomingEdges.insert(synonymsMotivated.at(i)->toConcept->incomingEdges.end(), tmp.begin(), tmp.end());
        insertNewEdges(tmp, usedForEdges);
    }

    // 3.b. select top bestWeightedEdges from CausesDesire concepts
    size = (synonymsDesire.size() < bestWeightedEdges ? synonymsDesire.size() : bestWeightedEdges);
    std::sort(synonymsMotivated.begin(), synonymsMotivated.end());
    // 4.b. ask ConceptNet for UsedFor(concept, WILDCARD)
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(conceptnet::Relation::UsedFor, synonymsDesire.at(i)->fromConcept->term, bestWeightedEdges);
        synonymsDesire.at(i)->fromConcept->incomingEdges.insert(synonymsDesire.at(i)->fromConcept->incomingEdges.end(), tmp.begin(), tmp.end());
        insertNewEdges(tmp, usedForEdges);
    }
    std::sort(usedForEdges.begin(), usedForEdges.end());

#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << usedForEdges.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif

    // 5. ask ConceptNet for Synonyms for top bestWeightedEdges results from 4
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    std::vector<conceptnet::Edge*> synonyms;
    getSynonyms(usedForEdges, (usedForEdges.size() < bestWeightedEdges ? usedForEdges.size() : bestWeightedEdges), synonyms, false);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonyms.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 6. return top bestWeightedEdges
    std::sort(synonyms.begin(), synonyms.end());
    size = (synonyms.size() < bestWeightedEdges ? synonyms.size() : bestWeightedEdges);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Sorted Synonyms for UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonyms.at(i)->toString() << std::endl;
    }
    std::cout << std::endl;
#endif

        // TODO create AnswerGraph
    std::vector<std::string> answer;
    for (int i = 0; i < size; i++) {
        answer.push_back(synonyms.at(i)->fromConcept->term);
    }
    std::cout << "AnswerGraph: " << answerGraph->toString() << std::endl;
    return answerGraph;
}

void BasicHumanNeeds::insertNewEdges(std::vector<conceptnet::Edge*>& from, std::vector<conceptnet::Edge*>& to) const
{
    bool insert;
    for (conceptnet::Edge* causesEdge : from) {
        insert = true;
        for (conceptnet::Edge* motivatedEdge : to) {
            if (&motivatedEdge == &causesEdge) {
                insert = false;
                break;
            }
        }
        if (insert) {
            to.push_back(causesEdge);
        }
    }
}

void BasicHumanNeeds::getSynonyms(std::vector<conceptnet::Edge*>& edges, int size, std::vector<conceptnet::Edge*>& synonyms, bool useToConcept) const
{
    insertNewEdges(edges, synonyms);
    std::vector<conceptnet::Edge*> tmp;
    conceptnet::Concept* concept;
    for (int i = 0; i < size; i++) {
        if (useToConcept) {
            concept = edges.at(i)->toConcept;
        } else {
            concept = edges.at(i)->fromConcept;
        }
        tmp = cn->getOutgoingEdges(conceptnet::Synonym, concept->term, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        insertNewEdges(tmp, concept->outgoingEdges);
        tmp = cn->getOutgoingEdges(conceptnet::IsA, concept->term, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        insertNewEdges(tmp, concept->outgoingEdges);
        tmp = cn->getOutgoingEdges(conceptnet::SimilarTo, concept->term, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        insertNewEdges(tmp, concept->outgoingEdges);
        tmp = cn->getOutgoingEdges(conceptnet::InstanceOf, concept->term, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        insertNewEdges(tmp, concept->outgoingEdges);
    }
}
} // namespace wm
} // namespace srg