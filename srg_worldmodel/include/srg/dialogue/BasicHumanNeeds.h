#pragma once

#include "srg/conceptnet/Edge.h"

#include <string>
#include <vector>

namespace srg
{
namespace conceptnet {
    class ConceptNet;
    class Concept;
    class CNManager;
}
class SRGWorldModel;
namespace dialogue
{
class AnswerGraph;
class BasicHumanNeeds
{
public:
    explicit BasicHumanNeeds(SRGWorldModel* wm);

    AnswerGraph* answerNeed(std::string need);
    static const int bestNumberOfElements;

private:
    SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;

    void createAnswerPaths(AnswerGraph* answerGraph, conceptnet::Concept *start);
};
} // namespace wm
} // namespace srg
