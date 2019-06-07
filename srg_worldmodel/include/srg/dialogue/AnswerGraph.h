#pragma once

#include <vector>

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

    srg::conceptnet::Concept* graph;
    std::vector<srg::conceptnet::Concept*> answerConcepts;
};
} // namespace dialogue
} // namespace srg