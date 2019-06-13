#pragma once

#include <vector>
#include <string>

namespace srg
{
namespace conceptnet
{
class Edge;
class Concept;
class ConceptPath
{
public:
    ConceptPath(Concept* end);
    ConceptPath(const ConceptPath& other);
    ~ConceptPath() = default;
    void addToPath(conceptnet::Edge* edge);
    void calculateUtility();
    bool containsNode(conceptnet::Concept* concept);
    conceptnet::Concept* getEnd() const;
    std::vector<conceptnet::Edge*> getPath() const;
    std::string toString(std::string indent = "") const;

private:
    conceptnet::Concept* end;
    std::vector<conceptnet::Edge*> path;
    double utility;
public:
    double getUtility() const;

private:

    double getEdgeWeight(conceptnet::Edge* edge);
};
} // namespace conceptnet
} // namespace srg
