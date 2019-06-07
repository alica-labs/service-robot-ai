#include "srg/wm/BasicHumanNeeds.h"
#include "srg/SRGWorldModel.h"
#include "srg/container/Relations.h"
#include "srg/wm/ConceptNet.h"

namespace srg
{
namespace wm
{
const int BasicHumanNeeds::bestWeightedEdges = 10;
#define HUMAN_NEEDS_DEBUG
BasicHumanNeeds::BasicHumanNeeds(SRGWorldModel* wm)
        : wm(wm)
{
    this->cn = this->wm->conceptNet;
}

std::vector<std::string> BasicHumanNeeds::answerNeed(std::string need)
{
    // TODO add CausesDesire to step 1
    /**
     *  1. ask ConceptNet for MotivatedByGoal(WILDCARD, need)
     *  2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  3. select top bestWeightedEdges from resulting concepts
     *  4. ask ConceptNet for UsedFor(concept, WILDCARD)
     *  bestWeightedEdges. ask ConceptNet for Synonyms for top bestWeightedEdges results from 4
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  6. return top bestWeightedEdges
     */
    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need)
    std::vector<container::Edge> motivatedEdges = this->cn->getIncomingEdges(container::Relation::MotivatedByGoal, need, bestWeightedEdges);
    std::vector<container::Edge> causesDesireEdges = this->cn->getOutgoingEdges(container::Relation::CausesDesire, need, bestWeightedEdges);
#ifdef HUMAN_NEEDS_DEBUG
    int size = (motivatedEdges.size() < bestWeightedEdges ? motivatedEdges.size() : bestWeightedEdges);
    std::cout << "BasicHumanNeeds: Motivated Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << motivatedEdges.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "BasicHumanNeeds: Desired Edges: " << std::endl;
    for (int i = 0; i < (causesDesireEdges.size() < bestWeightedEdges ? causesDesireEdges.size() : bestWeightedEdges); i++) {
        std::cout << "\t" << causesDesireEdges.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 2. ask ConceptNet for Synonyms for top bestWeightedEdges results from 1.
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    size = (motivatedEdges.size() < bestWeightedEdges ? motivatedEdges.size() : bestWeightedEdges);

    std::cout << std::endl;
    std::vector<container::Edge> synonymsMotivated;
    std::vector<container::Edge> synonymsDesire;
    getSynonyms(motivatedEdges, size, synonymsMotivated, false);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for MotivatedByGoal Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonymsMotivated.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    size = (causesDesireEdges.size() < bestWeightedEdges ? causesDesireEdges.size() : bestWeightedEdges);
    getSynonyms(causesDesireEdges, size, synonymsDesire, true);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for CausesDesire Edges : " << std::endl;
    for (int i = 0; i < synonymsDesire.size(); i++) {
        std::cout << "\t" << synonymsDesire.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 3. select top bestWeightedEdges from resulting concepts
    size = (synonymsMotivated.size() < bestWeightedEdges ? synonymsMotivated.size() : bestWeightedEdges);
    // 4. ask ConceptNet for UsedFor(concept, WILDCARD)
    std::vector<container::Edge> usedForEdges;
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(container::Relation::UsedFor, synonymsMotivated.at(i).fromConcept.term, bestWeightedEdges);
        insertNewEdges(tmp, usedForEdges);
    }
    // 3. select top bestWeightedEdges from resulting concepts
    size = (synonymsDesire.size() < bestWeightedEdges ? synonymsDesire.size() : bestWeightedEdges);
    // 4. ask ConceptNet for UsedFor(concept, WILDCARD)
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(container::Relation::UsedFor, synonymsDesire.at(i).fromConcept.term, bestWeightedEdges);
        insertNewEdges(tmp, usedForEdges);
    }
    std::sort(usedForEdges.begin(), usedForEdges.end());
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << usedForEdges.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // bestWeightedEdges. ask ConceptNet for Synonyms for top bestWeightedEdges results from 4
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    std::vector<container::Edge> synonyms;
    getSynonyms(usedForEdges, (usedForEdges.size() < bestWeightedEdges ? usedForEdges.size() : bestWeightedEdges), synonyms, false);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonyms.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 6. return top bestWeightedEdges
    std::sort(synonyms.begin(), synonyms.end());
    size = (synonyms.size() < bestWeightedEdges ? synonyms.size() : bestWeightedEdges);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Sorted Synonyms for UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonyms.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    std::vector<std::string> answer;
    for (int i = 0; i < size; i++) {
        answer.push_back(synonyms.at(i).fromConcept.term);
    }
    return answer;
}

void BasicHumanNeeds::insertNewEdges(std::vector<container::Edge>& from, std::vector<container::Edge>& to) const
{
    bool insert;
    for (container::Edge causesEdge : from) {
        insert = true;
        for (container::Edge motivatedEdge : to) {
            if (motivatedEdge == causesEdge) {
                insert = false;
                break;
            }
        }
        if (insert) {
            to.push_back(causesEdge);
        }
    }
}

void BasicHumanNeeds::getSynonyms(std::vector<container::Edge>& edges, int size, std::vector<container::Edge>& synonyms, bool useToConcept) const
{
    insertNewEdges(edges, synonyms);
    std::vector<container::Edge> tmp;
    std::string concept;
    for (int i = 0; i < size; i++) {
        if (useToConcept) {
            concept = edges.at(i).toConcept.term;
        } else {
            concept = edges.at(i).fromConcept.term;
        }
        tmp = cn->getOutgoingEdges(container::Synonym, concept, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::IsA, concept, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::SimilarTo, concept, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::InstanceOf, concept, bestWeightedEdges);
        insertNewEdges(tmp, synonyms);
    }
}
} // namespace wm
} // namespace srg