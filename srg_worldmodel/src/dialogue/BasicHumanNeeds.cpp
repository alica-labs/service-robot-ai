#include "srg/dialogue/BasicHumanNeeds.h"
#include "srg/SRGWorldModel.h"
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

    srg::dialogue::AnswerGraph* answerGraph = new srg::dialogue::AnswerGraph(this->cn->getConcept(need));

    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need) and CausesDesire(need, WILDCARD)
    std::vector<::srg::conceptnet::Edge*> motivatedEdges = this->cn->getIncomingEdges(srg::conceptnet::Relation::MotivatedByGoal, need, bestWeightedEdges);
    std::vector<::srg::conceptnet::Edge*> causesDesireEdges = this->cn->getOutgoingEdges(srg::conceptnet::Relation::CausesDesire, need, bestWeightedEdges);

    answerGraph->graph->incomingEdges.insert(answerGraph->graph->incomingEdges.end(), motivatedEdges.begin(), motivatedEdges.end());
    answerGraph->graph->outgoingEdges.insert(answerGraph->graph->outgoingEdges.end(), causesDesireEdges.begin(), causesDesireEdges.end());

    // 2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    getSynonyms(motivatedEdges, false);
    getSynonyms(causesDesireEdges, true);

    conceptnet::Concept* rootConcept = answerGraph->graph;

    // 3. ask ConceptNet for UsedFor(concept, WILDCARD) for MotivatedByGoal edges + 5.
    for (size_t i = 0; i < rootConcept->incomingEdges.size(); i++) {
        getUsedFor(rootConcept->incomingEdges.at(i)->fromConcept);
    }

    // 4. ask ConceptNet for UsedFor(concept, WILDCARD) for CausesDesire edges + 5.
    for (size_t i = 0; i < rootConcept->outgoingEdges.size(); i++) {
        getUsedFor(rootConcept->outgoingEdges.at(i)->toConcept);
    }

    // 6. return answer graph
    std::vector<srg::conceptnet::Edge*> bestAnswers;
    for (conceptnet::Edge* edge1 : rootConcept->outgoingEdges) {
        for (conceptnet::Edge* edge2 : edge1->toConcept->outgoingEdges) {
            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                insertNewEdges(edge3->fromConcept->outgoingEdges, bestAnswers);
            }
        }
    }
    for (conceptnet::Edge* edge1 : rootConcept->incomingEdges) {
        for (conceptnet::Edge* edge2 : edge1->fromConcept->outgoingEdges) {
            for (conceptnet::Edge* edge3 : edge2->toConcept->incomingEdges) {
                insertNewEdges(edge3->fromConcept->outgoingEdges, bestAnswers);
            }
        }
    }
    std::sort(bestAnswers.begin(), bestAnswers.end(), [](conceptnet::Edge* first, conceptnet::Edge* second) { return first->weight > second->weight; });
    std::cout << "Best Answers: " << std::endl;
    for (size_t i = 0; i < bestAnswers.size(); i++) {
        std::cout << "\t" << bestAnswers.at(i)->toString() << std::endl;
        if (bestAnswers.at(i)->relation == conceptnet::Relation::UsedFor || bestAnswers.at(i)->relation == conceptnet::Relation::IsA) {
            insertNewConcepts(bestAnswers.at(i)->fromConcept, answerGraph->answerConcepts);
        } else {
            insertNewConcepts(bestAnswers.at(i)->toConcept, answerGraph->answerConcepts);
        }
        if (answerGraph->answerConcepts.size() == this->bestWeightedEdges) {
            break;
        }
    }
    std::cout << "AnswerGraph: " << answerGraph->toString() << std::endl;
    return answerGraph;
}

void BasicHumanNeeds::getUsedFor(const conceptnet::Concept* current) const
{
    for (size_t j = 0; j < current->outgoingEdges.size(); j++) {
        std::vector<srg::conceptnet::Edge*> tmp;
        if (current->outgoingEdges.at(j)->relation == conceptnet::Relation::MotivatedByGoal) {
            tmp = cn->getIncomingEdges(conceptnet::UsedFor, current->outgoingEdges.at(j)->fromConcept->term, bestWeightedEdges);
        } else {
            tmp = cn->getIncomingEdges(conceptnet::UsedFor, current->outgoingEdges.at(j)->toConcept->term, bestWeightedEdges);
        }
        insertNewEdges(tmp, current->outgoingEdges.at(j)->toConcept->incomingEdges);
        getSynonyms(current->outgoingEdges.at(j)->toConcept->incomingEdges, false);
    }
}

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

void BasicHumanNeeds::getSynonyms(std::vector<conceptnet::Edge*>& edges, bool useToConcept) const
{
    std::vector<srg::conceptnet::Edge*> synonyms;
    std::vector<conceptnet::Edge*> tmp;
    conceptnet::Concept* concept;
    int size = (edges.size() < this->bestWeightedEdges ? edges.size() : this->bestWeightedEdges);
    for (int i = 0; i < size; i++) {
        if (useToConcept) {
            concept = edges.at(i)->toConcept;
        } else {
            concept = edges.at(i)->fromConcept;
        }
        synonyms.push_back(edges.at(i));
        tmp = cn->getOutgoingEdges(conceptnet::Synonym, concept->term, this->bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(conceptnet::IsA, concept->term, this->bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(conceptnet::SimilarTo, concept->term, this->bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(conceptnet::InstanceOf, concept->term, this->bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        std::sort(synonyms.begin(), synonyms.end(), [](conceptnet::Edge* first, conceptnet::Edge* second) { return first->weight > second->weight; });
        int size = (synonyms.size() < this->bestWeightedEdges ? synonyms.size() : this->bestWeightedEdges);
        for (int j = 0; j < size; j++) {
            concept->outgoingEdges.push_back(synonyms.at(j));
        }
        synonyms.clear();
    }
}

} // namespace dialogue
} // namespace srg