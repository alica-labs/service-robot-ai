#pragma once

#include "srg/container/Concept.h"
#include "srg/container/Relations.h"

#include <string>
#include <vector>

namespace srg
{
namespace container
{

class Edge
{
public:
    Edge(std::string id, std::string language, Concept fromConcept, Concept toConcept, srg::container::Relation relation, double weight);
    virtual ~Edge();
    Edge(const Edge& edge);

    std::string id;
    std::string language;
    Concept fromConcept;
    Concept toConcept;
    srg::container::Relation relation;
    double weight;
    std::vector<std::string> sources;

    std::string toString();

    bool operator< (const Edge& other) {
        return weight > other.weight;
    }
};
} // namespace container
} // namespace srg
