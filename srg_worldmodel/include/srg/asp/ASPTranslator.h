#pragma once

#include <map>
#include <srg/conceptnet/Concept.h>
#include <string>

namespace srg
{
class SRGWorldModel;
namespace conceptnet
{
class Edge;
class Concept;
} // namespace conceptnet
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
        UseExternals
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
    std::string createInconsistencyBackgroundKnowledgeRule(
            srg::conceptnet::Concept* root, srg::conceptnet::Concept* adjective, std::string antonym, std::string relation);
    std::string createPropertyBackgroundKnowledgeRule(srg::conceptnet::Concept* root, srg::conceptnet::Concept* adjective, std::string relation);

    std::vector<std::string> split(std::string toSplit);
    std::string trim(const std::string& s);

    static const std::string CONCEPTNET_PREFIX;
    srg::SRGWorldModel* wm;
};

} // namespace asp
} // namespace srg