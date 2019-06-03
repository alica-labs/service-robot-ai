#include "srg/container/Edge.h"

#include <sstream>

namespace srg
{
namespace container
{

Edge::Edge(std::string id, std::string language, Concept fromConcept, Concept toConcept, srg::container::Relation relation, double weight)
        : fromConcept(fromConcept)
        , toConcept(toConcept)
{
    this->id = id;
    this->language = language;
    this->weight = weight;
    this->relation = relation;
}

Edge::Edge(const Edge& edge)
        : fromConcept(edge.fromConcept)
        , toConcept(edge.toConcept)
{
    this->id = edge.id;
    this->language = edge.language;
    this->weight = edge.weight;
    this->relation = edge.relation;
}

Edge::~Edge() {}

std::string Edge::toString()
{
    std::stringstream ss;
    ss << "Edge with Language: " << this->language << " From Concept: " << this->fromConcept.term << " Sense: " << this->fromConcept.senseLabel
       << " Relation: " << relations[this->relation] << " to Concept: " << this->toConcept.term << " Sense: " << this->toConcept.senseLabel
       << " Weight: " << this->weight << std::endl;
    return ss.str();
}
} // namespace container
} // namespace srg