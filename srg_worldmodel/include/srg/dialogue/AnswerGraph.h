#pragma once

#include <vector>
#include <string>

namespace srg
{
namespace conceptnet
{
class Concept;
}
namespace dialogue
{
class AnswerGraph
{
public:
    AnswerGraph(srg::conceptnet::Concept* root);
    ~AnswerGraph();
    srg::conceptnet::Concept* graph;
    std::vector<srg::conceptnet::Concept*> answerConcepts;
    std::string toString();
};
} // namespace dialogue
} // namespace srg