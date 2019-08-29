#pragma once

#include <map>
#include <string>

namespace srg
{
namespace conceptnet
{
class Edge;
}
namespace dialogue
{
class AnswerGraph;
}
namespace asp
{

class ASPTranslator
{
public:
    void extractASPProgram(srg::dialogue::AnswerGraph* answerGraph);

private:
    std::string expandConceptNetPredicate(std::string predicate);
    std::string createASPPredicates(srg::dialogue::AnswerGraph* answerGraph);
    std::string conceptToASPPredicate(std::string concept);

    std::map<std::string, std::string> extractBackgroundKnowledgePrograms(srg::dialogue::AnswerGraph* answerGraph);
    std::string createBackgroundKnowledgeRule(std::string relation, srg::conceptnet::Edge* edge);

    static const std::string CONCEPTNET_PREFIX;
};

} // namespace asp
} // namespace srg