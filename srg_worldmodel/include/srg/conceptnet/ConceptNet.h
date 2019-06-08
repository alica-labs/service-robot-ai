#pragma once

#include "srg/conceptnet/Edge.h"
#include "srg/conceptnet/Relations.h"

#include <yaml-cpp/yaml.h>

#include <map>
#include <string>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{

class ConceptNet
{
public:
    explicit ConceptNet(SRGWorldModel* wm);
    virtual ~ConceptNet() = default;

    srg::conceptnet::Concept* getConcept(const std::string& concept);
    std::vector<srg::conceptnet::Edge*> getEdges(const std::string& concept, int limit=1000);
    std::vector<srg::conceptnet::Edge*> getEdges(srg::conceptnet::Relation relation, const std::string& concept, int limit=1000);
    std::vector<srg::conceptnet::Edge*> getCompleteEdge(srg::conceptnet::Relation relation, const std::string& fromConcept, const std::string& toConcept, int limit=1000);
    std::vector<srg::conceptnet::Edge*> getOutgoingEdges(srg::conceptnet::Relation relation, const std::string& fromConcept, int limit=1000);
    std::vector<srg::conceptnet::Edge*> getIncomingEdges(srg::conceptnet::Relation relation, const std::string& toConcept, int limit=1000);
    std::vector<srg::conceptnet::Edge*> getRelations(const std::string& concept, const std::string& otherConcept, int limit=1000);
    double getRelatedness(const std::string& firstConcept, const std::string& secondConcept);

private:
    SRGWorldModel* wm;
    std::string httpGet(const std::string& url);
    bool isValid(const YAML::Node& node);
    srg::conceptnet::Relation getRelation(const std::string& relation);
    bool conceptContainsNonASCII(const std::string& concept);
    std::string trimTerm(const std::string& term);
    void generateEdges(const std::string& json, std::vector<srg::conceptnet::Edge*>& edges, double minWeight = 1.0);

    /**
     * Containts the begin of a concept net query url.
     */
    static const std::string BASE_URL;
    /**
     * Query part: /query?start=/c/en/
     */
    static const std::string QUERYSTART;
    /**
     * Query part: /query?end=/c/en/
     */
    static const std::string QUERYEND;
    /**
     * Query part: /query?node=/c/en/
     */
    static const std::string QUERYNODE;
    /**
     * Query part: /query?other=/c/en/
     */
    static const std::string QUERYOTHER;
    /**
     * Wildcard string
     */
    static const std::string WILDCARD;
    /**
     * Limit part
     */
    static const std::string LIMIT;
    /**
     * Relation part
     */
    static const std::string RELATION;
    /**
     * End part
     */
    static const std::string END;
    /**
     * Relatedness part
     */
    static const std::string RELATEDNESS;
    /**
     * Nod1 part
     */
    static const std::string NODE1;
    /**
     * Node2 part
     */
    static const std::string NODE2;
};

} /* namespace wm */
} /* namespace srg */
