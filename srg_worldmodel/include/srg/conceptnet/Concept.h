#pragma once

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
    Concept(std::string term, std::string senseLabel, std::string id);
    virtual ~Concept() = default;
    Concept(const Concept& concept);

    std::string term;
    std::string senseLabel;
    std::string id;

    std::vector<Edge*> outgoingEdges;
    std::vector<Edge*> incomingEdges;
};
} // namespace container
} // namespace srg
