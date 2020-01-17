#pragma once

#include "srg/conceptnet/Edge.h"
#include <srg/agent/containers/SpeechAct.h>

#include <string>
#include <vector>
#include <memory>

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

    std::shared_ptr<srg::agent::SpeechAct> answerNeed(const srg::agent::SpeechAct needAct);
    static const int bestNumberOfElements;

private:
    SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;

    void createAnswerPaths(AnswerGraph* answerGraph, conceptnet::Concept *start);
};
} // namespace wm
} // namespace srg
