#pragma once

#include "srg/conceptnet/Relations.h"

#include <string>
#include <vector>

namespace srg
{
namespace conceptnet
{

class Edge;
class Concept
{
public:
    Concept(std::string id, std::string term, std::string senseLabel);
    virtual ~Concept() = default;

    std::string id;
    std::string term;
    std::string senseLabel;

    const std::vector<Edge*> getEdges();
    void addEdge(Edge* edge);
    void addEdges(std::vector<Edge*> edges);
    const std::vector<Concept*> getConnectedConcepts(conceptnet::Relation relation, bool includeEquivalents);
    const std::vector<Concept*> getConnectedConcepts(std::vector<conceptnet::Relation> relations, bool includeEquivalents);

private:
    std::vector<Edge*> edges;
};
} // namespace container
} // namespace srg
