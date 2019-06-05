#include "srg/wm/BasicHumanNeeds.h"
#include "srg/SRGWorldModel.h"
#include "srg/container/Relations.h"
#include "srg/wm/ConceptNet.h"

namespace srg
{
namespace wm
{
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
    motivatedEdges.insert(motivatedEdges.end(), causesDesireEdges.begin(), causesDesireEdges.end());
    std::sort(motivatedEdges.begin(), motivatedEdges.end());
    // 2. ask ConceptNet for Synonyms for top 5 results from 1.
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    int size = (motivatedEdges.size() < 5 ? motivatedEdges.size() : 5);
    std::vector<container::Edge> synonyms;
    getSynonyms(motivatedEdges, size, synonyms);
    // 3. select top 5 from resulting concepts
    std::sort(synonyms.begin(), synonyms.end());
    size = (synonyms.size() < 5 ? synonyms.size() : 5);
    // 4. ask ConceptNet for UsedFor(concept, WILDCARD)
    std::vector<container::Edge> usedForEdges;
    for (int i = 0; i < size; i++) {
        auto tmp = this->cn->getIncomingEdges(container::Relation::UsedFor, synonyms.at(i).fromConcept.term, 5);
        usedForEdges.insert(usedForEdges.end(), tmp.begin(), tmp.end());
    }
    // 5. ask ConceptNet for Synonyms for top 5 results from 4
    //=> Synonym, IsA(subtype), SimilarTo, InstanceOf
    synonyms.clear();
    getSynonyms(usedForEdges, (usedForEdges.size() < 5 ? usedForEdges.size() : 5), synonyms);
    // 6. return top 5
    std::sort(synonyms.begin(), synonyms.end());
    size = (synonyms.size() < 5 ? synonyms.size() : 5);
    std::vector<std::string> answer;
    for (int i = 0; i < size; i++) {
        answer.push_back(synonyms.at(i).fromConcept.term);
    }
    return answer;
}

void BasicHumanNeeds::getSynonyms(std::vector<container::Edge>& edges, int size, std::vector<container::Edge>& synonyms) const
{
    synonyms.insert(synonyms.begin(), edges.begin(), edges.begin() + size);
    std::vector<container::Edge> tmp;
    for (int i = 0; i < size; i++) {
        tmp = cn->getOutgoingEdges(container::Synonym, edges.at(i).toConcept.term, 5);
        synonyms.insert(synonyms.end(), tmp.begin(), tmp.end());
        tmp = cn->getOutgoingEdges(container::IsA, edges.at(i).toConcept.term , 5);
        synonyms.insert(synonyms.end(), tmp.begin(), tmp.end());
        tmp = cn->getOutgoingEdges(container::SimilarTo, edges.at(i).toConcept.term, 5);
        synonyms.insert(synonyms.end(), tmp.begin(), tmp.end());
        tmp = cn->getOutgoingEdges(container::InstanceOf, edges.at(i).toConcept.term, 5);
        synonyms.insert(synonyms.end(), tmp.begin(), tmp.end());
    }
}
} // namespace wm
} // namespace srg