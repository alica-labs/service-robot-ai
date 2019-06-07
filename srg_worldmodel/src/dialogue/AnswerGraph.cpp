#include "srg/dialogue/AnswerGraph.h"

#include "srg/conceptnet/Concept.h"

namespace srg
{
namespace dialogue
{

AnswerGraph::AnswerGraph(srg::conceptnet::Concept* root)
        : graph(root)
{
    this->graph = root;
}
} // namespace conceptnet

} // namespace srg