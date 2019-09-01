#pragma once

#include <map>
#include <string>

namespace srg
{
class SRGWorldModel;
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
    enum InconsistencyRemoval
    {
        None,
        KeepHighestWeight,
        External
    };

    ASPTranslator(srg::SRGWorldModel* wm);
    std::string extractASPProgram(srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval = InconsistencyRemoval::None);

private:
    std::string expandConceptNetPredicate(std::string predicate);
    std::string createASPPredicates(srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval = InconsistencyRemoval::None);
    std::string conceptToASPPredicate(std::string concept);

    std::map<std::string, std::string> extractBackgroundKnowledgePrograms(
            srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval = InconsistencyRemoval::None);
    std::string createBackgroundKnowledgeRule(std::string relation, srg::conceptnet::Edge* edge);
    std::string createInconsistencyBackgroundKnowledgeRule(std::string relation, std::string adjective, std::string antonym);

    static const std::string CONCEPTNET_PREFIX;
    srg::SRGWorldModel* wm;
};

} // namespace asp
} // namespace srg