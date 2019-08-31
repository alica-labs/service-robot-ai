#pragma once

#include "srg/conceptnet/Relations.h"
#include "srg/conceptnet/CNManager.h"

#include <vector>
#include <string>
#include <map>
#include <cgraph.h>

namespace srg
{
namespace conceptnet
{
class Concept;
class Edge;
class ConceptPath;
}
namespace dialogue
{
class AnswerGraph : public conceptnet::CNManager
{
public:
    AnswerGraph();
    ~AnswerGraph();
    srg::conceptnet::Concept* root;
    std::vector<srg::conceptnet::Concept*> answerConcepts;
    std::vector<srg::conceptnet::ConceptPath*> answerPaths;
    std::map<std::string, std::vector<srg::conceptnet::Edge*>> adjectiveAntonymMap;
    std::string toString();
    void renderDot(Agraph_t* g, bool markInconsistencies = false);

    void calculateUtilities();
    std::vector<srg::conceptnet::Concept*> getBestAnswers(int maxNumberOfAnswers);

    void setRoot(srg::conceptnet::Concept* root);
    conceptnet::Concept* getConcept(std::string conceptId) const override;
    conceptnet::Concept* createConcept(std::string conceptId, std::string term, std::string senseLabel) override;

    conceptnet::Edge* getEdge(std::string edgeId) const override;
    std::vector<conceptnet::Edge*> getEdges(conceptnet::Concept* firstConcept, conceptnet::Concept* secondConcept);
    conceptnet::Edge* createEdge(std::string edgeId, std::string language, conceptnet::Concept* fromConcept, conceptnet::Concept* toConcept, srg::conceptnet::Relation relation, double weight) override;

    const std::map<std::string, conceptnet::Concept *> &getConcepts() const;
    const std::map<std::string, conceptnet::Edge *> &getEdges() const;

    void markInconsistentEdges();

private:
    bool utilitiesCalculated;
    std::map<std::string, conceptnet::Concept*> concepts;
    std::map<std::string, conceptnet::Edge*> edges;
    std::map<conceptnet::Concept*, double> utilities;

    void generateEdge(Agraph_t *g, std::vector<conceptnet::Concept *> &openNodes,
                                   std::string term, const conceptnet::Edge *edge);
};
} // namespace dialogue
} // namespace srg