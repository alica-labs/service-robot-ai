#include "srg/dialogue/AnswerGraph.h"
#include "srg/conceptnet/Concept.h"
#include "srg/conceptnet/ConceptPath.h"
#include "srg/conceptnet/Edge.h"

#include <gvc.h>

#include <algorithm>
#include <boost/variant/detail/substitute.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace srg
{
namespace dialogue
{

AnswerGraph::AnswerGraph()
        : utilitiesCalculated(false)
{
}

AnswerGraph::~AnswerGraph()
{
    for (auto edgeEntry : edges) {
        delete edgeEntry.second;
    }
    for (auto conceptEntry : concepts) {
        delete conceptEntry.second;
    }
    for (auto conceptPath : answerPaths) {
        delete conceptPath;
    }
}

void AnswerGraph::setRoot(srg::conceptnet::Concept* root)
{
    this->root = root;
}

std::vector<srg::conceptnet::Concept*> AnswerGraph::getBestAnswers(int maxNumberOfAnswers)
{
    this->calculateUtilities();

    std::vector<conceptnet::Concept*> bestConcepts;
    for (auto utilityEntry : this->utilities) {
        if (bestConcepts.size() < maxNumberOfAnswers) {
            bestConcepts.push_back(utilityEntry.first);
            continue;
        }

        for (int i = 0; i < bestConcepts.size(); i++) {
            conceptnet::Concept* currentBestConcept = bestConcepts[i];
            if (this->utilities[currentBestConcept] < utilityEntry.second) {
                bestConcepts.push_back(utilityEntry.first);
                bestConcepts.erase(bestConcepts.begin() + i);
                break;
            }
        }
    }

    std::sort(bestConcepts.begin(), bestConcepts.end(),
            [this](conceptnet::Concept* a, conceptnet::Concept* b) -> bool { return this->utilities[a] < this->utilities[b]; });

    return bestConcepts;
}

void AnswerGraph::calculateUtilities()
{
    if (utilitiesCalculated) {
        return;
    }
    for (conceptnet::ConceptPath* conceptPath : this->answerPaths) {
        conceptPath->calculateUtility();
    }

    for (conceptnet::Concept* concept : this->answerConcepts) {
        double utility = 0;
        for (conceptnet::ConceptPath* conceptPath : this->answerPaths) {
            if (conceptPath->getEnd() == concept) {
                utility += conceptPath->getUtility();
            }
        }
        this->utilities.emplace(concept, utility);
    }
}

void AnswerGraph::markInconsistentEdges()
{
    for (auto pair : this->adjectiveAntonymMap) {
        if (pair.second.empty()) {
            continue;
        }
        std::cout << "markInconsistentEdges: adj " << pair.first << std::endl;
        conceptnet::Concept* adjective = this->getConcept("/c/en/" + pair.first->term);
        std::vector<conceptnet::Concept*> closed;
        for (conceptnet::Edge* edge : pair.second) {
            conceptnet::Concept* antonym;
            if (edge->fromConcept == adjective) {
                antonym = edge->toConcept;
            } else {
                antonym = edge->fromConcept;
            }
            if (std::find(closed.begin(), closed.end(), antonym) != closed.end()) {
                continue;
            }
            std::vector<conceptnet::Edge*> adjectiveEdges = this->getEdges(this->root, adjective);
            double adjectiveWeight = 0;
            for (conceptnet::Edge* edge : adjectiveEdges) {
                if(edge->causesInconsistency) {
                    continue;
                }
                adjectiveWeight += edge->weight;
            }
            std::vector<conceptnet::Edge*> antonymEdges = this->getEdges(this->root, antonym);
            double antonymWeight = 0;
            for (conceptnet::Edge* edge : antonymEdges) {
                if(edge->causesInconsistency) {
                    continue;
                }
                antonymWeight += edge->weight;
            }
            std::cout << "markInconsistentEdges: adj weight " << adjectiveWeight << " antonym weight " << antonymWeight << std::endl;
            if (adjectiveWeight >= antonymWeight) {
                for (conceptnet::Edge* edge : antonymEdges) {
                    std::cout << "markInconsistentEdges: " << edge->toString() << std::endl;
                    edge->causesInconsistency = true;
                }
            } else {
                for (conceptnet::Edge* edge : adjectiveEdges) {
                    std::cout << "markInconsistentEdges: " << edge->toString() << std::endl;
                    edge->causesInconsistency = true;
                }
            }
            closed.push_back(antonym);
        }
    }
}

std::string AnswerGraph::toString()
{
    std::stringstream ret;
    ret << "root: " << root->term << std::endl;
    ret << "edges: " << std::endl;
    for (conceptnet::Edge* edge : root->getEdges()) {
        ret << "\t" << edge->toString() << std::endl;
    }
    return ret.str();
}

void AnswerGraph::renderDot(Agraph_t* g, bool markInconsistencies)
{
    std::vector<conceptnet::Concept*> openNodes;
    std::vector<conceptnet::Concept*> closedNodes;

    Agnode_t* node = agnode(g, strdup(this->root->term.c_str()), TRUE);
    agsafeset(node, "color", "green", "");

    openNodes.push_back(this->root);

    if (!markInconsistencies) {
        while (!openNodes.empty()) {
            conceptnet::Concept* node = openNodes[0];
            // std::cout << "AnswerGraph:renderDot: " << node->term << " " << node << std::endl;
            openNodes.erase(openNodes.begin());
            if (std::find(closedNodes.begin(), closedNodes.end(), node) != closedNodes.end()) {
                continue;
            }
            closedNodes.push_back(node);

            for (const conceptnet::Edge* edge : node->getEdges()) {
                generateEdge(g, openNodes, node->term, edge);
            }
        }

        for (conceptnet::Concept* concept : this->getBestAnswers(5)) {
            Agnode_t* node = agnode(g, strdup(concept->term.c_str()), TRUE);
            Agnode_t* weightNode = agnode(g, strdup(std::to_string(this->utilities[concept]).c_str()), TRUE);
            Agedge_t* ed = agedge(g, weightNode, node, "weight", TRUE);
            agsafeset(node, "color", "red", "");
        }

    } else {
        for (auto pair : this->adjectiveAntonymMap) {
            if(pair.second.empty()) {
                continue;
            }
            for (srg::conceptnet::Edge* edge : this->root->getEdges()) {
                if (edge->fromConcept == pair.first || edge->toConcept == pair.first) {
                    generateEdge(g, openNodes, this->root->term, edge);
                }
            }
            Agnode_t* node = agnode(g, strdup(pair.first->term.c_str()), TRUE);
            if (!pair.second.empty()) {
                agsafeset(node, "color", "red", "");
            }

            for (srg::conceptnet::Edge* edge : pair.second) {
                if(adjectiveAntonymMap.find(edge->fromConcept) == adjectiveAntonymMap.end() || adjectiveAntonymMap.find(edge->toConcept) == adjectiveAntonymMap.end()) {
                    continue;
                }
                generateEdge(g, openNodes, pair.first->term, edge);
            }
        }
    }
}

void AnswerGraph::generateEdge(Agraph_t* g, std::vector<conceptnet::Concept*>& openNodes, std::string term, const conceptnet::Edge* edge)
{
    Agnode_t* to;
    Agnode_t* from;
    if (edge->fromConcept->term == term) {
        to = agnode(g, strdup(edge->toConcept->term.c_str()), TRUE);
        from = agnode(g, strdup(term.c_str()), TRUE);
        openNodes.push_back(edge->toConcept);
    } else {
        to = agnode(g, strdup(term.c_str()), TRUE);
        from = agnode(g, strdup(edge->fromConcept->term.c_str()), TRUE);
        openNodes.push_back(edge->fromConcept);
    }
    Agedge_t* ed = agedge(g, from, to, strdup(conceptnet::relations[edge->relation]), TRUE);
    agsafeset(ed, "label", strdup(std::string(conceptnet::relations[edge->relation]).append(" / " + std::to_string(edge->weight)).c_str()), "");
}

conceptnet::Concept* AnswerGraph::getConcept(std::string conceptId) const
{
    auto mapEntry = this->concepts.find(conceptId);
    if (mapEntry != this->concepts.end()) {
        return mapEntry->second;
    } else {
        return nullptr;
    }
}

conceptnet::Concept* AnswerGraph::createConcept(std::string conceptId, std::string term, std::string senseLabel)
{
    auto mapEntry = this->concepts.find(conceptId);
    if (mapEntry != this->concepts.end()) {
        return mapEntry->second;
    } else {
        return this->concepts.emplace(conceptId, new conceptnet::Concept(conceptId, term, senseLabel)).first->second;
    }
}

conceptnet::Edge* AnswerGraph::getEdge(std::string edgeId) const
{
    auto mapEntry = this->edges.find(edgeId);
    if (mapEntry != this->edges.end()) {
        return mapEntry->second;
    } else {
        return nullptr;
    }
}

std::vector<conceptnet::Edge*> AnswerGraph::getEdges(conceptnet::Concept* firstConcept, conceptnet::Concept* secondConcept)
{
    std::vector<conceptnet::Edge*> edges;
    for (auto pair : this->edges) {
        if (pair.second->fromConcept == firstConcept && pair.second->toConcept == secondConcept) {
            edges.push_back(pair.second);
        }
        if (pair.second->toConcept == firstConcept && pair.second->fromConcept == secondConcept) {
            edges.push_back(pair.second);
        }
    }
    return edges;
}

conceptnet::Edge* AnswerGraph::createEdge(std::string edgeId, std::string language, srg::conceptnet::Concept* fromConcept, srg::conceptnet::Concept* toConcept,
        srg::conceptnet::Relation relation, double weight)
{
    auto mapEntry = this->edges.find(edgeId);
    if (mapEntry != this->edges.end()) {
        return mapEntry->second;
    } else {
        return this->edges.emplace(edgeId, new conceptnet::Edge(edgeId, language, fromConcept, toConcept, relation, weight)).first->second;
    }
}

const std::map<std::string, conceptnet::Concept*>& AnswerGraph::getConcepts() const
{
    return this->concepts;
}

const std::map<std::string, conceptnet::Edge*>& AnswerGraph::getEdges() const
{
    return this->edges;
}

} // namespace dialogue

} // namespace srg