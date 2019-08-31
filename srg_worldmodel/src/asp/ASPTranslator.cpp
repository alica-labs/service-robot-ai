#include "srg/asp/ASPTranslator.h"

#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"
#include "srg/conceptnet/Relations.h"
#include "srg/dialogue/AnswerGraph.h"

#include <algorithm>
#include <vector>

namespace srg
{
namespace asp
{

const std::string ASPTranslator::CONCEPTNET_PREFIX = "cn5_";

ASPTranslator::ASPTranslator(srg::SRGWorldModel* wm)
        : wm(wm)
{
}

std::string ASPTranslator::extractASPProgram(srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval)
{

    std::string programSection = "#program cn5_commonsenseKnowledge";
    if (inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
        answerGraph->markInconsistentEdges();
    }
    std::string program = programSection;
    program.append(".\n");
    auto tmp = createASPPredicates(answerGraph, inconsistencyRemoval);
    program.append(tmp);

    program.append("\n");
    auto pgmMap = extractBackgroundKnowledgePrograms(answerGraph, inconsistencyRemoval);
    for (auto pair : pgmMap) {
        program.append(pair.second).append("\n");
        // this->gui->getSolver()->add(programSection.toStdString().c_str(), {}, pair.second.toStdString().c_str());
    }
    return program;
}

std::string ASPTranslator::conceptToASPPredicate(std::string concept)
{
    std::replace(concept.begin(), concept.end(), '.', '_');
    std::replace(concept.begin(), concept.end(), ',', '_');
    std::replace(concept.begin(), concept.end(), ' ', '_');
    return concept;
}

std::string ASPTranslator::createASPPredicates(srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval)
{
    std::string ret = "";
    for (auto pair : answerGraph->getEdges()) {
        if(inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
            if(pair.second->causesInconsistency) {
                continue;
            }
        }
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

std::map<std::string, std::string> ASPTranslator::extractBackgroundKnowledgePrograms(
        srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval)
{
    std::map<std::string, std::string> ret;
    std::vector<std::string> addedRelations;
    for (auto pair : answerGraph->getEdges()) {
        if(inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
            if(pair.second->causesInconsistency) {
                continue;
            }
        }
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
    std::string capitalRelation = relation;
    capitalRelation[0] = std::toupper(capitalRelation[0]);
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
            .append(capitalRelation)
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