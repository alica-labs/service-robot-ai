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
    BasicHumanNeeds(SRGWorldModel* wm);

    AnswerGraph* answerNeed(std::string need);
    static const int bestWeightedEdges;

private:
    SRGWorldModel* wm;
    conceptnet::ConceptNet* cn;

    void insertNewEdges(std::vector<conceptnet::Edge*> &from, std::vector<conceptnet::Edge*> &to) const;
    void insertNewConcepts(conceptnet::Concept* concept, std::vector<conceptnet::Concept*> &to) const;
    bool compareEdges(const conceptnet::Edge* first , const conceptnet::Edge* second);

//    void getUsedFor(const conceptnet::Concept *current) const;
};
} // namespace wm
} // namespace srg
