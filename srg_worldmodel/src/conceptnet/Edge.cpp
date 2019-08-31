#include "srg/conceptnet/Edge.h"

#include "srg/conceptnet/Concept.h"

#include <sstream>

namespace srg
{
namespace conceptnet
{

Edge::Edge(std::string id, std::string language, Concept* fromConcept, Concept* toConcept, srg::conceptnet::Relation relation, double weight)
{
    this->id = id;
    this->language = language;
    this->weight = weight;
    this->relation = relation;
    this->fromConcept = fromConcept;
    this->toConcept = toConcept;
}

Edge::~Edge() {
}

conceptnet::Concept* Edge::getOpposite(srg::conceptnet::Concept *concept) {
    if (this->fromConcept == concept) {
        return this->toConcept;
    } else if (this->toConcept == concept){
        return this->fromConcept;
    } else {
        return nullptr;
    }
}

std::string Edge::toString(std::string indent) const
{
    std::stringstream ss;
    ss << indent << "Edge with Language: " << this->language << " From Concept: " << this->fromConcept->term << " Sense: " << this->fromConcept->senseLabel
       << " Relation: " << relations[this->relation] << " to Concept: " << this->toConcept->term << " Sense: " << this->toConcept->senseLabel
       << " Weight: " << this->weight << " Causes Inconsistency: " << (this->causesInconsistency ? "\033[1;31mtrue\033[0m" : "false");
    return ss.str();
}

bool Edge::operator<(const srg::conceptnet::Edge& another)
{
    return weight > another.weight;
}

} // namespace conceptnet
} // namespace srg

bool operator==(const srg::conceptnet::Edge& one, const srg::conceptnet::Edge& another)
{
    return one.fromConcept->term == another.fromConcept->term && one.toConcept->term == another.toConcept->term && one.relation == another.relation;
}
