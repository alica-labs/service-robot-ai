#pragma once

#include "srg/conceptnet/Relations.h"

#include <map>
namespace srg
{
namespace conceptnet
{
class Concept;
class Edge;
class CNManager
{
public:
    virtual conceptnet::Concept* getConcept(std::string conceptId) const = 0;
    virtual conceptnet::Concept* createConcept(std::string conceptId, std::string term, std::string senseLabel) = 0;

    virtual conceptnet::Edge* getEdge(std::string edgeId) const = 0;
    virtual conceptnet::Edge* createEdge(std::string edgeId, std::string language, conceptnet::Concept* fromConcept, conceptnet::Concept* toConcept, srg::conceptnet::Relation relation, double weight) = 0;

private:
    std::map<std::string, conceptnet::Concept*> concepts;
    std::map<std::string, conceptnet::Edge*> edges;
};
} // namespace conceptnet
} // namespace srg
