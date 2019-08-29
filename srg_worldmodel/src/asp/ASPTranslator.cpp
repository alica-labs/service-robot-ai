#include "srg/asp/ASPTranslator.h"

#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"
#include "srg/conceptnet/Relations.h"
#include "srg/dialogue/AnswerGraph.h"

#include <vector>
#include <algorithm>

namespace srg
{
namespace asp
{

const std::string ASPTranslator::CONCEPTNET_PREFIX = "cn5_";

void ASPTranslator::extractASPProgram(srg::dialogue::AnswerGraph* answerGraph)
{

    std::string programSection = "#program cn5_commonsenseKnowledge";
    std::string program = programSection;
    program.append(".\n");
    auto tmp = createASPPredicates(answerGraph);
    program.append(tmp);

    auto pgmMap = extractBackgroundKnowledgePrograms(answerGraph);
    /*for (auto pair : pgmMap) {
        this->gui->getSolver()->add(programSection.toStdString().c_str(), {}, pair.second.toStdString().c_str());
    }*/
}

std::string ASPTranslator::conceptToASPPredicate(std::string concept)
{
    if (concept.find('.')) {
        concept.replace(concept.begin(), concept.end(),'.', '_');
    }
    if (concept.find(',')) {
        concept.replace(concept.begin(), concept.end(),',', '_');
    }
    if (concept.find(' ')) {
        concept.replace(concept.begin(), concept.end(),' ', '_');
    }
    return concept;
}

std::string ASPTranslator::createASPPredicates(srg::dialogue::AnswerGraph* answerGraph)
{
    std::string ret = "";
    for (auto pair : answerGraph->getEdges()) {
        std::string tmp = "";
        tmp.append(ASPTranslator::CONCEPTNET_PREFIX).append(srg::conceptnet::relations[pair.second->relation]);
        tmp.append("(")
                .append(ASPTranslator::CONCEPTNET_PREFIX)
                .append(conceptToASPPredicate(pair.second->fromConcept->term))
                .append(", ")
                .append(ASPTranslator::CONCEPTNET_PREFIX)
                .append(conceptToASPPredicate(pair.second->toConcept->term))
                .append(", ")
                .append(std::to_string(pair.second->weight))
                .append(").\n");
        ret.append(tmp);
    }
    return ret;
}

std::map<std::string, std::string> ASPTranslator::extractBackgroundKnowledgePrograms(srg::dialogue::AnswerGraph* answerGraph)
{
    std::map<std::string, std::string> ret;
    std::vector<std::string> addedRelations;
    for (auto pair : answerGraph->getEdges()) {
        std::string tmpRel = srg::conceptnet::relations[pair.second->relation];
        tmpRel[0] = std::tolower(tmpRel[0]);
        auto it = std::find(addedRelations.begin(), addedRelations.end(), tmpRel);
        if (it == addedRelations.end()) {
            addedRelations.push_back(tmpRel);
            std::string pgm = "#program cn5_situationalKnowledge(n,m).\n";
            pgm.append("#external -").append(tmpRel).append("(n, m).\n");
            pgm.append(createBackgroundKnowledgeRule(tmpRel, pair.second));
            ret.emplace(tmpRel, pgm);
        } else {
            ret.at(tmpRel).append(createBackgroundKnowledgeRule(tmpRel, pair.second));
        }
    }
    return ret;
}

std::string ASPTranslator::createBackgroundKnowledgeRule(std::string relation, srg::conceptnet::Edge* edge)
{
    std::string ret = relation;
    ret.append("(n, m, W) :- not -")
            .append(relation)
            .append("(n, m), typeOf(n, ")
            .append(ASPTranslator::CONCEPTNET_PREFIX)
            .append(conceptToASPPredicate(edge->fromConcept->term))
            .append("), typeOf(m, ")
            .append(ASPTranslator::CONCEPTNET_PREFIX)
            .append(conceptToASPPredicate(edge->toConcept->term))
            .append("), ")
            .append(ASPTranslator::CONCEPTNET_PREFIX)
            .append(relation.replace(0, 1, std::to_string(std::toupper(relation.at(0)))))
            .append("(")
            .append(ASPTranslator::CONCEPTNET_PREFIX)
            .append(conceptToASPPredicate(edge->fromConcept->term))
            .append(",")
            .append(ASPTranslator::CONCEPTNET_PREFIX)
            .append(conceptToASPPredicate(edge->toConcept->term))
            .append(", W).\n");
    return ret;
}

std::string ASPTranslator::expandConceptNetPredicate(std::string predicate)
{
    std::string ret = "";
    ret.append(predicate).append(" :- not ").append("-").append(predicate).append(".\n");
    return ret;
}

} // namespace asp
} // namespace srg