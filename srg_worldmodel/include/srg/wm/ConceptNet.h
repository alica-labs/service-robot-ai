#pragma once

#include "srg/container/Edge.h"
#include "srg/container/Relations.h"

#include <yaml-cpp/yaml.h>

#include <map>
#include <string>

namespace srg
{
class SRGWorldModel;
namespace wm
{

class ConceptNet
{
public:
    explicit ConceptNet(SRGWorldModel* wm);
    virtual ~ConceptNet() = default;

    std::vector<srg::container::Edge> getEdges(const std::string& concept);
    std::vector<srg::container::Edge> getCompleteEdge(srg::container::Relation relation, const std::string& fromConcept, const std::string& toConcept);
    std::vector<srg::container::Edge> getOutgoingEdges(srg::container::Relation relation, const std::string& fromConcept);
    std::vector<srg::container::Edge> getIncomingEdges(srg::container::Relation relation, const std::string& toConcept);
    std::vector<srg::container::Edge> getRelations(const std::string& concept, const std::string& otherConcept);
    double getRelatedness(const std::string& firstConcept, const std::string& secondConcept);

private:
    SRGWorldModel* wm;
    std::string httpGet(const std::string& url);
    bool isValid(const YAML::Node& node);
    srg::container::Relation getRelation(const std::string& relation);
    bool conceptContainsNonASCII(const std::string& concept);
    std::string trimTerm(const std::string& term);
    void generateEdges(const std::string& json, std::vector<srg::container::Edge>& edges);

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
