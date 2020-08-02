#include "srg/asp/ASPTranslator.h"

#include "srg/SRGWorldModel.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/Edge.h"
#include "srg/conceptnet/Relations.h"
#include "srg/dialogue/AnswerGraph.h"
#include "srg/asp/SRGKnowledgeManager.h"

#include <algorithm>
#include <vector>

namespace srg
{
namespace asp
{

const std::string ASPTranslator::CONCEPTNET_PREFIX = "cs_";

ASPTranslator::ASPTranslator(srg::SRGWorldModel* wm)
        : wm(wm)
{
}

std::string ASPTranslator::addToKnowledgeBase(srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval)
{
    if(answerGraph->getEdges().empty()) {
        return "";
    }

    if (inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
        answerGraph->markInconsistentEdges();
    }

    // add commonsense predicate facts
    std::string programSection = "commonsenseKnowledge";
    std::string commonsenseFacts = createASPPredicates(answerGraph, inconsistencyRemoval);
    this->wm->srgKnowledgeManager->add(programSection.c_str(), {}, commonsenseFacts.c_str());
    this->wm->srgKnowledgeManager->ground({{programSection.c_str(), {}}}, nullptr);
    this->wm->srgKnowledgeManager->solve();

    // ground program section with params, if exist
//    auto indexLeft = programSection.find("(");
//    auto indexRight = programSection.find(")");
//    if (indexLeft != std::string::npos && indexRight != std::string::npos) {
//        auto tmp = programSection.substr(indexLeft + 1, indexRight - indexLeft - 1);
//        programSection = programSection.substr(0, indexLeft);
//        auto symVec = Clingo::SymbolVector();
//        auto paramList = split(tmp);
//        for (auto it : paramList) {
//            symVec.push_back(this->wm->srgKnowledgeManager->parseValue(it.c_str()));
//        }
//        this->wm->srgKnowledgeManager->ground({{programSection.c_str(), symVec}}, nullptr);
//    } else {
//        this->wm->srgKnowledgeManager->ground({{programSection.c_str(), {}}}, nullptr);
//    }
//     solve
//    this->wm->srgKnowledgeManager->solve();

    // extract and add commonsense knowledge rules
    auto pgmMap = extractCommonsenseKnowledgePrograms(answerGraph, inconsistencyRemoval);
    std::string program = "#program " + programSection + ".\n" + commonsenseFacts + "\n";
    for (auto pair : pgmMap) {
        program.append(pair.second).append("\n");
        // TODO: print out what is added and see how this can be inserted via "addInformation", or just use "add" as well
        this->wm->srgKnowledgeManager->add(programSection.c_str(), {}, pair.second.c_str());
    }
    return program; // for debug
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
                .append(conceptToASPPredicate(pair.second->fromConcept->term))
                .append(", ")
                .append(conceptToASPPredicate(pair.second->toConcept->term))
                .append(", ")
                .append(std::to_string((int) (pair.second->weight * 100)))
                .append(").\n");
        ret.append(tmp);
    }
    return ret;
}

std::map<std::string, std::string> ASPTranslator::extractCommonsenseKnowledgePrograms(
        srg::dialogue::AnswerGraph* answerGraph, InconsistencyRemoval inconsistencyRemoval)
{
    std::map<std::string, std::string> ret;
    for (auto pair : answerGraph->getEdges()) {
        if (pair.second->relation == srg::conceptnet::Relation::IsA) {
            // makes no sense for IsA relations
            continue;
        }

        if (inconsistencyRemoval == InconsistencyRemoval::KeepHighestWeight) {
            if (pair.second->causesInconsistency) {
                continue;
            }
        }
        std::string tmpRel = srg::conceptnet::relations[pair.second->relation];
        tmpRel[0] = std::tolower(tmpRel[0]);
        if (ret.find(tmpRel) == ret.end()) {
            std::string pgm = "#program situationalKnowledge(n,m).\n";
            pgm.append("#external -").append(tmpRel).append("(n, m).\n");
            pgm.append(createBackgroundKnowledgeRule(tmpRel, pair.second));
            ret.emplace(tmpRel, pgm);
        } else {
            ret.at(tmpRel).append(createBackgroundKnowledgeRule(tmpRel, pair.second));
        }
    }
    if (inconsistencyRemoval == InconsistencyRemoval::UseExternals) {
        for (auto pair : answerGraph->adjectiveAntonymMap) {
            if (pair.second.empty()) {
                continue;
            }
            std::vector<std::string> closed;
            // std::cout << "getting edges: " << answerGraph->root->term << " " << pair.first->term << std::endl;
            std::vector<srg::conceptnet::Edge*> edges = answerGraph->getEdges(answerGraph->root, pair.first);
            if (edges.empty()) {
                continue;
            }
            std::string pgm = "#program situationalKnowledge(n).\n";
            for (srg::conceptnet::Edge* edge : edges) {
                std::string relation = srg::conceptnet::relations[edge->relation];
                relation[0] = std::tolower(relation[0]);
                pgm.append("#external -" + relation + "(n, " + pair.first->term + ").\n");
                for (srg::conceptnet::Edge *edge : pair.second) {
                    std::string antonym;
                    if (edge->fromConcept == pair.first) {
                        antonym = edge->toConcept->term;
                    } else {
                        antonym = edge->fromConcept->term;
                    }
                    if (std::find(closed.begin(), closed.end(), antonym) != closed.end()) {
                        continue;
                    }
                    pgm.append(
                            createInconsistencyBackgroundKnowledgeRule(answerGraph->root, pair.first, antonym, relation));
                    closed.push_back(antonym);
                }
                ret.emplace(pair.first->term, pgm);
            }
        }
    }
    return ret;
}

std::string ASPTranslator::createInconsistencyBackgroundKnowledgeRule(
        srg::conceptnet::Concept* root, srg::conceptnet::Concept* adjective, std::string antonym, std::string relation)
{
    std::string adj = conceptToASPPredicate(adjective->term);
    antonym = conceptToASPPredicate(antonym);
    std::string antonymRelation = srg::conceptnet::relations[srg::conceptnet::Relation::Antonym];
    std::string capitalRelation = relation;
    capitalRelation[0] = std::toupper(capitalRelation[0]);
    std::string ret = "";
    ret.append(relation + "(n, " + adj + ", W) ")
            .append(":- not -" + relation + "(n, " + adj + "), ")
            .append(ASPTranslator::CONCEPTNET_PREFIX + capitalRelation + "(n, " + adj + ", W), ")
            .append("is(n, " + root->term + "), ")
            .append("-" + relation + "(n, " + antonym + ") : " + ASPTranslator::CONCEPTNET_PREFIX + antonymRelation + "(" + adj + ", " + antonym +
                    ", W).\n");
    return ret;
}

std::string ASPTranslator::createBackgroundKnowledgeRule(std::string relation, srg::conceptnet::Edge* edge)
{
    std::string lowerRelation = relation;
    lowerRelation[0] = std::tolower(lowerRelation[0]);
    std::string ret = "";
    ret.append(lowerRelation + "(n, m, W)")
            .append(" :- not -" + lowerRelation + "(n, m),")
            .append(" is(n, " + conceptToASPPredicate(edge->fromConcept->term) + "), ")
            .append("is(m, " + conceptToASPPredicate(edge->toConcept->term) + "), ")
            .append(ASPTranslator::CONCEPTNET_PREFIX + relation + "(" + conceptToASPPredicate(edge->fromConcept->term) + ", " +
                    conceptToASPPredicate(edge->toConcept->term) + ", W).\n");
    return ret;
}

//std::string ASPTranslator::expandConceptNetPredicate(std::string predicate)
//{
//    std::string ret = "";
//    ret.append(predicate).append(" :- not ").append("-").append(predicate).append(".\n");
//    return ret;
//}

std::vector<std::string> ASPTranslator::split(std::string toSplit)
{
    std::stringstream stream;
    stream << toSplit;
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(stream, segment, ',')) {
        seglist.push_back(trim(segment));
    }
    return seglist;
}

std::string ASPTranslator::trim(const std::string& s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

} // namespace asp
} // namespace srg