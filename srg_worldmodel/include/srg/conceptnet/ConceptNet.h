#pragma once

#include "srg/conceptnet/Edge.h"
#include "srg/conceptnet/Relations.h"

#include <yaml-cpp/yaml.h>

#include <map>
#include <string>

namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
}
namespace conceptnet
{
class CNManager;
class ConceptNet
{
public:
    explicit ConceptNet(SRGWorldModel* wm);
    virtual ~ConceptNet() = default;

    Concept* getConcept(CNManager* cnManager, const std::string& conceptName);
    std::vector<Edge*> getEdges(CNManager* cnManager, const std::string& concept, int limit = 1000);
    std::vector<Edge*> getEdges(CNManager* cnManager, Relation relation, const std::string& concept, int limit = 1000);
    std::vector<Edge*> getCompleteEdge(CNManager* cnManager, Relation relation, const std::string& fromConcept, const std::string& toConcept, int limit = 1000);
    std::vector<Edge*> getOutgoingEdges(CNManager* cnManager, Relation relation, const std::string& fromConcept, int limit = 1000);
    std::vector<Edge*> getIncomingEdges(CNManager* cnManager, Relation relation, const std::string& toConcept, int limit = 1000);
    std::vector<Edge*> getRelations(CNManager* cnManager, const std::string& concept, const std::string& otherConcept, int limit = 1000);
    std::vector<Edge*> getEquivalentOutgoingEdges(CNManager* cnManager, const conceptnet::Concept* concept, int limit = 1000);
    void findInconsistencies(srg::dialogue::AnswerGraph* answerGraph, int limit = 1000);

    double getRelatedness(const std::string& firstConcept, const std::string& secondConcept);

private:
    SRGWorldModel* wm;
    std::string httpGet(const std::string& url);
    bool isValid(const YAML::Node& node);
    Relation getRelation(const std::string& relation);
    bool conceptContainsNonASCII(const std::string& concept);
    std::string trimTerm(const std::string& term);
    std::string generateEdges(CNManager* cnManager, const std::string& json, std::vector<Edge*>& edges, int limit = -1, double minWeight = 1.0);
    void collectAntonyms(srg::dialogue::AnswerGraph* answerGraph, int limit=1000);
    std::vector<Concept*> getNewAdjectives(srg::dialogue::AnswerGraph* answerGraph);

    /**
     * Contains the begin of a concept net query url.
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

} // namespace conceptnet
} /* namespace srg */
