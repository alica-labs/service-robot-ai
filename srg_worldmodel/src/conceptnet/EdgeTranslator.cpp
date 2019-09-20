#include "srg/conceptnet/EdgeTranslator.h"

#include "srg/conceptnet/Relations.h"
#include "srg/conceptnet/Concept.h"

#include <algorithm>
#include <map>

namespace srg
{
namespace wm
{

const std::string EdgeTranslator::PREFIX = "cn5_";
const std::string EdgeTranslator::COMMONSENSE_KNOWLEDGE = "#program cn5_commonsenseKnowledge";
const std::string EdgeTranslator::SITUATIONAL_KNOWLEDGE = "#program cn5_situationalKnowledge(n,m).\n";

std::string EdgeTranslator::translate(std::vector<srg::conceptnet::Edge>& edges)
{
    std::string program = EdgeTranslator::COMMONSENSE_KNOWLEDGE;
    program.append(".\n");
    auto tmp = createASPPredicates(edges);
    program.append(tmp);

    std::map<std::string, std::string> relationMap;
    std::vector<std::string> addedRelations;
    for (auto& edge : edges) {
        std::string rel = srg::conceptnet::relations[edge.relation];
        std::transform(rel.begin(), rel.begin() + 1, rel.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
        auto it = find(addedRelations.begin(), addedRelations.end(), rel);
        if (it == addedRelations.end()) {
            addedRelations.push_back(rel);
            std::string pgm = EdgeTranslator::SITUATIONAL_KNOWLEDGE;
            pgm.append("#external -").append(rel).append("(n, m).\n");
            pgm.append(createBackgroundKnowledgeRule(rel, edge));
            relationMap.emplace(rel, pgm);
        } else {
            relationMap.at(rel).append(createBackgroundKnowledgeRule(rel, edge));
        }
    }
    for (const auto& pair : relationMap) {
       program.append(pair.second).append("\n");
    }
    return program;
}

std::string EdgeTranslator::createBackgroundKnowledgeRule(std::string& relation, srg::conceptnet::Edge& edge)
{
    std::string ret = relation;
    ret.append("(n, m, W) :- not -")
            .append(relation)
            .append("(n, m), typeOf(n, ")
            .append(EdgeTranslator::PREFIX)
            .append(conceptToASPPredicate(edge.fromConcept->term))
            .append("), typeOf(m, ")
            .append(EdgeTranslator::PREFIX)
            .append(conceptToASPPredicate(edge.toConcept->term))
            .append("), ")
            .append(EdgeTranslator::PREFIX)
            .append(relation)
            .append("(")
            .append(EdgeTranslator::PREFIX)
            .append(conceptToASPPredicate(edge.fromConcept->term))
            .append(",")
            .append(EdgeTranslator::PREFIX)
            .append(conceptToASPPredicate(edge.toConcept->term))
            .append(", W).\n");
    return ret;
}

std::string EdgeTranslator::createASPPredicates(std::vector<srg::conceptnet::Edge>& edges)
{
    std::string ret;
    for (const auto& edge : edges) {
        std::string tmp;
        std::string rel = srg::conceptnet::relations[edge.relation];
        std::transform(rel.begin(), rel.begin() + 1, rel.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
        tmp.append(EdgeTranslator::PREFIX).append(rel);
        tmp.append("(")
                .append(EdgeTranslator::PREFIX)
                .append(conceptToASPPredicate(edge.fromConcept->term))
                .append(", ")
                .append(EdgeTranslator::PREFIX)
                .append(conceptToASPPredicate(edge.toConcept->term))
                .append(", ")
                .append(std::to_string(edge.weight))
                .append(").\n");
        ret.append(tmp);
    }
    return ret;
}

std::string EdgeTranslator::conceptToASPPredicate(std::string concept)
{
    std::replace(concept.begin(), concept.end(), '.', '_');
    std::replace(concept.begin(), concept.end(), ',', '_');
    std::replace(concept.begin(), concept.end(), ' ', '_');
    return concept;
}
} // namespace wm

} // namespace srg