#pragma once

#include "srg/conceptnet/Relations.h"

#include <string>
#include <vector>

namespace srg
{
namespace conceptnet
{
class Concept;
class Edge
{
public:
    Edge(std::string id, std::string language, Concept* fromConcept, Concept* toConcept, srg::conceptnet::Relation relation, double weight);
    virtual ~Edge();

    std::string id;
    std::string language;
    Concept* fromConcept;
    Concept* toConcept;
    srg::conceptnet::Relation relation;
    double weight;
    bool causesInconsistency = false;
    std::vector<std::string> sources;

    std::string toString(std::string indent = "") const;

    conceptnet::Concept* getOpposite(conceptnet::Concept* concept);

    bool operator<(const srg::conceptnet::Edge& another);
};
} // namespace container
} // namespace srg

bool operator==(const srg::conceptnet::Edge& one, const srg::conceptnet::Edge& another);

