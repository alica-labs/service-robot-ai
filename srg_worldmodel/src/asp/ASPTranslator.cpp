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
        if (inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
            if (pair.second->causesInconsistency) {
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
                .append(std::to_string((int) (pair.second->weight * 100)))
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
        if (inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
            if (pair.second->causesInconsistency) {
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
    if (inconsistencyRemoval == InconsistencyRemoval::UseExternals) {
        for (auto pair : answerGraph->adjectiveAntonymMap) {
            std::vector<std::string> closed;
            std::cout << "getting edges: " << answerGraph->root->term << " " << pair.first->term << std::endl;
            std::vector<srg::conceptnet::Edge*> edges = answerGraph->getEdges(answerGraph->root, pair.first);
            if (edges.empty()) {
                continue;
            }
            std::string pgm = "#program cn5_situationalKnowledge(n).\n";
            pgm.append("#external -").append("propertyOf").append("(n, ").append(ASPTranslator::CONCEPTNET_PREFIX + pair.first->term + ").\n");
            if (pair.second.empty()) {
                pgm.append(createInconsistencyBackgroundKnowledgeRule(pair.first, "",edges.at(0)));
            }
            for (srg::conceptnet::Edge* edge : pair.second) {
                std::string antonym;
                if (edge->fromConcept == pair.first) {
                    antonym = edge->toConcept->term;
                } else {
                    antonym = edge->fromConcept->term;
                }
                if (std::find(closed.begin(), closed.end(), antonym) != closed.end()) {
                    continue;
                }
                pgm.append(createInconsistencyBackgroundKnowledgeRule(pair.first, antonym, nullptr));
                closed.push_back(antonym);
            }
            ret.emplace(pair.first->term, pgm);
        }
    }
    return ret;
}

std::string ASPTranslator::createInconsistencyBackgroundKnowledgeRule(srg::conceptnet::Concept* adjective, std::string antonym, srg::conceptnet::Edge* edge)
{
    std::string adj = conceptToASPPredicate(adjective->term);
    antonym = conceptToASPPredicate(antonym);
    std::string capitalAntonymRelation = srg::conceptnet::relations[srg::conceptnet::Relation::Antonym];
    capitalAntonymRelation[0] = std::toupper(capitalAntonymRelation[0]);
    std::string ret = "";
    ret.append("propertyOf(n, " + ASPTranslator::CONCEPTNET_PREFIX + adj + ", W) ")
            .append(":- not -propertyOf(n, " + ASPTranslator::CONCEPTNET_PREFIX + adj + "), ")
            .append("is(n, " + ASPTranslator::CONCEPTNET_PREFIX + adj + ")");
    if (!antonym.empty()) {
        ret.append(", " + ASPTranslator::CONCEPTNET_PREFIX + capitalAntonymRelation + "(" + ASPTranslator::CONCEPTNET_PREFIX + adj + ", " +
                   ASPTranslator::CONCEPTNET_PREFIX + antonym + ", W), ")
                .append("-propertyOf(n, " + ASPTranslator::CONCEPTNET_PREFIX + antonym + ")");
    } else {
        std::cout << 3 << std::endl;
        std::cout << edge->relation << std::endl;
        std::string capitalRelation = srg::conceptnet::relations[edge->relation];
        std::cout << capitalRelation << std::endl;
        capitalRelation[0] = std::toupper(capitalRelation[0]);
        ret.append(", " + ASPTranslator::CONCEPTNET_PREFIX + capitalRelation + "(" + ASPTranslator::CONCEPTNET_PREFIX +
                   conceptToASPPredicate(edge->fromConcept->term) + ", " + ASPTranslator::CONCEPTNET_PREFIX + conceptToASPPredicate(edge->toConcept->term) +
                   ", W)");
    }
    ret.append(".\n");
    return ret;
}

std::string ASPTranslator::createBackgroundKnowledgeRule(std::string relation, srg::conceptnet::Edge* edge)
{
    std::string capitalRelation = relation;
    capitalRelation[0] = std::toupper(capitalRelation[0]);
    std::string ret = "";
    ret.append(relation + "(n, m, W)")
            .append(" :- not -" + relation + "(n, m),")
            .append(" is(n, " + ASPTranslator::CONCEPTNET_PREFIX + conceptToASPPredicate(edge->fromConcept->term) + "), ")
            .append("is(m, " + ASPTranslator::CONCEPTNET_PREFIX + conceptToASPPredicate(edge->toConcept->term) + "), ")
            .append(ASPTranslator::CONCEPTNET_PREFIX + capitalRelation + "(" + ASPTranslator::CONCEPTNET_PREFIX +
                    conceptToASPPredicate(edge->fromConcept->term) + "," + ASPTranslator::CONCEPTNET_PREFIX + conceptToASPPredicate(edge->toConcept->term) +
                    ", W).\n");
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