#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"

#include <iostream>
#include <algorithm>

namespace srg
{
namespace conceptnet
{
Concept::Concept(std::string id, std::string term, std::string senseLabel)
        : id(id)
        , term(term)
        , senseLabel(senseLabel)
{
}

void Concept::addEdges(std::vector<srg::conceptnet::Edge*> edges)
{
    for (Edge* edge : edges) {
        if (std::find(this->edges.begin(), this->edges.end(), edge) == this->edges.end()) {
            this->edges.push_back(edge);
        }
    }
}

const std::vector<Edge*> Concept::getEdges()
{
    return edges;
}

const std::vector<Concept*> Concept::getConnectedConcepts(Relation relation, bool includeEquivalents)
{
    std::vector<conceptnet::Relation> relations;
    relations.push_back(relation);
    return this->getConnectedConcepts(relations, includeEquivalents);
}

const std::vector<Concept*> Concept::getConnectedConcepts(std::vector<conceptnet::Relation> relations, bool includeEquivalents)
{
    std::vector<Concept*> resultConcepts;
    for (Edge* edge : this->edges) {
        for (conceptnet::Relation relation : relations) {
            if (edge->relation == relation) {
                std::cout << "Concept: " << this->id << " " << this << " " << edge->fromConcept->id << " " << edge->fromConcept << std::endl;
                if (this == edge->fromConcept) {
                    resultConcepts.push_back(edge->toConcept);
                } else {
                    resultConcepts.push_back(edge->fromConcept);
                }
            }
        }
    }

    if (!includeEquivalents) {
        return resultConcepts;
    }

    std::vector<Concept*> equivalentConcepts;
    for (Concept* concept : resultConcepts) {
        for (Edge* edge : concept->edges) {
            if (edge->relation == Relation::Synonym || edge->relation == Relation::InstanceOf || edge->relation == Relation::SimilarTo) {
                if (concept == edge->fromConcept) {
                    equivalentConcepts.push_back(edge->toConcept);
                } else {
                    equivalentConcepts.push_back(edge->fromConcept);
                }
            }
        }
    }

    resultConcepts.insert(resultConcepts.end(), equivalentConcepts.begin(), equivalentConcepts.end());
    return resultConcepts;
}

} // namespace conceptnet
} // namespace srg
