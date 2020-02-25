#include "srg/conceptnet/ConceptPath.h"

#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"

#include <sstream>
#include <iostream>

namespace srg
{
namespace conceptnet
{
ConceptPath::ConceptPath(Concept* end)
        : utility(1)
{
    this->end = end;
}

ConceptPath::ConceptPath(const srg::conceptnet::ConceptPath& other)
{
    this->utility = other.utility;
    for (Edge* edge : other.getPath()) {
        this->addToPath(edge);
    }
    this->end = other.getEnd();
}

void ConceptPath::addToPath(srg::conceptnet::Edge* edge)
{
    this->path.push_back(edge);
    if (edge->toConcept == this->end) {
        this->end = edge->fromConcept;
    } else {
        this->end = edge->toConcept;
    }
}

void ConceptPath::calculateUtility()
{
    if (path.size() == 0) {
        this->utility = 0;
        return;
    }

    for (int i = 0; i < this->path.size(); i++) {
        this->utility += getEdgeWeight(this->path[i]);
    }
}

double ConceptPath::getEdgeWeight(conceptnet::Edge* edge)
{
    if (edge->relation == conceptnet::Synonym || edge->relation == conceptnet::SimilarTo || edge->relation == conceptnet::InstanceOf) {
        return 0;
    } else {
        return edge->weight;
    }
}

bool ConceptPath::containsNode(srg::conceptnet::Concept* concept)
{
    for (Edge* edge : this->path) {
//        std::cout << "ConceptPath: Node " << concept->term << "\n Edge: " << edge->toString() << std::endl;
        if (edge->fromConcept == concept || edge->toConcept == concept) {
            return true;
        }
    }
    return false;
}

conceptnet::Concept* ConceptPath::getEnd() const
{
    return this->end;
}

std::vector<conceptnet::Edge*> ConceptPath::getPath() const
{
    return this->path;
}

double ConceptPath::getUtility() const
{
    return utility;
}

std::string ConceptPath::toString(std::string indent, bool verbose) const {
    std::stringstream ss;
    if (verbose) {
        ss << indent << "#ConceptPath: End: " << this->end->term << " Length: " << this->path.size() << std::endl;
        for (Edge *edge : path) {
            ss << edge->toString(indent) << std::endl;
        }
    } else {
        for (Edge *edge : path) {
            ss << indent << edge->fromConcept->term << " - " << relations[edge->relation] << " -> " << edge->toConcept->term << std::endl;
        }
    }
    return ss.str();
}
} // namespace conceptnet
} // namespace srg
