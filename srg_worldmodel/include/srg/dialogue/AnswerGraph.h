#pragma once

#include "srg/conceptnet/Relations.h"
#include "srg/conceptnet/CNManager.h"

#include <vector>
#include <string>
#include <map>

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
    std::string toString();
    void renderDot();

    void calculateUtilities();
    std::vector<srg::conceptnet::Concept*> getBestAnswers(int maxNumberOfAnswers);

    void setRoot(srg::conceptnet::Concept* root);
    conceptnet::Concept* getConcept(std::string conceptId) const override;
    conceptnet::Concept* createConcept(std::string conceptId, std::string term, std::string senseLabel) override;

    conceptnet::Edge* getEdge(std::string edgeId) const override;
    conceptnet::Edge* createEdge(std::string edgeId, std::string language, conceptnet::Concept* fromConcept, conceptnet::Concept* toConcept, srg::conceptnet::Relation relation, double weight) override;
private:
    bool utilitiesCalculated;
    std::map<std::string, conceptnet::Concept*> concepts;
    std::map<std::string, conceptnet::Edge*> edges;
    std::map<conceptnet::Concept*, double> utilities;
};
} // namespace dialogue
} // namespace srg