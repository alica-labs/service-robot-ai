#include "srg/wm/BasicHumanNeeds.h"
#include "srg/SRGWorldModel.h"
#include "srg/container/Relations.h"
#include "srg/wm/ConceptNet.h"

namespace srg
{
namespace wm
{

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
     *  2. ask ConceptNet for Synonyms for top 5 results from 1.
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  3. select top 5 from resulting concepts
     *  4. ask ConceptNet for UsedFor(concept, WILDCARD)
     *  5. ask ConceptNet for Synonyms for top 5 results from 4
     *  => Synonym, IsA(subtype), SimilarTo, InstanceOf
     *  6. return top 5
     */
    // 1. ask ConceptNet for MotivatedByGoal(WILDCARD, need)
    std::vector<container::Edge> motivatedEdges = this->cn->getIncomingEdges(container::Relation::MotivatedByGoal, need, 5);
    std::vector<container::Edge> causesDesireEdges = this->cn->getOutgoingEdges(container::Relation::CausesDesire, need, 5);
#ifdef HUMAN_NEEDS_DEBUG
    int size = (motivatedEdges.size() < 5 ? motivatedEdges.size() : 5);
    std::cout << "BasicHumanNeeds: Motivated Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << motivatedEdges.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "BasicHumanNeeds: Desired Edges: " << std::endl;
    for (int i = 0; i < (causesDesireEdges.size() < 5 ? causesDesireEdges.size() : 5); i++) {
        std::cout << "\t" << causesDesireEdges.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 2. ask ConceptNet for Synonyms for top 5 results from 1.
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    size = (motivatedEdges.size() < 5 ? motivatedEdges.size() : 5);

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
    size = (causesDesireEdges.size() < 5 ? causesDesireEdges.size() : 5);
    getSynonyms(causesDesireEdges, size, synonymsDesire, true);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for CausesDesire Edges : " << std::endl;
    for (int i = 0; i < synonymsDesire.size(); i++) {
        std::cout << "\t" << synonymsDesire.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 3. select top 5 from resulting concepts
    size = (synonymsMotivated.size() < 5 ? synonymsMotivated.size() : 5);
    // 4. ask ConceptNet for UsedFor(concept, WILDCARD)
    std::vector<container::Edge> usedForEdges;
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(container::Relation::UsedFor, synonymsMotivated.at(i).fromConcept.term, 5);
        insertNewEdges(tmp, usedForEdges);
    }
    // 3. select top 5 from resulting concepts
    size = (synonymsDesire.size() < 5 ? synonymsDesire.size() : 5);
    // 4. ask ConceptNet for UsedFor(concept, WILDCARD)
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(container::Relation::UsedFor, synonymsDesire.at(i).fromConcept.term, 5);
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
    // 5. ask ConceptNet for Synonyms for top 5 results from 4
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    std::vector<container::Edge> synonyms;
    getSynonyms(usedForEdges, (usedForEdges.size() < 5 ? usedForEdges.size() : 5), synonyms, false);
#ifdef HUMAN_NEEDS_DEBUG
    std::cout << "BasicHumanNeeds: Synonyms for UsedFor Edges: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "\t" << synonyms.at(i).toString() << std::endl;
    }
    std::cout << std::endl;
#endif
    // 6. return top 5
    std::sort(synonyms.begin(), synonyms.end());
    size = (synonyms.size() < 5 ? synonyms.size() : 5);
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
        tmp = cn->getOutgoingEdges(container::Synonym, concept, 5);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::IsA, concept, 5);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::SimilarTo, concept, 5);
        insertNewEdges(tmp, synonyms);
        tmp = cn->getOutgoingEdges(container::InstanceOf, concept, 5);
        insertNewEdges(tmp, synonyms);
    }
}
} // namespace wm
} // namespace srg