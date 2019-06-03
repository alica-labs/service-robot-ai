#include "srg/container/Concept.h"

namespace srg
{
namespace container
{
Concept::Concept(std::string term, std::string senseLabel, std::string id)
{
    this->term = term;
    this->senseLabel = senseLabel;
    this->id = id;
}
Concept::Concept(const Concept& concept)
{
    this->term = concept.term;
    this->senseLabel = concept.senseLabel;
    this->id = concept.id;
}
} // namespace container
} // namespace srg
