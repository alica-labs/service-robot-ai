#pragma once

#include "srg/container/Edge.h"

#include <string>
#include <vector>

namespace srg
{
class SRGWorldModel;
namespace wm
{
class ConceptNet;
class BasicHumanNeeds
{
public:
    BasicHumanNeeds(SRGWorldModel* wm);

    std::vector<std::string> answerNeed(std::string need);

private:
    SRGWorldModel* wm;
    ConceptNet* cn;

    void getSynonyms(std::vector<srg::container::Edge>& edges, int size, std::vector<srg::container::Edge>& synonyms) const;
    void insertNewEdges(std::vector<container::Edge> &from, std::vector<container::Edge> &to) const;
};
} // namespace wm
} // namespace srg
